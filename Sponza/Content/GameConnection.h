// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	PackReader
//		- Reads the content pack file into a block of memory to speed loading.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include <memory>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace net
{
	class Socket;
}

namespace content
{
	class ContentManager;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class GameConnection
{
public:
	GameConnection(
		ContentManager& manager
	);
	~GameConnection();

	void Update();

private:
	ContentManager&					m_contentManager;
	std::unique_ptr<net::Socket>	m_socket;
};

// -----------------------------------------------------------------------------

} //namespace content

/*#ifdef _DEBUG
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
#endif*/