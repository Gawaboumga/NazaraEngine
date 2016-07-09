#include "EnemyPainter.hpp"

#include <Nazara/Graphics/Sprite.hpp>

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

#include "../Core/StateContext.hpp"
#include "../Core/Enemy.hpp"
#include "../Core/URL.hpp"
#include "Dimensions.hpp"

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
		Nz::String goompa{ "enemies" };
		Nz::Vector2ui goompaSize = Nz::Vector2ui(Dimensions::GetTiles() * enemy.GetDimensions());

		if (!Nz::ImageLibrary::Has(goompa))
		{
			auto image = Nz::Image::New();
			if (!image->LoadFromFile(URL::GetEnemiesSpriteSheet()))
			{
				NazaraError("Error while loading assets");
				return false;
			}

			Nz::ImageLibrary::Register(goompa, image);
		}

		if (!Nz::SpriteLibrary::Has(goompa))
		{
			auto image = Nz::ImageLibrary::Get(goompa);
			auto goompaImage = Nz::Image::New(image->GetType(), image->GetFormat(), goompaSize.x, goompaSize.y);
			Nz::Boxui box {
				0, 4, 0, // Hard coded position of goompa
				goompaSize.x, goompaSize.y, 1
			};
			goompaImage->Copy(*image.Get(), box, Nz::Vector3ui::Zero());

			auto goompaTexture = Nz::Texture::New(*goompaImage.Get());
			Nz::TextureLibrary::Register(goompa, goompaTexture);

			// No idea why I need to do that
			Nz::MaterialRef material = Nz::MaterialLibrary::Get("Default");
			material->SetFaceFilling(Nz::FaceFilling_Fill);
			material->SetDiffuseMap(Nz::TextureLibrary::Get(goompa));
			Nz::SpriteRef goompaSprite = Nz::Sprite::New(material);
			Nz::SpriteLibrary::Register(goompa, goompaSprite);
		}

		auto goompaSprite = Nz::SpriteLibrary::Get(goompa);

		auto entity = m_context.world.CreateEntity();
		auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(enemy.GetPosition() * Dimensions::GetTiles());
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
		nodeComponent.SetPosition(enemy.GetPosition() * Dimensions::GetTiles());
	}

	Ndk::EntityHandle& EnemyPainter::GetEntityAssociatedWith(const Enemy& enemy)
	{
		return m_enemyEntityMap[enemy.GetID()];
	}
}
