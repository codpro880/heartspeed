/* Data Class to represent the cards a player currently has in play */
#pragma once

#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_set>

#include "../cards/bgs/BgBaseCard.hpp"

class Board {
public:
    Board(std::vector<std::shared_ptr<BgBaseCard> > cards) : cards(cards) {}
    int calculate_damage();
    auto empty() { return cards.empty(); }
    auto length() { return cards.size(); }
    auto operator[](const int& i) { return cards[i]; }
    friend std::ostream& operator<<(std::ostream& os, Board& board);    
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
    auto get_pos(std::shared_ptr<BgBaseCard> c) {
    	auto it = std::find(cards.begin(), cards.end(), c);
    	return std::distance(cards.begin(), it);
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
	    this->remove(front);
	    if (front->has_reborn()) {
		std::cerr << "REBORN!" << std::endl;
		front->reborn_self(this);
	    }
	    _has_died.push_back(front);
	    to_remove.pop();
	}
    }
    void do_deathrattles(Board* other) {
	bool at_least_one_dead = false;
	while (!deathrattle_q.empty()) {
	    at_least_one_dead = true;
	    auto card = deathrattle_q.front();
	    deathrattle_q.pop();
	    card->do_deathrattle(this, other);
	}
	if (at_least_one_dead) {
	    // Deathrattles can cause other deaths to occur
	    remove_and_mark_dead();
	    other->remove_and_mark_dead();
	    do_deathrattles(other);
	    other->do_deathrattles(this);
	}
    }
    void set_card(int i, std::shared_ptr<BgBaseCard> c) { cards[i] = c; }
    void insert_card(int pos, std::shared_ptr<BgBaseCard> c) {
	if (pos >= cards.size()) {
	    // This case can occur w/ certain deathrattle interactions
	    // TODO: Fix this case...if unstable ghoul nearly wipes board execpt one survivor,
	    // and mecharoo was to left of surviving card, then we want it in front.
	    // Else, if mecharoo was to right of surviving card, we
	    // want it in back...
	    cards.push_back(c);
	    card_names.insert(c->get_name());
	}
	else{
	    cards.insert(cards.begin() + pos, c);
	    card_names.insert(c->get_name());
	}
    }
    std::vector<std::shared_ptr<BgBaseCard> > const get_cards() { return cards;  } // TODO: Make this an iterator
    std::vector<std::shared_ptr<BgBaseCard> > has_died() { return _has_died; }
    bool is_in(std::string card_name) { return card_names.find(card_name) != card_names.end(); }
private:
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    std::queue<std::shared_ptr<BgBaseCard> > deathrattle_q;
    std::vector<std::shared_ptr<BgBaseCard> > _has_died;
    std::unordered_set<std::string> card_names;
};
