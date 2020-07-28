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
    auto remove(const int& i) { cards.erase(cards.begin() + i); }
    void set_card(int i, std::shared_ptr<BgBaseCard> c) { cards[i] = c; }
    std::vector<std::shared_ptr<BgBaseCard> > const get_cards() { return cards;  } // TODO: Make this an iterator
private:
    std::vector<std::shared_ptr<BgBaseCard> > cards;    
};



