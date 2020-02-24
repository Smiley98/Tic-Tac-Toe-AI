#pragma once
#define ROWS 3
#define COLS 3

class Game
{
public:
	void run();

private:
	char m_board[ROWS][COLS] = { 0 };
	char m_playerSymbol = 'O';
	char m_aiSymbol = 'X';
	bool m_isPlayerFirst = false;
	bool m_isPlayerTurn = false;

	//Game functions.
	void askSymbol();
	void doMoves();
	void render();

	//AI functions.
	int evaluate();
	int minmax(int depth, bool isMax);
	void aiMove();

	//Helper functions
	bool isBoardFull();
	bool isEmpty(int row, int column);
};

