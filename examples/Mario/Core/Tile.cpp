#include "Tile.hpp"

#include <NDK/World.hpp>
#include <NDK/Components/NodeComponent.hpp>

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
		return GetIndex() == 49;
	}

	Nz::String Tile::ToString() const
	{
		return m_type;
	}
}
