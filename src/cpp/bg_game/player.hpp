/* Class to represent the player */
#pragma once

#include <memory>
#include <string>
#include <vector>


#include "board.hpp"
#include "hand.hpp"

class Player {
public:
    Player(std::shared_ptr<Board> board_, std::string name) : board(board_),
							      health(40),
							      name(name),
							      original_board(std::make_shared<Board>(board_)),
							      tech_level(1) {}

    Player(std::string name) : board(new Board()),
			       health(40),
			       name(name),
			       original_board(new Board()),
			       tech_level(1) {}

    Player(Hand hand, std::string name) : board(new Board()),
					  hand(hand),
					  health(40),
					  name(name),
					  original_board(new Board()),					  
					  tech_level(1) {}
    
    Player(Player* player) {
    	board = std::make_shared<Board>(player->get_original_board());
	hand = player->get_hand();
	health = player->get_health();	
	name = player->get_name();
	original_board = std::make_shared<Board>(player->get_original_board());
	tech_level = player->get_tech_level();

    }

    // TODO: Impl bobs tav
    // void buy_card(std::shared_ptr<BgBaseCard> c) { hand.add_card(c); }
    int calculate_damage() const { return tech_level + board->calculate_damage(); }
    void inc_tech_level() { tech_level += 1; }
    std::shared_ptr<Board> get_board() const { return board; }
    Hand get_hand() const { return hand; }
    std::shared_ptr<Board> get_original_board() const { return original_board; }
    void set_board(std::shared_ptr<Board> b) { board = b; }
    friend std::ostream& operator<<(std::ostream& os, const Player& p);
    int get_health() const { return health; }
    std::string get_name() const { return name; }
    int get_tech_level() const { return tech_level; }
    
    void play_card(std::shared_ptr<BgBaseCard> card, uint8_t board_pos) {
	auto pos = hand.get_pos(card);
	play_card(pos, board_pos);
    }

    void play_card(uint8_t hand_pos, uint8_t board_pos) {
	auto card = hand.get_cards()[hand_pos];
	auto dmg_taken = board->insert_card(board_pos, card, true);
	std::cerr << "Taken: " << dmg_taken << std::endl;
	take_damage(dmg_taken);
	card->do_battlecry(this);
	hand.remove(card);
    }

    void play_card(uint8_t hand_pos, uint8_t target_pos, uint8_t board_pos) {
	auto card = hand.get_cards()[hand_pos];
	auto target = board->get_cards()[target_pos];
	// TODO: Enforce valid targets (e.g. MUST pick valid target if available)
	auto dmg_taken = board->insert_card(board_pos, card, true);	
	take_damage(dmg_taken);
	card->targeted_battlecry(target);
	hand.remove(card);
    }

    void take_damage(int dmg) { health -= dmg; }      
    // void reset() {
    // 	// TODO: Make this shared ptr
    // 	// Board* b = new Board(original_board);
    // 	board = std::make_shared<Board>(original_board);
    // }
private:    
    std::shared_ptr<Board> board;
    Hand hand;
    int health;
    std::string name;
    std::shared_ptr<Board> original_board; // Read-only board
    int tech_level;
};
