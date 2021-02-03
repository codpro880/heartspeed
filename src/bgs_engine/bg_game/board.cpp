#include "board.hpp"
#include "player.hpp"
#include "../cards/bgs/BgBaseCard.hpp"

using namespace std;

int Board::calculate_damage() const {
    int res = 0;
    for (auto card : cards) {
        res += card->get_tavern_tier();
    }
    return res;
}

int Board::insert_card(int pos,
                       std::shared_ptr<BgBaseCard> c,
                       Player* player,
                       bool from_hand) {
    if (cards.size() == (unsigned)7) return 0;
    int total_dmg = 0;
    if ((unsigned)pos >= cards.size()) {
        // This case can occur w/ certain deathrattle interactions
        // TODO: Fix this case...if unstable ghoul nearly wipes board execpt one survivor,
        // and mecharoo was to left of surviving card, then we want it in front.
        // Else, if mecharoo was to right of surviving card, we
        // want it in back...
        for (auto card : this->get_cards()) {
            total_dmg += card->mod_summoned(c, player, from_hand);
        }
        cards.push_back(c);
        card_names.insert(c->get_name());
        // c->do_battlecry(this);
    }
    else{
        for (auto card : this->get_cards()) {
            total_dmg += card->mod_summoned(c, player, from_hand);
        }
        if (c->is_magnetic() && pos < cards.size()) {
            auto card_to_mag = cards[pos];
            if (card_to_mag->get_race() == "MECHANICAL") {
                card_to_mag->set_base_attack(card_to_mag->get_base_attack() + c->get_base_attack());
                card_to_mag->set_base_health(card_to_mag->get_base_health() + c->get_base_health());
                card_to_mag->set_poison(c->has_poison() || card_to_mag->has_poison());
                card_to_mag->set_divine_shield(c->has_divine_shield() || card_to_mag->has_divine_shield());
                card_to_mag->set_taunt(c->has_taunt() || card_to_mag->has_taunt());
                card_to_mag->set_reborn(c->has_reborn() || card_to_mag->has_reborn());
                card_to_mag->set_windfury(c->has_windfury() || card_to_mag->has_windfury());
                card_to_mag->add_to_deathrattle_cards(c);
                    return total_dmg; // Short circuit this, don't want to insert
                }
            }
            cards.insert(cards.begin() + pos, c);
            card_names.insert(c->get_name());   
            // c->do_battlecry(this);
        }
        return total_dmg;
    }


std::ostream& operator<<(std::ostream& os, const Board& board) {
    auto cards = board.get_cards();
    for (auto c : cards) {
        auto card = c.get();
        os << (*card) << " ";
    }
    os << std::endl;
    return os;
}
