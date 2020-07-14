/* Data Class to represent the cards a player currently has in play */
#pragma once

#include <string>
#include <vector>

#include "../cards/bgs/BgBaseCard.hpp"

class Board {
public:
    Board(std::vector<BgBaseCard> cards) : cards(cards) {}
    int calculate_damage();
    auto empty() { return cards.empty(); }
    auto length() { return cards.size(); }
    BgBaseCard operator[](const int& i) { return cards[i]; }
    auto remove(const int& i) { cards.erase(cards.begin() + i); }
private:
    std::vector<BgBaseCard> cards;
    
};
