#ifndef __LEVELPAINTER_HPP__
#define __LEVELPAINTER_HPP__

#include <Nazara/Graphics/TileMap.hpp>

#include <NDK/Entity.hpp>

namespace Nz
{
	class String;
}

namespace SMB
{
	class Map;
	class StateContext;
	class Tile;
}

namespace SMB
{

	class LevelPainter
	{
		public:

			LevelPainter(SMB::StateContext& context);

			void Clear();
			bool CreateTiles(const SMB::Map& map, Nz::TextureRef texture, const Nz::Vector2ui& tileSize);

			Nz::TextureRef LoadSpriteSheet(const Nz::String& filePath);

		private:

			Nz::Rectui ConvertToTileCoords(const SMB::Tile& tile, const Nz::Vector2ui& numberOfImageTiles, const Nz::Vector2ui& tileSize);

			Nz::String GetResourceName(const Nz::String& filePath);

			SMB::StateContext& m_context;
			Ndk::EntityHandle m_mapEntity;
			Nz::TileMapRef m_tileMap;
	};

}

#endif // __LEVELPAINTER_HPP__
