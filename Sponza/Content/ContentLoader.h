#pragma once

#include "Content/ContentIDs.h"
#include <string>

class ContentLoader
{
public:
	ContentLoader(const std::string& contentRoot);
	~ContentLoader();

	template <typename T> T* Get( ContentID::Type contentId )
	{
		if (!m_contentLoaded[contentId])
			m_contentData[contentId] = new T(m_contentPaths[contentId]);

		m_contentLoaded[contentId] = true;
		return static_cast<T*>(m_contentData[contentId]);
	}

private:
	bool			m_contentLoaded[ContentID::CONTENT_MAX];
	void *			m_contentData  [ContentID::CONTENT_MAX];
	std::string 	m_contentPaths [ContentID::CONTENT_MAX];
};