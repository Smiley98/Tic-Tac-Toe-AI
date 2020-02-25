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
	struct Board {
		std::array<std::array<char, COLS>, ROWS> m_data;
	} m_board;

	//Default initialized, optionally reassigned on askSymbol().
	char m_playerSymbol = 'O';
	char m_aiSymbol = 'X';
	bool m_isPlayerTurn = false;

	//Game functions.
	void askSymbol();
	void doMoves();
	void render();

	//AI functions.
	void aiMove();
	void aiMoveInternal(Board& board, int row, int col, std::atomic_int& bestRow, std::atomic_int& bestCol, std::atomic_int& bestVal);
	int minmax(Board& board, int depth, bool isMax);
	int evaluate(const Board& board);

	//Helper functions
	bool isEmpty(const Board& board, int row, int column);
	bool isBoardFull(const Board& board);
};

