#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "BgBaseCard.hpp"
#include "BgCardFactory.hpp"

#include "../../bg_game/board.hpp"
#include "../../bg_game/player.hpp"

std::ostream& operator<<(std::ostream& os, const BgBaseCard& card) {
    os << "(" << card.get_attack() << "/" << card.get_health() << ")";
    if (card.has_divine_shield()) {
	os << "D";
    }
    os << " ";
    return os;
}

void BgBaseCard::adapt(std::string _test_adapt) {
    std::string adaptation;
    if (_test_adapt == "None") {
	std::vector<std::string> adapts = {"Crackling Shield",
					   "Flaming Claws",
					   "Living Spores",
					   "Lightning Speed",
					   "Massive",
					   "Volcanic Might",
					   "Rocky Carapace",
					   "Poison Spit"};
	adaptation = adapts[RngSingleton::getInstance().get_rand_int() % adapts.size()];
    }
    else {
	adaptation = _test_adapt;
    }
    if (adaptation == "Crackling Shield") {
	set_divine_shield();
    }
    else if (adaptation == "Flaming Claws") {
	set_attack(get_attack() + 3);
    }
    else if (adaptation == "Living Spores") {
	auto c = BgCardFactory().get_card("Living Spore Drattle");
	deathrattle_cards.push_back(c);
    }
    else if (adaptation == "Lightning Speed") {
	set_windfury();
    }
    else if (adaptation == "Massive") {
	set_taunt();
    }
    else if (adaptation == "Volcanic Might") {
	set_attack(get_attack() + 1);
	set_health(get_health() + 1);
    }
    else if (adaptation == "Rocky Carapace") {
	set_health(get_health() + 3);
    }
    else if (adaptation == "Poison Spit") {
	set_poison();
    }
    else {
	throw std::runtime_error("Unknown adapt");
    }
    adapt_count++;
}

void BgBaseCard::do_deathrattle(Player* p1, Player* p2) {
    if (!has_taunt()) return;
    auto total_buff = 0;
    for (auto c : p1->get_board()->get_cards()) {
	if (c->get_name() == "Qiraji Harbinger") {
	    total_buff += 2;
	}
	else if (c->get_name() == "Qiraji Harbinger (Golden)") {
	    total_buff += 4;
	}
    }

    std::vector<std::shared_ptr<BgBaseCard>> cards_to_buff;
    if (death_pos == 0) {
	auto card_to_buff = p1->get_board()->get_cards()[0];
	cards_to_buff.push_back(card_to_buff);
    }
    else if (death_pos == p1->get_board()->get_cards().size()) {
	auto end_ind = p1->get_board()->get_cards().size() - 1;
	auto card_to_buff = p1->get_board()->get_cards()[end_ind];
	cards_to_buff.push_back(card_to_buff);
    }
    else {
	auto left_neighbor = p1->get_board()->get_cards()[death_pos];
	auto right_neighbor = p1->get_board()->get_cards()[death_pos-1];
	cards_to_buff.push_back(left_neighbor);
	cards_to_buff.push_back(right_neighbor);
    }

    for (auto c : cards_to_buff) {
	c->set_attack(c->get_attack() + total_buff);
	c->set_health(c->get_health() + total_buff);
    }
}

void BgBaseCard::deathrattle(Player* p1, Player* p2) {
    if (p1->get_board()->contains("Baron Rivendare (Golden)")) {
	do_deathrattle(p1, p2);
	do_deathrattle(p1, p2);
	do_deathrattle(p1, p2);
	for (auto c : deathrattle_cards) {
	    c->set_death_pos(this->death_pos);
	    c->do_deathrattle(p1, p2);
	    c->do_deathrattle(p1, p2);
	    c->do_deathrattle(p1, p2);
	}
    }
    else if (p1->get_board()->contains("Baron Rivendare")) {
	do_deathrattle(p1, p2);
	do_deathrattle(p1, p2);
	for (auto c : deathrattle_cards) {
	    c->set_death_pos(this->death_pos);
	    c->do_deathrattle(p1, p2);
	    c->do_deathrattle(p1, p2);
	}
    }
    else {
	do_deathrattle(p1, p2);
	for (auto c : deathrattle_cards) {
	    c->set_death_pos(this->death_pos);
	    c->do_deathrattle(p1, p2);
	}
    }
}

