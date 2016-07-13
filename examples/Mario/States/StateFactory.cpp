#include "StateFactory.hpp"

#include "DeathState.hpp"
#include "GameState.hpp"

namespace SMB
{
	std::shared_ptr<Ndk::State> StateFactory::Get(State state)
	{
		return s_states[state];
	}

	bool StateFactory::Initialize(SMB::StateContext& context)
	{
		s_states.emplace(std::make_pair(State::Death, std::make_shared<SMB::DeathState>(context)));
		s_states.emplace(std::make_pair(State::Game, std::make_shared<SMB::GameState>(context)));

		return true;
	}

	void StateFactory::Uninitialize()
	{
		s_states.clear();
	}

	std::map<State, std::shared_ptr<Ndk::State>> StateFactory::s_states;
}
