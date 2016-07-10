#include "CoinPainter.hpp"

#include <Nazara/Graphics/Sprite.hpp>

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>

#include "../Core/StateContext.hpp"
#include "../Core/Coin.hpp"
#include "../Core/URL.hpp"
#include "Dimensions.hpp"

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
		Nz::String coinText{ "Coin" };
		Nz::Vector2ui coinSize = Nz::Vector2ui(Dimensions::GetTiles() * coin.GetDimensions());

		if (!Nz::ImageLibrary::Has("mario_tileset"))
		{
			auto image = Nz::Image::New();
			if (!image->LoadFromFile(URL::GetMarioSpriteSheet()))
			{
				NazaraError("Error while loading assets");
				return false;
			}

			Nz::ImageLibrary::Register("mario_tileset", image);
		}

		if (!Nz::SpriteLibrary::Has(coinText))
		{
			auto image = Nz::ImageLibrary::Get("mario_tileset");
			auto coinImage = Nz::Image::New(image->GetType(), image->GetFormat(), coinSize.x, coinSize.y);
			Nz::Boxui box {
				144, 144, 0, // Hard coded position of coin (not a coin but meh)
				coinSize.x, coinSize.y, 1
			};
			coinImage->Copy(*image.Get(), box, Nz::Vector3ui::Zero());

			auto coinTexture = Nz::Texture::New(*coinImage.Get());
			Nz::TextureLibrary::Register(coinText, coinTexture);

			// No idea why I need to do that
			Nz::MaterialRef material = Nz::MaterialLibrary::Get("Default");
			material->SetFaceFilling(Nz::FaceFilling_Fill);
			material->SetDiffuseMap(coinTexture);
			Nz::SpriteRef coinSprite = Nz::Sprite::New(material);
			Nz::SpriteLibrary::Register(coinText, coinSprite);
			/*auto coinSprite = Nz::Sprite::New(coinTexture.Get());
			Nz::SpriteLibrary::Register(coinText, coinSprite);*/
		}

		auto coinSprite = Nz::SpriteLibrary::Get(coinText);

		auto entity = m_context.world.CreateEntity();
		auto& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
		nodeComponent.SetPosition(coin.GetPosition() * Dimensions::GetTiles());
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
		nodeComponent.SetPosition(coin.GetPosition() * Dimensions::GetTiles());
	}

	Ndk::EntityHandle& CoinPainter::GetEntityAssociatedWith(const Coin& coin)
	{
		return m_coinEntityMap[coin.GetID()];
	}
}
