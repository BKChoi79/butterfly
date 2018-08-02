#ifndef __GAME_H__
#define  __GAME_H__

enum TURN
{
	SERVER = 0,
	USER,

	NONE,
};


#define MAX_BOARD 9

class Game
{
public:
	Game();
	~Game();

	static Game* create();
	bool Init();
	
	void SetBoard(int idx, int val);
	int* GetBoard();

	TURN GetTurn();
	void TurnChange();

private:
	TURN turn;
	int board[MAX_BOARD];
};

#endif // !__GAME_H__


