// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	ContentQueue
//		- Loads content items asynchronously.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include <functional>
#include "Thread/Queue.h"
#include <memory>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace thread
{
	class Thread;
}

namespace content
{

class ContentItem;
class ContentManager;

typedef std::function<void (ContentItem*)> ContentLoadedCallback;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

struct ContentWorkItem
{
	std::string				name;
	ContentItem*			item;
	ContentLoadedCallback	callback;
};

// -----------------------------------------------------------------------------

class ContentQueue
{
public:
	ContentQueue(
		ContentManager& manager
	);

	void
	Update();

	void
	PushWork(
		const std::string&		name,
		ContentItem*			newItem,
		ContentLoadedCallback&	callback
	);

private:
	void
	RunContentThread();

	ContentManager&					m_manager;
	std::unique_ptr<thread::Thread>	m_contentThread;
	thread::Queue<ContentWorkItem*>	m_pendingQueue;
	thread::Queue<ContentWorkItem*>	m_completedQueue;
};

// -----------------------------------------------------------------------------

}//namespace content