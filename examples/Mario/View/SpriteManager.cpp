#include "SpriteManager.hpp"

#include "../Core/URL.hpp"

namespace SMB
{
	Nz::String ToString(SpriteType type)
	{
		switch (type)
		{
			case SpriteType::Coin:
				return "Coin";
			case SpriteType::Goompa:
				return "Goompa";
			case SpriteType::Mario:
				return "Mario";
			default:
				return "Mario";
		}
	}

	Nz::SpriteRef SpriteManager::Get(SpriteType type)
	{
		auto name = ToString(type);
		switch (type)
		{
			case SpriteType::Coin:
			{
				if (!Nz::SpriteLibrary::Has(name))
					LoadCoin();
				return Nz::SpriteLibrary::Get(name);
			}
			case SpriteType::Goompa:
			{
				if (!Nz::SpriteLibrary::Has(name))
					LoadGoompa();
				return Nz::SpriteLibrary::Get(name);
			}
			case SpriteType::Mario:
			{
				if (!Nz::SpriteLibrary::Has(name))
					LoadMario();
				return Nz::SpriteLibrary::Get(name);
			}
			default:
				return {};
		}
	}

	void SpriteManager::LoadCoin()
	{
		Nz::String texturePath{ URL::GetEnemiesSpriteSheet() };
		LoadTexture(texturePath);

		auto name = ToString(SpriteType::Coin);
		if (!Nz::SpriteLibrary::Has(name))
		{
			auto texture = Nz::TextureLibrary::Get(texturePath);
			auto coinSprite = Nz::Sprite::New(texture.Get());
			coinSprite->SetTextureRect(Nz::Rectui{
				144, 144, // Hard coded position of coin (not a coin but meh)
				16, 16
			});
			Nz::SpriteLibrary::Register(name, coinSprite);
		}
	}

	void SpriteManager::LoadGoompa()
	{
		Nz::String texturePath{ URL::GetEnemiesSpriteSheet() };
		LoadTexture(texturePath);

		auto name = ToString(SpriteType::Goompa);
		if (!Nz::SpriteLibrary::Has(name))
		{
			auto texture = Nz::TextureLibrary::Get(texturePath);
			auto goompaSprite = Nz::Sprite::New(texture.Get());
			goompaSprite->SetTextureRect(Nz::Rectui{
				0, 4, // Hard coded position of goompa
				16, 16
			});
			Nz::SpriteLibrary::Register(name, goompaSprite);
		}
	}

	void SpriteManager::LoadMario()
	{
		Nz::String texturePath{ URL::GetMarioSpriteSheet() };
		NazaraWarning(texturePath);
		LoadTexture(texturePath);

		auto name = ToString(SpriteType::Mario);
		if (!Nz::SpriteLibrary::Has(name))
		{
			auto texture = Nz::TextureLibrary::Get(texturePath);
			auto marioSprite = Nz::Sprite::New(texture.Get());
			marioSprite->SetTextureRect(Nz::Rectui{
				208, 0, // Hard coded position of mario
				16, 16
			});
			const auto& mat = marioSprite->GetMaterial();
			Nz::SpriteLibrary::Register(name, marioSprite);
		}
	}

	void SpriteManager::LoadTexture(const Nz::String& texturePath)
	{
		if (!Nz::TextureLibrary::Has(texturePath))
		{
			auto texture = Nz::Texture::New();
			if (!texture->LoadFromFile(texturePath))
			{
				NazaraError("Error while loading assets: " + texturePath);
			}
			Nz::TextureLibrary::Register(texturePath, texture);
		}
	}
}
