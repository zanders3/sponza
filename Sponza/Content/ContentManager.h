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
#include <map>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

class PackReader;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class ContentManager
{
public:
	ContentManager(
		const std::string& contentRoot, 
		const std::string& packFile
	);

	template <typename T> T* Get( 
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

private:
	void
	ReadItem(
		const std::string& name,
		ContentItem* newItem
	);

	std::map<std::string, std::unique_ptr<ContentItem>> m_items;
	std::unique_ptr<PackReader>							m_packReader;
};

// -----------------------------------------------------------------------------

}//namespace content