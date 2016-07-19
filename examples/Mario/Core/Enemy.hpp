#ifndef __ENNEMY_HPP__
#define __ENNEMY_HPP__

#include "EnemyType.hpp"
#include "Entity.hpp"

namespace SMB
{
	class Enemy : public SMB::Entity
	{
		public:

			Enemy(int x, int y, SMB::EnemyType type);

			Nz::Vector2f GetDimensions() const override;

			int GetScore() const;

			void Update(float elapsedTime);

			static EnemyType FromString(const Nz::String& type);

		private:

			SMB::EnemyType m_type;
	};
}

#endif // __ENNEMY_HPP__
