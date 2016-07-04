#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <Nazara/Math/Rect.hpp>

#include "Tile.hpp"

#include "Matrix/Matrix.hpp"

namespace SMB
{
	class Character;
}

namespace SMB
{
	class Map
	{
		public:

			using dimension_t = int;
			using id = Tile;

			Map() = default;
			Map(const Matrix<Nz::String, 2>& map);

			dimension_t GetHeight() const;
			Nz::Vector2f GetPossibleMove(const SMB::Character& character, float elapsedTime) const;
			dimension_t GetWidth() const;

			bool IsTouchingGround(const SMB::Character& character) const;

			Map& operator=(Map&& other) = default;
			id& operator()(dimension_t i, dimension_t j);
			const id& operator()(dimension_t i, dimension_t j) const;

		private:

			const SMB::Tile& GetTile(const Nz::Vector2f& position) const;

			bool IsColliding(const Nz::Rectf& collisionBox) const;

			Matrix<id, 2> m_matrix;
	};
}

#endif // __MAP_HPP__
