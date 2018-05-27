// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/SDL2/JoystickImpl.hpp>
#include <Nazara/Core/Error.hpp>
#include <SDL2/SDL.h>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	JoystickImpl::JoystickImpl(unsigned int joystickId) :
	m_joystick(nullptr)
	{
		m_joystick = SDL_JoystickOpen(joystickId);
	}

	JoystickImpl::~JoystickImpl()
	{
		if (m_joystick)
			SDL_JoystickClose(m_joystick);
	}

	short JoystickImpl::GetAxis(unsigned int axis) const
	{
		return SDL_JoystickGetAxis(m_joystick, axis);
	}

	unsigned int JoystickImpl::GetAxisCount() const
	{
		return SDL_JoystickNumAxes(m_joystick);
	}

	Vector2i JoystickImpl::GetBall(unsigned int ball) const
	{
		Vector2i result = Vector2i::Zero();
		SDL_JoystickGetBall(m_joystick, ball, &result.x, &result.y);
		return result;
	}

	unsigned int JoystickImpl::GetBallCount() const
	{
		return SDL_JoystickNumBalls(m_joystick);
	}

	unsigned int JoystickImpl::GetButtonCount() const
	{
		return SDL_JoystickNumButtons(m_joystick);
	}

	JoystickHat JoystickImpl::GetHat(unsigned int hat) const
	{
		unsigned char direction = SDL_JoystickGetHat(m_joystick, hat);
		switch (direction)
		{
			case SDL_HAT_CENTERED:
				return JoystickHat_Centered;
			case SDL_HAT_UP:
				return JoystickHat_Up;
			case SDL_HAT_RIGHT:
				return JoystickHat_Right;
			case SDL_HAT_DOWN:
				return JoystickHat_Down;
			case SDL_HAT_LEFT:
				return JoystickHat_Left;
			case SDL_HAT_RIGHTUP:
				return JoystickHat_RightUp;
			case SDL_HAT_RIGHTDOWN:
				return JoystickHat_RightDown;
			case SDL_HAT_LEFTUP:
				return JoystickHat_LeftUp;
			case SDL_HAT_LEFTDOWN:
				return JoystickHat_LeftDown;
			default:
				NazaraError("Unknown hat: " + String::Number(direction, 16));
				return JoystickHat_Centered;
		}
	}

	unsigned int JoystickImpl::GetHatCount() const
	{
		return SDL_JoystickNumHats(m_joystick);
	}

	unsigned int JoystickImpl::GetId() const
	{
		return SDL_JoystickInstanceID(m_joystick);
	}

	String JoystickImpl::GetName() const
	{
		return SDL_JoystickName(m_joystick);
	}

	bool JoystickImpl::IsAttached() const
	{
		return SDL_JoystickGetAttached(m_joystick);
	}

	bool JoystickImpl::IsButtonPressed(unsigned int button) const
	{
		return SDL_JoystickGetButton(m_joystick, button);
	}

	bool JoystickImpl::IsValid() const
	{
		return m_joystick && GetId() != -1;
	}

	unsigned int JoystickImpl::GetJoystickCount()
	{
		int result = SDL_NumJoysticks();
		if (result < 0)
		{
			NazaraError(String("Could not retrieve joysticks: ") + SDL_GetError());
			return 0;
		}
		return result;
	}

	bool JoystickImpl::Initialize()
	{
		int result = SDL_InitSubSystem(SDL_INIT_JOYSTICK);
		if (result != 0)
			NazaraError(String("Could not initialize joystick: ") + SDL_GetError());

		return result == 0;
	}

	void JoystickImpl::Uninitialize()
	{
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
	}
}
