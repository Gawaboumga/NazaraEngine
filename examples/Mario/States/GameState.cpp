#include "GameState.hpp"

#include <Nazara/Utility/Event.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Graphics/ColorBackground.hpp>

#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include <NDK/Systems/RenderSystem.hpp>

#include "../Core/CommandQueue.hpp"
#include "../Core/Player.hpp"
#include "../Core/StateContext.hpp"

namespace SMB
{

    GameState::GameState(SMB::StateContext& context) :
    	State{},
    	m_context{ context },
    	m_level{ m_context },
    	m_painter{ m_context }
    {}

    void GameState::Enter(Ndk::StateMachine& fsm)
    {
    	m_context.world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
    	//m_context.world.GetSystem<Ndk::RenderSystem>().SetDefaultBackground(Nz::ColorBackground::New(Nz::Color(192, 100, 100)));

    	m_level.Load(m_context.player.GetLevel());
    	m_level.AddPlayer();
    	m_painter.Draw(m_level);
    	for (const auto& character : m_level.GetCharacters())
			m_painter.Draw(character);
    }

    void GameState::Leave(Ndk::StateMachine& fsm) {

    }

    bool GameState::Update(Ndk::StateMachine& fsm, float elapsedTime)
    {
        m_context.world.Update(elapsedTime);
        m_level.Update(elapsedTime);
        m_painter.Update(elapsedTime);

        if (!m_level.HasAlivePlayer())
		{
			m_context.player.AddDeath();
			NazaraNotice("Mario died");
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
