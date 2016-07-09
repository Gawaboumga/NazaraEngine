#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <Nazara/Math/Rect.hpp>
#include <Nazara/Math/Vector2.hpp>

#include <NDK/Entity.hpp>

#include "Entity.hpp"

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
	class Character : public SMB::Entity
	{
		public:

			Nz::Vector2f GetDimensions() const override;

			void Update(float elapsedTime);

			static Character MakeSpawn(const Nz::Vector2ui& spawnPosition);

		private:

			Character();

			void SetPosition(const Nz::Vector2f& translation);
	};
}

#endif // __CHARACTER_HPP__
