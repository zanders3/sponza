// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Content/PackReader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

PackReader::PackReader(
    const std::string& packFile
) : m_reader(packFile),
    m_packInfo()
{
    u32 numFiles = m_reader.Read<u32>();
    m_packInfo.reserve(numFiles);
    for (u32 i = 0; i<numFiles; ++i)
    {
        PackInfo info =
        {
            m_reader.ReadArray<char>(),
            m_reader.Read<u64>(),
            m_reader.Read<u32>()
        };
        m_packInfo.push_back(info);
    }
}

// -----------------------------------------------------------------------------

ContentReader
PackReader::Get(
	const std::string& fileName
)
{
    for (auto iter = m_packInfo.begin(); iter != m_packInfo.end(); ++iter)
    {
        if (iter->m_file == fileName)
        {
            return ContentReader(m_reader.m_buffer + iter->m_offset, m_reader.m_buffer + iter->m_offset + iter->m_size);
        }
    }
        
    std::string message = "Missing content: ";
    message += fileName;
    throw std::exception(message);
}

// -----------------------------------------------------------------------------

}//namespace content