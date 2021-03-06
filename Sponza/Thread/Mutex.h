// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
//	
//	Mutex
//		- Represents a mutable object that can be locked for threading stuff.
// -----------------------------------------------------------------------------
#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace thread
{
	
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Mutex
{
public:
	friend class Lock;

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

// -----------------------------------------------------------------------------

class Lock
{
public:
	Lock(const Mutex& mutex) : m_mutex(mutex)
	{
		WaitForSingleObject(m_mutex.m_handle, INFINITE);
	}
	~Lock()
	{
		ReleaseMutex(m_mutex.m_handle);
	}

private:
	const Mutex& m_mutex;
};

// -----------------------------------------------------------------------------

} //namespace thread