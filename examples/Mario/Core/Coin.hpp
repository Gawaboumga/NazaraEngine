#ifndef __COIN_HPP__
#define __COIN_HPP__

#include "Entity.hpp"

namespace SMB
{

	class Coin : public SMB::Entity
	{
		public:

			Nz::Vector2f GetDimensions() const override;

			void Update(float elapsedTime);

			static Coin MakeSpawn(const Nz::Vector2ui& spawnPosition);

		private:

			Coin();
	};

}

#endif // __COIN_HPP__
