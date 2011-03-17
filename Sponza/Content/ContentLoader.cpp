#include "stdafx.h"
#include "Content/ContentLoader.h"

ContentLoader::ContentLoader(const std::string& contentRoot)
{
	memset(&m_contentLoaded, 0, sizeof(bool)*ContentID::CONTENT_MAX);

	const std::string manifest = contentRoot + "\\Manifest.txt";
	FILE * pFile = fopen(manifest.c_str(), "r");
	
	if (pFile == NULL) throw std::exception("Manifest file not found!");
	char contentFile[256];

	int rootLength = contentRoot.size();
	size_t id = 0;
	while (!feof(pFile) && id < ContentID::CONTENT_MAX)
	{
		fgets( contentFile, 256, pFile );
		m_contentPaths[id] =  contentRoot;
		m_contentPaths[id] += std::string(contentFile);

		++id;
	}

	fclose( pFile );
}

ContentLoader::~ContentLoader()
{
	for (size_t id = 0; id<ContentID::CONTENT_MAX; ++id)
	{
		if (m_contentLoaded[id])
			delete m_contentData[id];
	}
}