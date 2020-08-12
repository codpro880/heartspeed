#include <iostream>
#include <memory>

#include "DeathrattleCards.hpp"
#include "BgCardFactory.hpp"
#include "../../bg_game/board.hpp"
#include "../../bg_game/battler.hpp"

void DeathrattleCard::basic_summon(std::string cardname, Board* b1, Board* b2) {
    auto f = BgCardFactory();
    auto summon = f.get_card(cardname);
    b1->insert_card(death_pos, summon);
}

void DeathrattleCard::multi_summon(std::string cardname, int num_summons, Board* b1, Board* b2) {
    auto spots_left = 7 - b1->length();
    auto spots_to_fill = num_summons < spots_left ? num_summons : spots_left;

    auto f = BgCardFactory();
    for (int i = 0; i < spots_to_fill; i++) {
	auto rat = f.get_card(cardname);
	b1->insert_card(death_pos + i, rat);
    }    
}

void FiendishServant::do_deathrattle(Board* b1, Board*b2) {
    auto buffed_pos = rand() % b1->length();
    auto card = b1->get_cards()[buffed_pos];
    card->set_attack(card->get_attack() + attack);
}

void FiendishServantGolden::do_deathrattle(Board* b1, Board*b2) {
    serv.set_attack(this->get_attack());
    for (int i = 0; i < 2; i++) {
	serv.do_deathrattle(b1, b2);
    }
}

void HarvestGolem::do_deathrattle(Board* b1, Board* b2) {
    basic_summon("Damaged Golem", b1, b2);
}

void HarvestGolemGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon("Damaged Golem (Golden)", b1, b2);
}

void Imprisoner::do_deathrattle(Board* b1, Board* b2) {
    basic_summon("Imp", b1, b2);
}

void ImprisonerGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon("Imp (Golden)", b1, b2);
}

void InfestedWolf::do_deathrattle(Board* b1, Board* b2) {
    multi_summon("Spider", 2, b1, b2);
}

void InfestedWolfGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon("Spider (Golden)", 2, b1, b2);
}

void KaboomBot::do_deathrattle(Board* b1, Board* b2) {    
    if (b2->length() == 0) {
	return;
    }
    auto bombed_pos = rand() % b2->length();
    auto bombed_card = b2->get_cards()[bombed_pos];
    bombed_card->take_damage(4, b2, b1);
}

void KaboomBotGolden::do_deathrattle(Board* b1, Board* b2) {    
    for (int i = 0; i < 2; i++) {
	kbot.do_deathrattle(b1, b2);
	b1->remove_and_mark_dead();
	b2->remove_and_mark_dead();
    }
    b1->do_deathrattles(b2);
    b2->do_deathrattles(b2);
}

void KindlyGrandmother::do_deathrattle(Board* b1, Board* b2) {
    basic_summon("Big Bad Wolf", b1, b2);
}

void KindlyGrandmotherGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon("Big Bad Wolf (Golden)", b1, b2);
}

void Mecharoo::do_deathrattle(Board* b1, Board* b2) {
    basic_summon("Jo-E Bot", b1, b2);
}

void MecharooGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon("Jo-E Bot (Golden)", b1, b2);
}

void RatPack::do_deathrattle(Board* b1, Board* b2) {
    auto attack = get_attack();
    multi_summon("Rat", attack, b1, b2);
}

void RatPackGolden::do_deathrattle(Board* b1, Board* b2) {
    auto attack = get_attack();
    multi_summon("Rat (Golden)", attack, b1, b2);
}

void ReplicatingMenace::do_deathrattle(Board* b1, Board* b2) {
    auto attack = get_attack();
    multi_summon("Microbot", 3, b1, b2);
}

void ReplicatingMenaceGolden::do_deathrattle(Board* b1, Board* b2) {
    auto attack = get_attack();
    multi_summon("Microbot (Golden)", 3, b1, b2);
}

void Scallywag::do_deathrattle(Board* b1, Board* b2) {
    auto f = BgCardFactory();
    auto sky_pirate = f.get_card("Sky Pirate");
    b1->insert_card(death_pos, sky_pirate);
    if (!b2->empty()) {
	BoardBattler().battle_boards(death_pos, b1, b2); // Modifies b1/b2
    }
}

void ScallywagGolden::do_deathrattle(Board* b1, Board* b2) {
    auto f = BgCardFactory();
    auto sky_pirate = f.get_card("Sky Pirate (Golden)");
    b1->insert_card(death_pos, sky_pirate);
    if (!b2->empty()) {
	BoardBattler().battle_boards(death_pos, b1, b2); // Modifies b1/b2
    }
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
    }
}

void SelflessHeroGolden::do_deathrattle(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	hero.do_deathrattle(b1, b2);
    }
}

void SpawnOfNzoth::do_deathrattle(Board* b1, Board* b2) {
    for (auto c : b1->get_cards()) {
	c->set_attack(c->get_attack() + 1);
	c->set_health(c->get_health() + 1);
    }
}

void SpawnOfNzothGolden::do_deathrattle(Board* b1, Board* b2) {
    for (auto c : b1->get_cards()) {
	c->set_attack(c->get_attack() + 2);
	c->set_health(c->get_health() + 2);
    }
}

void UnstableGhoul::do_deathrattle(Board* b1, Board* b2) {
    for (auto c : b1->get_cards()) {
	c->take_damage(1, b1, b2);
    }    
    for (auto c : b2->get_cards()) {
	c->take_damage(1, b2, b1);
    }
}

void UnstableGhoulGolden::do_deathrattle(Board* b1, Board* b2) {
    for (int i = 0; i < 2; i++) {
	ghoul.do_deathrattle(b1, b2);
	b1->remove_and_mark_dead();
	b2->remove_and_mark_dead();
	b1->do_deathrattles(b2);
	b2->do_deathrattles(b1);
    }
}
