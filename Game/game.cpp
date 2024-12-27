#include "game.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
#include <ctime>

#define TITLE_SCREEN    (1u)
#define PLAYER_TURN     (2u)
#define CPU_TURN        (3u)

using namespace std;

Game::Game() : gameOver(false), board(3, vector<char>(3, ' ')), row(0), col(0), gameState(TITLE_SCREEN), gen(random_device{}()), dis(0,2) {
    // Initialize any game state or resources here
    cout << "Game Start: Initializing game board..." << endl;
}

void Game::run() {

    //Setup
    renderBoard(board, 'X');

    //Start
    while (true) {
        switch ( gameState ) {
            case TITLE_SCREEN:
                cout << "Welcome Player X! Start Game? (Y/N) " << endl;
                handleInput();
                break;
            case PLAYER_TURN:
                handleInput();
                updateGameBoard('X');
                if (gameOver) {
                    renderBoard(board, 'X');
                    cout << "Player X wins!\n\n" << endl;
                    gameState = TITLE_SCREEN;
                } else {
                    renderBoard(board, 'X');
                    gameState = CPU_TURN;
                }
                break;
            case CPU_TURN:
                CPUMove();
                updateGameBoard('O');
                if (gameOver) {
                    renderBoard(board, 'O');
                    cout << "Player O wins!\n\n" << endl;
                    gameState = TITLE_SCREEN;
                } else {
                    renderBoard(board, 'O');
                    gameState = PLAYER_TURN;
                }
                break;
            default:
                cout << "Error: gameLoop switch case broke somehow" << endl;
                break;
        }
    }
}

bool Game::isValidMove(int x, int y)
{
    return ( previousMoves.find({x, y}) == previousMoves.end() );
}

void Game::handleInput()
{
    switch (gameState) {
        case TITLE_SCREEN:
            while (true) {
                char playerInput;
                cin >> playerInput;

                if (cin.fail()) {
                    cin.clear();  // Clears the fail state
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discards invalid input
                } else if ('n' == playerInput || 'N' == playerInput) {
                    exit(0);
                } else if ('y' == playerInput || 'Y' == playerInput) {
                    gameOver = false;
                    gameState = PLAYER_TURN;
                    resetBoard();  // Reset the board for a new game
                    break;
                } else {
                    cout << "Invalid input. Please enter 'Y' or 'N'." << endl;
                }  
            }
            break;
        case PLAYER_TURN:
            while (true) {
                cout << "Enter row (0-2): ";
                cin >> row;
                cout << "Enter column (0-2): ";
                cin >> col;

                // Check if input is valid
                if (cin.fail() || row < 0 || row > 2 || col < 0 || col > 2 || !isValidMove(row, col)) {
                    cin.clear();  // Clears the fail state
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discards invalid input
                    cout << "Invalid input. Please enter valid row and column (0-2) for an unused, valid move.\n";
                } else {
                    previousMoves[{row, col}] = 'X';
                    break;  // Breaks the loop if input is valid
                }
            }
            break;
        case CPU_TURN:
            cout << "Error: CPU Turn called handleInput!" << endl;
            break;
        default:
            cout << "Error: handleInput switch case broke somehow" << endl;
            break;
    }
}

void Game::CPUMove() {
    // CPU Turn
    do {
        //Randomly select a row and column
        row = dis(gen);
        col = dis(gen);
    } while (!isValidMove(row, col));

    previousMoves[{row, col}] = 'O';
    cout << "CPU chose (" << row << ", " << col << ")" << endl;
}

bool Game::checkWinCondition(char player) {

    // Check win conditions:
    // Check row win
    for (const auto& row : board) {
        if (all_of(row.begin(), row.end(), [player](char c) { return c == player; })) {
            return true;
        }
    }

    // Check column win
    for (size_t column = 0; column < board[0].size(); ++column) {
        if (all_of(board.begin(), board.end(), [column, player](const auto& row) {
                return row[column] == player;
            })) {
            return true;
        }
    }

    // Check diagonal win
    bool diag1 = true, diag2 = true;
    for (size_t i = 0; i < board[0].size(); ++i) {
        // Check main diagonal (0,0) (1,1) (2,2)
        if (board[i][i] != player) diag1 = false;
        // Check anti diagonal (0,2) (1,1) (2,0)
        if (board[i][board.size() - 1 - i] != player) diag2 = false;
    }
    if (diag1 || diag2) {
        return true;
    }

    return false;
}

void Game::updateGameBoard(char player)
{
    // Update game state
    board[row][col] = player;

    gameOver = checkWinCondition(player);
}

void Game::renderBoard(const vector<vector<char>>& newBoard, char player) 
{
    for (const auto& newRow : newBoard) {
        for (const auto& newCol : newRow) {
            cout << "| " << newCol << " "; // Print each character in the row
        }
        cout << "| " << endl; // Move to the next line after each row
    }
    cout << endl;
}

void Game::resetBoard() {
    // Reset the board for a new game
    board = vector<vector<char>>(3, vector<char>(3, ' '));
    previousMoves.clear();
}