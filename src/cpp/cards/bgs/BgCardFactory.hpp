#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "BgBaseCard.hpp"

class BgCardFactory {
public:
    BgCardFactory() { init_cards(); }
    std::shared_ptr<BgBaseCard> get_card(std::string name) const;
    std::vector<std::shared_ptr<BgBaseCard> > get_cards_of_cost(int cost) const;
    std::vector<std::shared_ptr<BgBaseCard> > get_cards_of_rarity(std::string rarity) const;
    std::vector<std::shared_ptr<BgBaseCard> > get_cards_with_deathrattle() const;
    std::vector<std::shared_ptr<BgBaseCard> > get_cards_of_race(std::string race, bool include_golden=false) const;
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
