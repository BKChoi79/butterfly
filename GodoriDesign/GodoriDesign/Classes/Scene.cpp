#include "Scene.h"
#include <stdio.h>

Scene::Scene()
{
}

Scene::~Scene()
{
	printf("~Scene is called,....\n");
}

bool Scene::Init()
{
	printf("scene init is called,....\n");
	return true;
}

Scene* Scene::create()
{
	Scene* pScene = new Scene();
	if (nullptr != pScene  &&
		true == pScene->Init())
	{
		return pScene;
	}

	delete pScene;
	return nullptr;
}

void Scene::Proc()
{
	printf( "Scene Proc is called...\n" );
}

void Scene::ProcServer()
{
	printf("Scene ProcServer is called...\n");
}

void Scene::KeyPressed(int c)
{
	printf("Scene KeyPressed %d...\n", c );
}
