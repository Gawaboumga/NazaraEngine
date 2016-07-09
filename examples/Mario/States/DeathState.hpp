#ifndef __DEATHSTATE_HPP__
#define __DEATHSTATE_HPP__

#include <NDK/State.hpp>

#include "../View/ScreenPainter.hpp"

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
	class DeathState : public Ndk::State
	{
		public:
			DeathState(SMB::StateContext& context);
			~DeathState() = default;

			virtual void Enter(Ndk::StateMachine& fsm) override;
			virtual void Leave(Ndk::StateMachine& fsm) override;
			virtual bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

		private:
			SMB::StateContext& m_context;
			SMB::ScreenPainter m_painter;
			float m_time;
    };
}

#endif // __DEATHSTATE_HPP__
