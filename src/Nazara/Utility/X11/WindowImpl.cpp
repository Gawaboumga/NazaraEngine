// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

// Un grand merci à Laurent Gomila pour la SFML qui m'aura bien aidé à réaliser cette implémentation

#define OEMRESOURCE

#include <Nazara/Utility/X11/WindowImpl.hpp>
#include <Nazara/Core/ConditionVariable.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/Mutex.hpp>
#include <Nazara/Core/Thread.hpp>
#include <Nazara/Utility/Config.hpp>
#include <Nazara/Utility/Cursor.hpp>
#include <Nazara/Utility/Image.hpp>
#include <Nazara/Utility/Icon.hpp>
#include <Nazara/Utility/X11/CursorImpl.hpp>
#include <Nazara/Utility/X11/IconImpl.hpp>
#include <cstdio>
#include <memory>
#include <Utfcpp/utf8.h>
#include <xcb/xcb.h>
#include <X11/XF86keysym.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_keysyms.h>
#include <iostream>
#include <locale>
#include <Nazara/Utility/Debug.hpp>

namespace
{
	NzWindowImpl*              fullscreenWindow = NULL;

	static const unsigned long            eventMask = XCB_EVENT_MASK_FOCUS_CHANGE   | XCB_EVENT_MASK_BUTTON_PRESS     |
													  XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_MOTION    |
													  XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_KEY_PRESS        |
													  XCB_EVENT_MASK_KEY_RELEASE    | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
													  XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW;
}

