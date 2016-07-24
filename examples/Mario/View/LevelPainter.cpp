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
		m_mapEntity->Kill();
	}

	bool LevelPainter::CreateTiles(const SMB::Map& map, Nz::TextureRef texture, const Nz::Vector2ui& tileSize)
	{
		auto tilesScale = Dimensions::WorldScale();
		m_tileMap = Nz::TileMap::New(Nz::Vector2ui{ map.GetWidth(), map.GetHeight() }, tilesScale);
		auto material = Nz::MaterialLibrary::Get("Basic2D");
		material->SetDiffuseMap(texture);
		m_tileMap->SetMaterial(0, material);

		m_mapEntity = m_context.world.CreateEntity();
		auto& nodeComponent = m_mapEntity->AddComponent<Ndk::NodeComponent>();
		auto& graphicsComponent = m_mapEntity->AddComponent<Ndk::GraphicsComponent>();

		auto numberOfImageTiles = Nz::Vector2ui{ texture->GetWidth(), texture->GetHeight() } / tileSize;
		for (auto i = 0; i < map.GetWidth(); ++i)
		{
			for (auto j = 0; j < map.GetHeight(); ++j)
			{
				auto coords = ConvertToTileCoords(map(i, j), numberOfImageTiles, tileSize);
				m_tileMap->EnableTile({ i, j }, coords);
			}
		}

		nodeComponent.SetPosition(Nz::Vector3f::Zero());
		graphicsComponent.Attach(m_tileMap);

		return true;
	}

	Nz::TextureRef LevelPainter::LoadSpriteSheet(const Nz::String& filePath)
	{
		auto resourceName = GetResourceName(filePath);
		if (!Nz::TextureLibrary::Has(resourceName))
		{
			auto texture = Nz::Texture::New();
			if (!texture->LoadFromFile(filePath))
			{
				NazaraError("Error while loading assets");
				return {};
			}

			Nz::TextureLibrary::Register(resourceName, texture);
		}

		return Nz::TextureLibrary::Get(resourceName);
	}

	Nz::Rectui LevelPainter::ConvertToTileCoords(const SMB::Tile& tile, const Nz::Vector2ui& numberOfImageTiles, const Nz::Vector2ui& tileSize)
	{
		auto index = tile.GetIndex() - 1; // Due to count from 0
		auto x = (index / numberOfImageTiles.x) * tileSize.x;
		auto y = (index % numberOfImageTiles.x) * tileSize.y;

		return { y, x, tileSize.x, tileSize.y };
	}

	Nz::String LevelPainter::GetResourceName(const Nz::String& filePath)
	{
		auto position = filePath.FindLast('/');
		auto resourceName = filePath.SubStringFrom('/', position - 1);
		resourceName = resourceName.SubStringTo('.');

		return resourceName;
	}

}
