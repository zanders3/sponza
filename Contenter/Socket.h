#pragma once
#include <string>
#include "winsock2.h"

class Socket
{
public:
	Socket();
	~Socket();

	bool Connect(const std::string& ipaddress, int port);

	const std::string& ReadData();
	void WriteData(const std::string& data);

private:
	SOCKET mSocket;
	std::string mData;
	char mBuffer[256];
	static int mSocketCount;
};