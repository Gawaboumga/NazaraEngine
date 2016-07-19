#include "EnemyPainter.hpp"

#include "Core/StateContext.hpp"
#include "Core/Character.hpp"
#include "Core/URL.hpp"

#include "Dimensions.hpp"
#include "SpriteManager.hpp"

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

namespace SMB
{
	EnemyPainter::EnemyPainter(SMB::StateContext& context) :
    	m_context{ context }
	{
	}

	void EnemyPainter::Clear()
	{
		for (auto& pair : m_enemyEntityMap)
			pair.second->Kill();
		m_enemyEntityMap.clear();
	}

	bool EnemyPainter::CreateEnemy(const Enemy& enemy)
	{
		Nz::Vector2f goompaSize = Nz::Vector2f(Dimensions::WorldScale() * enemy.GetDimensions());
		auto goompaSprite = SpriteManager::Get(TypeSprite::Goompa);
		goompaSprite->SetSize(goompaSize);

		auto entity = m_context.world.CreateEntity();
		auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(enemy.GetPosition() * Dimensions::WorldScale());
		auto& graphicsComponent = entity->AddComponent<Ndk::GraphicsComponent>();
		graphicsComponent.Attach(goompaSprite);

		m_enemyAnimationMap.insert(std::make_pair(enemy.GetID(), goompaSprite));
		m_enemyEntityMap.insert(std::make_pair(enemy.GetID(), entity));

		return true;
	}

	void EnemyPainter::Update(const Enemy& enemy, float elapsedTime)
	{
		auto& entity = GetEntityAssociatedWith(enemy);
		if (!enemy.IsAlive())
		{
			if (entity->IsEnabled())
				entity->Enable(false);

			return;
		}
		auto& nodeComponent = entity->GetComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition({ enemy.GetPosition() * Dimensions::WorldScale(), 1.f });

		auto& animatedSprite = GetAnimatedSpriteAssociatedWith(enemy);
		SetAnimation(animatedSprite, enemy);
		animatedSprite->Update(elapsedTime);
	}

	SMB::AnimatedSpriteRef& EnemyPainter::GetAnimatedSpriteAssociatedWith(const Enemy& enemy)
	{
		return m_enemyAnimationMap[enemy.GetID()];
	}

	Ndk::EntityHandle& EnemyPainter::GetEntityAssociatedWith(const Enemy& enemy)
	{
		return m_enemyEntityMap[enemy.GetID()];
	}

	void EnemyPainter::SetAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Enemy& enemy)
	{
		animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Goompa_Walk));
	}
}
