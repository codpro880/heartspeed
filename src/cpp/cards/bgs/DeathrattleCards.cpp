#include <iostream>
#include <memory>

#include "DeathrattleCards.hpp"
#include "BgCardFactory.hpp"
#include "../../bg_game/board.hpp"
#include "../../bg_game/battler.hpp"

void FiendishServant::do_deathrattle(Board* b1, Board*b2) {
    auto buffed_pos = rand() % b1->length();
    auto card = b1->get_cards()[buffed_pos];
    card->set_attack(card->get_attack() + attack);
    b1->set_card(buffed_pos, card);
}

void Mecharoo::do_deathrattle(Board* b1, Board* b2) {
    auto f = BgCardFactory();
    auto joebot = f.get_card("Jo-E Bot");
    b1->insert_card(death_pos, joebot);
}

void SelflessHero::do_deathrattle(Board* b1, Board*b2) {
    // Cards w/o divine shield
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    for (auto card : b1->get_cards()) {
	if (!card->has_divine_shield()) {
	    cards.push_back(card);
	}
    }
    if (cards.size() > 0) {
	auto buffed_pos = rand() % cards.size();
	auto card = cards[buffed_pos];
	card->set_divine_shield();
	b1->set_card(buffed_pos, card);
    }
}

void Scallywag::do_deathrattle(Board* b1, Board* b2) {
    auto f = BgCardFactory();
    auto sky_pirate = f.get_card("Sky Pirate");
    b1->insert_card(death_pos, sky_pirate);
    if (!b2->empty()) {
	BoardBattler().battle_boards(death_pos, b1, b2); // Modifies b1/b2
    }
}

void HarvestGolem::do_deathrattle(Board* b1, Board* b2) {
    auto f = BgCardFactory();
    auto damaged_golem = f.get_card("Damaged Golem");
    b1->insert_card(death_pos, damaged_golem);
}

void KaboomBot::do_deathrattle(Board* b1, Board* b2) {    
    if (b2->length() == 0) {
	return;
    }
    auto bombed_pos = rand() % b2->length();
    auto bombed_card = b2->get_cards()[bombed_pos];
    bombed_card->take_damage(4);
    bombed_card->deal_with_death(b2, b1);
}

void KindlyGrandmother::do_deathrattle(Board* b1, Board* b2) {
    auto f = BgCardFactory();
    auto bbw = f.get_card("Big Bad Wolf");
    b1->insert_card(death_pos, bbw);
}

void SpawnOfNzoth::do_deathrattle(Board* b1, Board* b2) {
    for (auto c : b1->get_cards()) {
	c->set_attack(c->get_attack() + 1);
	c->set_health(c->get_health() + 1);
    }
}

void UnstableGhoul::do_deathrattle(Board* b1, Board* b2) {
    for (auto c : b1->get_cards()) {
	c->take_damage(1);
	c->deal_with_death(b1, b2);
    }    
    for (auto c : b2->get_cards()) {
	c->take_damage(1);
	c->deal_with_death(b2, b1);
    }
    
}
