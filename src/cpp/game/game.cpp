#include <string>
#include <vector>

#include "../cards/BaseCard.hpp"
#include "game.hpp"

BaseCard* Deck::get_top_card() {
    auto result = cards.back();
    cards.pop_back();
    return result;
}

std::string Deck::get_class() {
    if (hsclass != "") {
	for (auto card : cards) {
	    if (card->get_class() != "NEUTRAL") {
		hsclass = card->get_class();
		return hsclass;
	    }
	}
	hsclass = "NO CLASS";
	return hsclass;
    }
    else {
	return hsclass;
    }
}
