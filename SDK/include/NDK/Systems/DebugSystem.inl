// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequesites.hpp

namespace Ndk
{
	/*!
	* \brief Registers a callback to debug
	*
	* \param action Action to perform when component is met
	*/

	template<typename Component>
	inline void DebugSystem::On(std::function<void(EntityHandle, float)> action)
	{
		m_registeredActions[GetComponentIndex<Component>()] = action;
	}
}