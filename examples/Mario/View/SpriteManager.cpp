#include "SpriteManager.hpp"

#include "Core/URL.hpp"

#include "AnimationParser.hpp"

namespace SMB
{

	Nz::String ToString(TypeAnimation type)
	{
		switch (type)
		{
		case TypeAnimation::Coin_Still:
			return "Coin_Still";
		case TypeAnimation::Goompa_Crushed:
			return "Goompa_Crushed";
		case TypeAnimation::Goompa_Walk:
			return "Goompa_Walk";
		case TypeAnimation::Mario_Jump_Left:
			return "Mario_Jump_Left";
		case TypeAnimation::Mario_Jump_Right:
			return "Mario_Jump_Right";
		case TypeAnimation::Mario_Run_Left:
			return "Mario_Run_Left";
		case TypeAnimation::Mario_Run_Right:
			return "Mario_Run_Right";
		case TypeAnimation::Mario_Still:
			return "Mario_Still";
		default:
			return "Mario_Still";
		}
	}

	const SMB::Animation& SpriteManager::Get(TypeAnimation type)
	{
		auto name = ToString(type);
		return s_animations[name];
	}

	Nz::String ToString(TypeSprite type)
	{
		switch (type)
		{
		case TypeSprite::Coin:
			return "Coin";
		case TypeSprite::Goompa:
			return "Goompa";
		case TypeSprite::Mario:
			return "Mario";
		default:
			return "Mario";
		}
	}

	SMB::AnimatedSpriteRef SpriteManager::Get(TypeSprite type)
	{
		auto name = ToString(type);
		switch (type)
		{
		case TypeSprite::Coin:
		{
			if (!SMB::AnimatedSpriteLibrary::Has(name))
				LoadCoin();
			return SMB::AnimatedSpriteLibrary::Get(name);
		}
		case TypeSprite::Goompa:
		{
			if (!SMB::AnimatedSpriteLibrary::Has(name))
				LoadGoompa();
			return SMB::AnimatedSpriteLibrary::Get(name);
		}
		case TypeSprite::Mario:
		{
			if (!SMB::AnimatedSpriteLibrary::Has(name))
				LoadMario();
			return SMB::AnimatedSpriteLibrary::Get(name);
		}
		default:
			return {};
		}
	}

	void SpriteManager::Uninitialize()
	{
		SMB::AnimatedSprite::Uninitialize();
		s_animations.clear();
	}

	void SpriteManager::LoadCoin()
	{
		LoadAnimatedSprite(URL::GetCoinAnimation(), ToString(TypeSprite::Coin));
	}

	void SpriteManager::LoadGoompa()
	{
		LoadAnimatedSprite(URL::GetGoompaAnimation(), ToString(TypeSprite::Goompa));
	}

	void SpriteManager::LoadMario()
	{
		LoadAnimatedSprite(URL::GetMarioAnimation(), ToString(TypeSprite::Mario));
	}

	void SpriteManager::LoadAnimatedSprite(const Nz::String& filepath, const Nz::String& registrationName)
	{
		auto nameTextures = AnimationParser::GetSpriteSheets(filepath);
		for (auto nameTexture : nameTextures)
		{
			LoadTexture(nameTexture.second);
		}

		auto nameAnimations = AnimationParser::Load(filepath);
		assert(nameTextures.size() == nameAnimations.size());

		LoadAnimations(nameTextures, nameAnimations);
		auto animatedSprite = SMB::AnimatedSprite::New(0.2f);
		SMB::AnimatedSpriteLibrary::Register(registrationName, animatedSprite);
	}

	void SpriteManager::LoadAnimations(const std::map<Nz::String, Nz::String>& nameTextures, std::map<Nz::String, SMB::Animation>& nameAnimations)
	{
		auto nameTextureIt = nameTextures.begin();
		for (auto nameAnimationIt = nameAnimations.begin(); nameAnimationIt != nameAnimations.end(); ++nameAnimationIt, ++nameTextureIt)
		{
			auto& nameOfAnimation = nameAnimationIt->first;
			if (s_animations.find(nameOfAnimation) == s_animations.end())
			{
				auto textureName = nameTextureIt->second;
				auto texture = Nz::TextureLibrary::Get(textureName);
				auto& animation = nameAnimationIt->second;
				animation.SetSpriteSheet(texture);
				s_animations.insert(std::make_pair(nameOfAnimation, animation));
			}
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

	std::map<Nz::String, SMB::Animation> SpriteManager::s_animations;

}
