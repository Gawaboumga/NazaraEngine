// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

// Interface inspired by the SFML of Laurent Gomila (and its team)

#pragma once

#ifndef NAZARA_EVENT_HPP
#define NAZARA_EVENT_HPP

#include <Nazara/Platform/Enums.hpp>
#include <Nazara/Platform/Keyboard.hpp>
#include <Nazara/Platform/Mouse.hpp>

namespace Nz
{
	struct WindowEvent
	{
		struct GameControllerEvent
		{
			unsigned int controllerId;
		};

		// Used by:
		// -WindowEventType_GameControllerAxis
		struct GameControllerAxisEvent : GameControllerEvent
		{
			ControllerAxis axis;
			short value;
		};

		// Used by:
		// -WindowEventType_GameControllerButton
		struct GameControllerButtonEvent : GameControllerEvent
		{
			ControllerButton button;
			bool pressed;
		};

		// Used by:
		// -WindowEventType_GameControllerDevice
		struct GameControllerDeviceEvent : GameControllerEvent
		{
			ControllerDevice status;
		};

		struct JoystickEvent
		{
			unsigned int joystickId;
		};

		// Used by:
		// -WindowEventType_JoystickAxis
		struct JoystickAxisEvent : JoystickEvent
		{
			unsigned char axis;
			short value;
		};

		// Used by:
		// -WindowEventType_JoystickBall
		struct JoystickBallEvent : JoystickEvent
		{
			unsigned char ball;
			short deltaX;
			short deltaY;
		};

		// Used by:
		// -WindowEventType_JoystickButton
		struct JoystickButtonEvent : JoystickEvent
		{
			unsigned char button;
			bool pressed;
		};

		// Used by:
		// -WindowEventType_JoystickDevice
		struct JoystickDeviceEvent : JoystickEvent
		{
			bool enabled;
		};

		// Used by:
		// -WindowEventType_JoystickHat
		struct JoystickHatEvent : JoystickEvent
		{
			unsigned char hat;
			JoystickHat direction;
		};

		// Used by:
		// -WindowEventType_KeyPressed
		// -WindowEventType_KeyReleased
		struct KeyEvent
		{
			Keyboard::Key code;
			bool alt;
			bool control;
			bool repeated;
			bool shift;
			bool system;
		};

		// Used by:
		// -WindowEventType_MouseButtonDoubleClicked
		// -WindowEventType_MouseButtonPressed
		struct MouseButtonEvent
		{
			Mouse::Button button;
			unsigned int x;
			unsigned int y;
		};

		// Used by:
		// -WindowEventType_MouseMoved
		struct MouseMoveEvent
		{
			int deltaX;
			int deltaY;
			unsigned int x;
			unsigned int y;
		};

		// Used by:
		// -WindowEventType_MouseWheelMoved
		struct MouseWheelEvent
		{
			float deltaX;
			float deltaY;
		};

		// Used by:
		// -WindowEventType_Moved
		struct PositionEvent
		{
			int x;
			int y;
		};

		// Used by:
		// -WindowEventType_Resized
		struct SizeEvent
		{
			unsigned int height;
			unsigned int width;
		};

		// Used by:
		// -WindowEventType_TextEntered
		struct TextEvent
		{
			bool repeated;
			char bytes[32];
		};

		WindowEventType type;

		union
		{
			// Used by:
			// -WindowEventType_GameControllerAxis
			GameControllerAxisEvent caxis;

			// Used by:
			// -WindowEventType_GameControllerButton
			GameControllerButtonEvent cbutton;

			// Used by:
			// -WindowEventType_GameControllerDevice
			GameControllerDeviceEvent cdevice;

			// Used by:
			// -WindowEventType_JoystickAxis
			JoystickAxisEvent jaxis;

			// Used by:
			// -WindowEventType_JoystickBall
			JoystickBallEvent jball;

			// Used by:
			// -WindowEventType_JoystickButton
			JoystickButtonEvent jbutton;

			// Used by:
			// -WindowEventType_JoystickDevice
			JoystickDeviceEvent jdevice;

			// Used by:
			// -WindowEventType_JoystickHat
			JoystickHatEvent jhat;

			// Used by:
			// -WindowEventType_KeyPressed
			// -WindowEventType_KeyReleased
			KeyEvent key;

			// Used by:
			// -WindowEventType_MouseButtonDoubleClicked
			// -WindowEventType_MouseButtonPressed
			MouseButtonEvent mouseButton;

			// Used by:
			// -WindowEventType_MouseMoved
			MouseMoveEvent mouseMove;

			// Used by:
			// -WindowEventType_MouseWheelMoved
			MouseWheelEvent mouseWheel;

			// Used by:
			// -WindowEventType_Moved
			PositionEvent position;

			// Used by:
			// -WindowEventType_Resized
			SizeEvent size;

			// Used by:
			// -WindowEventType_TextEntered
			TextEvent text;
		};
	};
}

#endif // NAZARA_EVENT_HPP
