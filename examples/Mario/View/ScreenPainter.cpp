#include "ScreenPainter.hpp"

#include "Core/Player.hpp"
#include "Core/StateContext.hpp"

#include <Nazara/Graphics/ColorBackground.hpp>
#include <Nazara/Graphics/TextSprite.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Utility/SimpleTextDrawer.hpp>

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Systems/RenderSystem.hpp>
#include <NDK/World.hpp>

namespace SMB
{
	ScreenPainter::ScreenPainter(SMB::StateContext& context) :
		m_context{ context }
	{
		m_context.world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
    	m_context.world.GetSystem<Ndk::RenderSystem>().SetDefaultBackground(Nz::ColorBackground::New(Nz::Color::Black));
	}

	void ScreenPainter::Clear()
	{
		m_textSprite->Clear();
		m_text->Kill();
	}

	void ScreenPainter::DrawDeathScreen(const SMB::Player& player)
	{
		m_text = m_context.world.CreateEntity();
		auto& nodeComponent = m_text->AddComponent<Ndk::NodeComponent>();
		auto& graphicsComponent = m_text->AddComponent<Ndk::GraphicsComponent>();

		m_textSprite = Nz::TextSprite::New();
		m_textSprite->Update(Nz::SimpleTextDrawer::Draw("You Have: " + Nz::String::Number(player.GetNumberOfLives()) + " live(s)", 72));
		graphicsComponent.Attach(m_textSprite);

		auto textBox = graphicsComponent.GetBoundingVolume().aabb;
		const auto& window = m_context.window;
		nodeComponent.SetPosition(window.GetWidth() / 2 - textBox.width / 2, window.GetHeight() / 2 - textBox.height / 2);

		auto& cameraNodeComponent = m_context.camera->GetComponent<Ndk::NodeComponent>();
		cameraNodeComponent.SetPosition(Nz::Vector3f::Zero());
	}

	void ScreenPainter::Update(float elapsedTime)
	{
	}
}
