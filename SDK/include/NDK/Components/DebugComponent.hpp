// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequesites.hpp

#pragma once

#ifndef NDK_COMPONENTS_DEBUGCOMPONENT_HPP
#define NDK_COMPONENTS_DEBUGCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/Enums.hpp>

namespace Ndk
{
	class NDK_API DebugComponent : public Component<DebugComponent>
	{
		public:
			inline DebugComponent(DebugModeFlags debugMode = DebugMode_All);
			~DebugComponent() = default;

			void Disable(DebugModeFlags debugMode);

			void Enable(DebugModeFlags debugMode);
			void Every(Nz::UInt32 milliseconds, std::function<void(EntityHandle, float)> action);

			bool Has(DebugModeFlags debugMode) const;

			void On(std::function<bool(EntityHandle, float)> condition, std::function<void(EntityHandle, float)> action);

			void Reset(DebugModeFlags debugMode);

			void Update(float elapsedTime);

			static ComponentIndex componentIndex;

		private:
			DebugModeFlags m_debugMode;
			std::function<void(EntityHandle, float)> m_action;
			std::function<bool(EntityHandle, float)> m_condition;
			float m_time;
	};
}

#include <NDK/Components/DebugComponent.inl>

#endif // NDK_COMPONENTS_DEBUGCOMPONENT_HPP