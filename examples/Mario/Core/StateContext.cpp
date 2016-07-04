#include "StateContext.hpp"

namespace SMB
{

	StateContext::StateContext(Nz::RenderWindow& windowContext, Ndk::World& worldContext, Ndk::EntityHandle& cameraContext, SMB::Player& playerContext) :
		window{ windowContext },
		world{ worldContext },
		camera{ cameraContext },
		player{ playerContext }
	{
	}

}
