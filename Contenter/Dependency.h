#pragma once
#include <string>
#include "Socket.h"

// Connects to the contenter build service and requests a dependency ID.
class Dependency
{
public:
	Dependency();
	~Dependency();

	int RequestDependency(const std::string& filename);

private:
	Socket m_socket;
	bool   m_connected;
};