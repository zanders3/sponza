// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Content/ContentManager.h"
#include "Content/PackReader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

ContentManager::ContentManager(
	const std::string& contentRoot, 
	const std::string& packFile
) : m_items(),
	m_packReader()
{
	m_packReader.reset(new PackReader(packFile));
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
	newItem->Load(reader);
}

// -----------------------------------------------------------------------------

}//namespace content