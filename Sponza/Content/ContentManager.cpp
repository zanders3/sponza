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
#include "Content/GameConnection.h"
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
	m_packReader(),
	m_contentRoot(contentRoot)
{
	m_contentQueue.reset(new ContentQueue(*this));
	m_packReader.reset(new PackReader(packFile));
	m_gameConnection.reset(new GameConnection(*this));
}

// -----------------------------------------------------------------------------

ContentManager::~ContentManager()
{
}

// -----------------------------------------------------------------------------

void
ContentManager::ReloadContent(
	const std::string& name
)
{
	std::string relName = name.substr(1);
	auto find = m_items.find(relName);
	if (find != m_items.end())
	{
		std::string filePath = m_contentRoot;
		filePath += name;
		filePath += ".dat";

		find->second->Load(ContentReader(filePath.c_str()));
	}
	else
	{
		OutputDebugStringA("Item not loaded - skipping!\n");
	}
}

// -----------------------------------------------------------------------------

void
ContentManager::Update()
{
	m_gameConnection->Update();
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