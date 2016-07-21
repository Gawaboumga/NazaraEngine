#ifndef __COMMANDQUEUE_HPP__
#define __COMMANDQUEUE_HPP__

#include "Command.hpp"

#include <queue>

namespace SMB
{

	class CommandQueue
	{
		public:

			bool IsEmpty() const;

			SMB::Command Pop();
			void Push(const SMB::Command& command);

		private:

			std::queue<SMB::Command> m_queue;
	};

}

#endif // __COMMANDQUEUE_HPP__
