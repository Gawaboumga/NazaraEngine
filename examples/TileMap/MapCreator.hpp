#ifndef MAP_CREATOR_HPP
#define MAP_CREATOR_HPP

#include <Nazara/Graphics/TileMap.hpp>

#include <TMX/Visitor.hpp>

class MapCreator : public tmx::Visitor
{
	public:
		void Enter(const tmx::Map& map) override;
		void Enter(const tmx::TileLayer& layer) override;
		void Enter(const tmx::TileSet& tileSet) override;

		std::vector<Nz::TileMapRef> tileMaps;

	private:
		Nz::Rectui GetTextureCoords(unsigned int tileId) const;

		struct TileSetInfo
		{
			Nz::MaterialRef material;
			Nz::Vector2ui tileSize;
			unsigned tileCount;
			unsigned int numberColumns;
		};

		std::vector<TileSetInfo> m_tileSetInfos;
		Nz::Vector2f m_tileSize;
};

#endif // MAP_CREATOR_HPP
