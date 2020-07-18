#pragma once

#include <string>

#include "BgBaseCard.hpp"

// This factory is for auto generated cards
class BgBaseCardFactory {
public:
    virtual BgBaseCard get_card(std::string name);
};

// This factory is for hand generated cards
class BgCardFactory : public BgBaseCardFactory {
public:
    BgBaseCard get_card(std::string name);
    
};
