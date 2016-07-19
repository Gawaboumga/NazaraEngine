#include "LevelPainter.hpp"

#include "Core/Map.hpp"
#include "Core/StateContext.hpp"
#include "Core/Tile.hpp"

#include "Dimensions.hpp"

#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Renderer/Texture.hpp>

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

namespace SMB
{
	LevelPainter::LevelPainter(SMB::StateContext& context) :
    	m_context{ context }
	{
	}

	void LevelPainter::Clear()
	{
		for (auto& tile : m_entities)
			tile->Kill();
		m_entities.clear();
	}

	bool LevelPainter::CreateTiles(const SMB::Map& map)
	{
		m_entities = std::move(m_context.world.CreateEntities(map.GetWidth() * map.GetHeight()));

		auto tilesScale = Dimensions::WorldScale();
		for (auto i = 0; i < map.GetWidth(); ++i)
		{
			for (auto j = 0; j < map.GetHeight(); ++j)
			{
				const Tile& tile = map(i, j);
				auto& entity = m_entities[i * map.GetHeight() + j];
				auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
				auto& graphicsComponent = entity->AddComponent<Ndk::GraphicsComponent>();

				auto sprite = Nz::SpriteLibrary::Get(tile.ToString());
				graphicsComponent.Attach(sprite);

				nodeComponent.SetPosition({ i * tilesScale.x, j * tilesScale.y , 0.f });

			}
		}

		return true;
	}

	Nz::String LevelPainter::GetResourceName(const Nz::String& filePath)
	{
		auto position = filePath.FindLast('/');
		auto resourceName = filePath.SubStringFrom('/', position - 1);
		resourceName = resourceName.SubStringTo('.');

		return resourceName;
	}

	bool LevelPainter::LoadSpriteSheet(const Nz::String& filePath)
	{
		auto resourceName = GetResourceName(filePath);
		if (!Nz::TextureLibrary::Has(resourceName))
		{
			auto texture = Nz::Texture::New();
			if (!texture->LoadFromFile(filePath))
			{
				NazaraError("Error while loading assets");
				return false;
			}

			Nz::TextureLibrary::Register(resourceName, texture);
		}

		return true;
	}

	bool LevelPainter::LoadSprites(const SMB::Map& map, const Nz::Vector2ui& tileSize, const Nz::String& resourceName)
	{
		auto texture = Nz::TextureLibrary::Get(resourceName);

		auto numberOfImageTiles = Nz::Vector2ui{ texture->GetWidth(), texture->GetHeight() } / tileSize;
		for (auto i = 0; i < map.GetWidth(); ++i)
		{
			for (auto j = 0; j < map.GetHeight(); ++j)
			{
				if (!Nz::SpriteLibrary::Has(map(i, j).ToString()))
				{
					auto sprite = Nz::Sprite::New(texture.Get());
					Nz::Vector2ui tilePosition = ConvertToImagePosition(map(i, j), numberOfImageTiles, tileSize);
					sprite->SetTextureRect({
						tilePosition.x, tilePosition.y,
						tileSize.x, tileSize.y, // dimensions
					});
					sprite->SetSize(Dimensions::WorldScale());
					Nz::SpriteLibrary::Register(map(i, j).ToString(), sprite);
				}
			}
		}

		return true;
	}

	Nz::Vector2ui LevelPainter::ConvertToImagePosition(const SMB::Tile& tile, const Nz::Vector2ui& numberOfImageTiles, const Nz::Vector2ui& tileSize)
	{
		auto index = tile.GetIndex() - 1; // Due to count from 0
		auto x = (index / numberOfImageTiles.x) * tileSize.x;
		auto y = (index % numberOfImageTiles.x) * tileSize.y;

		return { y, x };
	}
}
