#include "GameState.hpp"

#include "Core/CommandQueue.hpp"
#include "Core/Player.hpp"
#include "Core/StateContext.hpp"

#include "StateFactory.hpp"

#include <NDK/StateMachine.hpp>
#include <NDK/World.hpp>

namespace SMB
{

    GameState::GameState(SMB::StateContext& context) :
        State{},
    m_context{ context },
    m_level{ m_context },
    m_gui{ m_context },
    m_gamePainter{ m_context }
    {}

    void GameState::Enter(Ndk::StateMachine& fsm)
    {
        m_level.Load(m_context.player.GetLevel());
        m_level.AddPlayer();
        m_gamePainter.Draw(m_level);
        for (const auto& character : m_level.GetCharacters())
            m_gamePainter.Draw(character);
        for (const auto& enemy : m_level.GetEnemies())
            m_gamePainter.Draw(enemy);
        for (const auto& coin : m_level.GetCoins())
            m_gamePainter.Draw(coin);

        m_gui.Draw(m_context.player);
    }

    void GameState::Leave(Ndk::StateMachine& fsm)
    {
        m_gui.Clear();
        m_gamePainter.Clear();
    }

    bool GameState::Update(Ndk::StateMachine& fsm, float elapsedTime)
    {
        m_context.world.Update(elapsedTime);
        m_level.Update(elapsedTime);
        m_gamePainter.Update(elapsedTime);
        m_gui.Update(elapsedTime);
        if (m_context.player.HasChanged())
            m_gui.Update(m_context.player);

        if (!m_level.HasAlivePlayer())
        {
            m_context.player.AddDeath();
            NazaraNotice("Mario died");
            fsm.ChangeState(StateFactory::Get(SMB::State::Death));
        }
        else if (m_level.HasPlayerReachedEnd())
        {
            m_context.player.FinishLevel();
            NazaraNotice("Mario finished the level");
        }

        SMB::CommandQueue& commands = m_level.GetCommandQueue();
        m_context.player.HandleInput(commands);

        return true;
    }

}
