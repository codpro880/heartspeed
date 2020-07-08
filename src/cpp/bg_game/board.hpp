/* Data Class to represent the cards a player currently has in play */
#pragma once

#include <string>
#include <vector>

#include "../cards/bgs/BgBaseCard.hpp"

class Board {
public:
    Board(std::vector<BgBaseCard> cards) : cards(cards) {}
    int calculate_damage();
private:
    std::vector<BgBaseCard> cards;
};
