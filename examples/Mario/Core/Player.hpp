#ifndef __Player_HPP__
#define __Player_HPP__

#include "Command.hpp"
#include "KeyBinding.hpp"
#include "Level.hpp"

#include <map>

namespace SMB
{
	class CommandQueue;
	class KeyBinding;
}

namespace SMB
{
	class Player
	{
		public:

			using Action = PlayerAction;

			Player(const KeyBinding* binding);

			void AddCoin();
			void AddDeath();
			void AddScore(int score);

			void FinishLevel();

			int GetCoins() const;
			Level::Info GetLevel() const;
			int GetNumberOfLives() const;
			int GetScore() const;

			void HandleInput(CommandQueue& commandQueue);
			bool HasChanged();

		private:

			void InitializeActions();

			Level::Info m_level;
			int m_coins;
			int m_numberOfLives;
			int m_score;
			bool m_hasChanged;

			const KeyBinding* m_keyBinding;
			std::map<KeyBinding::Action, Command> m_actionBinding;

	};
}

#endif // __Player_HPP__
