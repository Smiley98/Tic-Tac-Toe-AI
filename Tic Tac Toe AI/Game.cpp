#include "Game.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <thread>

void Game::run()
{   //Zero-initialize for safety.
    std::fill(m_board.m_data[0].begin(), m_board.m_data[0].end(), 0);
    std::fill(m_board.m_data[1].begin(), m_board.m_data[1].end(), 0);
    std::fill(m_board.m_data[2].begin(), m_board.m_data[2].end(), 0);
    while (true) {
        render();
        doMoves();
    }
}

void Game::askSymbol()
{	//I don't feel like validating input.
    printf("Would you like to be X or O?\n");
    printf("Press X to be X, press O to be O.\n");
    char input;
    std::cin >> input;
    if (input == 'x' || input == 'X') {
        m_isPlayerTurn = true;
        m_playerSymbol = 'X';
        m_aiSymbol = 'O';
    }
}

void Game::doMoves()
{   //Solid first-year level code for the game logic.
    static bool once = true;
    if (once) {
        once = false;
        askSymbol();
    }
    if (isBoardFull(m_board)) {
        printf("Tie!\n");
        system("pause");
        exit(0);
    }
    else if (abs(evaluate(m_board)) != 0) {
        if (m_isPlayerTurn)
            printf("%c wins!\n", m_aiSymbol);
        else
            printf("%c wins!\n", m_playerSymbol);
        system("pause");
        exit(0);
    }

    if (m_isPlayerTurn) {
        printf("Player turn:\n");
        printf("Which row would you like to move to (1-3)?\n");
        int row;
        std::cin >> row;
        printf("Which column would you like to move to (1-3)?\n");
        int col;
        std::cin >> col;
        row -= 1;
        col -= 1;
        if (isEmpty(m_board, row, col)) {
            m_board.m_data[row][col] = m_playerSymbol;
        }
    }
    else {
        aiMove();
    }
    m_isPlayerTurn = !m_isPlayerTurn;
}

void Game::render()
{	//Function over form xD
	system("cls");
	for (int i = 0; i < ROWS; i++)
		printf("%c %c %c\n", m_board.m_data[i][0], m_board.m_data[i][1], m_board.m_data[i][2]);
}

//Checks for any potential victory.
int Game::evaluate(const Board& board)
{
    //Check row victory.
    for (int i = 0; i < ROWS; i++) {
        if (board.m_data[i][0] == board.m_data[i][1] && board.m_data[i][1] == board.m_data[i][2]) {
            if (board.m_data[i][0] == m_aiSymbol)
                return 10;
            if (board.m_data[i][0] == m_playerSymbol)
                return -10;
        }
    }

    //Check column victory. 
    for (int i = 0; i < COLS; i++) {
        if (board.m_data[0][i] == board.m_data[1][i] && board.m_data[1][i] == board.m_data[2][i]) {
            if (board.m_data[0][i] == m_aiSymbol)
                return 10;
            if (board.m_data[0][i] == m_playerSymbol)
                return -10;
        }
    }

    //Check diagonal victory. 
    if (board.m_data[0][0] == board.m_data[1][1] && board.m_data[1][1] == board.m_data[2][2]) {
        if (board.m_data[0][0] == m_aiSymbol)
            return 10;
        if (board.m_data[0][0] == m_playerSymbol)
            return -10;
    }

    //Check other diagonal victory. 
    if (board.m_data[0][2] == board.m_data[1][1] && board.m_data[1][1] == board.m_data[2][0]) {
        if (board.m_data[0][2] == m_aiSymbol)
            return 10;
        if (board.m_data[0][2] == m_playerSymbol)
            return -10;
    }

    //Otherwise its a draw.
    return 0;
}

//Where the magic happens.
int Game::minmax(Board& board, int depth, bool isMax)
{
    int score = evaluate(board);
    //Maximizer won.
    if (score == 10)
        return score;
    //Minimizer won.
    if (score == -10)
        return score;
    if (isBoardFull(board))
        return 0;

    if (isMax) {
        int best = INT_MIN;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (isEmpty(board, i, j)) {
                    board.m_data[i][j] = m_aiSymbol;
                    best = std::max(best, minmax(board, depth + 1, !isMax));
                    board.m_data[i][j] = 0;
                }
            }
        }
        return best;
    }
    else {
        int best = INT_MAX;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (isEmpty(board, i, j)) {
                    board.m_data[i][j] = m_playerSymbol;
                    best = std::min(best, minmax(board, depth + 1, !isMax));
                    board.m_data[i][j] = 0;
                }
            }
        }
        return best;
    }
}

//Where the real magic happens.
void Game::aiMove()
{
    std::atomic_int bestVal(INT_MIN);
    std::atomic_int bestRow(0);
    std::atomic_int bestCol(0);
    std::thread threadPool[ROWS * COLS];
    Board boards[ROWS * COLS];
    for (int i = 0; i < ROWS * COLS; i++)
        boards[i] = m_board;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (isEmpty(m_board, i, j)) {
                const int index = i * ROWS + j;
                threadPool[index] = std::thread(&Game::aiMoveInternal, this, std::ref(boards[index]), i, j, std::ref(bestRow), std::ref(bestCol), std::ref(bestVal));
            }
        }
    }
    for (int i = 0; i < ROWS * COLS; i++) {
        if(threadPool[i].joinable())
            threadPool[i].join();
    }
    //Do the move!
    m_board.m_data[bestRow.load()][bestCol.load()] = m_aiSymbol;
}

//Slightly magical as well ;)
void Game::aiMoveInternal(Board& board, int row, int col, std::atomic_int& bestRow, std::atomic_int& bestCol, std::atomic_int& bestVal)
{
    board.m_data[row][col] = m_aiSymbol;
    int moveVal = minmax(board, 0, false);
    m_board.m_data[row][col] = 0;
    if (moveVal > bestVal.load()) {
        bestRow.store(row);
        bestCol.store(col);
        bestVal.store(moveVal);
    }
}

//Back in the single-threaded days...
//int bestVal = INT_MIN;
//int bestRow = 0;
//int bestCol = 0;

//  if (isEmpty(m_board, i, j)) {
//      m_board.m_data[i][j] = m_aiSymbol;
//      int moveVal = minmax(m_board, 0, false);
//      m_board.m_data[i][j] = 0;
//      if (moveVal > bestVal) {
//          bestRow = i;
//          bestCol = j;
//          bestVal = moveVal;
//      }
//  }

bool Game::isEmpty(const Board& board, int row, int column)
{
    return board.m_data[row][column] != m_playerSymbol && board.m_data[row][column] != m_aiSymbol;
}

bool Game::isBoardFull(const Board& board)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (isEmpty(board, i, j))
				return false;
		}
	}
	return true;
}
