#include "KeyBinding.hpp"

namespace SMB
{

	KeyBinding::KeyBinding()
		: m_keyMap{}
	{
		m_keyMap[Nz::Keyboard::Left]	= PlayerAction::MoveLeft;
		m_keyMap[Nz::Keyboard::Right]	= PlayerAction::MoveRight;
		m_keyMap[Nz::Keyboard::Down]	= PlayerAction::MoveDown;
		m_keyMap[Nz::Keyboard::Up]		= PlayerAction::Jump;
	}

	bool KeyBinding::GetAction(Nz::Keyboard::Key key, Action& out) const
	{
		auto found { m_keyMap.find(key) };
		if (found == m_keyMap.end())
		{
			return false;
		}
		else
		{
			out = found->second;
			return true;
		}
	}

	std::vector<KeyBinding::Action> KeyBinding::GetRealtimeActions() const
	{
		std::vector<Action> actions;

		for (auto pair : m_keyMap)
			if (Nz::Keyboard::IsKeyPressed(pair.first) && IsRealtimeAction(pair.second))
				actions.push_back(pair.second);

		return actions;
	}

	bool KeyBinding::IsRealtimeAction(PlayerAction action) const
	{
		switch (action)
		{
			case PlayerAction::MoveLeft:
			case PlayerAction::MoveRight:
			case PlayerAction::MoveDown:
			case PlayerAction::Jump:
				return true;

			default:
				return false;
		}
	}

}
