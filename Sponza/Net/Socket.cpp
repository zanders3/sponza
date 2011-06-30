// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Net/Socket.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace
{
	int socketCount = 0;
}

namespace net
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

Socket::Socket(
) : m_socket(INVALID_SOCKET)
{
	ZeroMemory(m_buffer, sizeof(m_buffer));
	if (socketCount == 0)
	{
		WSADATA data;
		int error = WSAStartup(0x0202, &data);
		if (error) assert(error);
	}
	++socketCount;
}

// -----------------------------------------------------------------------------

Socket::~Socket()
{
	if (m_socket != INVALID_SOCKET) 
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	if (--socketCount == 0)
	{
		WSACleanup();
	}
}

// -----------------------------------------------------------------------------

bool 
Socket::Connect(
	const std::string& ipaddress, 
	int port
)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ipaddress.c_str());

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
		return false;

	if (connect(m_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		m_socket = INVALID_SOCKET;
		return false;
	}
	else
	{
		u_long enable = 1;
		ioctlsocket(m_socket, FIONBIO, &enable);

		return true;
	}
}

// -----------------------------------------------------------------------------

const std::string 
Socket::ReadData()
{
	if (m_socket != INVALID_SOCKET)
	{
		WSAPOLLFD poll;
		poll.fd = m_socket;
		poll.events = POLLRDNORM;
		int pollResult = WSAPoll(&poll, 1, 0);
		if (pollResult == 1 && poll.revents == POLLRDNORM)
		{
			ZeroMemory(m_buffer, sizeof(m_buffer));
			int len = recv(m_socket, (char*)&m_buffer, 255, 0);
			if (len > 0)
			{
				return std::string(m_buffer);
			}
		}
	}

	return std::string();
}

// -----------------------------------------------------------------------------

}//namespace net