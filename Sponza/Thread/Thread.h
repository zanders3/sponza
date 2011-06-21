// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Thread
//		- Allows for threading!
// -----------------------------------------------------------------------------
#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include <functional>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace thread
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Thread
{
public:
	typedef std::function<void ()> ThreadStart;

	Thread(
		ThreadStart callback
	);
	~Thread();

	// Runs the callback on a different thread.
	bool Run();

	bool IsRunning()
	{
		return m_isRunning;
	}

private:
	Thread(const Thread& other) {}
	void operator =(const Thread& other) {}

	// Static Thread Proc - The ThreadProc called by the
	// Windows CreateThread() function.  The parameter is
	// a pointer to the thread instance that is being
	// started.  
	static DWORD WINAPI StartThread( void* pParam );

	ThreadStart		m_callback;
	HANDLE			m_hThread;
	DWORD			m_threadID;

	volatile bool	m_isRunning;
};

// -----------------------------------------------------------------------------

} //namespace thread