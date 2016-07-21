#ifndef __LEVEL_HPP__
#define __LEVEL_HPP__

#include "Character.hpp"
#include "CommandQueue.hpp"
#include "Coin.hpp"
#include "Enemy.hpp"
#include "Map.hpp"

namespace SMB
{
	class Entity;
	class StateContext;
}

namespace SMB
{

	class Level
	{
		public:
			using Info = unsigned int;

		public:

			Level(SMB::StateContext& context);

			void AddPlayer();

			const std::vector<Character>& GetCharacters() const;
			const std::vector<Coin>& GetCoins() const;
			SMB::CommandQueue& GetCommandQueue();
			const std::vector<Enemy>& GetEnemies() const;
			Info GetInfo() const;
			const SMB::Map& GetMap() const;

			bool HasAlivePlayer() const;
			bool HasPlayerReachedEnd() const;

			bool Load(Info level);

			Nz::Vector2f Move(SMB::Entity& entity, float elapsedTime);

			void Update(float elapsedTime);

		private:

			void ApplyGravity(float elapsedTime);

			void HandleEntitiesCollision(float elapsedTime);

			void Unload();

			SMB::CommandQueue m_commandQueue;
			std::vector<Character> m_characters;
			std::vector<Coin> m_coins;
			std::vector<Enemy> m_enemies;
			SMB::Map m_map;
			SMB::StateContext& m_context;
			Info m_info;
	};

}

#endif // __LEVEL_HPP__
