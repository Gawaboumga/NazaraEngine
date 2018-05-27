// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_JOYSTICKIMPL_HPP
#define NAZARA_JOYSTICKIMPL_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Platform/Enums.hpp>

struct _SDL_Joystick;
typedef struct _SDL_Joystick SDL_Joystick;

namespace Nz
{
	class JoystickImpl
	{
		friend class GameControllerImpl;

		public:
			JoystickImpl(unsigned int joystickId);
			~JoystickImpl();

			short GetAxis(unsigned int axis) const;
			unsigned int GetAxisCount() const;
			Vector2i GetBall(unsigned int ball) const;
			unsigned int GetBallCount() const;
			unsigned int GetButtonCount() const;
			JoystickHat GetHat(unsigned int hat) const;
			unsigned int GetHatCount() const;
			unsigned int GetId() const;
			String GetName() const;

			bool IsAttached() const;
			bool IsButtonPressed(unsigned int button) const;
			bool IsValid() const;

			static unsigned int GetJoystickCount();

			static bool Initialize();
			static void Uninitialize();

		private:
			SDL_Joystick* m_joystick;
	};
}

#endif // NAZARA_JOYSTICKIMPL_HPP
