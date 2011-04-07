#include "stdafx.h"
#include "Net/Socket.h"

int Socket::mSocketCount = 0;

Socket::Socket() :
	mSocket(INVALID_SOCKET),
	mData()
{
	if (mSocketCount == 0)
	{
		WSADATA data;
		int error = WSAStartup(0x0202, &data);
		if (error) throw error;
	}
	++mSocketCount;
}

Socket::~Socket()
{
	if (mSocket != INVALID_SOCKET) closesocket(mSocket);

	if (--mSocketCount == 0)
	{
		WSACleanup();
	}
}

bool Socket::Connect(const std::string& ipaddress, int port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ipaddress.c_str());

	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
		return false;

	if (connect(mSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		mSocket = INVALID_SOCKET;
		return false;
	}
	else
	{
		u_long enable = 1;
		ioctlsocket(mSocket, FIONBIO, &enable);

		return true;
	}
}

const std::string* Socket::ReadData()
{
	if (mSocket != INVALID_SOCKET)
	{
		int len = recv(mSocket, (char*)&mBuffer, 255, 0);
		if (len > 0)
		{
			mData = std::string(mBuffer, len);
			return &mData;
		}
	}
	return nullptr;
}