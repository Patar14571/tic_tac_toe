#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <set>
#include <unordered_map>
#include <utility>
#include <random>

using namespace std;

struct pair_hash {
    template <class T1, class T2>
    size_t operator() (const pair<T1, T2>& p) const {
        auto h1 = hash<T1>{}(p.first);  // Hash the first element
        auto h2 = hash<T2>{}(p.second); // Hash the second element
        return h1 ^ h2;  // Combine the two hash values
    }
};

class Game {
public:
    // Constructor
    Game();

    // Public member functions
    void run();

private:
    //char board[3][3]; // Tic-tac-toe board
    int gameState;
    bool gameOver;
    vector<vector<char>> board;
    int row, col;
    unordered_map<pair<int,int>, char, pair_hash> previousMoves;
    random_device rd;               // Obtain a random number from hardware (if available)
    mt19937 gen;                    // Mersenne Twister engine with the random seed
    uniform_int_distribution<> dis; // Define the range [0, 2]

    bool isValidMove(int row, int col);
    void handleInput();
    void CPUMove();
    bool checkWinCondition(char player);
    void updateGameBoard(char player);
    void renderBoard(const std::vector<std::vector<char>>& board, char player);
    void resetBoard();
};

#endif