#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "BgBaseCard.hpp"

class BgCardFactory {
public:
    BgCardFactory() { init_cards(); }
    std::shared_ptr<BgBaseCard> get_card(std::string name);
    std::vector<std::shared_ptr<BgBaseCard> > get_cards_of_cost(int cost);
    std::vector<std::shared_ptr<BgBaseCard> > get_cards_of_rarity(std::string rarity);
private:
    void init_cards();
    std::unordered_map<std::string, std::shared_ptr<BgBaseCard> > cards;
    std::shared_ptr<BgBaseCard> init_card(int attack,
				      std::string card_class,
				      int cost,
				      int health,
				      std::string name,
				      std::string mechanics,
				      std::string race,
				      std::string rarity,
				      int tech_level,
				      std::string type);
};
