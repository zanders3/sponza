#include "stdafx.h"
#include "Content/ContentLoader.h"

using namespace std;

ContentLoader::ContentLoader(const std::string& contentRoot)
{
	memset(&m_contentLoaded, 0, sizeof(bool)*ContentID::CONTENT_MAX);
	memset(m_contentData,	 0, sizeof(ContentItem*)*ContentID::CONTENT_MAX);

	const string manifest = contentRoot + "\\Manifest.txt";
	ifstream filestr(manifest);
	
	if (filestr.good())
	{
		char contentFile[256];

		size_t id = 0;
		while (!filestr.eof())
		{
			filestr.getline(contentFile, 255);
			if (*contentFile == 0) break;

			m_contentPaths[id] =  contentRoot;
			m_contentPaths[id] += contentFile;

			++id;
		}
	}

	filestr.close();

#ifdef _DEBUG
	m_socket = unique_ptr<Socket>(new Socket());
	if (!m_socket->Connect("127.0.0.1", 4567))
		cout << "Failed to connect to Contenter!" << endl;
#endif
}

ContentLoader::~ContentLoader()
{
	for (size_t id = 0; id<ContentID::CONTENT_MAX; ++id)
	{
		if (m_contentLoaded[id])
			delete m_contentData[id];
	}
}

void ContentLoader::Update()
{
#ifdef _DEBUG
	const string& data = m_socket->ReadData();
	if (data.size() > 0)
	{
		cout << "Got sum data:" << endl;
		cout << data << endl;
	}
#endif
}