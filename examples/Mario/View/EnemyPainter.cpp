#include "EnemyPainter.hpp"

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

#include "../Core/StateContext.hpp"
#include "../Core/Enemy.hpp"
#include "../Core/URL.hpp"
#include "Dimensions.hpp"
#include "SpriteManager.hpp"

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
		/*auto goompaSprite = SpriteManager::Get(SpriteType::Goompa);
		goompaSprite->SetSize(goompaSize);*/
		Nz::MaterialRef material = Nz::MaterialLibrary::Get("Default");
		material->SetFaceFilling(Nz::FaceFilling_Fill);
		material->SetDiffuseMap(Nz::TextureLibrary::Get("Goompa"));
		Nz::SpriteRef goompaSprite = Nz::Sprite::New(material);
		goompaSprite->SetSize(goompaSize);

		auto entity = m_context.world.CreateEntity();
		auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(enemy.GetPosition() * Dimensions::WorldScale());
		auto& graphicsComponent = entity->AddComponent<Ndk::GraphicsComponent>();
		graphicsComponent.Attach(goompaSprite);

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
		nodeComponent.SetPosition(enemy.GetPosition() * Dimensions::WorldScale());
	}

	Ndk::EntityHandle& EnemyPainter::GetEntityAssociatedWith(const Enemy& enemy)
	{
		return m_enemyEntityMap[enemy.GetID()];
	}
}
