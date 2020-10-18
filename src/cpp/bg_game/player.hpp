/* Class to represent the player */
#pragma once

#include <memory>
#include <string>
#include <vector>


#include "board.hpp"

class Player {
public:
    Player(Board* board_, std::string name) : board(board_), original_board(new Board(board_)), name(name), tech_level(1) {}
    int calculate_damage() { return tech_level + board->calculate_damage(); }
    void inc_tech_level() { tech_level += 1; }
    Board* get_board() { return board; }
    void set_board(Board* b) { board = b; }
    friend std::ostream& operator<<(std::ostream& os, Player& p);
    std::string get_name() { return name; }
    void reset() {
	// TODO: Make this shared ptr
	Board* b = new Board(original_board);
	board = b;
    }
private:
    Board* original_board; // Read-only board
    Board* board;
    std::string name;
    int tech_level;
};
