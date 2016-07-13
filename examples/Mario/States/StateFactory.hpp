#ifndef __STATEFACTORY_HPP__
#define __STATEFACTORY_HPP__

#include <NDK/State.hpp>

#include <map>
#include <memory>

namespace SMB
{
	class StateContext;
}

namespace SMB
{
	enum class State
	{
		Death,
		Game
	};

	class StateFactory
	{
		public:
			static std::shared_ptr<Ndk::State> Get(State state);

			static bool Initialize(SMB::StateContext& stateContext);

			static void Uninitialize();

		private:
			static std::map<State, std::shared_ptr<Ndk::State>> s_states;
	};
}

#endif // __STATEFACTORY_HPP__
