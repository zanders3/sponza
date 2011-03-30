#include "stdafx.h"
#include "Content/ContentLoader.h"

using namespace std;

ContentLoader::ContentLoader(const std::string& contentRoot)
{
	const string manifest = contentRoot + "\\Manifest.txt";
	ifstream filestr(manifest);
	
	if (filestr.good())
	{
		char line[256];

		while (!filestr.eof())
		{
			filestr.getline(line, 255);
			if (*line == 0) break;

			std::string lineStr(line);

			ContentState state;
			state.m_loaded = false;
			state.m_data   = nullptr;

			int hashIndex = lineStr.find(',');
			size_t contentID = atoi(lineStr.substr(hashIndex+1).c_str());
			state.m_path = contentRoot + lineStr.substr(0, hashIndex);
			
			m_pContent.insert(std::pair<size_t, ContentState>(contentID, state));
		}
	}

	filestr.close();

#ifdef _DEBUG
	m_reloader.SetLoader(this);
#endif
}

ContentLoader::~ContentLoader()
{
	auto end = m_pContent.end();
	for (auto iter = m_pContent.begin(); iter != end; ++iter)
	{
		if (iter->second.m_loaded)
			delete iter->second.m_data;
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
		auto iter = m_pLoader->m_pContent.find(value);
		if (iter != m_pLoader->m_pContent.end())
		{
			iter->second.m_loaded = false;
			m_pLoader->Get<ContentItem>(value);
		}
	}
}
#endif