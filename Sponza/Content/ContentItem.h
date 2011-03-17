#pragma once
#include "stdafx.h"

class ContentItem
{
private:
	ContentItem(const ContentItem& item) {}
	void operator=(const ContentItem& item) {}

public:
	ContentItem() {}
	virtual ~ContentItem() {}

	ID3D10Device* m_pDevice;
};