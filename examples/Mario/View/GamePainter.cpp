#include "GamePainter.hpp"

#include "Core/Level.hpp"
#include "Core/StateContext.hpp"
#include "Core/TMXParser.hpp"
#include "Core/URL.hpp"

#include "Dimensions.hpp"

#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Renderer/TextureSampler.hpp>

#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Systems/RenderSystem.hpp>
#include <NDK/World.hpp>

namespace SMB
{

	GamePainter::GamePainter(SMB::StateContext& context) :
		m_context{ context },
		m_entityPainter{ m_context },
		m_levelPainter{ m_context },
		m_level{ nullptr }
	{
		m_context.world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
		Nz::TextureSampler::SetDefaultFilterMode(Nz::SamplerFilter_Nearest);
	}

	void GamePainter::Clear()
	{
		m_entityPainter.Clear();
		m_levelPainter.Clear();
		m_level = nullptr;
	}

	void GamePainter::Draw(const SMB::Character& character)
	{
		m_entityPainter.CreateCharacter(character);
	}

	void GamePainter::Draw(const SMB::Coin& coin)
	{
		m_entityPainter.CreateCoin(coin);
	}

	void GamePainter::Draw(const SMB::Enemy& enemy)
	{
		m_entityPainter.CreateEnemy(enemy);
	}

	void GamePainter::Draw(const SMB::Level& level)
	{
		m_level = &level;

		auto info = level.GetInfo();
		auto imagePath = URL::GetImage(info);
		auto value = m_levelPainter.LoadSpriteSheet(imagePath);

		auto resourceName = m_levelPainter.GetResourceName(imagePath);

		auto tmxPath = URL::GetTMXPath(info);
		auto tileSize = TMXParser::GetTileSize(tmxPath);
		value &= m_levelPainter.LoadSprites(level.GetMap(), tileSize, resourceName);

		value &= m_levelPainter.CreateTiles(level.GetMap());

		if (!value)
		{
			NazaraError("Could not create map");
		}
	}

	void GamePainter::Update(float elapsedTime)
	{
		if (!m_level)
			return;

		const auto& coins = m_level->GetCoins();
		for (const auto& coin : coins)
			m_entityPainter.Update(coin, elapsedTime);

		const auto& enemies = m_level->GetEnemies();
		for (const auto& enemy : enemies)
			m_entityPainter.Update(enemy, elapsedTime);

		const auto& characters = m_level->GetCharacters();
		for (const auto& character : characters)
			m_entityPainter.Update(character, elapsedTime);

		CameraUpdate(elapsedTime);
	}

	void GamePainter::CameraUpdate(float elapsedTime)
	{
		const Map& map = m_level->GetMap();
		const auto& characters = m_level->GetCharacters();
		auto tileSize = Dimensions::WorldScale();
		auto characterPosition = characters.front().GetPosition() * tileSize;

		auto& cameraComponent = m_context.camera->GetComponent<Ndk::CameraComponent>();
		auto& cameraNodeComponent = m_context.camera->GetComponent<Ndk::NodeComponent>();
		auto cameraPosition = cameraNodeComponent.GetPosition();
		auto cameraViewPort = Nz::Rectf(cameraComponent.GetViewport());
		auto cameraBox = cameraViewPort.Translate({ cameraPosition.x, cameraPosition.y });
		auto limitBox = cameraBox * 6.f / 10.f;
		limitBox.Translate({ (cameraBox.width - limitBox.width) * 0.5f, (cameraBox.height - limitBox.height) * 0.5f });

		if (characterPosition.x > limitBox.x + limitBox.width)
			cameraPosition.x += (characterPosition.x - (limitBox.x + limitBox.width));
		if (characterPosition.y > limitBox.y + limitBox.height)
			cameraPosition.y += (characterPosition.y - (limitBox.y + limitBox.height));
		if (characterPosition.x < limitBox.x)
			cameraPosition.x -= (limitBox.x - characterPosition.x);
		if (characterPosition.y < limitBox.y)
			cameraPosition.y -= (limitBox.y - characterPosition.y);

		auto halfViewPort = cameraViewPort / 2.f;
		cameraPosition.x = Nz::Clamp(cameraPosition.x, 0.f, map.GetWidth() * tileSize.x - cameraViewPort.width);
		cameraPosition.y = Nz::Clamp(cameraPosition.y, 0.f, map.GetHeight() * tileSize.y - cameraViewPort.height);
		cameraNodeComponent.SetPosition(cameraPosition);
	}

}
