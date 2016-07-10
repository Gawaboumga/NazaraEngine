#include "ScreenPainter.hpp"

#include <Nazara/Graphics/TextSprite.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Utility/SimpleTextDrawer.hpp>

#include <NDK/World.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>

#include "../Core/Player.hpp"
#include "../Core/StateContext.hpp"

namespace SMB
{
	ScreenPainter::ScreenPainter(SMB::StateContext& context) :
		m_context{ context }
	{
	}

	void ScreenPainter::Clear()
	{
		m_text->Kill();
	}

	void ScreenPainter::DrawDeathScreen(const SMB::Player& player)
	{
		m_text = m_context.world.CreateEntity();
		auto& nodeComponent = m_text->AddComponent<Ndk::NodeComponent>();
		auto& graphicsComponent = m_text->AddComponent<Ndk::GraphicsComponent>();

		auto textSprite = Nz::TextSprite::New();
		textSprite->Update(Nz::SimpleTextDrawer::Draw("You Have: " + Nz::String::Number(player.GetNumberOfLives()) + " live(s)", 72));
		graphicsComponent.Attach(textSprite);

		auto textBox = graphicsComponent.GetBoundingVolume().aabb;
		const auto& window = m_context.window;
		nodeComponent.SetPosition(window.GetWidth() / 2 - textBox.width / 2, window.GetHeight() / 2 - textBox.height / 2);
	}

	void ScreenPainter::Update(float elapsedTime)
	{
	}
}
