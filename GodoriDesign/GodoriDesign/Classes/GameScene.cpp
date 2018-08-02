#include "GameScene.h"
#include <stdio.h>


GameScene::GameScene()
{
	pBuffer = nullptr;
}

GameScene::~GameScene()
{
	printf("~GameScene is called.");

	if (nullptr != pBuffer)
	{
		delete pBuffer;
		pBuffer = nullptr;
	}

	if (nullptr != pServer)
	{
		delete pServer;
		pServer = nullptr;
	}
}

bool GameScene::Init()
{
	//printf("GameScene init is called....\n");
	pBuffer = Buffer::create();
	if (nullptr == pBuffer)
	{
		return false;
	}

	pServer = GameServer::create();
	if (nullptr == pServer)
	{
		return false;
	}

	pServer->SetRepondCallback(std::bind(&GameScene::OnRecive, this, std::placeholders::_1));

	printf("====================================================\n");
	printf("press 's' to start! \n");
	printf("====================================================\n");
	return true;
}

GameScene* GameScene::create()
{
	GameScene* pScene = new GameScene();
	if (nullptr != pScene &&
		true == pScene->Init())
	{
		return pScene;
	}

	delete pScene;
	return nullptr;
}

void GameScene::Proc()
{
	//printf("GameScene Proc is called...\n");
}

void GameScene::ProcServer()
{
	if (nullptr != pServer)
	{
		if (0 < pBuffer->Size())
		{
			Json::Value v = pBuffer->Pop();
			pServer->Read(v);
		}
	}
}

void GameScene::KeyPressed(int c)
{
	//printf("GameScene KeyPressed %d...\n", c);
	Json::Value v;
	std::string str;

	switch (c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
		int index = c - '0';
		v[PT] = PROTOCOLS::REQUEST_CARD_PICK;
		v["turn"] = USER;
		v["card_no"] = 0;
		v["card_index"] = index;
		pBuffer->Push(v);
		break;
	}
	case 'q':
		v[PT] = 10000;
		v["msg"] = "hellow";
		pBuffer->Push(v);

	   break;
	case 'w':
		v = pBuffer->Pop();
		if (false == v.isNull())
		{
			str = v.toStyledString();
			printf("Pop : %s", str.c_str());
		}
		break;
	
	case 's':
		v[PT] = PROTOCOLS::REQUEST_GAME_START;
		pBuffer->Push(v);
		break;

	case 'c':
		v[PT] = PROTOCOLS::REQUEST_CLEAR_SCREEN;
		pBuffer->Push(v);
		break;

	default:
		break;
	}
}

/**
 OnRecive
  - 서버로 부터 전달받는 결과
*/
void GameScene::OnRecive(Json::Value v)
{
	if (true == v[PT].isNull())
	{
		return;
	}

	int protocol = v[PT].asInt();
	switch (protocol)
	{
		case PROTOCOLS::RESPOND_GAMEOVER:
		{
			printf("# Game is over....\n\n\n");

			std::string str = v.toStyledString();
			printf("%s\n", str.c_str());
			break;
		}

		case PROTOCOLS::RESPOND_CLEAR_SCREEN:
			system("cls");
			break;

		case PROTOCOLS::RESPOND_TURN_CHANGE:
		{
			TURN turn = (TURN)v["turn"].asInt();
			if (SERVER == turn)
				printf("# Server Turn.....\n");
			else
				printf("# User Turn.....\n");

			break;
		}
		case PROTOCOLS::RESPOND_CARD_LIST:
		{
			printf("\n\n\n");

			int n = v["board"].size();
			for (int i = 0; i < n; i++)
			{
				char mark[2] = { 'O', 'X' };
				char displayChar = '-';

				int n = v["board"][i].asInt();
				if (0 <= n)
				{
					displayChar = mark[n];
				}

				if (0 == i % 3)
				{
					printf("            %c", displayChar);
				}
				else
				{
					printf(" %c", displayChar);
				}
				
				if ( 0 == (i + 1 ) % 3)
				{
					printf("\n");
				}
			}

			printf("\n\n\n");

			break;
		}
			

		default:
		{
			std::string str = v.toStyledString();
			printf("%s\n", str.c_str());
			break;
		}
	}


}