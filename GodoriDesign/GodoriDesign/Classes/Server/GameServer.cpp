#include "GameServer.h"
#include "Protocols.h"
#include "Buffer.h"
#include <cstdlib>

GameServer::GameServer()
{
	pickCardTrriger = false;
	pGame = nullptr;
	respondCallback = nullptr;
}

GameServer::~GameServer()
{
	if (nullptr != pGame)
	{
		delete pGame;
		pGame = nullptr;
	}

	if (nullptr != pBuffer)
	{
		delete pBuffer;
		pBuffer = nullptr;
	}

	respondCallback = nullptr;
}

GameServer* GameServer::create()
{
	GameServer* pRet = new GameServer();
	if (nullptr != pRet && true == pRet->Init())
	{
		return pRet;
	}

	delete pRet;
	return nullptr;
}

bool GameServer::Init()
{
	respondCallback = nullptr;

	pBuffer = Buffer::create();
	if (nullptr == pBuffer)
	{
		return false;
	}

	std::thread fnProc(std::bind(&GameServer::Proc, this), nullptr);
	fnProc.detach();

	pickCardTrriger = false;
	return true;
}

bool GameServer::Read(Json::Value v)
{
	if (nullptr == respondCallback)
	{
		return false;
	}

	pBuffer->Push(v);

	std::string str = v.toStyledString();
	printf("push : %s", str.c_str());
	return true;
}

