// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Platform module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Platform/SDL2/GameControllerImpl.hpp>
#include <Nazara/Core/Error.hpp>
#include <SDL2/SDL.h>
#include <Nazara/Platform/Debug.hpp>

namespace Nz
{
	GameControllerImpl::GameControllerImpl(unsigned int joystickId) :
		m_gameController(nullptr)
	{
		m_gameController = SDL_GameControllerOpen(joystickId);
	}

	GameControllerImpl::~GameControllerImpl()
	{
		if (m_gameController)
			SDL_GameControllerClose(m_gameController);
	}

	short GameControllerImpl::GetAxis(ControllerAxis axis) const
	{
		switch (axis)
		{
			case ControllerAxis_LeftX:
				return SDL_GameControllerGetAxis(m_gameController, SDL_CONTROLLER_AXIS_LEFTX);
			case ControllerAxis_LeftY:
				return SDL_GameControllerGetAxis(m_gameController, SDL_CONTROLLER_AXIS_LEFTY);
			case ControllerAxis_RightX:
				return SDL_GameControllerGetAxis(m_gameController, SDL_CONTROLLER_AXIS_RIGHTX);
			case ControllerAxis_RightY:
				return SDL_GameControllerGetAxis(m_gameController, SDL_CONTROLLER_AXIS_RIGHTY);
			case ControllerAxis_TriggerLeft:
				return SDL_GameControllerGetAxis(m_gameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			case ControllerAxis_TriggerRight:
				return SDL_GameControllerGetAxis(m_gameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
			case ControllerAxis_Invalid:
				return SDL_GameControllerGetAxis(m_gameController, SDL_CONTROLLER_AXIS_INVALID);
			default:
				NazaraError("Unknown controller axis: " + String::Number(axis, 16));
				return SDL_GameControllerGetAxis(m_gameController, SDL_CONTROLLER_AXIS_INVALID);
		};
		
	}

	String GameControllerImpl::GetName() const
	{
		return SDL_GameControllerName(m_gameController);
	}

	bool GameControllerImpl::IsAttached() const
	{
		return SDL_GameControllerGetAttached(m_gameController);
	}

	bool GameControllerImpl::IsButtonPressed(ControllerButton button) const
	{
		switch (button)
		{
			case ControllerButton_A:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_A);
			case ControllerButton_B:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_B);
			case ControllerButton_Back:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_BACK);
			case ControllerButton_DPAD_Down:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
			case ControllerButton_DPAD_Left:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
			case ControllerButton_DPAD_Right:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
			case ControllerButton_DPAD_Up:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_DPAD_UP);
			case ControllerButton_Guide:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_GUIDE);
			case ControllerButton_LeftShoulder:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
			case ControllerButton_LeftStick:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_LEFTSTICK);
			case ControllerButton_RightShoulder:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
			case ControllerButton_RightStick:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
			case ControllerButton_Start:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_START);
			case ControllerButton_X:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_X);
			case ControllerButton_Y:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_Y);
			case ControllerButton_Invalid:
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_INVALID);
			default:
				NazaraError("Unknown controller button: " + String::Number(button, 16));
				return SDL_GameControllerGetButton(m_gameController, SDL_CONTROLLER_BUTTON_INVALID);
		};
	}

	bool GameControllerImpl::IsGameController(unsigned int joystickId)
	{
		return SDL_IsGameController(joystickId);
	}

	bool GameControllerImpl::IsValid() const
	{
		return m_gameController;
	}

	bool GameControllerImpl::Initialize()
	{
		int result = SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
		if (result != 0)
			NazaraError(String("Could not initialize game controller: ") + SDL_GetError());

		return result == 0;
	}

	void GameControllerImpl::Uninitialize()
	{
		SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	}
}
