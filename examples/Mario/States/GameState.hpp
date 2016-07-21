#ifndef __GAMESTATE_HPP__
#define __GAMESTATE_HPP__

#include <NDK/State.hpp>

#include "Core/Level.hpp"
#include "View/GUI.hpp"
#include "View/GamePainter.hpp"

namespace Ndk
{
	class StateMachine;
}

namespace SMB
{
	class StateContext;
}

namespace SMB
{

	class GameState : public Ndk::State
	{
		public:

			GameState(SMB::StateContext& context);
			~GameState() = default;

			virtual void Enter(Ndk::StateMachine& fsm) override;
			virtual void Leave(Ndk::StateMachine& fsm) override;
			virtual bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

		private:

			SMB::StateContext& m_context;
			SMB::Level m_level;
			SMB::GUI m_gui;
			SMB::GamePainter m_gamePainter;
	};

}

#endif // __GAMESTATE_HPP__
