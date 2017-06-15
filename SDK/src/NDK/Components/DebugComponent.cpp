// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequesites.hpp

#include <NDK/Components/DebugComponent.hpp>

namespace Ndk
{
	/*!
	* \ingroup NDK
	* \class Ndk::DebugComponent
	* \brief NDK class that represents a way to show debug information
	*/

	/*!
	* \brief Disables the specified flags
	*
	* \param debugMode Flags to disable
	*/

	void DebugComponent::Disable(DebugModeFlags debugMode)
	{
		m_debugMode &= ~debugMode;
	}

	/*!
	* \brief Enables the specified flags
	*
	* \param debugMode Flags to enable
	*/

	void DebugComponent::Enable(DebugModeFlags debugMode)
	{
		m_debugMode |= debugMode;
	}

	/*!
	* \brief Performs the action every X milliseconds
	*
	* \param milliseconds Number of milliseconds before next action
	* \param action Action to perform
	*
	* \see On
	*/

	void DebugComponent::Every(Nz::UInt32 milliseconds, std::function<void(Ndk::EntityHandle, float)> action)
	{
		m_condition = [this, milliseconds](const Ndk::EntityHandle& /* entity */, float elapsedTime) {
			m_time += elapsedTime * 1000.f;
			if (m_time >= milliseconds)
			{
				m_time -= milliseconds;
				return true;
			}
			return false;
		};
		m_action = action;
	}

	/*!
	* \brief Checks whether the specified flags are set
	* \return True if the specified flags are set
	*
	* \param debugMode Flags to test
	*/

	bool DebugComponent::Has(DebugModeFlags debugMode) const
	{
		return static_cast<bool>(m_debugMode & debugMode);
	}

	/*!
	* \brief Performs the action when condition is met
	*
	* \param condition Condition to meet for the action
	* \param action Action to perform
	*
	* \see Every
	*/

	void DebugComponent::On(std::function<bool(Ndk::EntityHandle, float)> condition, std::function<void(Ndk::EntityHandle, float)> action)
	{
		m_condition = condition;
		m_action = action;
	}

	/*!
	* \brief Resets the debug flags
	*
	* \param debugMode Flags to reset
	*/

	void DebugComponent::Reset(DebugModeFlags debugMode)
	{
		m_debugMode = debugMode;
	}

	/*!
	* \brief Updates the component (and perform the action registered if required)
	*
	* \param elapsedTime Delta time used for the update
	*/

	void DebugComponent::Update(float elapsedTime)
	{
		if (m_condition && m_action)
		{
			const EntityHandle& entity = GetEntity();
			if (m_condition(entity, elapsedTime))
				m_action(entity, elapsedTime);
		}
	}

	ComponentIndex DebugComponent::componentIndex;
}
