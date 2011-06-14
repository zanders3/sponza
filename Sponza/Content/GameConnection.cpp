/*#ifdef _DEBUG
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
		const std::string* data = m_socket->ReadData();
		if (data != nullptr)
		{
			Lock lock(&m_mutex);
			m_reloadList.push_back(*data);
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

	m_reloadList.clear();
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
#endif*/