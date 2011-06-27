// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Content/ContentManager.h"
#include "Content/PackReader.h"
#include "Content/ContentQueue.h"
#include "Thread/Thread.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

ContentManager::ContentManager(
	const char* contentRoot, 
	const char* packFile
) : m_items(),
	m_packReader()
{
	m_contentQueue.reset(new ContentQueue(*this));
	m_packReader.reset(new PackReader(packFile));
}

// -----------------------------------------------------------------------------

ContentManager::~ContentManager()
{
}

// -----------------------------------------------------------------------------

void
ContentManager::Update()
{
	m_contentQueue->Update();
}

// -----------------------------------------------------------------------------

void
ContentManager::ReadItem(
	const std::string& name,
	ContentItem*	   newItem
)
{
	//Load the content from the pack file
	ContentReader reader = m_packReader->Get(name);
	newItem->SetContent(this);
	newItem->Load(reader);

	m_items.insert(std::make_pair(name, std::unique_ptr<ContentItem>(newItem)));
}

// -----------------------------------------------------------------------------

void
ContentManager::ReadItemAsync(
	const std::string&		name,
	ContentItem*			newItem,
	ContentLoadedCallback&	callback
)
{
	m_contentQueue->PushWork(name, newItem, callback);
}

// -----------------------------------------------------------------------------

}//namespace content