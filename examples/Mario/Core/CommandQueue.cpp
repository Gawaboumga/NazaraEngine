#include "CommandQueue.hpp"

namespace SMB
{

	bool CommandQueue::IsEmpty() const
	{
		return m_queue.empty();
	}

	Command CommandQueue::Pop()
	{
		auto tmp = m_queue.front();
		m_queue.pop();
		return tmp;
	}

	void CommandQueue::Push(const Command& command)
	{
		m_queue.push(command);
	}

}
