#include "Game.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <thread>


void Game::run()
{
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
        m_isPlayerFirst = true;
        m_isPlayerTurn = true;
        m_playerSymbol = 'X';
        m_aiSymbol = 'O';
    }
}

void Game::doMoves()
{
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
            m_board[row][col] = m_playerSymbol;
        }
    }
    else {
        aiMove();
    }
    m_isPlayerTurn = !m_isPlayerTurn;
}

void Game::render()
{	//I can make this look pretty later.
	system("cls");
	for (int i = 0; i < ROWS; i++) {
		printf("%c %c %c\n", m_board[i][0], m_board[i][1], m_board[i][2]);
	}
}

//Checks for any potential victory.
//This is only ever used for AI so make AI the positive thing.
int Game::evaluate(char board[3][3])
{
    //Check row victory.
    for (int i = 0; i < ROWS; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == m_aiSymbol)
                return 10;
            if (board[i][0] == m_playerSymbol)
                return -10;
        }
    }

    //Check column victory. 
    for (int i = 0; i < COLS; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == m_aiSymbol)
                return 10;
            if (board[0][i] == m_playerSymbol)
                return -10;
        }
    }

    //Check diagonal victory. 
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == m_aiSymbol)
            return 10;
        if (board[0][0] == m_playerSymbol)
            return -10;
    }

    //Check other diagonal victory. 
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == m_aiSymbol)
            return 10;
        if (board[0][2] == m_playerSymbol)
            return -10;
    }

    //Otherwise its a draw.
    return 0;
}

int Game::minmax(char board[3][3], int depth, bool isMax)
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
                    board[i][j] = m_aiSymbol;
                    best = std::max(best, minmax(board, depth + 1, !isMax));
                    board[i][j] = 0;
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
                    board[i][j] = m_playerSymbol;
                    best = std::min(best, minmax(board, depth + 1, !isMax));
                    board[i][j] = 0;
                }
            }
        }
        return best;
    }
}

void Game::aiMove()
{   //Only works when the player is O.
    int bestVal = INT_MIN;
    int bestRow = 0;
    int bestCol = 0;
    std::thread threadPool[9];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (isEmpty(m_board, i, j)) {
                m_board[i][j] = m_aiSymbol;
                int moveVal = minmax(m_board, 0, false);
                m_board[i][j] = 0;
                if (moveVal > bestVal) {
                    bestRow = i;
                    bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    //Do the move!
    m_board[bestRow][bestCol] = m_aiSymbol;
}

void Game::aiMoveInternal(char board[3][3], int row, int col, std::atomic_int& bestRow, std::atomic_int& bestCol, std::atomic_int& bestVal)
{
    if (isEmpty(board, row, col)) {
        m_board[row][col] = m_aiSymbol;
        int moveVal = minmax(board, 0, false);
        m_board[row][col] = 0;
        if (moveVal > bestVal) {
            bestRow = row;
            bestCol = col;
            bestVal = moveVal;
        }
    }
}

bool Game::isBoardFull(char board[3][3])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] != m_playerSymbol && board[i][j] != m_aiSymbol)
				return false;
		}
	}
	return true;
}

bool Game::isEmpty(char board[3][3], int row, int column)
{
    return board[row][column] != m_playerSymbol && board[row][column] != m_aiSymbol;
}
