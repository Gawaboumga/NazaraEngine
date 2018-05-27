// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_ENUMS_PLATFORM_HPP
#define NAZARA_ENUMS_PLATFORM_HPP

#include <Nazara/Core/Flags.hpp>

namespace Nz
{
	enum JoystickHat
	{
		JoystickHat_Centered,
		JoystickHat_Down,
		JoystickHat_Left,
		JoystickHat_LeftDown,
		JoystickHat_LeftUp,
		JoystickHat_Right,
		JoystickHat_RightDown,
		JoystickHat_RightUp,
		JoystickHat_Up,

		JoystickHat_Max = JoystickHat_Up
	};

	enum SystemCursor
	{
		SystemCursor_Crosshair,
		SystemCursor_Default,
		SystemCursor_Hand,
		SystemCursor_None,
		SystemCursor_Pointer,
		SystemCursor_Progress,
		SystemCursor_Resize,
		SystemCursor_ResizeNESW,
		SystemCursor_ResizeNS,
		SystemCursor_ResizeNWSE,
		SystemCursor_ResizeWE,
		SystemCursor_Text,
		SystemCursor_Wait,

		SystemCursor_Max = SystemCursor_Wait
	};

	enum WindowEventType
	{
		WindowEventType_GainedFocus,
		WindowEventType_LostFocus,
		WindowEventType_JoystickAxis,
		WindowEventType_JoystickBall,
		WindowEventType_JoystickButton,
		WindowEventType_JoystickDevice,
		WindowEventType_JoystickHat,
		WindowEventType_KeyPressed,
		WindowEventType_KeyReleased,
		WindowEventType_MouseButtonDoubleClicked,
		WindowEventType_MouseButtonPressed,
		WindowEventType_MouseButtonReleased,
		WindowEventType_MouseEntered,
		WindowEventType_MouseLeft,
		WindowEventType_MouseMoved,
		WindowEventType_MouseWheelMoved,
		WindowEventType_Moved,
		WindowEventType_Quit,
		WindowEventType_Resized,
		WindowEventType_TextEntered,

		WindowEventType_Max = WindowEventType_TextEntered
	};

	enum WindowStyle
	{
		WindowStyle_AllowHighDPI, ///< Allows high dpi handling.
		WindowStyle_Fullscreen,   ///< At the window creation, the OS tries to set it in fullscreen.
		WindowStyle_GrabInput,    ///< At the window creation, the window grabs the input focus.
		WindowStyle_None,         ///< Window has no border nor titlebar.
		WindowStyle_OpenGL,       ///< Required for OpenGL context.
		WindowStyle_Resizable,    ///< Allows the window to be resized by dragging its corners or by a button of the titlebar.
		WindowStyle_Threaded,     ///< Runs the window into a thread, allowing the application to keep updating while resizing/dragging the window.

		WindowStyle_Max = WindowStyle_Threaded
	};

	template<>
	struct EnumAsFlags<WindowStyle>
	{
		static constexpr WindowStyle max = WindowStyle_Max;
	};

	using WindowStyleFlags = Flags<WindowStyle>;

	constexpr WindowStyleFlags WindowStyle_Default = WindowStyle_Resizable;
}

#endif // NAZARA_ENUMS_PLATFORM_HPP
