#ifndef __SCREENPAINTER_HPP__
#define __SCREENPAINTER_HPP__

#include <NDK/Entity.hpp>

namespace SMB
{
	class Player;
	class StateContext;
}

namespace SMB
{
	class ScreenPainter
	{
		public:
			ScreenPainter(SMB::StateContext& context);

			void Clear();

			void DrawDeathScreen(const SMB::Player& player);

			void Update(float elapsedTime);

		private:
			StateContext& m_context;
			Ndk::EntityHandle m_text;
	};
}

#endif // __SCREENPAINTER_HPP__
