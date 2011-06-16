// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	PackReader
//		- Reads the content pack file into a block of memory to speed loading.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include <vector>
#include <exception>
#include "Content/ContentReader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------
struct PackInfo
{
    std::string m_file;
    char*       m_start;
    char*       m_end;
	u64			m_fileSize;
};

class PackReader
{
public:
    PackReader(
        const std::string& packFile
    );

    ContentReader Get(const std::string& fileName);
private:
	char*						m_front;
	char*						m_end;

    ContentReader               m_reader;
    std::vector<PackInfo>       m_packInfo;
};

// -----------------------------------------------------------------------------

} //namespace content