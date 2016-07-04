#ifndef __LEVELPAINTER_HPP__
#define __LEVELPAINTER_HPP__


#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Math/Vector3.hpp>

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

			Nz::Vector3ui ConvertToImagePosition(const SMB::Tile& tile, const Nz::Vector2ui& numberOfImageTiles, const Nz::Vector2ui& tileSize);
			bool CreateTiles(const SMB::Map& map);

			Nz::String GetResourceName(const Nz::String& filePath);

			bool LoadSpriteSheet(const Nz::String& filePath);
			bool LoadSprites(const SMB::Map& map, const Nz::Vector2ui& tileSize, const Nz::String& resourceName);

		private:

			SMB::StateContext& m_context;
	};
}

#endif // __LEVELPAINTER_HPP__
