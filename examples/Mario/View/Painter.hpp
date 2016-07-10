#ifndef __PAINTER_HPP__
#define __PAINTER_HPP__

#include "CharacterPainter.hpp"
#include "CoinPainter.hpp"
#include "EnemyPainter.hpp"
#include "LevelPainter.hpp"

namespace SMB
{
	class Character;
	class Coin;
	class Enemy;
	class Level;
	class StateContext;
}

namespace SMB
{
	class Painter
	{
		public:

			Painter(StateContext& context);

			void Clear();

			void Draw(const SMB::Character& character);
			void Draw(const SMB::Coin& coin);
			void Draw(const SMB::Enemy& enemy);
			void Draw(const SMB::Level& level);

			void Update(float elapsedTime);

		private:

			void CameraUpdate(float elapsedTime);

			StateContext& m_context;
			CharacterPainter m_characterPainter;
			CoinPainter m_coinPainter;
			EnemyPainter m_enemyPainter;
			LevelPainter m_levelPainter;
			const Level* m_level;
	};
}

#endif // __PAINTER_HPP__
