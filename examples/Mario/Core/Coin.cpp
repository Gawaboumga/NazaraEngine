#include "Coin.hpp"

namespace SMB
{

	Nz::Vector2f Coin::GetDimensions() const
	{
		return { 0.5f, 0.5f }; // Tile = 32 * 32, and little mario 16*16
	}

	void Coin::Update(float elapsedTime)
	{
	}

	Coin::Coin() :
		Entity()
	{
	}

	Coin Coin::MakeSpawn(const Nz::Vector2ui& spawnPosition)
	{
		Coin coin{};
		coin.SetPosition(Nz::Vector2f{ spawnPosition });
		return coin;
	}

	void Coin::SetPosition(const Nz::Vector2f& position)
	{
		m_position = position;
	}
}
