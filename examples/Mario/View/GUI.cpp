#include "GUI.hpp"

#include "../Core/Player.hpp"
#include "../Core/StateContext.hpp"

#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Utility/SimpleTextDrawer.hpp>

#include <NDK/World.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>

namespace SMB
{
	GUI::GUI(SMB::StateContext& stateContext) :
		m_context{ stateContext }
	{
	}

	void GUI::Clear()
	{
		m_scoreText->Clear();
		m_coinsText->Clear();
		m_worldText->Clear();
		m_timeText->Clear();

		m_score->Kill();
		m_coins->Kill();
		m_world->Kill();
		m_time->Kill();
	}

	void GUI::Draw(const SMB::Player& player)
	{
		auto& world = m_context.world;
		m_score = world.CreateEntity();

		m_scoreText = Nz::TextSprite::New();
		m_score->AddComponent<Ndk::NodeComponent>();
		Ndk::GraphicsComponent& scoreGraphicsComponent = m_score->AddComponent<Ndk::GraphicsComponent>();
		scoreGraphicsComponent.Attach(m_scoreText);

		m_coins = world.CreateEntity();

		m_coinsText = Nz::TextSprite::New();
		m_coins->AddComponent<Ndk::NodeComponent>();
		Ndk::GraphicsComponent& coinsGraphicsComponent = m_coins->AddComponent<Ndk::GraphicsComponent>();
		coinsGraphicsComponent.Attach(m_coinsText);

		m_world = world.CreateEntity();

		m_worldText = Nz::TextSprite::New();
		m_world->AddComponent<Ndk::NodeComponent>();
		Ndk::GraphicsComponent& worldGraphicsComponent = m_world->AddComponent<Ndk::GraphicsComponent>();
		worldGraphicsComponent.Attach(m_worldText);

		m_time = world.CreateEntity();

		m_timeText = Nz::TextSprite::New();
		m_time->AddComponent<Ndk::NodeComponent>();
		Ndk::GraphicsComponent& timeGraphicsComponent = m_time->AddComponent<Ndk::GraphicsComponent>();
		timeGraphicsComponent.Attach(m_timeText);
	}

	void GUI::Update(const SMB::Player& player)
	{
		m_scoreText->Update(Nz::SimpleTextDrawer::Draw("Mario\n" + Nz::String::Number(player.GetScore()), 18));
		m_coinsText->Update(Nz::SimpleTextDrawer::Draw("\n" + Nz::String::Number(player.GetCoins()), 18));
		m_worldText->Update(Nz::SimpleTextDrawer::Draw("World\n" + Nz::String::Number(player.GetLevel()), 18));
		m_timeText->Update(Nz::SimpleTextDrawer::Draw("Time\n" + Nz::String::Number(10), 18));
	}

	void GUI::Update(float elapsedTime)
	{
		auto& cameraNodeComponent = m_context.camera->GetComponent<Ndk::NodeComponent>();
		auto cameraX = cameraNodeComponent.GetPosition().x;
		auto cameraY = cameraNodeComponent.GetPosition().y;
		auto& scoreNodeComponent = m_score->GetComponent<Ndk::NodeComponent>();
		scoreNodeComponent.SetPosition(cameraX + m_context.window.GetWidth() * 1.f / 5.f, cameraY, 50.f);
		auto& coinsNodeComponent = m_coins->GetComponent<Ndk::NodeComponent>();
		coinsNodeComponent.SetPosition(cameraX + m_context.window.GetWidth() * 2.f / 5.f, cameraY, 10.f);
		auto& worldNodeComponent = m_world->GetComponent<Ndk::NodeComponent>();
		worldNodeComponent.SetPosition(cameraX + m_context.window.GetWidth() * 3.f / 5.f, cameraY, 10.f);
		auto& timeNodeComponent = m_time->GetComponent<Ndk::NodeComponent>();
		timeNodeComponent.SetPosition(cameraX + m_context.window.GetWidth() * 4.f / 5.f, cameraY, 10.f);
	}
}
