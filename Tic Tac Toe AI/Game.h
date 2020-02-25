#pragma once
#define ROWS 3
#define COLS 3
#include <atomic>

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
	int evaluate(char board[3][3]);
	int minmax(char board[3][3], int depth, bool isMax);
	void aiMove();
	void aiMoveInternal(char board[3][3], int row, int col, std::atomic_int& bestRow, std::atomic_int& bestCol, std::atomic_int& bestVal);

	//Helper functions
	bool isBoardFull(char board[3][3]);
	bool isEmpty(char board[3][3], int row, int column);
};

