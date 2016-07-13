#ifndef __SPRITEMANAGER_HPP__
#define __SPRITEMANAGER_HPP__

#include <Nazara/Graphics/Sprite.hpp>

namespace SMB
{
	enum class SpriteType
	{
		Coin,
		Goompa,
		Mario,
	};

	Nz::String ToString(SpriteType type);

	class SpriteManager
	{
		public:

			static Nz::SpriteRef Get(SpriteType type);

		private:

			static void LoadCoin();
			static void LoadGoompa();
			static void LoadMario();

			static void LoadTexture(const Nz::String& texturePath);
	};
}

#endif // __SPRITEMANAGER_HPP__
