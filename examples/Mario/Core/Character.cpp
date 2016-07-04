#include "Character.hpp"

#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/VelocityComponent.hpp>
#include <NDK/World.hpp>

#include "Level.hpp"

namespace SMB
{

	void Character::Accelerate(const Nz::Vector2f& acceleration, float elapsedTime)
	{
		m_velocity += acceleration * elapsedTime;
	}

	Nz::Vector2f Character::GetDimensions() const
	{
		return { 0.5f, 0.5f }; // Tile = 32 * 32, and little mario 16*16
	}

	int Character::GetID() const
	{
		return m_id;
	}

	Nz::Vector2f Character::GetNextPosition(float elapsedTime) const
	{
		return m_position + m_velocity * elapsedTime;
	}

	Nz::Vector2f Character::GetPosition() const
	{
		return m_position;
	}

	Nz::Vector2f Character::GetVelocity() const
	{
		return m_velocity;
	}

	bool Character::IsTouchingGround() const
	{
		return m_onGround;
	}

	void Character::Move(const Nz::Vector2f& translation)
	{
		m_position += translation;
	}

	void Character::TouchGround(bool onGround)
	{
		m_onGround = onGround;
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
		m_position{ 0.f, 0.f },
		m_velocity{ 0.f, 0.f },
		m_id{ s_id },
		m_onGround{ false }
	{
		++s_id;
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

	int Character::s_id = 0;
}
