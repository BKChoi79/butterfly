#include "AppMain.h"

AppMain::AppMain()
{
	count = 0;
	pScene = nullptr;
}

AppMain::~AppMain()
{
	if (nullptr != pScene)
	{
		delete pScene;
		pScene = nullptr;
	}

	printf("~AppMain called.");
}

bool AppMain::Init()
{
	printf("AppMain init called.\n");
	return true;
}

void AppMain::Run(Scene* pScene)
{
	this->pScene = pScene;
		
	std::thread fnThreadMainProc(std::bind(&AppMain::Proc, this), nullptr);
	std::thread fnThreadKeyProc(std::bind(&AppMain::Proc_Event, this), nullptr);
	std::thread fnThreadServerProc(std::bind(&AppMain::Proc_Server, this), nullptr);

#ifdef DETACH_THREAD
	fnThreadMainProc.detach();
	fnThreadKeyProc.detach();
	fnThreadServerProc.detach();
#else
	fnThreadMainProc.join();
	fnThreadKeyProc.join();
	fnThreadServerProc.join();
#endif
}

void AppMain::Proc()
{
	while (true)
	{
		mutex.lock(); // lock
		DWORD id = GetCurrentThreadId();
		int n = count;
		n = n + 1;

		if (nullptr != pScene)
		{
			pScene->Proc();
		}

		count = n;
		mutex.unlock(); // unlock

		//printf("[thread id %d]count = %d\n", id, count);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void AppMain::Proc_Event()
{
	while (true)
	{
		int c = _getch();
		//printf("%d\n", c);
		switch (c)
		{
		case VK_ESCAPE:
			exit(0);
			break;

		default:
			if (nullptr != pScene)
			{
				pScene->KeyPressed(c);
			}
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void AppMain::Proc_Server()
{
	while (true)
	{
		if (nullptr != pScene)
		{
			pScene->ProcServer();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}