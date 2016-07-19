#ifndef __KEYBINDING_HPP__
#define __KEYBINDING_HPP__

#include <Nazara/Utility/Keyboard.hpp>

#include <map>

namespace SMB
{
	enum class PlayerAction
	{
		MoveLeft,
		MoveRight,
		MoveDown,
		Jump,
	};

	class KeyBinding
	{
		public:

			using Action = PlayerAction;

			KeyBinding();

			bool GetAction(Nz::Keyboard::Key key, Action& out) const;
			std::vector<Action> GetRealtimeActions() const;

		private:

			bool IsRealtimeAction(PlayerAction action) const;

			std::map<Nz::Keyboard::Key, Action> m_keyMap;
	};
}

#endif // __KEYBINDING_HPP__
