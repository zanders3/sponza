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

class ContentLoader;

class ContentItem
{
private:
	ContentItem(const ContentItem& item) {}
	void operator=(const ContentItem& item) {}

public:
	ContentItem() {}
	virtual ~ContentItem() {}

	virtual void Load(ContentReader& reader) = 0;

protected:
	ID3D10Device*  m_pDevice;
	ContentLoader* m_pContent;
};

// -----------------------------------------------------------------------------

}//namespace content