/* Data Class to represent the cards a player currently has in play */
#pragma once

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_set>

#include "../cards/bgs/BgBaseCard.hpp"

class Board {
public:
    Board(std::vector<std::shared_ptr<BgBaseCard> > cards) : cards(cards), attacker_pos(0) {
	for (auto c : cards) {
	    card_names.insert(c->get_name());
	}
    }
    
    Board(Board* b) : attacker_pos(0) {
	// std::vector<std::shared_ptr<BgBaseCard> > cards_copy;
	cards.clear();
	card_names.clear();
	for (auto c : b->get_cards()) {
	    cards.push_back(c->get_copy());
	    card_names.insert(c->get_name());
	}
	//return Board(cards_copy);
    }

    Board(std::shared_ptr<Board> b) : Board(b.get()) {}

    Board() {}
    
    int calculate_damage() const;
    
    auto empty() const { return cards.empty(); }
    
    auto length() const { return cards.size(); }

    auto size() const { return cards.size(); }
    
    auto operator[](const int& i) { return cards[i]; }
    
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
    
    void remove(BgBaseCard* c) {
	int pos = 0;
	for (auto card : cards) {
	    if (c == card.get()) {
		remove(pos);
		return;
	    }
	    pos++;
	}
	std::cerr << "NOT FOUND" << std::endl;
	// std::cerr << "Removing pointer." << std::endl;
	// remove(std::make_shared<BgBaseCard>(*c));	
    }
    
    void remove(std::shared_ptr<BgBaseCard> c) {
	auto it = std::find(cards.begin(), cards.end(), c);
	cards.erase(it);
	card_names.erase(c->get_name());
    }
    
    void remove(const int& i) {
	if (cards.size() > (unsigned)i) {
	    cards.erase(cards.begin() + i);
	    card_names.erase(cards[i]->get_name());
	}
    }
    
    auto get_pos(std::shared_ptr<BgBaseCard> c) const {
    	auto it = std::find(cards.begin(), cards.end(), c);
    	return std::distance(cards.begin(), it);
    }
    
    auto get_pos(BgBaseCard* c) const {
	int pos = 0;
	for (auto card : cards) {
	    if (card.get() == c) {
		return pos;
	    }
	    pos++;
	}
	return -1;
    	// auto it = std::find(cards.begin(), cards.end(), std::shared_ptr<BgBaseCard>(c));
    	// return std::distance(cards.begin(), it);
    }
    
    bool contains(std::shared_ptr<BgBaseCard> c) const {
	auto pos = get_pos(c);
	return pos != -1 && (unsigned)pos != cards.size();
    }
    // auto get_pos(BgBaseCard* card) {
    // 	auto pos = 0;
    // 	for (auto c : cards) {
    // 	    if (c.get() == card) {
    // 		return pos;
    // 	    }	   
    // 	}
    // 	return -1;
    // }
    
    void remove_and_mark_dead() {
	std::queue<std::shared_ptr<BgBaseCard> > to_remove;
	for (auto c : cards) {
	    if (c->is_dead()) {
		// auto death_pos = this->get_pos(c.get());
		auto death_pos = this->get_pos(c);
		c->set_death_pos(death_pos);
		deathrattle_q.push(c);
		to_remove.push(c);
	    }
	}
	while (!to_remove.empty()) {
	    auto front = to_remove.front();
	    auto death_pos = this->get_pos(front);
	    if (death_pos < attacker_pos) {
		attacker_pos--;
	    }
	    this->remove(front);
	    if (front->has_reborn()) {
		front->reborn_self(this);
		attacker_pos++;
	    }
	    _has_died.push_back(front);
	    to_remove.pop();
	}
    }

    void do_deathrattles(std::shared_ptr<Board> b) {
	return do_deathrattles(b.get());
    }
    
    void do_deathrattles(Board* other) {
	bool at_least_one_dead = false;
	while (!deathrattle_q.empty()) {
	    at_least_one_dead = true;
	    auto card = deathrattle_q.front();
	    deathrattle_q.pop();
	    card->deathrattle(this, other);
	}
	if (at_least_one_dead) {
	    // Deathrattles can cause other deaths to occur
	    remove_and_mark_dead();
	    other->remove_and_mark_dead();
	    do_deathrattles(other);
	    other->do_deathrattles(this);
	}
    }
    
    int insert_card(int pos, std::shared_ptr<BgBaseCard> c, bool from_hand=false) {
	int total_dmg = 0;
	if ((unsigned)pos >= cards.size()) {
	    // This case can occur w/ certain deathrattle interactions
	    // TODO: Fix this case...if unstable ghoul nearly wipes board execpt one survivor,
	    // and mecharoo was to left of surviving card, then we want it in front.
	    // Else, if mecharoo was to right of surviving card, we
	    // want it in back...
	    for (auto card : this->get_cards()) {
		total_dmg += card->mod_summoned(c, from_hand);
	    }
	    cards.push_back(c);
	    card_names.insert(c->get_name());
	    // c->do_battlecry(this);
	}
	else{
	    for (auto card : this->get_cards()) {
		total_dmg += card->mod_summoned(c, from_hand);
	    }
	    cards.insert(cards.begin() + pos, c);
	    card_names.insert(c->get_name());
	    // c->do_battlecry(this);
	}
	return total_dmg;
    }
    
    std::vector<std::shared_ptr<BgBaseCard> > const get_cards() const { return cards;  } // TODO: Make this an iterator
    
    std::vector<std::shared_ptr<BgBaseCard> > has_died() const { return _has_died; }
    
    bool contains(std::string card_name) const { return card_names.find(card_name) != card_names.end(); }

    int get_attacker_pos() { return attacker_pos; }

    void increment_attacker_pos() {
	attacker_pos++;
	clip_attacker_pos();
    }

    void clip_attacker_pos() {
	if (attacker_pos >= length()) {
	    attacker_pos = 0;
	}
    }
	
    
private:
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    std::queue<std::shared_ptr<BgBaseCard> > deathrattle_q;
    std::vector<std::shared_ptr<BgBaseCard> > _has_died;
    std::unordered_set<std::string> card_names;
    int attacker_pos;
};
