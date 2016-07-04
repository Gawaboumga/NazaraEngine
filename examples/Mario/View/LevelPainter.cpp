#include "LevelPainter.hpp"

#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Utility/Image.hpp>

#include <NDK/Components/CollisionComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

#include "../Core/Map.hpp"
#include "../Core/StateContext.hpp"
#include "../Core/Tile.hpp"

#include "Dimensions.hpp"

namespace SMB
{
	LevelPainter::LevelPainter(SMB::StateContext& context) :
    	m_context{ context }
	{
	}

	Nz::Vector3ui LevelPainter::ConvertToImagePosition(const SMB::Tile& tile, const Nz::Vector2ui& numberOfImageTiles, const Nz::Vector2ui& tileSize)
	{
		auto index = tile.GetIndex() - 1; // Due to count from 0
		auto x = (index / numberOfImageTiles.x) * tileSize.x;
		auto y = (index % numberOfImageTiles.x) * tileSize.y;

		return { y, x, 0 };
	}

	bool LevelPainter::CreateTiles(const SMB::Map& map)
	{
		auto entities = m_context.world.CreateEntities(map.GetWidth() * map.GetHeight());

		auto tilesScale = Dimensions::GetTiles();
		for (auto i = 0; i < map.GetWidth(); ++i)
		{
			for (auto j = 0; j < map.GetHeight(); ++j)
			{
				const Tile& tile = map(i, j);
				auto& entity = entities[i * map.GetHeight() + j];
				auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
				auto& graphicsComponent = entity->AddComponent<Ndk::GraphicsComponent>();

				auto texture = Nz::TextureLibrary::Get(tile.ToString());

				if (!Nz::SpriteLibrary::Has(tile.ToString()))
				{
					auto sprite = Nz::Sprite::New(texture.Get());
					sprite->SetSize(tilesScale);
					Nz::SpriteLibrary::Register(tile.ToString(), sprite);
					graphicsComponent.Attach(sprite);
				}
				else
				{
					auto sprite = Nz::SpriteLibrary::Get(tile.ToString());
					graphicsComponent.Attach(sprite);
				}

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
		if (!Nz::ImageLibrary::Has(resourceName))
		{
			auto image = Nz::Image::New();
			if (!image->LoadFromFile(filePath))
			{
				NazaraError("Error while loading assets");
				return false;
			}

			Nz::ImageLibrary::Register(resourceName, image);
		}

		return true;
	}

	bool LevelPainter::LoadSprites(const SMB::Map& map, const Nz::Vector2ui& tileSize, const Nz::String& resourceName)
	{
		auto image = Nz::ImageLibrary::Get(resourceName);

		auto numberOfImageTiles = Nz::Vector2ui{ image->GetWidth(), image->GetHeight() } / tileSize;

		for (auto i = 0; i < map.GetWidth(); ++i)
		{
			for (auto j = 0; j < map.GetHeight(); ++j)
			{
				if (!Nz::TextureLibrary::Has(map(i, j).ToString()))
				{
					auto tile = Nz::Image::New(image->GetType(), image->GetFormat(), tileSize.x, tileSize.y);
					Nz::Vector3ui tilePosition = ConvertToImagePosition(map(i, j), numberOfImageTiles, tileSize);
					Nz::Boxui tileDimensions {
						tilePosition.x, tilePosition.y, tilePosition.z,
						tileSize.x, tileSize.y, 1 // dimensions
					};
					tile->Copy(*image.Get(), tileDimensions, Nz::Vector3ui::Zero());
					auto textureTile = Nz::Texture::New(*tile.Get());
					Nz::TextureLibrary::Register(map(i, j).ToString(), textureTile);
				}
			}
		}

		return true;
	}
}
