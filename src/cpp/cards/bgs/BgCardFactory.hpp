#pragma once

#include <string>

#include "BgBaseCard.hpp"

class BgCardFactory {
public:
    BgCardFactory(){}
    BgBaseCard get_card(std::string name);
};
