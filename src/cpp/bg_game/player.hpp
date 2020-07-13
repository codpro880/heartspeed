/* Class to represent the player */
#pragma once

#include <string>
#include <vector>

#include "Board.hpp"

class Player {
public:
    Player(Board* board) : board(board), tech_level(1) {}
    int calculate_damage() { return tech_level + board.calculate_damage(); }
    void inc_tech_level() { tech_level += 1; }
private:
    Board* board;
    int tech_level;
};
