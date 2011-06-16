// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Content/ContentReader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

ContentReader::ContentReader(
    char* start,
    char* end
) : m_buffer(nullptr),
    m_current(start),
    m_end(end)
{
}

// -----------------------------------------------------------------------------
    
ContentReader::ContentReader(
    const std::string& file)
{
    FILE* pFile;
	fopen_s(&pFile, file.c_str(), "rb");

    fseek(pFile, 0, SEEK_END);
    long size = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
        
    m_buffer = new char[size];
    fread(m_buffer, size, 1, pFile);
        
    fclose(pFile);
        
    m_current = m_buffer;
    m_end = m_buffer;
}
    
// -----------------------------------------------------------------------------

ContentReader::ContentReader(
    ContentReader&& other
) : m_buffer(other.m_buffer),
    m_current(other.m_current),
    m_end(other.m_end)
{
}

// -----------------------------------------------------------------------------

ContentReader::~ContentReader()
{
    delete[] m_buffer;
}

// -----------------------------------------------------------------------------

}//namespace content