#pragma once

#include "stdafx.h"

#include "Content/ContentIDs.h"
#include "Content/ContentItem.h"
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#ifdef _DEBUG
#include "Net/Socket.h"
#include "Thread/Thread.h"
#include "Thread/Mutex.h"

class ContentLoader;

class ContentReloader
{
public:
	ContentReloader();
	~ContentReloader();

	void Run(int);

	void SetLoader(ContentLoader* pLoader) { m_pLoader = pLoader; }
	void ReloadContent();

private:
	void HandleCommand(const std::string& name, const std::string& data);

	bool						 m_threadRunning;
	std::unique_ptr<Socket>		 m_socket;
	ContentLoader*				 m_pLoader;

	std::vector<std::string>	 m_reloadList;
	Thread<ContentReloader, int> m_thread;
	Mutex						 m_mutex;
};
#endif

class ContentLoader
{
public:
	friend class ContentReloader;

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
			if (m_contentData[contentId] == nullptr)
			{
				m_contentData[contentId] = new T();
			}

			std::ifstream infile;
			std::string& path = m_contentPaths[contentId];
			infile.open(path, std::ios::in|std::ios::binary);

			if (infile.good())
			{
				ContentItem* item = m_contentData[contentId];
				item->m_pDevice = m_pDevice;
				item->Load(infile);
				
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

#ifdef _DEBUG
	ContentReloader m_reloader;

public:
	void ReloadContent() { m_reloader.ReloadContent(); }
#endif
};