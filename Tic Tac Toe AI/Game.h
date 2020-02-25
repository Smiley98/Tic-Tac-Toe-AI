#pragma once
#define ROWS 3
#define COLS 3
#include <array>
#include <atomic>

class Game
{
public:
	void run();

private:
	//char m_board[ROWS][COLS] = { 0 };
	char m_playerSymbol = 'O';
	char m_aiSymbol = 'X';
	bool m_isPlayerFirst = false;
	bool m_isPlayerTurn = false;

	struct Board {
		std::array<std::array<char, COLS>, ROWS> m_data;
	} m_board;

	//Game functions.
	void askSymbol();
	void doMoves();
	void render();

	//AI functions.
	int evaluate(/*char board[3][3]*/const Board& board);
	int minmax(/*char board[3][3]*/Board& board, int depth, bool isMax);
	void aiMove();
	void aiMoveInternal(/*char board[3][3]*/Board& board, int row, int col, std::atomic_int& bestRow, std::atomic_int& bestCol, std::atomic_int& bestVal);

	//Helper functions
	bool isBoardFull(/*char board[3][3]*/const Board& board);
	bool isEmpty(/*char board[3][3]*/const Board& board, int row, int column);
};

