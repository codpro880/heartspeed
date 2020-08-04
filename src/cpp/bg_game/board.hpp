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
	std::cerr << "Removing shared ptr." << std::endl;
	auto it = std::find(cards.begin(), cards.end(), c);
	std::cerr << "Found it." << std::endl;
	cards.erase(it);
	std::cerr << "Removed it." << std::endl;
    }
    void remove(const int& i) {
	if (cards.size() > (unsigned)i) {
	    cards.erase(cards.begin() + i);
	}
    }
    auto get_pos(std::shared_ptr<BgBaseCard> c) {
    	auto it = std::find(cards.begin(), cards.end(), c);
    	return std::distance(cards.begin(), it);
    }
    auto get_pos(BgBaseCard* card) {
    	auto pos = 0;
    	for (auto c : cards) {
    	    if (c.get() == card) {
    		return pos;
    	    }	   
    	}
    	return -1;
    }
    void set_card(int i, std::shared_ptr<BgBaseCard> c) { cards[i] = c; }
    void insert_card(int pos, std::shared_ptr<BgBaseCard> c) { cards.insert(cards.begin() + pos, c); }
    std::vector<std::shared_ptr<BgBaseCard> > const get_cards() { return cards;  } // TODO: Make this an iterator
private:
    std::vector<std::shared_ptr<BgBaseCard> > cards;    
};
