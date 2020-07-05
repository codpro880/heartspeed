#pragma once

#include <string>

#include "BaseCard.hpp"

class CardFactory {
public:
    CardFactory(){}
    BaseCard get_card(std::string name);
};
