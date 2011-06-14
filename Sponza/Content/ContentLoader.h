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
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <map>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class ContentLoader
{
public:
	ContentLoader(const std::string& contentRoot, const std::string& packFile);
	~ContentLoader();

	template <typename T> T* Get( const std::string& name )
	{
		auto iter = m_pContent.find(contentID);

		if (iter == m_pContent.end())
		{
			std::stringstream str;
			str << "Failed to load asset ID: " << contentID << std::endl;
			throw std::exception(str.str().c_str());
		}

		ContentState state = iter->second;

		if (!state.m_loaded)
		{
			if (state.m_data == nullptr)
			{
				state.m_data = new T();
			}

			std::ifstream infile;
			infile.open(state.m_path, std::ios::in|std::ios::binary);

			if (infile.good())
			{
				ContentItem* item = state.m_data;
				item->m_pDevice = m_pDevice;
				item->m_pContent = this;
				item->Load(infile);
				
				state.m_loaded = true;
				m_pContent[contentID] = state;
			}

			infile.close();
		}

		return static_cast<T*>(state.m_data);
	}

private:
	
};

// -----------------------------------------------------------------------------

}//namespace content