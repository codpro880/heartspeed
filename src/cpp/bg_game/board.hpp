/* Data Class to represent the cards a player currently has in play */
#pragma once

#include <string>
#include <vector>

#include "../cards/BaseCard.hpp"

class Board {
public:
    Board(std::vector<BaseCard> cards) : cards(cards) {}
    int calculate_damage();
private:
    std::vector<BaseCard> cards;
}
