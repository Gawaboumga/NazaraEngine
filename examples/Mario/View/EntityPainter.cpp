#include "EntityPainter.hpp"

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
	EntityPainter::EntityPainter(SMB::StateContext& context) :
    	m_context{ context }
	{
	}

	void EntityPainter::Clear()
	{
		for (auto& pair : m_entityWorldEntityMap)
			pair.second->Kill();
		m_entityWorldEntityMap.clear();
	}

	bool EntityPainter::CreateCharacter(const Character& character)
	{
		Nz::Vector2f marioSize = Nz::Vector2f(Dimensions::WorldScale() * character.GetDimensions());
		auto marioSprite = SpriteManager::Get(TypeSprite::Mario);
		marioSprite->SetSize(marioSize);

		CreateEntity(character, marioSprite);

		return true;
	}

	bool EntityPainter::CreateCoin(const Coin& coin)
	{
		Nz::Vector2f coinSize = Nz::Vector2f(Dimensions::WorldScale() * coin.GetDimensions());
		auto coinSprite = SpriteManager::Get(TypeSprite::Coin);
		coinSprite->SetSize(coinSize);

		CreateEntity(coin, coinSprite);

		return true;
	}

	bool EntityPainter::CreateEnemy(const Enemy& enemy)
	{
		Nz::Vector2f enemySize = Nz::Vector2f(Dimensions::WorldScale() * enemy.GetDimensions());
		auto enemySprite = SpriteManager::Get(TypeSprite::Goompa);
		enemySprite->SetSize(enemySize);

		CreateEntity(enemy, enemySprite);

		return true;
	}

	void EntityPainter::Update(const Character& character, float elapsedTime)
	{
		auto animatedSprite = UpdateEntity(character, elapsedTime);
		if (animatedSprite == nullptr)
			return;
		SetCharacterAnimation(animatedSprite, character);
		animatedSprite->Update(elapsedTime);
	}

	void EntityPainter::Update(const Coin& coin, float elapsedTime)
	{
		auto animatedSprite = UpdateEntity(coin, elapsedTime);
		if (animatedSprite == nullptr)
			return;
		SetCoinAnimation(animatedSprite, coin);
		animatedSprite->Update(elapsedTime);
	}

	void EntityPainter::Update(const Enemy& enemy, float elapsedTime)
	{
		auto animatedSprite = UpdateEntity(enemy, elapsedTime);
		if (animatedSprite == nullptr)
			return;
		SetEnemyAnimation(animatedSprite, enemy);
		animatedSprite->Update(elapsedTime);
	}

	void EntityPainter::CreateEntity(const Entity& entity, SMB::AnimatedSpriteRef sprite)
	{
		auto worldEntity = m_context.world.CreateEntity();
		auto& nodeComponent = worldEntity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(entity.GetPosition() * Dimensions::WorldScale());
		auto& graphicsComponent = worldEntity->AddComponent<Ndk::GraphicsComponent>();
		graphicsComponent.Attach(sprite);

		m_entityAnimationMap.insert(std::make_pair(entity.GetID(), sprite));
		m_entityWorldEntityMap.insert(std::make_pair(entity.GetID(), worldEntity));
	}

	SMB::AnimatedSpriteRef EntityPainter::GetAnimatedSpriteAssociatedWith(const Entity& entity)
	{
		return m_entityAnimationMap[entity.GetID()];
	}

	Ndk::EntityHandle& EntityPainter::GetWorldEntityAssociatedWith(const Entity& entity)
	{
		return m_entityWorldEntityMap[entity.GetID()];
	}

	void EntityPainter::SetCharacterAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Character& character)
	{
		if (character.IsTouchingGround())
		{
			if (character.GetVelocity().x > 0.0001f)
				animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Mario_Run_Right));
			else if (character.GetVelocity().x < -0.0001f)
				animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Mario_Run_Left));
			else
				animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Mario_Still));
		}
		else
		{
			if (character.GetVelocity().x > 0.0001f)
				animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Mario_Jump_Right));
			else if (character.GetVelocity().x < -0.0001f)
				animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Mario_Jump_Left));
			else
				animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Mario_Still));
		}
	}

	void EntityPainter::SetCoinAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Coin& coin)
	{
		animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Coin_Still));
	}

	void EntityPainter::SetEnemyAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Enemy& enemy)
	{
		animatedSprite->SetAnimation(SpriteManager::Get(TypeAnimation::Goompa_Walk));
	}

	SMB::AnimatedSpriteRef EntityPainter::UpdateEntity(const Entity& entity, float elapsedTime)
	{
		auto& worldEntity = GetWorldEntityAssociatedWith(entity);
		if (!entity.IsAlive())
		{
			if (worldEntity->IsEnabled())
				worldEntity->Enable(false);

			return {};
		}
		auto& nodeComponent = worldEntity->GetComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition({ entity.GetPosition() * Dimensions::WorldScale(), 1.f });

		return GetAnimatedSpriteAssociatedWith(entity);
	}
}
