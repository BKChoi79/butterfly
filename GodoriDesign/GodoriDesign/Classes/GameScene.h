#ifndef __GAME_SCENE_H__
#define  __GAME_SCENE_H__

#include "Scene.h"
#include "Buffer.h"
#include "protocols.h"
#include "Server/GameServer.h"
#include <vector>

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

	static GameScene* create();

	virtual bool Init();
	virtual void Proc();
	virtual void ProcServer();
	virtual void KeyPressed(int c);

	void OnRecive(Json::Value v);
private:
	
	Buffer* pBuffer;
	GameServer* pServer;

};

#endif