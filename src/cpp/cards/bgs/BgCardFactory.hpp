#pragma once

#include <string>
#include <unordered_map>

#include "BgBaseCard.hpp"

class BgCardFactory {
public:
    BgCardFactory() { init_cards(); }
    BgBaseCard* get_card(std::string name);
private:
    void init_cards();
    std::unordered_map<std::string, BgBaseCard*> cards;
};
