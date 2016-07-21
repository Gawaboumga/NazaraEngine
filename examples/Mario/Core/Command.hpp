#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <cassert>
#include <functional>

namespace SMB
{
	class Character;
	class Level;
}

namespace SMB
{

	struct Command
	{
		using Action = std::function<void(SMB::Level&, SMB::Character&, float)>;

		void operator()(SMB::Level& level, SMB::Character& entity, float elapsedTime)
		{
			assert(action);
			return action(level, entity, elapsedTime);
		}

		Action action;
	};

}

#endif // __COMMAND_HPP__
