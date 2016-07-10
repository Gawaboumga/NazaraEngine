#include "Entity.hpp"

namespace SMB
{
	Entity::Entity() :
		m_position{ 0.f, 0.f },
		m_velocity{ 0.f, 0.f },
		m_id{ s_id++ },
		m_alive{ true },
		m_onGround{ false }
	{
	}

	Entity::Entity(int x, int y) :
		m_position(Nz::Vector2f(x, y)),
		m_velocity{ 0.f, 0.f },
		m_id{ s_id++ },
		m_alive{ true },
		m_onGround{ false }
	{
	}

	Entity::~Entity() = default;

	void Entity::Accelerate(const Nz::Vector2f& acceleration, float elapsedTime)
	{
		m_velocity += acceleration * elapsedTime;
		m_velocity.x = Nz::Clamp(m_velocity.x, -5.f, 5.f);
	}

	int Entity::GetID() const
	{
		return m_id;
	}

	Nz::Vector2f Entity::GetNextPosition(float elapsedTime) const
	{
		return m_position + m_velocity * elapsedTime;
	}

	Nz::Vector2f Entity::GetPosition() const
	{
		return m_position;
	}

	Nz::Vector2f Entity::GetVelocity() const
	{
		return m_velocity;
	}

	bool Entity::IsAlive() const
	{
		return m_alive;
	}

	bool Entity::IsTouchingGround() const
	{
		return m_onGround;
	}

	void Entity::ResetVelocity()
	{
		m_velocity = { 0.f, 0.f };
	}

	void Entity::TakeDamage()
	{
		m_alive = false;
	}

	void Entity::TouchGround(bool onGround)
	{
		m_onGround = onGround;
	}

	void Entity::Move(const Nz::Vector2f& translation)
	{
		m_position += translation;
	}

	int Entity::s_id = 0;
}
