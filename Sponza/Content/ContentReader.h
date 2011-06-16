// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	ContentReader
//		- Provides a low level way to read a memory buffer of data.
//		- Used across all content loading classes to read data.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include <exception>
#include <string>

typedef unsigned int u32;

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class ContentReader
{
public:
	friend class PackReader;

    ContentReader(
        char* start,
        char* end
    );
    
    ContentReader(
        const std::string& file
	);
    
    ContentReader(
        ContentReader&& other
    );
    
    ~ContentReader();
    
private:
    ContentReader(const ContentReader& other) {}
    void operator =(const ContentReader& other) {}

public:
    template <typename T> T* Read(u32 count = 1)
    {
        T* item = reinterpret_cast<T*>(m_current);
        m_current += sizeof(T) * count;
        
        _assert(m_current <= m_end);
        
        return item;
    }
    
    template <typename T> T* ReadArray()
    {
        u32 size = *Read<u32>();
        return Read<T>(size);
    }

	u32 Size()
	{
		return m_end - m_start;
	}
    
private:
    char*   m_buffer;
	char*	m_start;
    char*   m_current;
    char*   m_end;
};

// -----------------------------------------------------------------------------

} //namespace content