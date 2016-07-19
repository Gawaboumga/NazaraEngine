#include "DeathState.hpp"

#include "Core/StateContext.hpp"

#include "StateFactory.hpp"

#include <NDK/StateMachine.hpp>

namespace SMB
{
    DeathState::DeathState(SMB::StateContext& context) :
    	State{},
    	m_context{ context },
    	m_painter{ m_context },
    	m_time{ 0.f }
    {
    }

    void DeathState::Enter(Ndk::StateMachine& fsm)
    {
    	m_time = 0.f;
    	m_painter.DrawDeathScreen(m_context.player);
    }

    void DeathState::Leave(Ndk::StateMachine& fsm) {
		m_time = 0.f;
		m_painter.Clear();
    }

    bool DeathState::Update(Ndk::StateMachine& fsm, float elapsedTime)
    {
    	m_painter.Update(elapsedTime);

    	if (m_time > 2.f /* s */)
			fsm.ChangeState(StateFactory::Get(SMB::State::Game));

		m_time += elapsedTime;

        return true;
    }
}
