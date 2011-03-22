#pragma once
#include "stdafx.h"
#include <fstream>

class ContentItem
{
private:
	ContentItem(const ContentItem& item) {}
	void operator=(const ContentItem& item) {}

public:
	ContentItem() {}
	virtual ~ContentItem() {}

	virtual void Load(std::istream& input)
	{
		throw std::exception("WTF?");
	}

	ID3D10Device* m_pDevice;
};