// Deals w/ base taunt-was-attacked effects
void BgBaseCard::do_predefense(std::shared_ptr<BgBaseCard> attacker,
			       Player* p1,
			       Player* p2) {
    if (!has_taunt()) return;
    Board* b1 = p1->get_board().get();
    int total_attack_buff = 0;
    for (auto c : b1->get_cards()) {
	if (c->get_name() == "Arm of the Empire") {
	    total_attack_buff += 3;
	}
	else if (c->get_name() == "Arm of the Empire (Golden)") {
	    total_attack_buff += 6;
	}
	else if (c->get_name() == "Champion of Y'Shaarj") {
	    c->set_base_health(c->get_base_health() + 1);
	    c->set_base_attack(c->get_base_attack() + 1);
	}
	else if (c->get_name() == "Champion of Y'Shaarj (Golden)") {
	    c->set_base_health(c->get_base_health() + 2);
	    c->set_base_attack(c->get_base_attack() + 2);
	}
    }
    set_attack(get_attack() + total_attack_buff);
}

void BgBaseCard::take_damage(int damage, std::string who_from_race, Player* p1, Player* p2) {
    if (divine_shield) {
	divine_shield = false;
	Board* b1 = p1->get_board().get();
	for (auto c : b1->get_cards()) {
	    // TODO: make "lost_divine_shield" hook...
	    if (c->get_name() == "Bolvar") {
		c->set_attack(c->get_attack() + 2);
	    }
	    else if (c->get_name() == "Bolvar (Golden)") {
		c->set_attack(c->get_attack() + 4);
	    }
	    else if (c->get_name() == "Drakonid Enforcer") {
		c->set_attack(c->get_attack() + 2);
		c->set_health(c->get_health() + 2);
	    }
	    else if (c->get_name() == "Drakonid Enforcer (Golden)") {
		c->set_attack(c->get_attack() + 4);
		c->set_health(c->get_health() + 4);
	    }
	}
    }
    else {
	health -= damage;
    }
    last_dmg_race = who_from_race;
}

std::shared_ptr<BgBaseCard> BgBaseCard::get_copy() const {
    return std::make_shared<BgBaseCard>(*this);
}

std::shared_ptr<BgBaseCard> BgBaseCard::do_summon(Player* p1, bool from_hand) {
    auto summoned = summon();
    // for (auto c : b1->get_cards()) {
    // 	c->mod_summoned(summoned, from_hand);
    // }
    return summoned;
}

// void BgBaseCard::basic_summon(Player* p1, bool from_hand) {
//     return basic_summon(p1->get_board().get(), from_hand);
// }

void BgBaseCard::basic_summon(Player* p1, bool from_hand) {
    return multi_summon(1, p1, from_hand);
}

void BgBaseCard::reborn_self(Player* p1) {
    auto f = BgCardFactory();    
    auto summoned_card = f.get_card(this->get_name());
    if (summoned_card->get_mechanics().find("DIVINE SHIELD") != std::string::npos) {
	summoned_card->set_divine_shield();
    }
    summoned_card->set_reborn(false);
    summoned_card->set_health(1);
    p1->get_board()->insert_card(death_pos, summoned_card, p1);
}

// void BgBaseCard::multi_summon(int num_summons, Player* p1, bool from_hand) {
//     return multi_summon(num_summons, p1->get_board().get(), from_hand);
// }


void BgBaseCard::multi_summon(int num_summons, Player* p1, bool from_hand) {
    Board* b1 = p1->get_board().get();
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
	auto summoned_card = do_summon(p1, from_hand);
	int insert_pos;
	if (this->is_dead()) {
	    insert_pos = death_pos + i;
	}
	else {
	    insert_pos = b1->get_pos(this) + 1;
	}
	if (insert_pos < b1->get_attacker_pos()) b1->increment_attacker_pos();
	b1->insert_card(insert_pos, summoned_card, p1, from_hand);
    }
}

void BgBaseCard::on_sell(Player* p1) {
    p1->add_gold(1);
}
