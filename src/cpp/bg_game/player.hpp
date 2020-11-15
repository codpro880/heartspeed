/* Class to represent the player */
#pragma once

#include <memory>
#include <string>
#include <vector>


#include "board.hpp"
#include "hand.hpp"

class Player {
public:
    Player(std::shared_ptr<Board> board_, std::string name) : board(board_), original_board(std::make_shared<Board>(board_)), name(name), tech_level(1) {}

    Player(std::string name) : board(new Board()), original_board(new Board()), name(name), tech_level(1) {}
    
    Player(Player* player) {
    	board = std::make_shared<Board>(player->get_original_board());
	original_board = std::make_shared<Board>(player->get_original_board());
	name = player->get_name();
	tech_level = player->get_tech_level();
    }

    void buy_card(std::shared_ptr<BgBaseCard> c) { hand.add_card(c); }
    int calculate_damage() const { return tech_level + board->calculate_damage(); }
    void inc_tech_level() { tech_level += 1; }
    std::shared_ptr<Board> get_board() const { return board; }
    Hand get_hand() const { return hand; }
    std::shared_ptr<Board> get_original_board() const { return original_board; }
    void set_board(std::shared_ptr<Board> b) { board = b; }
    friend std::ostream& operator<<(std::ostream& os, const Player& p);
    std::string get_name() const { return name; }
    int get_tech_level() const { return tech_level; }
    
    void play_card(std::shared_ptr<BgBaseCard> card, uint8_t board_pos) {
	auto pos = hand.get_pos(card);
	play_card(pos, board_pos);
    }

    void play_card(uint8_t hand_pos, uint8_t board_pos) {
	auto card = hand.get_cards()[hand_pos];
	board->insert_card(board_pos, card);
	hand.remove(card);
    }
    // void reset() {
    // 	// TODO: Make this shared ptr
    // 	// Board* b = new Board(original_board);
    // 	board = std::make_shared<Board>(original_board);
    // }
private:
    Hand hand;
    std::shared_ptr<Board> board;
    std::shared_ptr<Board> original_board; // Read-only board
    std::string name;
    int tech_level;
};
