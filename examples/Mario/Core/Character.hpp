#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include "Entity.hpp"

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
