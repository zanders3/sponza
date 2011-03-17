#pragma once

#include "stdafx.h"

#include "Content/ContentIDs.h"
#include "Content/ContentItem.h"
#include <string>
#include <iostream>
#include <fstream>

class ContentLoader
{
public:
	ContentLoader(const std::string& contentRoot);
	~ContentLoader();

	inline void SetDevice(ID3D10Device* pDevice)
	{
		m_pDevice = pDevice;
	}

	template <typename T> T* Get( ContentID::Type contentId )
	{
		if (!m_contentLoaded[contentId])
		{
			if (m_contentData[contentId] != nullptr)
			{
				delete m_contentData[contentId];
				m_contentData[contentId] = nullptr;
			}

			std::ifstream infile;
			std::string& path = m_contentPaths[contentId];
			infile.open(path, std::ios::in|std::ios::binary);

			if (infile.good())
			{
				T* t = new T();
				t->m_pDevice = m_pDevice;
				t->Load(infile);
				m_contentData[contentId] = t;
				m_contentLoaded[contentId] = true;
			}

			infile.close();
		}

		return static_cast<T*>(m_contentData[contentId]);
	}

private:
	bool			m_contentLoaded[ContentID::CONTENT_MAX];
	ContentItem *	m_contentData  [ContentID::CONTENT_MAX];
	std::string 	m_contentPaths [ContentID::CONTENT_MAX];
	ID3D10Device*	m_pDevice;
};