#include "Dependency.h"

Dependency::Dependency() : m_connected(false)
{
}

Dependency::~Dependency()
{
}

int Dependency::RequestDependency(const std::string& filename)
{
	if (!m_connected)
	{
		if (!m_socket.Connect("127.0.0.1", 5678))
			return -1;

		m_connected = true;
	}

	m_socket.WriteData(filename + "\n");
	const std::string& idStr = m_socket.ReadData();

	int id = atoi(idStr.c_str());
	return id;
}