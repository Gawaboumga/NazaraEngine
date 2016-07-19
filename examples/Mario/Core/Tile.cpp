#include "Tile.hpp"

namespace SMB
{
	Tile::Tile(const Nz::String& type) :
		m_type{ type }
	{
	}

	int Tile::GetIndex() const
	{
		return std::stoi(m_type);
	}

	bool Tile::IsBlock() const
	{
		static const std::vector<int> m_solidTiles{{ 49, 250, 132, 42, 43, 61, 62, 3 }};
		auto it = std::find(m_solidTiles.begin(), m_solidTiles.end(), GetIndex());
		return it != m_solidTiles.end();
	}

	Nz::String Tile::ToString() const
	{
		return m_type;
	}
}
