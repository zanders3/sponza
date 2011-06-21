// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Queue
//		- A thread safe Queue
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include <queue>
#include "Thread/Mutex.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace thread
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

template <typename T>
class Queue
{
public:
	void 
	Push(
		T& item
	)
	{
		Lock lock(m_mutex);
		m_queue.push(item);
	}

	bool 
	Dequeue(
		T& item
	)
	{
		Lock lock(m_mutex);

		if (m_queue.empty())
			return false;

		item = m_queue.front();
		m_queue.pop();

		return true;
	}

	size_t
	Size()
	{
		Lock lock(m_mutex);

		return m_queue.size();
	}

private:
	Mutex			m_mutex;
	std::queue<T>	m_queue;
};

// -----------------------------------------------------------------------------

}//namespace thread