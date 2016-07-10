#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <Nazara/Graphics/TextSprite.hpp>

#include <NDK/Entity.hpp>

namespace SMB
{
	class Player;
	class StateContext;
}

namespace SMB
{
	class GUI
	{
		public:

			GUI(SMB::StateContext& stateContext);

			void Clear();

			void Draw(const SMB::Player& player);

			void Update(const SMB::Player& player);
			void Update(float elapsedTime);

		private:

			SMB::StateContext& m_context;
			Ndk::EntityHandle m_score;
			Ndk::EntityHandle m_coins;
			Ndk::EntityHandle m_world;
			Ndk::EntityHandle m_time;

			Nz::TextSpriteRef m_scoreText;
			Nz::TextSpriteRef m_coinsText;
			Nz::TextSpriteRef m_worldText;
			Nz::TextSpriteRef m_timeText;
	};
}

#endif // __GUI_HPP__
