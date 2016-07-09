#ifndef __ENNEMYPAINTER_HPP__
#define __ENNEMYPAINTER_HPP__

#include <NDK/Entity.hpp>

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

			Ndk::EntityHandle& GetEntityAssociatedWith(const Enemy& enemy);

			std::unordered_map<int, Ndk::EntityHandle> m_enemyEntityMap;
			StateContext& m_context;
	};
}

#endif // __ENNEMYPAINTER_HPP__
