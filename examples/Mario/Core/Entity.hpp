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
			virtual Nz::Vector2f GetNextPosition(float elapsedTime) const;
			virtual Nz::Vector2f GetPosition() const;
			virtual Nz::Vector2f GetVelocity() const;

			bool IsAlive() const;
			bool IsTouchingGround() const;

			void ResetVelocity();

			void TakeDamage();
			void TouchGround(bool onGround);

			virtual void Move(const Nz::Vector2f& translation);

		protected:

			Nz::Vector2f m_position;
			Nz::Vector2f m_velocity;
			int m_id;
			bool m_alive;
			bool m_onGround;

			static int s_id;
	};
}

#endif // __ENTITY_HPP__
