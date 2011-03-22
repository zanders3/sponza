#pragma once
#include "stdafx.h"

class Lock;

class Mutex
{
public:
	friend Lock;

	Mutex()
	{
		m_handle = CreateMutex(NULL, FALSE, NULL);
	}
	~Mutex()
	{
		CloseHandle(m_handle);
	}

private:
	HANDLE m_handle;
};

class Lock
{
public:
	Lock(const Mutex* mutex) : m_mutex(mutex)
	{
		WaitForSingleObject(m_mutex->m_handle, INFINITE);
	}
	~Lock()
	{
		ReleaseMutex(m_mutex->m_handle);
	}

private:
	const Mutex* m_mutex;
};