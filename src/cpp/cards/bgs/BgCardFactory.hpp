#pragma once

#include <string>
#include <unordered_map>

#include "BgBaseCard.hpp"

// This factory is for auto generated cards
class BgBaseCardFactory {
public:
    BgBaseCardFactory() {}
    virtual BgBaseCard get_card(std::string name);
private:
    std::unordered_map<std::string, BgBaseCard> cards;
};

// This factory is for hand generated cards
class BgCardFactory : public BgBaseCardFactory {
public:
    BgCardFactory() { init_cards(); }
    BgBaseCard get_card(std::string name);
private:
    void init_cards();
    std::unordered_map<std::string, BgBaseCard> cards;
};
