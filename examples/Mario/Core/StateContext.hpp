#ifndef __STATECONTEXT_HPP__
#define __STATECONTEXT_HPP__

#include <NDK/Entity.hpp>

namespace Ndk
{
	class World;
}

namespace Nz
{
	class RenderWindow;
}

namespace SMB
{
	class Player;
}

namespace SMB
{
	class StateContext
	{
		public:
			StateContext(Nz::RenderWindow& window, Ndk::World& world, Ndk::EntityHandle& camera, SMB::Player& player);

			Nz::RenderWindow& window;
			Ndk::World& world;
			Ndk::EntityHandle& camera;
			SMB::Player& player;
	};
}

#endif // __STATECONTEXT_HPP__
