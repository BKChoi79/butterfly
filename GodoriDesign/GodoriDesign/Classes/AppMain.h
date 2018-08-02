#ifndef __APP_MAIN_H__
#define  __APP_MAIN_H__

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "Singleton.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <functional>
#include <vector>
#include "Scene.h"

class AppMain : public Singleton< AppMain >
{
public:
	AppMain();
	~AppMain();
	virtual bool Init();

	void Run(Scene* pScene);
	void Proc();
	void Proc_Event();
	void Proc_Server();
private:
	int count;
	std::mutex mutex;
	Scene* pScene;
};

#endif