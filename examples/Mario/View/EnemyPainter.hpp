#ifndef __ENNEMYPAINTER_HPP__
#define __ENNEMYPAINTER_HPP__

#include <NDK/Entity.hpp>

#include "AnimatedSprite.hpp"

#include <unordered_map>

namespace SMB
{
	class Enemy;
	class StateContext;
}

namespace SMB
{
	class EnemyPainter
	{
		public:

			EnemyPainter(StateContext& context);

			void Clear();
			bool CreateEnemy(const Enemy& enemy);

			void Update(const Enemy& enemy, float elapsedTime);

		private:

			SMB::AnimatedSpriteRef& GetAnimatedSpriteAssociatedWith(const Enemy& enemy);
			Ndk::EntityHandle& GetEntityAssociatedWith(const Enemy& enemy);

			void SetAnimation(SMB::AnimatedSpriteRef& animatedSprite, const Enemy& enemy);

			std::unordered_map<int, SMB::AnimatedSpriteRef> m_enemyAnimationMap;
			std::unordered_map<int, Ndk::EntityHandle> m_enemyEntityMap;
			StateContext& m_context;
	};
}

#endif // __ENNEMYPAINTER_HPP__
