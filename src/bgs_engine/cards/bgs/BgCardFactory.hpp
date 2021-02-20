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
    std::vector<std::shared_ptr<BgBaseCard> > get_cards_with_deathrattle(bool include_golden=false) const;
    std::vector<std::shared_ptr<BgBaseCard> > get_cards_of_race(std::string race, bool include_golden=false) const;
    std::unordered_map<int, std::vector<std::string>> get_card_names_by_tier() const;
private:
    void init_cards();
    std::unordered_map<std::string, std::shared_ptr<BgBaseCard> > cards;
};
