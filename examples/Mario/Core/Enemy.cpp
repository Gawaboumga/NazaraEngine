#include "Enemy.hpp"

namespace SMB
{

	Enemy::Enemy(int x, int y, SMB::EnemyType type) :
		Entity(x, y),
		m_type{ type }
	{
	}

	Nz::Vector2f Enemy::GetDimensions() const
	{
		return { 0.5f, 0.5f }; // Tile = 32 * 32, and little goompa 16*16
	}

	int Enemy::GetScore() const
	{
		return 200;
	}

	void Enemy::Update(float elapsedTime)
	{
	}

	EnemyType Enemy::FromString(const Nz::String& type)
	{
		if (type == "goompa")
			return EnemyType::Goompa;
		else
			return EnemyType::Unknown;
	}

}
