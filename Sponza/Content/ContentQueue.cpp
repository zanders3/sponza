// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Content/ContentQueue.h"
#include "Content/ContentManager.h"
#include "Thread/Thread.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

ContentQueue::ContentQueue(
	ContentManager& manager
) : m_manager(manager)
{
	m_contentThread.reset(new thread::Thread(
		[=]()
		{
			this->RunContentThread();
		}));
}

// -----------------------------------------------------------------------------

void
ContentQueue::Update()
{
	ContentWorkItem* item;
	while (m_completedQueue.Dequeue(item))
	{
		item->callback(item->item);
		delete item;
	}
}

// -----------------------------------------------------------------------------

void
ContentQueue::PushWork(
	const std::string&		name,
	ContentItem*			newItem,
	ContentLoadedCallback&	callback
)
{
	ContentWorkItem* item = new ContentWorkItem();
	item->name = name;
	item->item = newItem;
	item->callback = callback;
	m_pendingQueue.Push(item);

	if (!m_contentThread->IsRunning())
	{
		m_contentThread->Run();
	}
}

// -----------------------------------------------------------------------------

void
ContentQueue::RunContentThread()
{
	ContentWorkItem* item;
	while (m_pendingQueue.Dequeue(item))
	{
		m_manager.ReadItem(item->name, item->item);

		m_completedQueue.Push(item);
	}
}

// -----------------------------------------------------------------------------

}//namespace content