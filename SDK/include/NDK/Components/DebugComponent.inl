// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequesites.hpp

namespace Ndk
{
	/*!
	* \brief Constructs an DebugComponent object by default
	*
	* \param debugMode Flags to set
	*/

	inline DebugComponent::DebugComponent(DebugModeFlags debugMode) :
	m_debugMode(debugMode),
	m_action(nullptr),
	m_condition(nullptr),
	m_time(0.f)
	{
	}
}
