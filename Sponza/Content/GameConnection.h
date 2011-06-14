/*#ifdef _DEBUG
#include "Net/Socket.h"
#include "Thread/Thread.h"
#include "Thread/Mutex.h"

class ContentLoader;

class ContentReloader
{
public:
	ContentReloader();
	~ContentReloader();

	void Run(int);

	void SetLoader(ContentLoader* pLoader) { m_pLoader = pLoader; }
	void ReloadContent();

private:
	void HandleCommand(const std::string& name, const std::string& data);

	bool						 m_threadRunning;
	std::unique_ptr<Socket>		 m_socket;
	ContentLoader*				 m_pLoader;

	std::vector<std::string>	 m_reloadList;
	Thread<ContentReloader, int> m_thread;
	Mutex						 m_mutex;
};
#endif*/