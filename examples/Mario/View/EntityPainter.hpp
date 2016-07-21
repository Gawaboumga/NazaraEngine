#ifndef __ENTITYPAINTER_HPP__
#define __ENTITYPAINTER_HPP__

#include "AnimatedSprite.hpp"

#include <NDK/Entity.hpp>

namespace SMB
{
	class Character;
	class Coin;
	class Enemy;
	class Entity;
	class StateContext;
}

namespace SMB
{

	class EntityPainter
	{
		public:

			EntityPainter(StateContext& context);

			bool CreateCharacter(const Character& character);
			bool CreateCoin(const Coin& coin);
			bool CreateEnemy(const Enemy& enemy);

			void Clear();

			void Update(const Character& character, float elapsedTime);
			void Update(const Coin& coin, float elapsedTime);
			void Update(const Enemy& enemy, float elapsedTime);

		private:

			void CreateEntity(const Entity& entity, SMB::AnimatedSpriteRef sprite);

			SMB::AnimatedSpriteRef GetAnimatedSpriteAssociatedWith(const Entity& entity);
			Ndk::EntityHandle& GetWorldEntityAssociatedWith(const Entity& entity);

			void SetCharacterAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Character& character);
			void SetCoinAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Coin& coin);
			void SetEnemyAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Enemy& enemy);

			SMB::AnimatedSpriteRef UpdateEntity(const Entity& entity, float elapsedTime);

			std::unordered_map<int, SMB::AnimatedSpriteRef> m_entityAnimationMap;
			std::unordered_map<int, Ndk::EntityHandle> m_entityWorldEntityMap;
			StateContext& m_context;
	};

}

#endif // __ENTITYPAINTER_HPP__
