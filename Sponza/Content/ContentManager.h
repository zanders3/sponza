// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	ContentLoader
//		- Loads content items from a pack file or from disk if not found in the pack file.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include "Content/ContentItem.h"
#include <unordered_map>
#include <functional>
#include <memory>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

class PackReader;
class ContentQueue;

typedef std::function<void (ContentItem*)> ContentLoadedCallback;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class ContentManager
{
public:
	friend class ContentQueue;

	ContentManager(
		const std::string& contentRoot, 
		const std::string& packFile
	);

	~ContentManager();

	template <typename T> T* GetContent( 
		const std::string& name 
	)
	{
		//Has the item already been loaded?
		auto find = m_items.find(name);
		if (find != m_items.end())
		{
			return static_cast<T*>(find->second.get());
		}
		else
		{
			T* newItem = new T();
			ReadItem(name, newItem);
			return newItem;
		}
	}

	template <typename T> void GetContentAsync(
		const std::string&		name,
		ContentLoadedCallback	callback
	)
	{
		//Has the item already been loaded?
		auto find = m_items.find(name);
		if (find != m_items.end())
		{
			callback(find->second.get());
		}
		else
		{
			T* newItem = new T();
			ReadItemAsync(name, newItem, callback);
		}
	}

	void
	Update();

private:
	void
	ReadItem(
		const std::string& name,
		ContentItem* newItem
	);

	void
	ReadItemAsync(
		const std::string&		name,
		ContentItem*			newItem,
		ContentLoadedCallback&	callback
	);

	std::unordered_map<std::string, std::unique_ptr<ContentItem>>	m_items;
	std::unique_ptr<PackReader>										m_packReader;

	std::unique_ptr<ContentQueue>									m_contentQueue;
};

// -----------------------------------------------------------------------------

}//namespace content