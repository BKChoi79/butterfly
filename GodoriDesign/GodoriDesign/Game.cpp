#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

Game::Game()
{
	turn = USER;
	memset(board, -1, sizeof(board));
}

Game::~Game()
{
}

Game* Game::create()
{
	Game* pRet = new Game();
	if (nullptr != pRet && true == pRet->Init())
	{
		return pRet;
	}

	delete pRet;
	return nullptr;
}


bool Game::Init()
{
	turn = USER;
	memset(board, -1, sizeof(board));
	return true;
}


void Game::SetBoard(int idx, int val)
{
	board[idx] = val;
}

int* Game::GetBoard()
{
	return board;
}

TURN Game::GetTurn()
{
	return turn;
}

void Game::TurnChange()
{
	if (USER == turn)
	{
		turn = SERVER;
	}
	else
	{
		turn = USER;
	}
}