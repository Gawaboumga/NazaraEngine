#include "Character.hpp"

#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/VelocityComponent.hpp>
#include <NDK/World.hpp>

#include "Level.hpp"

namespace SMB
{

	Nz::Vector2f Character::GetDimensions() const
	{
		return { 0.5f, 0.5f }; // Tile = 32 * 32, and little mario 16*16
	}

	void Character::Update(float elapsedTime)
	{
		// Ground friction
		if (m_onGround)
		{
			m_velocity.y = 0.f;

			if (m_velocity.x > 0.f)
				m_velocity.x = std::max(m_velocity.x - 0.1f, 0.f);
			else if (m_velocity.x < 0.f)
				m_velocity.x = std::min(m_velocity.x + 0.1f, 0.f);
		}
	}

	Character::Character() :
		Entity()
	{
	}

	Character Character::MakeSpawn(const Nz::Vector2ui& spawnPosition)
	{
		Character character{};
		character.SetPosition(Nz::Vector2f{ spawnPosition });
		return character;
	}

	void Character::SetPosition(const Nz::Vector2f& position)
	{
		m_position = position;
	}
}
