// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_GAMECONTROLLER_HPP
#define NAZARA_GAMECONTROLLER_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Platform/Joystick.hpp>

namespace Nz
{
	class GameControllerImpl;

	class NAZARA_PLATFORM_API GameController
	{
		friend class Platform;

		public:
			~GameController();

			short GetAxis(ControllerAxis axis) const;
			String GetName() const;

			bool IsAttached() const;
			bool IsButtonPressed(ControllerButton button) const;

			static GameController Get(unsigned int joystickId);
			static GameController Get(const String& name);

			static bool IsGameController(unsigned int joystickId);

		private:
			GameController(unsigned int joystickId);

			bool IsValid() const;

			static bool Initialize();
			static void Uninitialize();

			MovablePtr<GameControllerImpl> m_impl;
	};
}

#endif // NAZARA_GAMECONTROLLER_HPP
