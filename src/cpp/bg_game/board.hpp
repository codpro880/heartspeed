/* Data Class to represent the cards a player currently has in play */
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../cards/bgs/BgBaseCard.hpp"

class Board {
public:
    Board(std::vector<std::shared_ptr<BgBaseCard> > cards) : cards(cards) {}
    int calculate_damage();
    auto empty() { return cards.empty(); }
    auto length() { return cards.size(); }
    auto operator[](const int& i) { return cards[i]; }
    friend std::ostream& operator<<(std::ostream& os, Board& board);
    auto remove(std::shared_ptr<BgBaseCard> c) {
	auto it = std::find(cards.begin(), cards.end(), c);
	cards.erase(it);
    }
    auto remove(const int& i) {
	if (cards.size() > (unsigned)i) {
	    cards.erase(cards.begin() + i);
	}
    }
    // auto get_pos(std::shared_ptr<BgBaseCard> c) {
    // 	auto it = std::find(cards.begin(), cards.end(), c);
    // 	return std::distance(cards.begin(), it);
    // }
    // auto get_pos(BgBaseCard* card) {
    // 	auto pos = 0;
    // 	for (auto c : cards) {
    // 	    if (c.get() == card) {
    // 		return pos;
    // 	    }	   
    // 	}
    // 	return -1;
    // }
    void set_card(int i, std::shared_ptr<BgBaseCard> c) { cards[i] = c; }
    void insert_card(int pos, std::shared_ptr<BgBaseCard> c) { cards.insert(cards.begin() + pos, c); }
    std::vector<std::shared_ptr<BgBaseCard> > const get_cards() { return cards;  } // TODO: Make this an iterator
private:
    std::vector<std::shared_ptr<BgBaseCard> > cards;    
};
