// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_GAMECONTROLLERIMPL_HPP
#define NAZARA_GAMECONTROLLERIMPL_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Platform/Enums.hpp>
#include <Nazara/Platform/Joystick.hpp>

struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

namespace Nz
{
	class GameControllerImpl
	{
		public:
			GameControllerImpl(unsigned int joystickId);
			~GameControllerImpl();

			short GetAxis(ControllerAxis axis) const;
			String GetName() const;

			bool IsAttached() const;
			bool IsButtonPressed(ControllerButton button) const;
			bool IsValid() const;

			static bool IsGameController(unsigned int joystickId);

			static bool Initialize();
			static void Uninitialize();

		private:
			SDL_GameController* m_gameController;
	};
}

#endif // NAZARA_GAMECONTROLLERIMPL_HPP
