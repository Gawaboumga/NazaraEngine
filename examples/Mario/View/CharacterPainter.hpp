#ifndef __CHARACTERPAINTER_HPP__
#define __CHARACTERPAINTER_HPP__

#include <NDK/Entity.hpp>

#include "AnimatedSprite.hpp"

#include <unordered_map>

namespace SMB
{
	class Character;
	class StateContext;
}

namespace SMB
{
	class CharacterPainter
	{
		public:

			CharacterPainter(StateContext& context);

			void Clear();
			bool CreateCharacter(const Character& character);

			void Update(const Character& character, float elapsedTime);

		private:

			SMB::AnimatedSpriteRef GetAnimatedSpriteAssociatedWith(const Character& character);
			Ndk::EntityHandle& GetEntityAssociatedWith(const Character& character);

			void SetAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Character& character);

			std::unordered_map<int, SMB::AnimatedSpriteRef> m_characterAnimationMap;
			std::unordered_map<int, Ndk::EntityHandle> m_characterEntityMap;
			StateContext& m_context;
	};
}

#endif // __CHARACTERPAINTER_HPP__
