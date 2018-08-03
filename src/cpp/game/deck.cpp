#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "../cards/BaseCard.hpp"
#include "../cards/CardFactory.hpp"
#include "game.hpp"

BaseCard Deck::get_top_card() {
    auto result = cards.back();
    cards.pop_back();
    return result;
}

std::string Deck::get_class() {
    if (hsclass == "") {
	for (auto card : cards) {
	    if (card.get_class() != "NEUTRAL") {
		hsclass = card.get_class();
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

Deck DeckFactory::get_deck(std::string name, bool shuffle) {
    std::vector<std::string> card_names;
    if (name == "Mage") {
	card_names = {"ArcaneMissiles", "ArcaneMissiles",
		      "ArcaneExplosion", "ArcaneExplosion",
		      "ArcaneIntellect", "ArcaneIntellect",
		      "Fireball", "Fireball",
		      "Polymorph", "Polymorph",
		      "MurlocRaider", "MurlocRaider",
		      "BloodfenRaptor", "BloodfenRaptor",
		      "NoviceEngineer", "NoviceEngineer",
		      "RiverCrocolisk", "RiverCrocolisk",
		      "RaidLeader", "RaidLeader",
		      "Wolfrider", "Wolfrider",
		      "OasisSnapjaw", "OasisSnapjaw",
		      "Sen'jinShieldmasta", "Sen'jinShieldmasta",
		      "Nightblade", "Nightblade",
		      "BoulderfistOgre", "BoulderfistOgre"};
    }

    CardFactory factory = CardFactory();
    std::vector<BaseCard> cards;
    for (auto card_name : card_names) {
	cards.push_back(factory.get_card(card_name));
    }
    if (shuffle) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(cards.begin(), cards.end(), g);
    }
    return Deck(cards);
}
