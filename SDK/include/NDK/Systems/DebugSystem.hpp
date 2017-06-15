// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequesites.hpp

#pragma once

#ifndef NDK_SYSTEMS_DEBUGSYSTEM_HPP
#define NDK_SYSTEMS_DEBUGSYSTEM_HPP

#include <NDK/System.hpp>

#include <map>

namespace Ndk
{
	class NDK_API DebugSystem : public System<DebugSystem>
	{
		public:
			DebugSystem();
			~DebugSystem() = default;

			template<typename Component>
			void On(std::function<void(EntityHandle, float)> action);

			static SystemIndex systemIndex;

		private:
			void OnUpdate(float elapsedTime) override;

			std::map<ComponentIndex, std::function<void(EntityHandle, float)>> m_registeredActions;
	};
}

#include <NDK/Systems/DebugSystem.inl>

#endif // NDK_SYSTEMS_DEBUGSYSTEM_HPP
