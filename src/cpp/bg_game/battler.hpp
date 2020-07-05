/* Battler takes two boards and simulates the result of the battle */
#pragma once

#include <string>
#include <vector>

#include "Board.hpp"

// Takes two boards. Simulates one attack. Two new boards -> recursively battle
class Battler {
public:
    Battler(Board board1, Board board2) : board1(board1), board2(board2) {}
private:
    Board board1, board2;
}
