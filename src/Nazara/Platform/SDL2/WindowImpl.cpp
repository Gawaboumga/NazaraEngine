// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

// Un grand merci à Laurent Gomila pour la SFML qui m'aura bien aidé à réaliser cette implémentation

#include <Nazara/Platform/SDL2/WindowImpl.hpp>
#include <Nazara/Core/ConditionVariable.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/Mutex.hpp>
#include <Nazara/Core/Thread.hpp>
#include <Nazara/Platform/Config.hpp>
#include <Nazara/Platform/Cursor.hpp>
#include <Nazara/Platform/Icon.hpp>
#include <Nazara/Platform/SDL2/CursorImpl.hpp>
#include <Nazara/Platform/SDL2/IconImpl.hpp>
#include <Nazara/Utility/Image.hpp>
#include <cstdio>
#include <memory>
#include <SDL2/SDL.h>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	namespace
	{
		const char* className = "Nazara Window";
		WindowImpl* fullscreenWindow = nullptr;

		int ConvertFromNazaraToSDLFlags(WindowStyleFlags flags)
		{
			int resultFlags = 0;
			if (flags & WindowStyle_AllowHighDPI)
				resultFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
			if (flags & WindowStyle_Fullscreen)
				resultFlags |= SDL_WINDOW_FULLSCREEN;
			if (flags & WindowStyle_GrabInput)
				resultFlags |= SDL_WINDOW_INPUT_GRABBED;
			if (flags & WindowStyle_None)
				resultFlags |= SDL_WINDOW_BORDERLESS;
			if (flags & WindowStyle_OpenGL)
				resultFlags |= SDL_WINDOW_OPENGL;
			if (flags & WindowStyle_Resizable)
				resultFlags |= SDL_WINDOW_RESIZABLE;
			return resultFlags;
		}

		Keyboard::Key ConvertFromSDLKeyToNazara(const SDL_KeyboardEvent& e)
		{
			switch (e.keysym.sym)
			{
				// Keys
				case SDLK_a:
					return Keyboard::A;
				case SDLK_b:
					return Keyboard::B;
				case SDLK_c:
					return Keyboard::C;
				case SDLK_d:
					return Keyboard::D;
				case SDLK_e:
					return Keyboard::E;
				case SDLK_f:
					return Keyboard::F;
				case SDLK_g:
					return Keyboard::G;
				case SDLK_h:
					return Keyboard::H;
				case SDLK_i:
					return Keyboard::I;
				case SDLK_j:
					return Keyboard::J;
				case SDLK_k:
					return Keyboard::K;
				case SDLK_l:
					return Keyboard::L;
				case SDLK_m:
					return Keyboard::M;
				case SDLK_n:
					return Keyboard::N;
				case SDLK_o:
					return Keyboard::O;
				case SDLK_p:
					return Keyboard::P;
				case SDLK_q:
					return Keyboard::Q;
				case SDLK_r:
					return Keyboard::R;
				case SDLK_s:
					return Keyboard::S;
				case SDLK_t:
					return Keyboard::T;
				case SDLK_u:
					return Keyboard::U;
				case SDLK_v:
					return Keyboard::V;
				case SDLK_w:
					return Keyboard::W;
				case SDLK_x:
					return Keyboard::X;
				case SDLK_y:
					return Keyboard::Y;
				case SDLK_z:
					return Keyboard::Z;

				// Functional keys
				case SDLK_F1:
					return Keyboard::F1;
				case SDLK_F2:
					return Keyboard::F2;
				case SDLK_F3:
					return Keyboard::F3;
				case SDLK_F4:
					return Keyboard::F4;
				case SDLK_F5:
					return Keyboard::F5;
				case SDLK_F6:
					return Keyboard::F6;
				case SDLK_F7:
					return Keyboard::F7;
				case SDLK_F8:
					return Keyboard::F8;
				case SDLK_F9:
					return Keyboard::F9;
				case SDLK_F10:
					return Keyboard::F10;
				case SDLK_F11:
					return Keyboard::F11;
				case SDLK_F12:
					return Keyboard::F12;
				case SDLK_F13:
					return Keyboard::F13;
				case SDLK_F14:
					return Keyboard::F14;
				case SDLK_F15:
					return Keyboard::F15;

				// Directional arrows
				case SDLK_DOWN:
					return Keyboard::Down;
				case SDLK_LEFT:
					return Keyboard::Left;
				case SDLK_RIGHT:
					return Keyboard::Right;
				case SDLK_UP:
					return Keyboard::Up;

				// Numeric pad
				case SDLK_KP_PLUS:
					return Keyboard::Add;
				case SDLK_DECIMALSEPARATOR:
					return Keyboard::Decimal;
				case SDLK_KP_DIVIDE:
					return Keyboard::Divide;
				case SDLK_KP_MULTIPLY:
					return Keyboard::Multiply;
				case SDLK_KP_0:
					return Keyboard::Numpad0;
				case SDLK_KP_1:
					return Keyboard::Numpad1;
				case SDLK_KP_2:
					return Keyboard::Numpad2;
				case SDLK_KP_3:
					return Keyboard::Numpad3;
				case SDLK_KP_4:
					return Keyboard::Numpad4;
				case SDLK_KP_5:
					return Keyboard::Numpad5;
				case SDLK_KP_6:
					return Keyboard::Numpad6;
				case SDLK_KP_7:
					return Keyboard::Numpad7;
				case SDLK_KP_8:
					return Keyboard::Numpad8;
				case SDLK_KP_9:
					return Keyboard::Numpad9;
				case SDLK_KP_MINUS:
					return Keyboard::Subtract;

				// Misc
				case SDLK_BACKSLASH:
					return Keyboard::Backslash;
				case SDLK_BACKSPACE:
					return Keyboard::Backspace;
				case SDLK_CLEAR:
					return Keyboard::Clear;
				case SDLK_COMMA:
					return Keyboard::Comma;
				case SDLK_MINUS:
					return Keyboard::Dash;
				case SDLK_DELETE:
					return Keyboard::Delete;
				case SDLK_END:
					return Keyboard::End;
				case SDLK_KP_EQUALS:
					return Keyboard::Equal;
				case SDLK_ESCAPE:
					return Keyboard::Escape;
				case SDLK_HOME:
					return Keyboard::Home;
				case SDLK_INSERT:
					return Keyboard::Insert;
				case SDLK_LALT:
					return Keyboard::LAlt;
				case SDLK_LEFTBRACKET:
					return Keyboard::LBracket;
				case SDLK_LCTRL:
					return Keyboard::LControl;
				case SDLK_LSHIFT:
					return Keyboard::LShift;
				case SDLK_LGUI:
					return Keyboard::LSystem;
				case SDLK_0:
					return Keyboard::Num0;
				case SDLK_1:
					return Keyboard::Num1;
				case SDLK_2:
					return Keyboard::Num2;
				case SDLK_3:
					return Keyboard::Num3;
				case SDLK_4:
					return Keyboard::Num4;
				case SDLK_5:
					return Keyboard::Num5;
				case SDLK_6:
					return Keyboard::Num6;
				case SDLK_7:
					return Keyboard::Num7;
				case SDLK_8:
					return Keyboard::Num8;
				case SDLK_9:
					return Keyboard::Num9;
				case SDLK_PAGEDOWN:
					return Keyboard::PageDown;
				case SDLK_PAGEUP:
					return Keyboard::PageUp;
				case SDLK_PAUSE:
					return Keyboard::Pause;
				case SDLK_KP_PERIOD:
					return Keyboard::Period;
				case SDLK_PRINTSCREEN:
					return Keyboard::PrintScreen;
				case SDLK_QUOTE:
					return Keyboard::Quote;
				case SDLK_RALT:
					return Keyboard::RAlt;
				case SDLK_RIGHTBRACKET:
					return Keyboard::RBracket;
				case SDLK_RCTRL:
					return Keyboard::RControl;
				case SDLK_RETURN:
					return Keyboard::Return;
				case SDLK_RSHIFT:
					return Keyboard::RShift;
				case SDLK_RGUI:
					return Keyboard::RSystem;
				case SDLK_SEMICOLON:
					return Keyboard::Semicolon;
				case SDLK_SLASH:
					return Keyboard::Slash;
				case SDLK_SPACE:
					return Keyboard::Space;
				case SDLK_TAB:
					return Keyboard::Tab;
				case SDLK_BACKQUOTE:
					return Keyboard::Tilde;

				// Navigator keys
				case SDLK_AC_BACK:
					return Keyboard::Browser_Back;
				case SDLK_AC_BOOKMARKS:
					return Keyboard::Browser_Favorites;
				case SDLK_AC_FORWARD:
					return Keyboard::Browser_Forward;
				case SDLK_AC_HOME:
					return Keyboard::Browser_Home;
				case SDLK_AC_REFRESH:
					return Keyboard::Browser_Refresh;
				case SDLK_AC_SEARCH:
					return Keyboard::Browser_Search;
				case SDLK_AC_STOP:
					return Keyboard::Browser_Stop;

				// Audio keys
				case SDLK_AUDIONEXT:
					return Keyboard::Media_Next;
				case SDLK_AUDIOPLAY:
					return Keyboard::Media_Play;
				case SDLK_AUDIOPREV:
					return Keyboard::Media_Previous;
				case SDLK_AUDIOSTOP:
					return Keyboard::Media_Stop;

				// Volume keys
				case SDLK_VOLUMEDOWN:
					return Keyboard::Volume_Down;
				case SDLK_AUDIOMUTE:
					return Keyboard::Volume_Mute;
				case SDLK_VOLUMEUP:
					return Keyboard::Volume_Up;

				// Lock keys
				case SDLK_CAPSLOCK:
					return Keyboard::CapsLock;
				case SDLK_NUMLOCKCLEAR:
					return Keyboard::NumLock;
				case SDLK_SCROLLLOCK:
					return Keyboard::ScrollLock;

				default:
					return Keyboard::Undefined;
			}
		}

		Mouse::Button ConvertFromSDLButtonToNazara(const SDL_MouseButtonEvent& e)
		{
			switch (e.button)
			{
				case SDL_BUTTON_LEFT:
					return Mouse::Left;
				case SDL_BUTTON_MIDDLE:
					return Mouse::Middle;
				case SDL_BUTTON_RIGHT:
					return Mouse::Right;
				case SDL_BUTTON_X1:
					return Mouse::XButton1;
				case SDL_BUTTON_X2:
					return Mouse::XButton2;
			}

			NazaraError("Mouse button not handled (0x" + String::Number(e.button, 16) + ')');
			return Mouse::Left;
		}
	}

	WindowImpl::WindowImpl(Window* parent) :
	m_style(0),
	m_parent(parent),
	m_keyRepeat(true),
	m_smoothScrolling(false)
	{
	}

	bool WindowImpl::Create(const VideoMode& mode, const String& title, WindowStyleFlags style)
	{
		bool async = (style & WindowStyle_Threaded) != 0;
		bool fullscreen = (style & WindowStyle_Fullscreen) != 0;

		m_eventListener = true;
		m_ownsWindow = true;
		m_style = style;

		if (async)
		{
			Mutex mutex;
			ConditionVariable condition;
			m_threadActive = true;

			// On attend que la fenêtre soit créée
			mutex.Lock();
			m_thread = Thread(WindowThread, this, mode, title, style, &mutex, &condition);
			condition.Wait(&mutex);
			mutex.Unlock();
		}
		else
		{
			m_handle = SDL_CreateWindow(title.GetConstBuffer(),
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // x and y unspecified
				mode.width, mode.height,
				ConvertFromNazaraToSDLFlags(style)
			);
		}

		m_style = style;

		if (!m_handle)
		{
			NazaraError(String("Failed to create window: ") + SDL_GetError());
			return false;
		}

		return true;
	}

	bool WindowImpl::Create(WindowHandle handle)
	{
		m_handle = SDL_CreateWindowFrom(handle);

		if (!m_handle)
		{
			NazaraError(String("Invalid handle: ") + SDL_GetError());
			return false;
		}

		unsigned int flags = SDL_GetWindowFlags(m_handle);
		m_style = WindowStyleFlags();
		if (flags & SDL_WINDOW_ALLOW_HIGHDPI)
			m_style |= WindowStyle_AllowHighDPI;
		if (flags & SDL_WINDOW_FULLSCREEN || flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
			m_style |= WindowStyle_Fullscreen;
		if (flags & SDL_WINDOW_INPUT_GRABBED)
			m_style |= WindowStyle_GrabInput;
		if (flags & SDL_WINDOW_BORDERLESS)
			m_style |= WindowStyle_None;
		if (flags & WindowStyle_Resizable)
			m_style |= SDL_WINDOW_RESIZABLE;

		m_ownsWindow = false;
		m_threadActive = false;

		return true;
	}

	void WindowImpl::Destroy()
	{
		if (m_ownsWindow)
		{
			if (m_style & WindowStyle_Threaded)
			{
				if (m_thread.IsJoinable())
				{
					m_threadActive = false;
					//PostMessageW(m_handle, WM_NULL, 0, 0); // Wake up our thread

					m_thread.Join();
				}
			}
			else
			{
				if (m_handle)
					SDL_DestroyWindow(m_handle);
			}
		}
		else
			SetEventListener(false);
	}

	void WindowImpl::EnableKeyRepeat(bool enable)
	{
		m_keyRepeat = enable;
	}

	void WindowImpl::EnableSmoothScrolling(bool enable)
	{
		m_smoothScrolling = enable;
	}

	WindowHandle WindowImpl::GetHandle() const
	{
		return m_handle;
	}

	Vector2i WindowImpl::GetPosition() const
	{
		Vector2i pos;
		SDL_GetWindowPosition(m_handle, &pos.x, &pos.y);
		return pos;
	}

	Vector2ui WindowImpl::GetSize() const
	{
		Vector2i sizes;
		SDL_GetWindowSize(m_handle, &sizes.x, &sizes.y);
		return Vector2ui(sizes);
	}

	WindowStyleFlags WindowImpl::GetStyle() const
	{
		return m_style;
	}

	String WindowImpl::GetTitle() const
	{
		return SDL_GetWindowTitle(m_handle);
	}

	bool WindowImpl::HasFocus() const
	{
		unsigned int flags = SDL_GetWindowFlags(m_handle);
		return (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
	}

	void WindowImpl::IgnoreNextMouseEvent(int mouseX, int mouseY)
	{
		// Small trick ...
	}

	bool WindowImpl::IsMinimized() const
	{
		return SDL_GetWindowFlags(m_handle) & SDL_WINDOW_MINIMIZED;
	}

	bool WindowImpl::IsVisible() const
	{
		return SDL_GetWindowFlags(m_handle) & SDL_WINDOW_SHOWN;
	}

	void WindowImpl::ProcessEvents(bool block)
	{
		if (m_ownsWindow)
		{
			SDL_Event event;
			if (block)
			{
				SDL_WaitEvent(&event);
				Dispatch(event);
			}
			else
			{
				while (SDL_PollEvent(&event))
					Dispatch(event);
			}
		}
	}

	void WindowImpl::SetCursor(const Cursor& cursor)
	{
		SDL_Cursor* toSet = cursor.m_impl->GetCursor();
		if (toSet)
			SDL_SetCursor(toSet);
		else
			SDL_ShowCursor(0);
	}

	void WindowImpl::SetEventListener(bool listener)
	{
		if (m_ownsWindow)
			m_eventListener = listener;
		else if (listener != m_eventListener)
		{
			if (listener)
			{
				//SDL_EventState(?, SDL_ENABLE);
				m_eventListener = true;
			}
			else if (m_eventListener)
			{
				//SDL_EventState(? , SDL_IGNORE);
				m_eventListener = false;
			}
		}
	}

	void WindowImpl::SetFocus()
	{
		SDL_RaiseWindow(m_handle);
	}

	void WindowImpl::SetIcon(const Icon& icon)
	{
		SDL_SetWindowIcon(m_handle, icon.m_impl->GetIcon());
	}

	void WindowImpl::SetMaximumSize(int width, int height)
	{
		SDL_SetWindowMaximumSize(m_handle, width, height);
	}

	void WindowImpl::SetMinimumSize(int width, int height)
	{
		SDL_SetWindowMinimumSize(m_handle, width, height);
	}

	void WindowImpl::SetPosition(int x, int y)
	{
		SDL_SetWindowPosition(m_handle, x, y);
	}

	void WindowImpl::SetSize(unsigned int width, unsigned int height)
	{
		SDL_SetWindowSize(m_handle, width, height);
	}

	void WindowImpl::SetStayOnTop(bool stayOnTop)
	{
		/*if (stayOnTop)
			SetWindowPos(m_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_ASYNCWINDOWPOS);
		else
			SetWindowPos(m_handle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_ASYNCWINDOWPOS);*/
	}

	void WindowImpl::SetTitle(const String& title)
	{
		SDL_SetWindowTitle(m_handle, title.GetConstBuffer());
	}

	void WindowImpl::SetVisible(bool visible)
	{
		if (visible)
			SDL_ShowWindow(m_handle);
		else
			SDL_HideWindow(m_handle);
	}

	bool WindowImpl::Dispatch(const SDL_Event& e)
	{
		if (m_eventListener)
		{
			switch (e.type)
			{
				case SDL_QUIT:
				{
					WindowEvent event;
					event.type = WindowEventType_Quit;
					m_parent->PushEvent(event);

					return true; // Afin que Windows ne ferme pas la fenêtre automatiquement
				}

				case SDL_WINDOWEVENT:
				{
					switch (e.window.event)
					{
						case SDL_WINDOWEVENT_ENTER:
						{
							WindowEvent event;
							event.type = WindowEventType_MouseEntered;
							m_parent->PushEvent(event);

							break;
						}

						case SDL_WINDOWEVENT_FOCUS_GAINED:
						case SDL_WINDOWEVENT_TAKE_FOCUS:
						{
							WindowEvent event;
							event.type = WindowEventType_GainedFocus;
							m_parent->PushEvent(event);

							break;
						}

						case SDL_WINDOWEVENT_FOCUS_LOST:
						{
							WindowEvent event;
							event.type = WindowEventType_LostFocus;
							m_parent->PushEvent(event);

							break;
						}

						case SDL_WINDOWEVENT_MOVED:
						{
							WindowEvent event;
							event.type = WindowEventType_Moved;
							event.position.x = e.window.data1;
							event.position.y = e.window.data2;
							m_parent->PushEvent(event);

							break;
						}

						case SDL_WINDOWEVENT_LEAVE:
						{
							WindowEvent event;
							event.type = WindowEventType_MouseLeft;
							m_parent->PushEvent(event);

							break;
						}

						case SDL_WINDOWEVENT_CLOSE:
						{
							WindowEvent event;
							event.type = WindowEventType_Quit;
							m_parent->PushEvent(event);

							break;
						}

						case SDL_WINDOWEVENT_SIZE_CHANGED:
						case SDL_WINDOWEVENT_RESIZED:
						{
							WindowEvent event;
							event.type = WindowEventType_Resized;
							event.size.width = e.window.data1;
							event.size.height = e.window.data2;
							m_parent->PushEvent(event);

							break;
						}
					}
				}

				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					WindowEvent event;
					event.type = e.type == SDL_KEYDOWN ? WindowEventType_KeyPressed : WindowEventType_KeyReleased;
					event.key.code = ConvertFromSDLKeyToNazara(e.key);
					event.key.alt = e.key.keysym.mod & KMOD_ALT;
					event.key.control = e.key.keysym.mod & KMOD_CTRL;
					event.key.repeated = e.key.repeat;
					event.key.shift = e.key.keysym.mod & KMOD_SHIFT;
					event.key.system = e.key.keysym.mod & KMOD_GUI;
					if (m_keyRepeat || !e.key.repeat)
						m_parent->PushEvent(event);

					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				{
					WindowEvent event;
					event.type = e.type == SDL_MOUSEBUTTONDOWN ? WindowEventType_MouseButtonPressed : WindowEventType_MouseButtonReleased;
					event.type = e.button.clicks > 1 ? WindowEventType_MouseButtonDoubleClicked : event.type;
					event.mouseButton.button = ConvertFromSDLButtonToNazara(e.button);
					event.mouseButton.x = e.button.x;
					event.mouseButton.y = e.button.y;
					m_parent->PushEvent(event);

					break;
				}

				case SDL_MOUSEMOTION:
				{
					WindowEvent event;
					event.type = WindowEventType_MouseMoved;
					event.mouseMove.x = e.motion.x;
					event.mouseMove.y = e.motion.y;
					event.mouseMove.deltaX = e.motion.xrel;
					event.mouseMove.deltaY = e.motion.yrel;
					if (event.mouseMove.deltaX != 0 || event.mouseMove.deltaY != 0)
						m_parent->PushEvent(event);

					break;
				}

				case SDL_MOUSEWHEEL:
				{
					WindowEvent event;
					event.type = WindowEventType_MouseWheelMoved;
					event.mouseWheel.deltaX = e.wheel.x;
					event.mouseWheel.deltaY = e.wheel.y;
					m_parent->PushEvent(event);

					break;
				}

				case SDL_TEXTINPUT:
				{
					static char previousEntry[32]{ '\0' };
					WindowEvent event;
					event.type = WindowEventType_TextEntered;
					std::strcpy(event.text.bytes, e.text.text);
					event.text.repeated = std::strcmp(previousEntry, event.text.bytes) == 0 ? true : false;
					std::strcpy(previousEntry, e.text.text);
					if (m_keyRepeat || !event.text.repeated)
						m_parent->PushEvent(event);

					break;
				}

				default:
					break;
			}
		}

		return false;
	}

	bool WindowImpl::Initialize()
	{
		int res = SDL_Init(SDL_INIT_VIDEO);
		if (res != 0)
			NazaraError(String("Could not initialize SDL: ") + SDL_GetError());
		return res == 0;
	}

	void WindowImpl::Uninitialize()
	{
		SDL_Quit();
	}

	void WindowImpl::WindowThread(WindowImpl* self, const VideoMode& mode, const String& title, WindowStyleFlags style, Mutex* mutex, ConditionVariable* condition)
	{
		self->m_handle = SDL_CreateWindow(title.GetConstBuffer(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // x and y unspecified
			mode.width, mode.height,
			ConvertFromNazaraToSDLFlags(style)
		);

		mutex->Lock();
		condition->Signal();
		mutex->Unlock(); // mutex and condition may be destroyed after this line

		if (!self->m_handle)
			return;

		while (self->m_threadActive)
			self->ProcessEvents(true);

		if (self->m_handle)
			SDL_DestroyWindow(self->m_handle);
	}
}
