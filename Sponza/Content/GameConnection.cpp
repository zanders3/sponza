// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Content/GameConnection.h"

#include "Content/ContentManager.h"
#include "Net/Socket.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

GameConnection::GameConnection(
	ContentManager& manager
) : m_contentManager(manager)
{
	m_socket.reset(new net::Socket());
	if (!m_socket->Connect("127.0.0.1", 1234))
	{
		m_socket.release();
	}
}

// -----------------------------------------------------------------------------

GameConnection::~GameConnection()
{
}

// -----------------------------------------------------------------------------

void 
GameConnection::Update()
{
	if (m_socket)
	{
		const std::string& message = m_socket->ReadData();
		if (!message.empty())
		{
			OutputDebugStringA("[Game Connection]: ");
			OutputDebugStringA(message.c_str());

			if (message.find("Reload:") != std::string::npos)
			{
				m_contentManager.ReloadContent(message.substr(8));
			}
		}
	}
}

// -----------------------------------------------------------------------------

}//namespace content