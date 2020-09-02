#include "BgBaseCard.hpp"
#include "BgCardFactory.hpp"

#include "../../bg_game/board.hpp"

std::ostream& operator<<(std::ostream& os, BgBaseCard& card) {
    os << "(" << card.get_attack() << "/" << card.get_health() << ")";
    if (card.has_divine_shield()) {
	os << "D";
    }
    os << " ";
    return os;
}

void BgBaseCard::take_damage(int damage, std::string who_from_race, Board* b1) {
    if (divine_shield) {
	divine_shield = false;
    }
    else {
	health -= damage;
    }
    last_dmg_race = who_from_race;
}

std::shared_ptr<BgBaseCard> BgBaseCard::get_copy() {
    return std::make_shared<BgBaseCard>(*this);
}

void BgBaseCard::deal_with_death(Board* b1, Board* b2) {
    // std::cerr << "Checking if dead." << std::endl;
    // if (this->is_dead()) {
    // 	death_pos = b1->get_pos(this);
    // 	std::cerr << "Dead." << std::endl;
    // 	b1->remove(this);
    // 	do_deathrattle(b1, b2);
    // }
}

std::shared_ptr<BgBaseCard> BgBaseCard::do_summon(Board* b1) {
    auto summoned = summon();
    for (auto c : b1->get_cards()) {
	c->mod_summoned(summoned);
    }
    return summoned;
}

void BgBaseCard::basic_summon(Board* b1) {
    // auto summoned_card = summon();
    // b1->insert_card(death_pos, summoned_card);
    multi_summon(1, b1);
}

void BgBaseCard::multi_summon(int num_summons, Board* b1) {
    auto original_num_summons = num_summons;
    for (auto c : b1->get_cards()) {
	if (c->get_name() == "Khadgar") {
	    num_summons += original_num_summons;
	}
	if (c->get_name() == "Khadgar (Golden)") {
	    num_summons += original_num_summons*2;
	}
    }
    auto spots_left = 7 - b1->length();
    auto spots_to_fill = num_summons < spots_left ? num_summons : spots_left;

    auto f = BgCardFactory();
    for (int i = 0; i < spots_to_fill; i++) {
	auto summoned_card = do_summon(b1);
	b1->insert_card(death_pos + i, summoned_card);
    }    
}
