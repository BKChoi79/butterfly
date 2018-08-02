#ifndef __GAME_SERVER_H__
#define  __GAME_SERVER_H__

#include "JsonCpp/json/json.h"
#include <functional>
#include <queue>
#include <thread>
#include <mutex>

#include "Game.h"
#include "Buffer.h"


class GameServer
{
public:
	GameServer();
	~GameServer();
	static GameServer* create();
	bool Init();
	
	bool Read(Json::Value v);
	void Parse(Json::Value v);
	void Proc();
	void SetRepondCallback(std::function< void(Json::Value v) > callback);
	void PickCard();

private:
	std::function< void(Json::Value v) > respondCallback;
	Buffer* pBuffer;
	Game* pGame;

	bool pickCardTrriger;
};

#endif
