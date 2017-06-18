// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequesites.hpp

#include <NDK/Systems/DebugSystem.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <NDK/Components/DebugComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <Nazara/Renderer/DebugDrawer.hpp>
#include <Nazara/Renderer/Renderer.hpp>

#include <iostream>

namespace Ndk
{
	/*!
	* \ingroup NDK
	* \class Ndk::DebugSystem
	* \brief NDK class that represents the debug system
	*/

	/*!
	* \brief Constructs an DebugSystem object by default
	*/

	DebugSystem::DebugSystem()
	{
		Requires<DebugComponent>();
		SetUpdateOrder(200); //< Update last
		SetUpdateRate(0.f);
	}

	/*!
	* \brief Operation to perform when system is updated
	*
	* \param elapsedTime Delta time used for the update
	*/

	void DebugSystem::OnUpdate(float elapsedTime)
	{
		for (const EntityHandle& entity : GetEntities())
		{
			Ndk::DebugComponent& debugComponent = entity->GetComponent<DebugComponent>();
			debugComponent.Update(elapsedTime);

			#ifndef NDK_SERVER
			if (entity->HasComponent<GraphicsComponent>())
			{
				const GraphicsComponent& graphicsComponent = entity->GetComponent<GraphicsComponent>();
				Nz::DebugDrawer::SetPrimaryColor(Nz::Color::Red);
				Nz::DebugDrawer::Draw(graphicsComponent.GetBoundingVolume());
			}

			if (entity->HasComponent<NodeComponent>())
			{
				const NodeComponent& nodeComponent = entity->GetComponent<NodeComponent>();
				Nz::DebugDrawer::SetPrimaryColor(Nz::Color::Green);
				Nz::DebugDrawer::Draw(nodeComponent.GetPosition());
			}

			if (entity->HasComponent<PhysicsComponent2D>())
			{
				const PhysicsComponent2D& physicsComponent2D = entity->GetComponent<PhysicsComponent2D>();
				Nz::DebugDrawer::SetPrimaryColor(Nz::Color::Magenta);
				Nz::DebugDrawer::Draw(physicsComponent2D.GetAABB());
			}
			else if (entity->HasComponent<CollisionComponent2D>())
			{
				const CollisionComponent2D& collisionComponent2D = entity->GetComponent<CollisionComponent2D>();
				Nz::DebugDrawer::SetPrimaryColor(Nz::Color::Yellow);
				Nz::DebugDrawer::Draw(collisionComponent2D.GetStaticBody()->GetAABB());
			}
			#endif

			for (std::pair<const ComponentIndex, std::function<void(EntityHandle, float)>>& registeredAction : m_registeredActions)
			{
				if (entity->HasComponent(registeredAction.first))
					registeredAction.second(entity, elapsedTime);

			}
		}
	}

	SystemIndex DebugSystem::systemIndex;
}
