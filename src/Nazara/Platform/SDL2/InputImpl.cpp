// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/SDL2/InputImpl.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Platform/Window.hpp>
#include <SDL2/SDL.h>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	namespace
	{
		SDL_Scancode vKeys[Keyboard::Count] = {
			// Keys
			SDL_SCANCODE_A, // Key::A
			SDL_SCANCODE_B, // Key::B
			SDL_SCANCODE_C, // Key::C
			SDL_SCANCODE_D, // Key::D
			SDL_SCANCODE_E, // Key::E
			SDL_SCANCODE_F, // Key::F
			SDL_SCANCODE_G, // Key::G
			SDL_SCANCODE_H, // Key::H
			SDL_SCANCODE_I, // Key::I
			SDL_SCANCODE_J, // Key::J
			SDL_SCANCODE_K, // Key::K
			SDL_SCANCODE_L, // Key::L
			SDL_SCANCODE_M, // Key::M
			SDL_SCANCODE_N, // Key::N
			SDL_SCANCODE_O, // Key::O
			SDL_SCANCODE_P, // Key::P
			SDL_SCANCODE_Q, // Key::Q
			SDL_SCANCODE_R, // Key::R
			SDL_SCANCODE_S, // Key::S
			SDL_SCANCODE_T, // Key::T
			SDL_SCANCODE_U, // Key::U
			SDL_SCANCODE_V, // Key::V
			SDL_SCANCODE_W, // Key::W
			SDL_SCANCODE_X, // Key::X
			SDL_SCANCODE_Y, // Key::Y
			SDL_SCANCODE_Z, // Key::Z

			// Functional keys
			SDL_SCANCODE_F1,  // Key::F1
			SDL_SCANCODE_F2,  // Key::F2
			SDL_SCANCODE_F3,  // Key::F3
			SDL_SCANCODE_F4,  // Key::F4
			SDL_SCANCODE_F5,  // Key::F5
			SDL_SCANCODE_F6,  // Key::F6
			SDL_SCANCODE_F7,  // Key::F7
			SDL_SCANCODE_F8,  // Key::F8
			SDL_SCANCODE_F9,  // Key::F9
			SDL_SCANCODE_F10, // Key::F10
			SDL_SCANCODE_F11, // Key::F11
			SDL_SCANCODE_F12, // Key::F12
			SDL_SCANCODE_F13, // Key::F13
			SDL_SCANCODE_F14, // Key::F14
			SDL_SCANCODE_F15, // Key::F15

			// Directional arrows
			SDL_SCANCODE_DOWN,  // Key::Down
			SDL_SCANCODE_LEFT,  // Key::Left
			SDL_SCANCODE_RIGHT, // Key::Right
			SDL_SCANCODE_UP,    // Key::Up

			// Numeric pad
			SDL_SCANCODE_KP_PLUS,          // Key::Add
			SDL_SCANCODE_DECIMALSEPARATOR, // Key::Decimal
			SDL_SCANCODE_KP_DIVIDE,        // Key::Divide
			SDL_SCANCODE_KP_MULTIPLY,      // Key::Multiply
			SDL_SCANCODE_KP_0,             // Key::Numpad0
			SDL_SCANCODE_KP_1,             // Key::Numpad1
			SDL_SCANCODE_KP_2,             // Key::Numpad2
			SDL_SCANCODE_KP_3,             // Key::Numpad3
			SDL_SCANCODE_KP_4,             // Key::Numpad4
			SDL_SCANCODE_KP_5,             // Key::Numpad5
			SDL_SCANCODE_KP_6,             // Key::Numpad6
			SDL_SCANCODE_KP_7,             // Key::Numpad7
			SDL_SCANCODE_KP_8,             // Key::Numpad8
			SDL_SCANCODE_KP_9,             // Key::Numpad9
			SDL_SCANCODE_KP_MINUS,         // Key::Subtract

			// Misc
			SDL_SCANCODE_BACKSLASH,    // Key::Backslash
			SDL_SCANCODE_BACKSPACE,    // Key::Backspace
			SDL_SCANCODE_CLEAR,        // Key::Clear
			SDL_SCANCODE_COMMA,        // Key::Comma,
			SDL_SCANCODE_MINUS,        // Key::Dash
			SDL_SCANCODE_DELETE,       // Key::Delete
			SDL_SCANCODE_END,          // Key::End
			SDL_SCANCODE_KP_EQUALS,    // Key::Equal
			SDL_SCANCODE_ESCAPE,       // Key::Escape
			SDL_SCANCODE_HOME,         // Key::Home
			SDL_SCANCODE_INSERT,       // Key::Insert
			SDL_SCANCODE_LALT,         // Key::LAlt
			SDL_SCANCODE_LEFTBRACKET,  // Key::LBracket
			SDL_SCANCODE_LCTRL,        // Key::LControl
			SDL_SCANCODE_LSHIFT,       // Key::LShift
			SDL_SCANCODE_LGUI,         // Key::LSystem
			SDL_SCANCODE_0,            // Key::Num0
			SDL_SCANCODE_1,            // Key::Num1
			SDL_SCANCODE_2,            // Key::Num2
			SDL_SCANCODE_3,            // Key::Num3
			SDL_SCANCODE_4,            // Key::Num4
			SDL_SCANCODE_5,            // Key::Num5
			SDL_SCANCODE_6,            // Key::Num6
			SDL_SCANCODE_7,            // Key::Num7
			SDL_SCANCODE_8,            // Key::Num8
			SDL_SCANCODE_9,            // Key::Num9
			SDL_SCANCODE_PAGEDOWN,     // Key::PageDown
			SDL_SCANCODE_PAGEUP,       // Key::PageUp
			SDL_SCANCODE_PAUSE,        // Key::Pause
			SDL_SCANCODE_KP_PERIOD,    // Key::Period
			SDL_SCANCODE_PRINTSCREEN,  // Key::PrintScreen
			SDL_SCANCODE_APOSTROPHE,   // Key::Quote
			SDL_SCANCODE_RALT,         // Key::RAlt
			SDL_SCANCODE_RIGHTBRACKET, // Key::RBracket
			SDL_SCANCODE_RCTRL,        // Key::RControl
			SDL_SCANCODE_RETURN,       // Key::Return
			SDL_SCANCODE_RSHIFT,       // Key::RShift
			SDL_SCANCODE_RGUI,         // Key::RSystem
			SDL_SCANCODE_SEMICOLON,    // Key::Semicolon
			SDL_SCANCODE_SLASH,        // Key::Slash
			SDL_SCANCODE_SPACE,        // Key::Space
			SDL_SCANCODE_TAB,          // Key::Tab
			SDL_SCANCODE_GRAVE,        // Key::Tilde

			// Navigator keys
			SDL_SCANCODE_AC_BACK,      // Key::Browser_Back
			SDL_SCANCODE_AC_BOOKMARKS, // Key::Browser_Favorites
			SDL_SCANCODE_AC_FORWARD,   // Key::Browser_Forward
			SDL_SCANCODE_AC_HOME,      // Key::Browser_Home
			SDL_SCANCODE_AC_REFRESH,   // Key::Browser_Refresh
			SDL_SCANCODE_AC_SEARCH,    // Key::Browser_Search
			SDL_SCANCODE_AC_STOP,      // Key::Browser_Stop

			// Audio keys
			SDL_SCANCODE_AUDIONEXT, // Key::Media_Next,
			SDL_SCANCODE_AUDIOPLAY, // Key::Media_Play,
			SDL_SCANCODE_AUDIOPREV, // Key::Media_Previous,
			SDL_SCANCODE_AUDIOSTOP, // Key::Media_Stop,

			// Volume keys
			SDL_SCANCODE_VOLUMEDOWN, // Key::Volume_Down
			SDL_SCANCODE_AUDIOMUTE,  // Key::Volume_Mute
			SDL_SCANCODE_VOLUMEUP,   // Key::Volume_Up

			// Lock keys
			SDL_SCANCODE_CAPSLOCK,     // Key::CapsLock
			SDL_SCANCODE_NUMLOCKCLEAR, // Key::NumLock
			SDL_SCANCODE_SCROLLLOCK    // Key::ScrollLock
		};

		SDL_Scancode ConvertFromNazaraToSDLScanCode(Keyboard::Key key)
		{
			return vKeys[key];
		}
	}

	String EventImpl::GetKeyName(Keyboard::Key key)
	{
		return SDL_GetKeyName(SDL_GetKeyFromScancode(ConvertFromNazaraToSDLScanCode(key)));
	}

	Vector2i EventImpl::GetMousePosition()
	{
		Vector2i pos;
		SDL_GetGlobalMouseState(&pos.x, &pos.y);
		return pos;
	}

	Vector2i EventImpl::GetMousePosition(const Window& relativeTo)
	{
		Vector2i pos;
		SDL_GetMouseState(&pos.x, &pos.y);
		return pos;
	}

	bool EventImpl::IsKeyPressed(Keyboard::Key key)
	{
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		SDL_Scancode scanCode = ConvertFromNazaraToSDLScanCode(key);
		return state[scanCode];
	}

	bool EventImpl::IsMouseButtonPressed(Mouse::Button button)
	{
		static int vButtons[Mouse::Max+1] = {
			SDL_BUTTON_LEFT,   // Button::Left
			SDL_BUTTON_MIDDLE, // Button::Middle
			SDL_BUTTON_RIGHT,  // Button::Right
			SDL_BUTTON_X1,     // Button::XButton1
			SDL_BUTTON_X2      // Button::XButton2
		};

		return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(vButtons[button]);
	}

	void EventImpl::SetMousePosition(int x, int y)
	{
		SDL_WarpMouseGlobal(x, y);
	}

	void EventImpl::SetMousePosition(int x, int y, const Window& relativeTo)
	{
		SDL_WarpMouseInWindow(static_cast<SDL_Window*>(relativeTo.GetHandle()), x, y);
	}
}
