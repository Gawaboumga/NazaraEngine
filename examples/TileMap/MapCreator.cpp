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
	for (const tmx::TileLayer::Tile& tile : layer.tileIds)
	{
		if (tile.gid != 0)
		{
			Nz::Rectui textureCoords = GetTextureCoords(tile.GetId() - 1, tile.DiagonallyFlipped(), tile.HorizontallyFlipped(), tile.VerticallyFlipped());
			tileMap->EnableTile(localPosition, textureCoords);
		}

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
	Nz::MaterialRef material = Nz::Material::New("Translucent2D");
	material->SetDiffuseMap(tileSet.texture);
	tileSetInfo.material = material;
	tileSetInfo.tileSize = tileSet.tileSize;
	tileSetInfo.firstgid = tileSet.firstgid;
	tileSetInfo.margin = tileSet.margin;
	tileSetInfo.numberColumns = tileSet.columns;
	tileSetInfo.spacing = tileSet.spacing;
	tileSetInfo.tileCount = tileSet.tileCount;
	m_tileSetInfos.push_back(tileSetInfo);
}

Nz::Rectui MapCreator::GetTextureCoords(unsigned int tileId, bool diagonallyFlipped, bool horizontalFlipped, bool verticalFlipped) const
{
	auto it = std::find_if(m_tileSetInfos.rbegin(), m_tileSetInfos.rend(), [&](const TileSetInfo& tileSetInfo) {
		return tileSetInfo.firstgid - 1 <= tileId;
	});

	if (it == m_tileSetInfos.rend())
	{
		NazaraError("Invald tile id");
		return Nz::Rectui::Zero();
	}

	const TileSetInfo& tileSetInfo = *it;
	if (tileSetInfo.tileCount != 0 && tileId > tileSetInfo.tileCount)
		NazaraError("Invald tile id");

	const Nz::TextureRef& textureRef = tileSetInfo.material->GetDiffuseMap();
	Nz::Vector2ui textureSize = Nz::Vector2ui(textureRef->GetSize());
	Nz::Vector2ui tileSize = tileSetInfo.tileSize;

	Nz::Vector2ui numberOfTiles = textureSize / tileSize;
	unsigned int line = tileId / numberOfTiles.x;
	unsigned int column = tileId % numberOfTiles.x;

	unsigned int margin = tileSetInfo.margin;
	Nz::Vector2ui margins(margin, margin);

	Nz::Vector2ui lowerCorner = Nz::Vector2ui(column, line) * (tileSize + margins) + margins;
	Nz::Vector2ui lowerRight = lowerCorner + tileSize - 2u * margins;
	Nz::Rectui rect(lowerCorner, lowerCorner + tileSize - 2u * margins);
	return rect;
}