void GameServer::Parse(Json::Value v)
{
	int protocol = v[PT].asInt();

	auto ScreenClear = [](std::function< void(Json::Value v) > callback) -> void
	{
		Json::Value json;
		json[PT] = RESPOND_CLEAR_SCREEN;
		json["msg"] = "clear. +_+";
		callback(json);
	};

	switch (protocol)
	{
		case REQUEST_REFRASH:
		{
			ScreenClear(respondCallback);

			pGame = Game::create();

			int* pBoard = pGame->GetBoard();
			Json::Value jsonBoard;
			jsonBoard[PT] = RESPOND_CARD_LIST;
			for (int i = 0; i < MAX_BOARD; i++)
			{
				int n = pBoard[i];
				jsonBoard["board"][i] = n;
			}

			respondCallback(jsonBoard);
			break;
		}
		case REQUEST_GAME_START:
		{
			if (nullptr == pGame)
			{
				ScreenClear(respondCallback);

				pGame = Game::create();

				int* pBoard = pGame->GetBoard();
				Json::Value jsonBoard;
				jsonBoard[PT] = RESPOND_CARD_LIST;
				for (int i = 0; i < MAX_BOARD; i++)
				{
					int n = pBoard[i];
					jsonBoard["board"][i] = n;
				}

				respondCallback(jsonBoard);

				Json::Value json;
				json[PT] = RESPOND_GAME_START;
				json["msg"] = "hellow client. 1~9 key press.";
				respondCallback(json);
			}
			else
			{
				Json::Value json;
				json[PT] = RESPOND_ERR;
				json["msg"] = "game is allready started.";
				respondCallback(json);
			}
			break;
		}

	case REQUEST_CARD_PICK:
	{
		if (nullptr == pGame)
		{
			ScreenClear(respondCallback);

			Json::Value json;
			json[PT] = RESPOND_ERR;
			json["msg"] = "game is not started. press 's' button";
			respondCallback(json);
		}
		else
		{
			TURN turnParam = (TURN)v["turn"].asInt();
			TURN turn = pGame->GetTurn();
			if (turnParam != turn)
			{
				ScreenClear(respondCallback);

				int* pBoard = pGame->GetBoard();
				Json::Value jsonBoard;
				jsonBoard[PT] = RESPOND_CARD_LIST;
				for (int i = 0; i < MAX_BOARD; i++)
				{
					int n = pBoard[i];
					jsonBoard["board"][i] = n;
				}

				respondCallback(jsonBoard);

				Json::Value json;
				json[PT] = RESPOND_ERR;
				json["msg"] = "not your turn. wait...";
				respondCallback(json);
			}
			else
			{
				Json::Value json;

				int cardIndex = v["card_index"].asInt() - 1;
				if (0 <= cardIndex && MAX_BOARD > cardIndex)
				{
					int* pBoard = pGame->GetBoard();
					int n = pBoard[cardIndex];
					if (n == -1)
					{
						// 스크린을 한번 지우게하고.
						ScreenClear(respondCallback);

						// 턴인 사람의 값으로 보드를 채웁니다.
						pGame->SetBoard(cardIndex, turn);

						int emptyCount = 0;
						json[PT] = RESPOND_CARD_LIST;
						for (int i = 0; i < MAX_BOARD; i++)
						{
							int n = pBoard[i];
							json["board"][i] = n;
							if (n == -1)
							{
								emptyCount++;
							}
						}

						respondCallback(json);

						// 턴을 변경합니다.
						pGame->TurnChange();
						turn = pGame->GetTurn();
						if (turn == SERVER)
						{
							pickCardTrriger = true;
						}

						Json::Value jsonTurn;
						jsonTurn[PT] = RESPOND_TURN_CHANGE;
						jsonTurn["turn"] = turn;
						respondCallback(jsonTurn);

						// 게임이 끝난 경우 결과를 보냅니다.
						if (0 == emptyCount)
						{
							Json::Value jsonGameOver;
							jsonGameOver[PT] = RESPOND_GAMEOVER;
							jsonGameOver["msg"] = "game over.. press 's' new game.";
							respondCallback(jsonGameOver);

							delete pGame;
							pGame = nullptr;
						}
					}
					else
					{
						// 스크린을 한번 지우게하고.
						ScreenClear(respondCallback);

						Json::Value jsonBoard;
						jsonBoard[PT] = RESPOND_CARD_LIST;
						for (int i = 0; i < MAX_BOARD; i++)
						{
							int n = pBoard[i];
							jsonBoard["board"][i] = n;
						}

						respondCallback(jsonBoard);

						Json::Value jsonErr;
						jsonErr[PT] = RESPOND_ERR;
						jsonErr["msg"] = "allready play cell.";
						jsonErr["cardIndex"] = cardIndex;
						respondCallback(jsonErr);

						if (turn == SERVER)
						{
							pickCardTrriger = true;
						}
					}
				}
				else
				{
					json[PT] = RESPOND_ERR;
					json["msg"] = "index must be 1 ~ 9";
					respondCallback(json);
				}
			}
		}

		break;
	}

	case REQUEST_CLEAR_SCREEN:
		ScreenClear(respondCallback);
		break;

	default:
		break;
	}
}

void GameServer::Proc()
{
	while (true)
	{
		if (nullptr != pBuffer)
		{
			int n = pBuffer->Size();
			if (0 < n)
			{
				Json::Value v = pBuffer->Pop();
				Parse(v);
			}
		}

		if (true == pickCardTrriger)
		{
			PickCard();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void GameServer::PickCard()
{
	if (nullptr != pGame)
	{
		// 서버의 턴이라면...
		if (SERVER == pGame->GetTurn())
		{
			int* pBoard = pGame->GetBoard();
			std::vector<int> listEmpty;
			for (int i = 0; i < MAX_BOARD; i++)
			{
				if (-1 == pBoard[i])
				{
					listEmpty.push_back(i);
				}
			}

			int n = rand() % listEmpty.size();
			int index = listEmpty.at(n);

			Json::Value json;
			json[PT] = PROTOCOLS::REQUEST_CARD_PICK;
			json["turn"] = SERVER;
			json["card_no"] = 0;
			json["card_index"] = index + 1;
			pBuffer->Push(json);

			pickCardTrriger = false;
		}
	}
}

void GameServer::SetRepondCallback(std::function< void(Json::Value v) > callback)
{
	respondCallback = callback;
}