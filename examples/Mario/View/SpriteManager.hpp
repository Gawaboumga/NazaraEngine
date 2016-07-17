#ifndef __SPRITEMANAGER_HPP__
#define __SPRITEMANAGER_HPP__

#include "AnimatedSprite.hpp"

#include <map>

namespace SMB
{
	enum class TypeSprite
	{
		Coin,
		Goompa,
		Mario,
	};

	Nz::String ToString(TypeSprite type);

	enum class TypeAnimation
	{
		Coin_Still,
		Goompa_Crushed,
		Goompa_Walk,
		Mario_Run_Left,
		Mario_Run_Right,
		Mario_Still,
	};

	Nz::String ToString(TypeAnimation type);

	class SpriteManager
	{
		public:

			static SMB::AnimatedSpriteRef Get(TypeSprite type);
			static const SMB::Animation& Get(TypeAnimation type);

			static void Uninitialize();

		private:

			static void LoadCoin();
			static void LoadGoompa();
			static void LoadMario();

			static void LoadAnimatedSprite(const Nz::String& filepath, const Nz::String& registrationName);
			static void LoadAnimations(const std::map<Nz::String, Nz::String>& nameTextures, std::map<Nz::String, SMB::Animation>& nameAnimations);
			static void LoadTexture(const Nz::String& texturePath);

			static std::map<Nz::String, SMB::Animation> s_animations;
	};
}

#endif // __SPRITEMANAGER_HPP__
