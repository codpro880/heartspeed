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
							      gold(3),
							      health(40),
							      max_health(40),
							      name(name),
							      original_board(std::make_shared<Board>(board_)),
							      tavern_tier(1) {}

    Player(std::string name) : board(new Board()),
			       gold(3),
			       health(40),
			       max_health(40),
			       name(name),
			       original_board(new Board()),
			       tavern_tier(1) {}

    Player(Hand hand, std::string name) : board(new Board()),
					  gold(3),
					  hand(hand),
					  health(40),
					  max_health(40),
					  name(name),
					  original_board(new Board()),					  
					  tavern_tier(1) {}
    
    Player(Player* player) {
    	board = std::make_shared<Board>(player->get_original_board());
	hand = player->get_hand();
	health = player->get_health();
	max_health = player->get_max_health();
	name = player->get_name();
	original_board = std::make_shared<Board>(player->get_original_board());
	tavern_tier = player->get_tavern_tier();
    }

    // TODO: Impl bobs tav
    // void buy_card(std::shared_ptr<BgBaseCard> c) { hand.add_card(c); }
    int calculate_damage() const { return tavern_tier + board->calculate_damage(); }
    void inc_tavern_tier() { tavern_tier += 1; }
    std::shared_ptr<Board> get_board() const { return board; }
    Hand get_hand() const { return hand; }
    std::shared_ptr<Board> get_original_board() const { return original_board; }
    void set_board(std::shared_ptr<Board> b) { board = b; }
    friend std::ostream& operator<<(std::ostream& os, const Player& p);
    int get_health() const { return health; }
    int get_max_health() const { return max_health; }
    int get_damage_taken() const { std::cerr << "Dmg taken: " << max_health - health << std::endl; return max_health - health; }
    std::string get_name() const { return name; }
    int get_tavern_tier() const { return tavern_tier; }
    void set_tavern_tier(int tav_tier) { tavern_tier = tav_tier; }

    void add_card_to_hand(std::shared_ptr<BgBaseCard> card) {
	hand.add_card(card);
    }

    std::shared_ptr<BgBaseCard> remove_card_from_board(int pos) {
	auto card = board->get_cards()[pos];
	board->remove(pos);
	return card;
    }
    
    void play_card(std::shared_ptr<BgBaseCard> card, uint8_t board_pos) {
	auto pos = hand.get_pos(card);
	play_card(pos, board_pos);
    }

    void play_card(uint8_t hand_pos, uint8_t board_pos) {
	auto card = hand.get_cards()[hand_pos];
	auto dmg_taken = board->insert_card(board_pos, card, true);
	// Responsible for floating watcher effects...
	// TODO: Make more efficient, does linear searching
	//floating_watcher_hook(board.get(), dmg_taken);
	take_damage(dmg_taken, true);
	card->battlecry(this);
	hand.remove(card);
    }

    void play_card(uint8_t hand_pos, uint8_t target_pos, uint8_t board_pos) {
	auto card = hand.get_cards()[hand_pos];
	auto target = board->get_cards()[target_pos];
	// TODO: Enforce valid targets (e.g. MUST pick valid target if available)
	auto dmg_taken = board->insert_card(board_pos, card, true);
	//floating_watcher_hook(board.get(), dmg_taken);
	take_damage(dmg_taken);
	card->targeted_battlecry(target, this);
	hand.remove(card);
    }

    void take_damage(int dmg, bool our_turn=false) {
	health -= dmg;
	if (our_turn) {
	    floating_watcher_hook(get_board().get(), dmg);
	}
    }   

    void add_gold(int g) { gold += g; }
    void lose_gold(int g) { gold -= g; }
    void set_gold(int g) { gold = g; }
    int get_gold() { return gold; }
private:
    std::shared_ptr<Board> board;
    int gold;
    Hand hand;
    int health;
    int max_health;
    std::string name;
    std::shared_ptr<Board> original_board; // Read-only board
    int tavern_tier;

    // TODO: Make this more efficient
    void floating_watcher_hook(Board* b1, int dmg_taken) {
	bool floating_watcher_on_board = b1->contains("Floating Watcher") || b1->contains("Floating Watcher (Golden)");
	if (floating_watcher_on_board) {
	    bool malganis_on_board = b1->contains("Mal'ganis") || b1->contains("Mal'ganis (Golden)");
	    if (dmg_taken > 0 && !malganis_on_board) {
		for (auto c : b1->get_cards()) {
		    if (c->get_name() == "Floating Watcher") {
			c->set_attack(c->get_attack() + 2);
			c->set_health(c->get_health() + 2);
		    }
		    else if (c->get_name() == "Floating Watcher (Golden)") {
			c->set_attack(c->get_attack() + 4);
			c->set_health(c->get_health() + 4);
		    }
		}
	    }
	}
    }
};
