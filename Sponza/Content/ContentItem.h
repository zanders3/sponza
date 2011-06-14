// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
//	
//	ContentItem
//		- Base class for all game content types.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include <fstream>
#include "Content/ContentReader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class ContentManager;

class ContentItem
{
private:
	void operator=(const ContentItem& item);

public:
	ContentItem(
		ContentManager& pContent
	) : m_pDevice(*GetDevice()),
		m_pContent(pContent)
	{
	}

	virtual ~ContentItem() {}

	virtual void Load(ContentReader& reader) = 0;

protected:
	ID3D10Device&  m_pDevice;
	ContentManager& m_pContent;
};

// -----------------------------------------------------------------------------

}//namespace content