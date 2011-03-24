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
	m_reloader.SetLoader(this);
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

#ifdef _DEBUG
ContentReloader::ContentReloader() :
	m_threadRunning(true)
{
	m_socket = unique_ptr<Socket>(new Socket());
	if (!m_socket->Connect("127.0.0.1", 4567))
	{
		m_threadRunning = false;
	}
	else
	{
		m_thread.Run(this, &ContentReloader::Run, 0);
	}
}

ContentReloader::~ContentReloader()
{
	m_threadRunning = false;
}

void ContentReloader::Run(int)
{
	while (m_threadRunning)
	{
		const std::string& data = m_socket->ReadData();
		if (data.size() > 0)
		{
			Lock lock(&m_mutex);
			m_reloadList.push_back(data);
		}
	}
}

void ContentReloader::ReloadContent()
{
	Lock lock(&m_mutex);
	for (size_t i = 0; i<m_reloadList.size(); ++i)
	{
		int s = m_reloadList[i].find(' ');
		if (s > 0)
		{
			const std::string commandName = m_reloadList[i].substr(0, s);
			const std::string commandData = m_reloadList[i].substr(s + 1);

			HandleCommand(commandName, commandData);
		}
	}
}

void ContentReloader::HandleCommand(const std::string& name, const std::string& data)
{
	if (name == "LOAD")
	{
		ContentID::Type value = static_cast<ContentID::Type>(atoi(data.c_str()));
		if (value >= 0 && value < ContentID::CONTENT_MAX)
		{
			m_pLoader->m_contentLoaded[value] = false;
			m_pLoader->Get<ContentItem>(value);
		}
	}
}
#endif