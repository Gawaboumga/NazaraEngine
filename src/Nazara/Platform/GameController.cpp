// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/GameController.hpp>
#include <Nazara/Platform/SDL2/GameControllerImpl.hpp>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	GameController::~GameController()
	{
		delete m_impl;
		m_impl = nullptr;
	}

	short GameController::GetAxis(ControllerAxis axis) const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Gamecontroller invalid");
			return 0;
		}
		#endif
		return m_impl->GetAxis(axis);
	}

	String GameController::GetName() const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Gamecontroller invalid");
			return false;
		}
		#endif
		return m_impl->GetName();
	}

	bool GameController::IsAttached() const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Gamecontroller invalid");
			return false;
		}
		#endif
		return m_impl->IsAttached();
	}

	bool GameController::IsButtonPressed(ControllerButton button) const
	{
		#if NAZARA_PLATFORM_SAFE
		if (!IsValid())
		{
			NazaraError("Gamecontroller invalid");
			return false;
		}
		#endif
		return m_impl->IsButtonPressed(button);
	}

	GameController GameController::Get(unsigned int joystickId)
	{
		return GameController(joystickId);
	}

	GameController GameController::Get(const String& name)
	{
		unsigned int joystickId = Joystick::Get(name).GetId();
		return GameController(joystickId);
	}

	bool GameController::IsGameController(unsigned int joystickId)
	{
		return GameControllerImpl::IsGameController(joystickId);
	}

	GameController::GameController(unsigned int joystickId)
	{
		m_impl = new GameControllerImpl(joystickId);
	}

	bool GameController::IsValid() const
	{
		return m_impl->IsValid();
	}

	bool GameController::Initialize()
	{
		return GameControllerImpl::Initialize();
	}

	void GameController::Uninitialize()
	{
		return GameControllerImpl::Uninitialize();
	}
}
