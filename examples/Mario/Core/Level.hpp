#ifndef __LEVEL_HPP__
#define __LEVEL_HPP__

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Math/Vector3.hpp>

#include "Character.hpp"
#include "CommandQueue.hpp"
#include "LevelData.hpp"

namespace SMB
{
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
			SMB::CommandQueue& GetCommandQueue();
			Info GetInfo() const;
			const SMB::Map& GetMap() const;

			bool HasAlivePlayer() const;
			bool HasPlayerReachedEnd() const;

			bool Load(Info level);

			Nz::Vector2f Move(SMB::Character& character, float elapsedTime);

			void Update(float elapsedTime);

		private:

			void ApplyGravity(float elapsedTime);

			void Unload();

			SMB::CommandQueue m_commandQueue;
			std::vector<Character> m_characters;
			SMB::Map m_map;
			SMB::StateContext& m_context;
			Info m_info;
	};
}

#endif // __LEVEL_HPP__
