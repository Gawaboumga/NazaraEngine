#ifndef __GAMEPAINTER_HPP__
#define __GAMEPAINTER_HPP__

#include "EntityPainter.hpp"
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

	class GamePainter
	{
		public:

			GamePainter(StateContext& context);

			void Clear();

			void Draw(const SMB::Character& character);
			void Draw(const SMB::Coin& coin);
			void Draw(const SMB::Enemy& enemy);
			void Draw(const SMB::Level& level);

			void Update(float elapsedTime);

		private:

			void CameraUpdate(float elapsedTime);

			StateContext& m_context;
			EntityPainter m_entityPainter;
			LevelPainter m_levelPainter;
			const Level* m_level;
	};

}

#endif // __GAMEPAINTER_HPP__
