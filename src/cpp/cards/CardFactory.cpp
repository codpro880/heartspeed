#include <string>

#include "BaseCard.hpp"
#include "CardFactory.hpp"

BaseCard CardFactory::get_card(std::string name) {
    if (name == "FlameLance") {
	return BaseCard("FlameLance", 5, "AT_001", "Deal $8 damage to a minion.");
    }
    else {
	throw std::runtime_error("No card has name " + name);
    }
}
