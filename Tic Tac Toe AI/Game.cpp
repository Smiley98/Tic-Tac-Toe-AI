#include "Game.h"
#include <iostream>
#include <limits>
#include <algorithm>


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
    if (isBoardFull()) {
        printf("Tie!\n");
        system("pause");
        exit(0);
    }
    else if (abs(evaluate()) != 0) {
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
        if (isEmpty(row, col)) {
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
int Game::evaluate()
{
    //Check row victory.
    for (int i = 0; i < ROWS; i++) {
        if (m_board[i][0] == m_board[i][1] && m_board[i][1] == m_board[i][2]) {
            if (m_board[i][0] == m_aiSymbol)
                return 10;
            if (m_board[i][0] == m_playerSymbol)
                return -10;
        }
    }

    //Check column victory. 
    for (int i = 0; i < COLS; i++) {
        if (m_board[0][i] == m_board[1][i] && m_board[1][i] == m_board[2][i]) {
            if (m_board[0][i] == m_aiSymbol)
                return 10;
            if (m_board[0][i] == m_playerSymbol)
                return -10;
        }
    }

    //Check diagonal victory. 
    if (m_board[0][0] == m_board[1][1] && m_board[1][1] == m_board[2][2]) {
        if (m_board[0][0] == m_aiSymbol)
            return 10;
        if (m_board[0][0] == m_playerSymbol)
            return -10;
    }

    //Check other diagonal victory. 
    if (m_board[0][2] == m_board[1][1] && m_board[1][1] == m_board[2][0]) {
        if (m_board[0][2] == m_aiSymbol)
            return 10;
        if (m_board[0][2] == m_playerSymbol)
            return -10;
    }

    //Otherwise its a draw.
    return 0;
}

int Game::minmax(int depth, bool isMax)
{
    int score = evaluate();
    //Maximizer won.
    if (score == 10)
        return score;
    //Minimizer won.
    if (score == -10)
        return score;

    if (isMax) {
        int best = INT_MIN;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (isEmpty(i, j)) {
                        m_board[i][j] = m_aiSymbol;
                    best = std::max(best, minmax(depth + 1, !isMax));
                    m_board[i][j] = 0;
                }
            }
        }
        return best;
    }
    else {
        int best = INT_MAX;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (isEmpty(i, j)) {
                        m_board[i][j] = m_playerSymbol;
                    best = std::min(best, minmax(depth + 1, !isMax));
                    m_board[i][j] = 0;
                }
            }
        }
        return best;
    }
}

void Game::aiMove()
{
    int bestVal;// = INT_MIN;
    if (m_isPlayerFirst)
        bestVal = INT_MAX;
    else
        bestVal = INT_MIN;
    int bestRow = 0;
    int bestCol = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (isEmpty(i, j)) {
                int moveVal;
                bool bestMove = false;
                m_board[i][j] = m_aiSymbol;
                moveVal = minmax(0, false);
                m_board[i][j] = 0;
                bestMove = m_isPlayerFirst ? (moveVal > bestVal) : (moveVal < bestVal);
                if (bestMove) {
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

bool Game::isBoardFull()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (m_board[i][j] != m_playerSymbol || m_board[i][j] != m_aiSymbol)
				return false;
		}
	}
	return true;
}

bool Game::isEmpty(int row, int column)
{
    return m_board[row][column] != m_playerSymbol && m_board[row][column] != m_aiSymbol;
}
