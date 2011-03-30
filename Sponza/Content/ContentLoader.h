#pragma once

#include "stdafx.h"

#include "Content/ContentIDs.h"
#include "Content/ContentItem.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <map>

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

struct ContentState
{
	bool		 m_loaded;
	std::string  m_path;
	ContentItem* m_data;
};

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

	template <typename T> T* Get( ContentID::Type contentID )
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
	std::map<size_t, ContentState>	m_pContent;
	ID3D10Device*					m_pDevice;

#ifdef _DEBUG
	ContentReloader m_reloader;

public:
	void ReloadContent() { m_reloader.ReloadContent(); }
#endif
};