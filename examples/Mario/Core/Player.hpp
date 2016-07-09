#ifndef __Player_HPP__
#define __Player_HPP__

#include "Command.hpp"
#include "KeyBinding.hpp"
#include "Level.hpp"

#include <map>

namespace Nz
{
	class WindowEvent;
}

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

			void AddDeath();

			void FinishLevel();

			Level::Info GetLevel() const;
			int GetNumberOfLives() const;

			void HandleInput(CommandQueue& commandQueue);

		private:

			void InitializeActions();

			Level::Info m_level;
			int m_numberOfLives;

			const KeyBinding* m_keyBinding;
			std::map<KeyBinding::Action, Command> m_actionBinding;

	};
}

#endif // __Player_HPP__
