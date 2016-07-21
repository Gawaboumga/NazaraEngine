#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <Nazara/Math/Vector2.hpp>

namespace SMB
{

	class Entity
	{
		public:

			Entity();
			Entity(int x, int y);
			virtual ~Entity();

			void Accelerate(const Nz::Vector2f& acceleration, float elapsedTime);

			virtual Nz::Vector2f GetDimensions() const = 0;
			int GetID() const;
			Nz::Vector2f GetNextPosition(float elapsedTime) const;
			Nz::Vector2f GetPosition() const;
			Nz::Vector2f GetVelocity() const;

			bool IsAlive() const;
			bool IsTouchingGround() const;

			void ResetVelocity();

			void TakeDamage();
			void TouchGround(bool onGround);

			void Move(const Nz::Vector2f& translation);

		protected:

			void SetPosition(const Nz::Vector2f& position);

			Nz::Vector2f m_position;
			Nz::Vector2f m_velocity;
			int m_id;
			bool m_alive;
			bool m_onGround;

			static int s_id;
	};

}

#endif // __ENTITY_HPP__
