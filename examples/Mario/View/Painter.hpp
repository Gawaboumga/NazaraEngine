#ifndef __PAINTER_HPP__
#define __PAINTER_HPP__

#include "CharacterPainter.hpp"
#include "LevelPainter.hpp"

namespace SMB
{
	class Character;
	class Level;
	class StateContext;
}

namespace SMB
{
	class Painter
	{
		public:

			Painter(StateContext& context);

			void Draw(const SMB::Character& character);
			void Draw(const SMB::Level& level);

			void Update(float elapsedTime);

		private:

			void CameraUpdate(float elapsedTime);

			StateContext& m_context;
			CharacterPainter m_characterPainter;
			LevelPainter m_levelPainter;
			const Level* m_level;
	};
}

#endif // __PAINTER_HPP__
