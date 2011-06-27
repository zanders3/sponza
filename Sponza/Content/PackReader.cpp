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
    const char* packFile
) : m_reader(packFile),
    m_packInfo()
{
    u32 numFiles = *m_reader.Read<u32>();
    m_packInfo.reserve(numFiles);

	u64 offset = 0;
    for (u32 i = 0; i<numFiles; ++i)
    {
		char* filePath = m_reader.ReadArray<char>();
		u64 fileSize = *m_reader.Read<u64>();

        PackInfo info =
        {
            filePath,
			nullptr,
			nullptr,
			fileSize
        };
        m_packInfo.push_back(info);
    }

	m_front = m_reader.m_current;
	m_end = m_reader.m_end;

	char* current = m_front;
	for (u32 i = 0; i<numFiles; ++i)
	{
		PackInfo& info = m_packInfo[i];

		info.m_start = current;
		current += info.m_fileSize;
		info.m_end = current;

		//Do some sanity checks
		_assert(info.m_start >= m_front);
		_assert(info.m_end >= info.m_start);
		_assert(info.m_end <= m_end);
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
			return ContentReader(iter->m_start, iter->m_end);
        }
    }
    
    std::string message = "Missing content: ";
    message += fileName;
    throw std::exception(message.c_str());
}

// -----------------------------------------------------------------------------

}//namespace content