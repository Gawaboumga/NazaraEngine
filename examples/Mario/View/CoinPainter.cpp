#include "CoinPainter.hpp"

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

#include "../Core/StateContext.hpp"
#include "../Core/Coin.hpp"
#include "../Core/URL.hpp"
#include "Dimensions.hpp"
#include "SpriteManager.hpp"

namespace SMB
{
	CoinPainter::CoinPainter(SMB::StateContext& context) :
    	m_context{ context }
	{
	}

	void CoinPainter::Clear()
	{
		for (auto& pair : m_coinEntityMap)
			pair.second->Kill();
		m_coinEntityMap.clear();
	}

	bool CoinPainter::CreateCoin(const Coin& coin)
	{
		Nz::Vector2f coinSize = Nz::Vector2f(Dimensions::WorldScale() * coin.GetDimensions());
		/*auto coinSprite = SpriteManager::Get(SpriteType::Coin);
		coinSprite->SetSize(coinSize);*/
		Nz::MaterialRef material = Nz::MaterialLibrary::Get("Default");
		material->SetFaceFilling(Nz::FaceFilling_Fill);
		material->SetDiffuseMap(Nz::TextureLibrary::Get("Coin"));
		Nz::SpriteRef coinSprite = Nz::Sprite::New(material);
		coinSprite->SetSize(coinSize);


		auto entity = m_context.world.CreateEntity();
		auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(coin.GetPosition() * Dimensions::WorldScale());
		auto& graphicsComponent = entity->AddComponent<Ndk::GraphicsComponent>();
		graphicsComponent.Attach(coinSprite);

		m_coinEntityMap.insert(std::make_pair(coin.GetID(), entity));

		return true;
	}

	void CoinPainter::Update(const Coin& coin, float elapsedTime)
	{
		auto& entity = GetEntityAssociatedWith(coin);
		if (!coin.IsAlive())
		{
			if (entity->IsEnabled())
				entity->Enable(false);

			return;
		}
		auto& nodeComponent = entity->GetComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(coin.GetPosition() * Dimensions::WorldScale());
	}

	Ndk::EntityHandle& CoinPainter::GetEntityAssociatedWith(const Coin& coin)
	{
		return m_coinEntityMap[coin.GetID()];
	}
}
