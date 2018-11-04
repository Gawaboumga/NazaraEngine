#include "MapCreator.hpp"

#include <TMX/Map.hpp>

void MapCreator::Enter(const tmx::Map& map)
{
	m_tileSize = Nz::Vector2f(map.tileSize);
}

void MapCreator::Enter(const tmx::TileLayer& layer)
{
	Nz::Vector2ui size = layer.size;
	tileMaps.emplace_back(Nz::TileMap::New(size, m_tileSize));
	Nz::TileMapRef& tileMap = tileMaps.back();
	tileMap->SetMaterial(0, m_tileSetInfos[0].material);

	Nz::Vector2ui localPosition(0u, 0u);
	for (unsigned int tileId : layer.tileIds)
	{
		tileMap->EnableTile(localPosition, GetTextureCoords(tileId - 1));

		++localPosition.x;
		if (localPosition.x >= size.x)
		{
			localPosition.x = 0;
			++localPosition.y;
		}
	}
}

void MapCreator::Enter(const tmx::TileSet& tileSet)
{
	TileSetInfo tileSetInfo;
	Nz::MaterialRef material = Nz::Material::New();
	material->SetDiffuseMap(tileSet.texture);
	tileSetInfo.material = material;
	tileSetInfo.tileSize = tileSet.tileSize;
	tileSetInfo.tileCount = tileSet.tileCount;
	tileSetInfo.numberColumns = tileSet.columns;
	m_tileSetInfos.push_back(tileSetInfo);
}

Nz::Rectui MapCreator::GetTextureCoords(unsigned int tileId) const
{
	const TileSetInfo& tileSetInfo = m_tileSetInfos[0];
	if (tileSetInfo.tileCount != 0 && tileId > tileSetInfo.tileCount)
		NazaraError("Invald tile id");

	const Nz::TextureRef& textureRef = tileSetInfo.material->GetDiffuseMap();
	Nz::Vector2ui textureSize = Nz::Vector2ui(textureRef->GetSize());
	Nz::Vector2ui tileSize = tileSetInfo.tileSize;

	Nz::Vector2ui numberOfTiles = textureSize / tileSize;
	unsigned int line = tileId / numberOfTiles.x;
	unsigned int column = tileId % numberOfTiles.x;

	Nz::Vector2ui lowerCorner = Nz::Vector2ui(column, line) * tileSize;
	Nz::Rectui rect(lowerCorner, lowerCorner + tileSize);
	return rect;
}
