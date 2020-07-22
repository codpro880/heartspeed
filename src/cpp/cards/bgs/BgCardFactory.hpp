#pragma once

#include <string>
#include <unordered_map>

#include "BgBaseCard.hpp"

// This factory is for auto generated cards
// class BgBaseCardFactory {
// public:
//     BgBaseCardFactory() {}
//     virtual BgBaseCard get_card(std::string name);
// private:
//     BgBaseCard _get_card(std::string name);
//     void init_cards();
//     std::unordered_map<std::string, BgBaseCard> cards;
// };

// This factory is for hand generated cards
class BgCardFactory {
public:
    BgCardFactory() { init_cards(); }
    BgBaseCard get_card(std::string name);
private:
    void init_cards();
    BgBaseCard _get_card(std::string name);
    std::unordered_map<std::string, BgBaseCard> cards;
};
