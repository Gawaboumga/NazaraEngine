#ifndef __TILE_HPP__
#define __TILE_HPP__

#include <Nazara/Core/String.hpp>

namespace SMB
{

	class Tile
	{
		public:

			Tile() = default;
			Tile(const Nz::String& type);
			Tile(Tile&& other) = default;

			int GetIndex() const;

			bool IsBlock() const;

			Nz::String ToString() const;

			Tile& operator=(Tile&& other) = default;

		private:

			Nz::String m_type;
	};

}

#endif // __TILE_HPP__
