#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <Nazara/Math/Rect.hpp>
#include <Nazara/Math/Vector2.hpp>

#include <NDK/Entity.hpp>

namespace Ndk
{
	class World;
}

namespace SMB
{
	class Level;
}

namespace SMB
{
	class Character
	{
		public:

			void Accelerate(const Nz::Vector2f& acceleration, float elapsedTime);

			Nz::Vector2f GetDimensions() const;
			int GetID() const;
			Nz::Vector2f GetNextPosition(float elapsedTime) const;
			Nz::Vector2f GetPosition() const;
			Nz::Vector2f GetVelocity() const;

			bool IsTouchingGround() const;

			void Move(const Nz::Vector2f& translation);

			void TouchGround(bool onGround);

			void Update(float elapsedTime);

			static Character MakeSpawn(const Nz::Vector2ui& spawnPosition);

		private:

			Character();

			void SetPosition(const Nz::Vector2f& translation);

			Nz::Vector2f m_position;
			Nz::Vector2f m_velocity;
			int m_id;
			bool m_onGround;

			static int s_id;
	};
}

#endif // __CHARACTER_HPP__