NzWindowImpl::NzWindowImpl(NzWindow* parent) :
m_window(0),
m_hiddenCursor(0),
m_size_hints{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
m_style(0),
m_parent(parent),
m_smoothScrolling(false),
m_scrolling(0),
m_mousePos(0, 0),
m_keyRepeat(true)
{
	// Open a connection with the X server
	m_connection = X11::OpenConnection();
}

NzWindowImpl::~NzWindowImpl()
{
	// Cleanup graphical resources
	CleanUp();

	// Destroy the cursor
	if (m_hiddenCursor)
		xcb_free_cursor(m_connection, m_hiddenCursor);

	// Close the connection with the X server
	X11::CloseConnection(m_connection);
}

bool NzWindowImpl::Create(const NzVideoMode& mode, const NzString& title, nzUInt32 style)
{
	bool fullscreen = (style & nzWindowStyle_Fullscreen) != 0;
	m_ownsWindow = true;

	std::memset(&m_oldVideoMode, 0, sizeof(m_oldVideoMode));

	m_screen = X11::XCBDefaultScreen(m_connection);

	// Compute position and size
	int left = fullscreen ? 0 : (m_screen->width_in_pixels  - mode.width) / 2;
	int top = fullscreen ? 0 : (m_screen->height_in_pixels - mode.height) / 2;
	int width  = mode.width;
	int height = mode.height;

	m_size_hints.x = left;
	m_size_hints.y = top;
	m_size_hints.width = width;
	m_size_hints.height = height;

	// Define the window attributes
	xcb_colormap_t colormap = xcb_generate_id(m_connection);
	xcb_create_colormap(m_connection, XCB_COLORMAP_ALLOC_NONE, colormap, m_screen->root, m_screen->root_visual);
	const uint32_t value_list[] = {fullscreen, static_cast<uint32_t>(eventMask), colormap};

	// Create the window
	m_window = xcb_generate_id(m_connection);

	if (!X11::TestCookie(
		m_connection,
		xcb_create_window_checked(
			m_connection,
			XCB_COPY_FROM_PARENT,
			m_window,
			m_screen->root,
			left, top,
			width, height,
			0,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			m_screen->root_visual,
			XCB_CW_EVENT_MASK | XCB_CW_OVERRIDE_REDIRECT | XCB_CW_COLORMAP,
			value_list
		), "Failed to create window"
	))
		return false;

	// Flush the commands queue
	xcb_flush(m_connection);

	// Do some common initializations
	CommonInitialize();

	// Flush the commands queue
	xcb_flush(m_connection);

	// Set the window's name
	SetTitle(title);

	// Set fullscreen video mode and switch to fullscreen if necessary
	if (fullscreen)
	{
		SetPosition(0, 0);
		SetVideoMode(mode);
		SwitchToFullscreen();
	}

	return true;
}

bool NzWindowImpl::Create(NzWindowHandle handle)
{
	// Open a connection with the X server
	m_connection = X11::OpenConnection();

	std::memset(&m_oldVideoMode, 0, sizeof(m_oldVideoMode));

	if (!m_connection)
	{
		NazaraError("Failed cast Display object to an XCB connection object");
		return false;
	}

	m_screen = X11::XCBDefaultScreen(m_connection);

	// Save the window handle
	m_window = handle;

	if (m_window)
	{
		// Make sure the window is listening to all the required events
		const uint32_t value_list[] = {static_cast<uint32_t>(eventMask)};

		xcb_change_window_attributes(
			m_connection,
			m_window,
			XCB_CW_EVENT_MASK,
			value_list
		);

		NzScopedXCB<xcb_generic_error_t> error(nullptr);

		xcb_icccm_get_wm_normal_hints_reply(
			m_connection,
			xcb_icccm_get_wm_normal_hints(
				m_connection,
				m_window),
			&m_size_hints,
			&error
		);

		if (error)
		{
			NazaraError("Failed to obtain sizes and positions");
			return false;
		}

		// Do some common initializations
		CommonInitialize();

		// Flush the commands queue
		xcb_flush(m_connection);
	}

	return true;
}

void NzWindowImpl::Destroy()
{
	if (m_ownsWindow)
	{
		#if NAZARA_UTILITY_THREADED_WINDOW
		if (m_thread.IsJoinable())
		{
			m_threadActive = false;
			//PostMessageW(m_handle, WM_NULL, 0, 0); // Pour réveiller le thread

			m_thread.Join();
		}
		#else
		// Destroy the window
		if (m_window && m_ownsWindow)
		{
			// Unhide the mouse cursor (in case it was hidden)
			SetCursor(nzWindowCursor_Default);

			xcb_destroy_window(m_connection, m_window);

			xcb_flush(m_connection);
		}
		#endif
	}
	else
		SetEventListener(false);
}

void NzWindowImpl::EnableKeyRepeat(bool enable)
{
	m_keyRepeat = enable;
}

void NzWindowImpl::EnableSmoothScrolling(bool enable)
{
	m_smoothScrolling = enable;
}

NzWindowHandle NzWindowImpl::GetHandle() const
{
	return m_window;
}

unsigned int NzWindowImpl::GetHeight() const
{
	return m_size_hints.height;
}

NzVector2i NzWindowImpl::GetPosition() const
{
	return { m_size_hints.x, m_size_hints.y };
}

NzVector2ui NzWindowImpl::GetSize() const
{
	return NzVector2ui(m_size_hints.height, m_size_hints.height);
}

nzUInt32 NzWindowImpl::GetStyle() const
{
	return m_style;
}

NzString NzWindowImpl::GetTitle() const
{
	xcb_ewmh_connection_t* ewmh_connection = X11::OpenEWMHConnection(m_connection);

	NzScopedXCB<xcb_generic_error_t> error(NULL);

	xcb_ewmh_get_utf8_strings_reply_t data;
	xcb_ewmh_get_wm_name_reply(ewmh_connection,
		xcb_ewmh_get_wm_name(ewmh_connection, m_window), &data, &error);

	if (error)
		NazaraError("Failed to get window's title");

	X11::CloseEWMHConnection(ewmh_connection);

	NzString tmp(data.strings, data.strings_len);

	xcb_ewmh_get_utf8_strings_reply_wipe(&data);

	return tmp;
}

unsigned int NzWindowImpl::GetWidth() const
{
	return m_size_hints.width;
}

bool NzWindowImpl::HasFocus() const
{
	NzScopedXCB<xcb_generic_error_t> error(nullptr);

	xcb_ewmh_connection_t* ewmh_connection = X11::OpenEWMHConnection(m_connection);

	NzScopedXCB<xcb_get_input_focus_reply_t> reply(xcb_get_input_focus_reply(
		m_connection,
		xcb_get_input_focus_unchecked(
			m_connection
		),
		&error
	));

	if (error)
		NazaraError("Failed to check if window has focus");

	return (reply->focus == m_window);
}

void NzWindowImpl::IgnoreNextMouseEvent(int mouseX, int mouseY)
{
	// Petite astuce ...
	m_mousePos.x = mouseX;
	m_mousePos.y = mouseY;
}

bool NzWindowImpl::IsMinimized() const
{
	xcb_ewmh_connection_t* ewmh_connection = X11::OpenEWMHConnection(m_connection);

	NzScopedXCB<xcb_generic_error_t> error(NULL);
	bool isMinimized = false;

	xcb_ewmh_get_atoms_reply_t atomReply;
	if (xcb_ewmh_get_wm_state_reply(ewmh_connection,
			xcb_ewmh_get_wm_state(ewmh_connection, m_window), &atomReply, &error) == 1)
	{
		for (unsigned int i = 0; i < atomReply.atoms_len; i++)
			if (atomReply.atoms[i] == ewmh_connection->_NET_WM_STATE_HIDDEN)
				isMinimized = true;

		xcb_ewmh_get_atoms_reply_wipe(&atomReply);
	}

	if (error)
		NazaraError("Failed to determine if window is minimized");

	X11::CloseEWMHConnection(ewmh_connection);

	return isMinimized;
}

bool NzWindowImpl::IsVisible() const
{
	return !IsMinimized(); // Is it right ?
}

void NzWindowImpl::ProcessEvents(bool block)
{
	if (m_ownsWindow)
	{
		xcb_generic_event_t* event = nullptr;

		if (block)
		{
			event = xcb_wait_for_event(m_connection);
			ProcessEvent(event);
			std::free(event);
		}
		else
		{
			while (event = xcb_poll_for_event(m_connection))
			{
				ProcessEvent(event);
				std::free(event);
			}
		}
	}
}

void NzWindowImpl::SetCursor(nzWindowCursor windowCursor)
{
	if (windowCursor == nzWindowCursor_None)
		SetCursor(m_hiddenCursor);
	else
	{
		const char* name = ConvertWindowCursorToXName(windowCursor);

		xcb_cursor_context_t *ctx;
		if (xcb_cursor_context_new(m_connection, m_screen, &ctx) >= 0)
		{
			xcb_cursor_t cursor = xcb_cursor_load_cursor(ctx, name);
			SetCursor(cursor);
			xcb_free_cursor(m_connection, cursor);
			xcb_cursor_context_free(ctx);
		}
	}
}

void NzWindowImpl::SetCursor(const NzCursor& cursor)
{
	SetCursor(cursor.m_impl->GetCursor());
}

void NzWindowImpl::SetEventListener(bool listener)
{
	/*if (m_ownsWindow)
		m_eventListener = listener;
	else if (listener != m_eventListener)
	{
		if (listener)
		{
			SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
			m_callback = SetWindowLongPtr(m_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MessageHandler));
			m_eventListener = true;
		}
		else if (m_eventListener)
		{
			SetWindowLongPtr(m_handle, GWLP_WNDPROC, m_callback);
			m_eventListener = false;
		}
	}*/
}

void NzWindowImpl::SetFocus()
{
	if (!X11::TestCookie(
		m_connection,
		xcb_set_input_focus(
			m_connection,
			XCB_INPUT_FOCUS_POINTER_ROOT,
			m_window,
			XCB_CURRENT_TIME
		), "Failed to change active window (set_input_focus)"
	))
		return;

	const uint32_t values[] = { XCB_STACK_MODE_ABOVE };

	X11::TestCookie(
		m_connection,
		xcb_configure_window(
			m_connection,
			m_window,
			XCB_CONFIG_WINDOW_STACK_MODE,
			values
		), "Failed to change active window (configure_window)"
	);
}

void NzWindowImpl::SetIcon(const NzIcon& icon)
{
	xcb_pixmap_t icon_pixmap = icon.m_impl->GetIcon();
	xcb_pixmap_t mask_pixmap = icon.m_impl->GetMask();

	xcb_icccm_wm_hints_t hints;
	std::memset(&hints, 0, sizeof(hints));
	xcb_icccm_wm_hints_set_icon_pixmap(&hints, icon_pixmap);
	xcb_icccm_wm_hints_set_icon_mask(&hints, mask_pixmap);

	xcb_icccm_set_wm_hints(m_connection, m_window, &hints);

	xcb_flush(m_connection);
}

void NzWindowImpl::SetMaximumSize(int width, int height)
{
	if (width < 0)
		width = m_screen->width_in_pixels;
	if (height < 0)
		height = m_screen->height_in_pixels;

	xcb_icccm_size_hints_set_max_size(&m_size_hints, width, height);
	X11::TestCookie(
		m_connection,
		xcb_icccm_set_wm_normal_hints(
			m_connection,
			m_window,
			&m_size_hints
		), "Failed to set maximum size"
	);

	xcb_flush(m_connection);
}

void NzWindowImpl::SetMinimumSize(int width, int height)
{
	xcb_icccm_size_hints_set_min_size(&m_size_hints, width, height);
	X11::TestCookie(
		m_connection,
		xcb_icccm_set_wm_normal_hints(
			m_connection,
			m_window,
			&m_size_hints
		), "Failed to set minimum size"
	);

	xcb_flush(m_connection);
}

void NzWindowImpl::SetPosition(int x, int y)
{
	xcb_icccm_size_hints_set_position(&m_size_hints, true, x, y);
	X11::TestCookie(
		m_connection,
		xcb_icccm_set_wm_normal_hints(
			m_connection,
			m_window,
			&m_size_hints
		), "Failed to set position"
	);

	uint32_t values[] = { static_cast<uint32_t>(x), static_cast<uint32_t>(y) };
	X11::TestCookie(
		m_connection,
		xcb_configure_window(
			m_connection,
			m_window,
			XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
			values
		), "Failed to set position (configure_window)"
	);

	xcb_flush(m_connection);
}

void NzWindowImpl::SetSize(unsigned int width, unsigned int height)
{
	xcb_icccm_size_hints_set_size(&m_size_hints, true, width, height);
	X11::TestCookie(
		m_connection,
		xcb_icccm_set_wm_normal_hints(
			m_connection,
			m_window,
			&m_size_hints
		), "Failed to set sizes"
	);

	uint32_t values[] = { width, height };
    X11::TestCookie(
		m_connection,
		xcb_configure_window(
			m_connection,
			m_window,
			XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
			values
		), "Failed to set sizes (configure_window)"
	);

	xcb_flush(m_connection);
}

void NzWindowImpl::SetStayOnTop(bool stayOnTop)
{
	xcb_ewmh_connection_t* ewmh_connection = X11::OpenEWMHConnection(m_connection);

	xcb_atom_t onTop; // It is not really working
	if (stayOnTop)
		onTop = ewmh_connection->_NET_WM_STATE_ABOVE;
	else
		onTop = ewmh_connection->_NET_WM_STATE_BELOW;

	X11::TestCookie(
		m_connection,
		xcb_ewmh_set_wm_state(
			ewmh_connection,
			m_window,
			1,
			&onTop
		), "Failed to set stay on top"
	);

	X11::CloseEWMHConnection(ewmh_connection);

	xcb_flush(m_connection);
}

void NzWindowImpl::SetTitle(const NzString& title)
{
	xcb_ewmh_connection_t* ewmh_connection = X11::OpenEWMHConnection(m_connection);

	xcb_ewmh_set_wm_name(ewmh_connection, m_window, title.GetSize(), title.GetConstBuffer());

	X11::CloseEWMHConnection(ewmh_connection);

	xcb_flush(m_connection);
}

void NzWindowImpl::SetVisible(bool visible)
{
	if (visible)
	{
		X11::TestCookie(
			m_connection,
			xcb_map_window(
				m_connection,
				m_window
			), "Failed to change window visibility"
		);
	}
	else
	{
		X11::TestCookie(
			m_connection,
			xcb_unmap_window(
				m_connection,
				m_window
			), "Failed to change window visibility"
		);
	}

	xcb_flush(m_connection);
}

bool NzWindowImpl::Initialize()
{
	return true;
}

void NzWindowImpl::Uninitialize()
{
}

NzKeyboard::Key NzWindowImpl::ConvertVirtualKey(KeySym symbol)
{
	// First convert to uppercase (to avoid dealing with two different keysyms for the same key)
	KeySym lower, key;
	XConvertCase(symbol, &lower, &key);

	switch (key)
	{
		// Lettres
		case XK_A: return NzKeyboard::A;
		case XK_B: return NzKeyboard::B;
		case XK_C: return NzKeyboard::C;
		case XK_D: return NzKeyboard::D;
		case XK_E: return NzKeyboard::E;
		case XK_F: return NzKeyboard::F;
		case XK_G: return NzKeyboard::G;
		case XK_H: return NzKeyboard::H;
		case XK_I: return NzKeyboard::I;
		case XK_J: return NzKeyboard::J;
		case XK_K: return NzKeyboard::K;
		case XK_L: return NzKeyboard::L;
		case XK_M: return NzKeyboard::M;
		case XK_N: return NzKeyboard::N;
		case XK_O: return NzKeyboard::O;
		case XK_P: return NzKeyboard::P;
		case XK_Q: return NzKeyboard::Q;
		case XK_R: return NzKeyboard::R;
		case XK_S: return NzKeyboard::S;
		case XK_T: return NzKeyboard::T;
		case XK_U: return NzKeyboard::U;
		case XK_V: return NzKeyboard::V;
		case XK_W: return NzKeyboard::W;
		case XK_X: return NzKeyboard::X;
		case XK_Y: return NzKeyboard::Y;
		case XK_Z: return NzKeyboard::Z;

		// Touches de fonction
		case XK_F1: return NzKeyboard::F1;
		case XK_F2: return NzKeyboard::F2;
		case XK_F3: return NzKeyboard::F3;
		case XK_F4: return NzKeyboard::F4;
		case XK_F5: return NzKeyboard::F5;
		case XK_F6: return NzKeyboard::F6;
		case XK_F7: return NzKeyboard::F7;
		case XK_F8: return NzKeyboard::F8;
		case XK_F9: return NzKeyboard::F9;
		case XK_F10: return NzKeyboard::F10;
		case XK_F11: return NzKeyboard::F11;
		case XK_F12: return NzKeyboard::F12;
		case XK_F13: return NzKeyboard::F13;
		case XK_F14: return NzKeyboard::F14;
		case XK_F15: return NzKeyboard::F15;

		// Flèches directionnelles
		case XK_Down: return NzKeyboard::Down;
		case XK_Left: return NzKeyboard::Left;
		case XK_Right: return NzKeyboard::Right;
		case XK_Up: return NzKeyboard::Up;

		// Pavé numérique
		case XK_KP_Add: return NzKeyboard::Add;
		case XK_KP_Decimal: return NzKeyboard::Decimal;
		case XK_KP_Divide: return NzKeyboard::Divide;
		case XK_KP_Multiply: return NzKeyboard::Multiply;
		case XK_KP_0: return NzKeyboard::Numpad0;
		case XK_KP_1: return NzKeyboard::Numpad1;
		case XK_KP_2: return NzKeyboard::Numpad2;
		case XK_KP_3: return NzKeyboard::Numpad3;
		case XK_KP_4: return NzKeyboard::Numpad4;
		case XK_KP_5: return NzKeyboard::Numpad5;
		case XK_KP_6: return NzKeyboard::Numpad6;
		case XK_KP_7: return NzKeyboard::Numpad7;
		case XK_KP_8: return NzKeyboard::Numpad8;
		case XK_KP_9: return NzKeyboard::Numpad9;
		case XK_KP_Subtract: return NzKeyboard::Subtract;

		// Divers
		case XK_backslash: return NzKeyboard::Backslash;
		case XK_BackSpace: return NzKeyboard::Backspace;
		case XK_Clear: return NzKeyboard::Clear;
		case XK_comma: return NzKeyboard::Comma;
		case XK_minus: return NzKeyboard::Dash;
		case XK_Delete: return NzKeyboard::Delete;
		case XK_End: return NzKeyboard::End;
		case XK_equal: return NzKeyboard::Equal;
		case XK_Escape: return NzKeyboard::Escape;
		case XK_Home: return NzKeyboard::Home;
		case XK_Insert: return NzKeyboard::Insert;
		case XK_Alt_L: return NzKeyboard::LAlt;
		case XK_bracketleft: return NzKeyboard::LBracket;
		case XK_Control_L: return NzKeyboard::LControl;
		case XK_Shift_L: return NzKeyboard::LShift;
		case XK_Super_L: return NzKeyboard::LSystem;
		case XK_0: return NzKeyboard::Num0;
		case XK_1: return NzKeyboard::Num1;
		case XK_2: return NzKeyboard::Num2;
		case XK_3: return NzKeyboard::Num3;
		case XK_4: return NzKeyboard::Num4;
		case XK_5: return NzKeyboard::Num5;
		case XK_6: return NzKeyboard::Num6;
		case XK_7: return NzKeyboard::Num7;
		case XK_8: return NzKeyboard::Num8;
		case XK_9: return NzKeyboard::Num9;
		case XK_Page_Down: return NzKeyboard::PageDown;
		case XK_Page_Up: return NzKeyboard::PageUp;
		case XK_Pause: return NzKeyboard::Pause;
		case XK_period: return NzKeyboard::Period;
		case XK_Print: return NzKeyboard::Print;
		case XK_Sys_Req: return NzKeyboard::PrintScreen;
		case XK_quotedbl: return NzKeyboard::Quote;
		case XK_Alt_R: return NzKeyboard::RAlt;
		case XK_bracketright: return NzKeyboard::RBracket;
		case XK_Control_R: return NzKeyboard::RControl;
		case XK_Return: return NzKeyboard::Return;
		case XK_Shift_R: return NzKeyboard::RShift;
		case XK_Super_R: return NzKeyboard::RSystem;
		case XK_semicolon: return NzKeyboard::Semicolon;
		case XK_slash: return NzKeyboard::Slash;
		case XK_space: return NzKeyboard::Space;
		case XK_Tab: return NzKeyboard::Tab;
		case XK_grave: return NzKeyboard::Tilde;

		// Touches navigateur
		case XF86XK_Back: return NzKeyboard::Browser_Back;
		case XF86XK_Favorites: return NzKeyboard::Browser_Favorites;
		case XF86XK_Forward: return NzKeyboard::Browser_Forward;
		case XF86XK_HomePage: return NzKeyboard::Browser_Home;
		case XF86XK_Refresh: return NzKeyboard::Browser_Refresh;
		case XF86XK_Search: return NzKeyboard::Browser_Search;
		case XF86XK_Stop: return NzKeyboard::Browser_Stop;

		// Touches de contrôle
		case XF86XK_AudioNext: return NzKeyboard::Media_Next;
		case XF86XK_AudioPlay: return NzKeyboard::Media_Play;
		case XF86XK_AudioPrev: return NzKeyboard::Media_Previous;
		case XF86XK_AudioStop: return NzKeyboard::Media_Stop;

		// Touches de contrôle du volume
		case XF86XK_AudioLowerVolume: return NzKeyboard::Volume_Down;
		case XF86XK_AudioMute: return NzKeyboard::Volume_Mute;
		case XF86XK_AudioRaiseVolume: return NzKeyboard::Volume_Up;

		// Touches à verrouillage
		case XK_Caps_Lock: return NzKeyboard::CapsLock;
		case XK_Num_Lock: return NzKeyboard::NumLock;
		case XK_Scroll_Lock: return NzKeyboard::ScrollLock;

		default:
			return NzKeyboard::Undefined;
	}
}

void NzWindowImpl::SwitchToFullscreen()
{
	SetFocus();

	xcb_ewmh_connection_t* ewmh_connection = X11::OpenEWMHConnection(m_connection);

	X11::TestCookie(
		m_connection,
		xcb_ewmh_set_wm_fullscreen_monitors_checked(
			ewmh_connection,
			m_window,
			m_size_hints.x, m_size_hints.x + m_size_hints.width,
			m_size_hints.y, m_size_hints.y + m_size_hints.height
		), "Switching to fullscreen failed"
	);

	X11::CloseEWMHConnection(ewmh_connection);
}

void NzWindowImpl::CommonInitialize()
{
	// Show the window
	SetVisible(true);

	// Raise the window and grab input focus
	SetFocus();

	// Create the hidden cursor
	CreateHiddenCursor();

	xcb_atom_t protocols[] =
	{
		X11::GetAtom("WM_DELETE_WINDOW"),
	};
	xcb_icccm_set_wm_protocols(m_connection, m_window, X11::GetAtom("WM_PROTOCOLS"),
		sizeof(protocols), protocols);

	// Flush the commands queue
	xcb_flush(m_connection);
}

void NzWindowImpl::CreateHiddenCursor()
{
	xcb_pixmap_t cursorPixmap = xcb_generate_id(m_connection);

	if (!X11::TestCookie(
		m_connection,
		xcb_create_pixmap(
			m_connection,
			1,
			cursorPixmap,
			m_window,
			1,
			1
		), "Failed to create pixmap for hidden cursor"
	))
		return;

	m_hiddenCursor = xcb_generate_id(m_connection);

	// Create the cursor, using the pixmap as both the shape and the mask of the cursor
	X11::TestCookie(
		m_connection,
		xcb_create_cursor(
			m_connection,
			m_hiddenCursor,
			cursorPixmap,
			cursorPixmap,
			0, 0, 0, // Foreground RGB color
			0, 0, 0, // Background RGB color
			0,       // X
			0        // Y
		), "Failed to create hidden cursor"
	);

	// We don't need the pixmap any longer, free it
	X11::TestCookie(
		m_connection,
		xcb_free_pixmap(
			m_connection,
			cursorPixmap
		), "Failed to free pixmap for hidden cursor"
	);
}

bool NzWindowImpl::ProcessEvent(xcb_generic_event_t* windowEvent)
{
	// Convert the X11 event to a sf::NzEvent
	switch (windowEvent->response_type & ~0x80)
	{
		// Destroy event
		case XCB_DESTROY_NOTIFY:
		{
			// The window is about to be destroyed: we must cleanup resources
			CleanUp();
			break;
		}

		// Gain focus event
		case XCB_FOCUS_IN:
		{
			NzEvent event;
			event.type = nzEventType_GainedFocus;
			m_parent->PushEvent(event);

			break;
		}

		// Lost focus event
		case XCB_FOCUS_OUT:
		{
			NzEvent event;
			event.type = nzEventType_LostFocus;
			m_parent->PushEvent(event);

			break;
		}

		// Resize event
		case XCB_CONFIGURE_NOTIFY:
		{
			xcb_configure_notify_event_t* configureNotifyEvent = (xcb_configure_notify_event_t*)windowEvent;
			// ConfigureNotify can be triggered for other reasons, check if the size has actually changed
			if ((configureNotifyEvent->width != m_size_hints.width) || (configureNotifyEvent->height != m_size_hints.width))
			{
				NzEvent event;
				event.type        = nzEventType_Resized;
				event.size.width  = configureNotifyEvent->width;
				event.size.height = configureNotifyEvent->height;
				m_parent->PushEvent(event);

				m_size_hints.x = configureNotifyEvent->width;
				m_size_hints.y = configureNotifyEvent->height;
			}
			if ((configureNotifyEvent->x != m_size_hints.x) || (configureNotifyEvent->y != m_size_hints.y))
			{
				NzEvent event;
				event.type        = nzEventType_Moved;
				event.size.width  = configureNotifyEvent->x;
				event.size.height = configureNotifyEvent->y;
				m_parent->PushEvent(event);

				m_size_hints.x = configureNotifyEvent->x;

				m_size_hints.y = configureNotifyEvent->y;
			}
			break;
		}

		// Close event
		case XCB_CLIENT_MESSAGE:
		{
			xcb_client_message_event_t* clientMessageEvent = (xcb_client_message_event_t*)windowEvent;

			if (clientMessageEvent->type != X11::GetAtom("WM_PROTOCOLS"))
				break;
			if (clientMessageEvent->data.data32[0] == X11::GetAtom("WM_DELETE_WINDOW"))
			{
				NzEvent event;
				event.type = nzEventType_Quit;
				m_parent->PushEvent(event);
			}

			break;
		}

		// Key down event
		case XCB_KEY_PRESS:
		{
			xcb_key_press_event_t* keyPressEvent = (xcb_key_press_event_t*)windowEvent;

			auto keysym = ConvertKeyCodeToKeySym(keyPressEvent->detail, keyPressEvent->state);

			NzEvent event;
			event.type        = nzEventType_KeyPressed;
			event.key.code    = ConvertVirtualKey(keysym);
			event.key.alt     = keyPressEvent->state & XCB_MOD_MASK_1;
			event.key.control = keyPressEvent->state & XCB_MOD_MASK_CONTROL;
			event.key.shift   = keyPressEvent->state & XCB_MOD_MASK_SHIFT;
			event.key.system  = keyPressEvent->state & XCB_MOD_MASK_4;
			m_parent->PushEvent(event);

			char32_t codePoint = static_cast<char32_t>(keysym);

			// WTF if (std::isprint(codePoint, std::locale(""))) + handle combining ?
			{
				NzEvent event;
				event.type           = nzEventType_TextEntered;
				event.text.character = codePoint;
				event.text.repeated  = false;
				m_parent->PushEvent(event);
			}

			break;
		}

		// Key up event
		case XCB_KEY_RELEASE:
		{
			xcb_key_release_event_t* keyReleaseEvent = (xcb_key_release_event_t*)windowEvent;

			auto keysym = ConvertKeyCodeToKeySym(keyReleaseEvent->detail, keyReleaseEvent->state);

			NzEvent event;
			event.type        = nzEventType_KeyReleased;
			event.key.code    = ConvertVirtualKey(keysym);
			event.key.alt     = keyReleaseEvent->state & XCB_MOD_MASK_1;
			event.key.control = keyReleaseEvent->state & XCB_MOD_MASK_CONTROL;
			event.key.shift   = keyReleaseEvent->state & XCB_MOD_MASK_SHIFT;
			event.key.system  = keyReleaseEvent->state & XCB_MOD_MASK_4;
			m_parent->PushEvent(event);

			break;
		}

		// Mouse button pressed
		case XCB_BUTTON_PRESS:
		{
			xcb_button_press_event_t* buttonPressEvent = (xcb_button_press_event_t*)windowEvent;

			NzEvent event;
			event.type          = nzEventType_MouseButtonPressed;
			event.mouseButton.x = buttonPressEvent->event_x;
			event.mouseButton.y = buttonPressEvent->event_y;

			if (buttonPressEvent->state & (1 << XCB_BUTTON_INDEX_1))
				event.mouseButton.button = NzMouse::Left;
			else if (buttonPressEvent->state & (1 << XCB_BUTTON_INDEX_2))
				event.mouseButton.button = NzMouse::Middle;
			else if (buttonPressEvent->state & (1 << XCB_BUTTON_INDEX_3))
				event.mouseButton.button = NzMouse::Right;
			else if (buttonPressEvent->state & (1 << XCB_BUTTON_INDEX_4))
				event.mouseButton.button = NzMouse::XButton1;
			else if (buttonPressEvent->state & (1 << XCB_BUTTON_INDEX_5))
				event.mouseButton.button = NzMouse::XButton2;
			else
				NazaraError("Mouse button not handled");

			m_parent->PushEvent(event);

			break;
		}

		// Mouse button released
		case XCB_BUTTON_RELEASE:
		{
			xcb_button_release_event_t* buttonReleaseEvent = (xcb_button_release_event_t*)windowEvent;

			NzEvent event;

			switch (buttonReleaseEvent->detail)
			{
				case XCB_BUTTON_INDEX_4:
				case XCB_BUTTON_INDEX_5:
				{
					event.type             = nzEventType_MouseWheelMoved;
					event.mouseWheel.delta = (buttonReleaseEvent->detail == XCB_BUTTON_INDEX_4) ? 1 : -1;
					break;
				}
				default:
				{
					event.type          = nzEventType_MouseButtonReleased;
					event.mouseButton.x = buttonReleaseEvent->event_x;
					event.mouseButton.y = buttonReleaseEvent->event_y;

					if (buttonReleaseEvent->state & (1 << XCB_BUTTON_INDEX_1))
						event.mouseButton.button = NzMouse::Left;
					else if (buttonReleaseEvent->state & (1 << XCB_BUTTON_INDEX_2))
						event.mouseButton.button = NzMouse::Middle;
					else if (buttonReleaseEvent->state & (1 << XCB_BUTTON_INDEX_3))
						event.mouseButton.button = NzMouse::Right;
					else if (buttonReleaseEvent->state & (1 << XCB_BUTTON_INDEX_4))
						event.mouseButton.button = NzMouse::XButton1;
					else if (buttonReleaseEvent->state & (1 << XCB_BUTTON_INDEX_5))
						event.mouseButton.button = NzMouse::XButton2;
					else
						NazaraError("Mouse button not handled");
				}
			}

			m_parent->PushEvent(event);

			break;
		}

		// Mouse moved
		case XCB_MOTION_NOTIFY:
		{
			xcb_motion_notify_event_t* motionNotifyEvent = (xcb_motion_notify_event_t*)windowEvent;

			if (m_mousePos.x == motionNotifyEvent->event_x && m_mousePos.y == motionNotifyEvent->event_y)
				break;

			NzEvent event;
			event.type        = nzEventType_MouseMoved;
			event.mouseMove.deltaX = motionNotifyEvent->event_x - m_mousePos.x;
			event.mouseMove.deltaY = motionNotifyEvent->event_y - m_mousePos.y;
			event.mouseMove.x = motionNotifyEvent->event_x;
			event.mouseMove.y = motionNotifyEvent->event_y;
			m_parent->PushEvent(event);

			m_mousePos.x = motionNotifyEvent->event_x;
			m_mousePos.y = motionNotifyEvent->event_y;

			break;
		}

		// Mouse entered
		case XCB_ENTER_NOTIFY:
		{
			NzEvent event;
			event.type = nzEventType_MouseEntered;
			m_parent->PushEvent(event);

			break;
		}

		// Mouse left
		case XCB_LEAVE_NOTIFY:
		{
			NzEvent event;
			event.type = nzEventType_MouseLeft;
			m_parent->PushEvent(event);

			break;
		}

		// Parent window changed
		case XCB_REPARENT_NOTIFY:
		{
			// Catch reparent events to properly apply fullscreen on
			// some "strange" window managers (like Awesome) which
			// seem to make use of temporary parents during mapping
			if (m_style & nzWindowStyle_Fullscreen)
				SwitchToFullscreen();

			break;
		}
	}

	return true;
}

void NzWindowImpl::CleanUp()
{
	// Restore the previous video mode (in case we were running in fullscreen)
	ResetVideoMode();
}

void NzWindowImpl::SetVideoMode(const NzVideoMode& mode)
{
	// Skip mode switching if the new mode is equal to the desktop mode
	if (mode == NzVideoMode::GetDesktopMode())
		return;

	NzScopedXCB<xcb_generic_error_t> error(NULL);

	// Check if the RandR extension is present
	const xcb_query_extension_reply_t* randrExt = xcb_get_extension_data(m_connection, &xcb_randr_id);

	if (!randrExt || !randrExt->present)
	{
		// RandR extension is not supported: we cannot use fullscreen mode
		NazaraError("Fullscreen is not supported, switching to window mode");
		return;
	}

	// Load RandR and check its version
	NzScopedXCB<xcb_randr_query_version_reply_t> randrVersion(xcb_randr_query_version_reply(
		m_connection,
		xcb_randr_query_version(
			m_connection,
			1,
			1
		),
		&error
	));

	if (error)
	{
		NazaraError("Failed to load RandR, switching to window mode");
		return;
	}

	// Get the current configuration
	NzScopedXCB<xcb_randr_get_screen_info_reply_t> config(xcb_randr_get_screen_info_reply(
		m_connection,
		xcb_randr_get_screen_info(
			m_connection,
			m_screen->root
		),
		&error
	));

	if (error || !config)
	{
		// Failed to get the screen configuration
		NazaraError("Failed to get the current screen configuration for fullscreen mode, switching to window mode");
		return;
	}

	// Save the current video mode before we switch to fullscreen
	m_oldVideoMode = *config.get();

	// Get the available screen sizes
	xcb_randr_screen_size_t* sizes = xcb_randr_get_screen_info_sizes(config.get());

	if (!sizes || !config->nSizes)
	{
		NazaraError("Failed to get the fullscreen sizes, switching to window mode");
		return;
	}

	// Search for a matching size
	for (int i = 0; i < config->nSizes; ++i)
	{
		if (config->rotation == XCB_RANDR_ROTATION_ROTATE_90 ||
			config->rotation == XCB_RANDR_ROTATION_ROTATE_270)
			std::swap(sizes[i].height, sizes[i].width);

		if ((sizes[i].width  == static_cast<int>(mode.width)) &&
			(sizes[i].height == static_cast<int>(mode.height)))
		{
			// Switch to fullscreen mode
			NzScopedXCB<xcb_randr_set_screen_config_reply_t> setScreenConfig(xcb_randr_set_screen_config_reply(
				m_connection,
				xcb_randr_set_screen_config(
					m_connection,
					config->root,
					XCB_CURRENT_TIME,
					config->config_timestamp,
					i,
					config->rotation,
					config->rate
				),
				&error
			));

			if (error)
				NazaraError("Failed to set new screen configuration");

			// Set "this" as the current fullscreen window
			fullscreenWindow = this;
			return;
		}
	}

	NazaraError("Failed to find matching fullscreen size, switching to window mode");
}

void NzWindowImpl::ResetVideoMode()
{
	if (fullscreenWindow == this)
	{
		// Get current screen info
		NzScopedXCB<xcb_generic_error_t> error(NULL);

		// Reset the video mode
		NzScopedXCB<xcb_randr_set_screen_config_reply_t> setScreenConfig(xcb_randr_set_screen_config_reply(
			m_connection,
			xcb_randr_set_screen_config(
				m_connection,
				m_oldVideoMode.root,
				XCB_CURRENT_TIME,
				m_oldVideoMode.config_timestamp,
				m_oldVideoMode.sizeID,
				m_oldVideoMode.rotation,
				m_oldVideoMode.rate
			),
			&error
		));

		if (error)
			NazaraError("Failed to reset old screen configuration");

		// Reset the fullscreen window
		fullscreenWindow = nullptr;
	}
}

xcb_keysym_t NzWindowImpl::ConvertKeyCodeToKeySym(xcb_keycode_t keycode, uint16_t state)
{
	xcb_key_symbols_t* keysyms;
	if (!(keysyms = xcb_key_symbols_alloc(m_connection)))
		return 0;

	int col = state & XCB_MOD_MASK_SHIFT ? 1 : 0;
	const int altGrOffset = 4;
	if (state & XCB_MOD_MASK_5)
		col += altGrOffset;
	xcb_keysym_t keysym = xcb_key_symbols_get_keysym(keysyms, keycode, col);
	if (keysym == XCB_NO_SYMBOL)
		keysym = xcb_key_symbols_get_keysym(keysyms, keycode, col ^ 0x1);
	xcb_key_symbols_free(keysyms);

	return keysym;
}

const char* NzWindowImpl::ConvertWindowCursorToXName(nzWindowCursor cursor)
{
	// http://gnome-look.org/content/preview.php?preview=1&id=128170&file1=128170-1.png&file2=&file3=&name=Dummy+X11+cursors&PHPSESSID=6
	switch (cursor)
	{
		case nzWindowCursor_Crosshair:
			return "crosshair";
		case nzWindowCursor_Default:
			return "left_ptr";
		case nzWindowCursor_Hand:
			return "hand";
		case nzWindowCursor_Help:
			return "help";
		case nzWindowCursor_Move:
			return "fleur";
		case nzWindowCursor_None:
			return "none"; // Handled in set cursor
		case nzWindowCursor_Pointer:
			return "hand";
		case nzWindowCursor_Progress:
			return "watch";
		case nzWindowCursor_ResizeE:
			return "right_side";
		case nzWindowCursor_ResizeN:
			return "top_side";
		case nzWindowCursor_ResizeNE:
			return "top_right_corner";
		case nzWindowCursor_ResizeNW:
			return "top_left_corner";
		case nzWindowCursor_ResizeS:
			return "bottom_side";
		case nzWindowCursor_ResizeSE:
			return "bottom_right_corner";
		case nzWindowCursor_ResizeSW:
			return "bottom_left_corner";
		case nzWindowCursor_ResizeW:
			return "left_side";
		case nzWindowCursor_Text:
			return "xterm";
		case nzWindowCursor_Wait:
			return "watch";
	}

	NazaraError("Cursor is not handled by enumeration");
	return "X_cursor";
}

void NzWindowImpl::SetCursor(xcb_cursor_t cursor)
{
	X11::TestCookie(
		m_connection,
		xcb_change_window_attributes(
			m_connection,
			m_window,
			XCB_CW_CURSOR,
			&cursor
		), "Failed to change mouse cursor"
	);

	xcb_flush(m_connection);
}