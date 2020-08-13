#include <iostream>
#include <memory>

#include "DeathrattleCards.hpp"
#include "BgCardFactory.hpp"
#include "../../bg_game/board.hpp"
#include "../../bg_game/battler.hpp"

void DeathrattleCard::basic_summon(Board* b1) {
    // auto summoned_card = summon();
    // b1->insert_card(death_pos, summoned_card);
    multi_summon(1, b1);
}

void DeathrattleCard::multi_summon(int num_summons, Board* b1) {
    auto spots_left = 7 - b1->length();
    auto spots_to_fill = num_summons < spots_left ? num_summons : spots_left;

    auto f = BgCardFactory();
    for (int i = 0; i < spots_to_fill; i++) {
	auto summoned_card = summon();
	b1->insert_card(death_pos + i, summoned_card);
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
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> HarvestGolem::summon() {
    auto f = BgCardFactory();
    return f.get_card("Damaged Golem");
}

void HarvestGolemGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> HarvestGolemGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Damaged Golem (Golden)");
}

void Imprisoner::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> Imprisoner::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp");
}

void ImprisonerGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> ImprisonerGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp (Golden)");
}

// void ImprisonerGolden::do_deathrattle(Board* b1, Board* b2) {
//     basic_summon("Imp (Golden)", b1, b2);
// }

void InfestedWolf::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> InfestedWolf::summon() {
    auto f = BgCardFactory();
    return f.get_card("Spider");
}

void InfestedWolfGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> InfestedWolfGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Spider (Golden)");
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
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> KindlyGrandmother::summon() {
    auto f = BgCardFactory();
    return f.get_card("Big Bad Wolf");
}

void KindlyGrandmotherGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> KindlyGrandmotherGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Big Bad Wolf (Golden)");
}

void Mecharoo::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> Mecharoo::summon() {
    auto f = BgCardFactory();
    return f.get_card("Jo-E Bot");
}

void MecharooGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> MecharooGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Jo-E Bot (Golden)");
}

void PilotedShredder::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(1, b1);
}

std::shared_ptr<BgBaseCard> PilotedShredder::summon() {
    auto f = BgCardFactory();
    auto two_cost_cards = f.get_cards_of_cost(2);
    auto card = two_cost_cards[rand() % two_cost_cards.size()];
    return card;
}

void PilotedShredderGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> PilotedShredderGolden::summon() {
    return shredder.summon();
}

void RatPack::do_deathrattle(Board* b1, Board* b2) {
    auto attack = get_attack();
    multi_summon(attack, b1);
}

std::shared_ptr<BgBaseCard> RatPack::summon() {
    auto f = BgCardFactory();
    return f.get_card("Rat");
}

void RatPackGolden::do_deathrattle(Board* b1, Board* b2) {
    auto attack = get_attack();
    multi_summon(attack, b1);
}

std::shared_ptr<BgBaseCard> RatPackGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Rat (Golden)");
}

void ReplicatingMenace::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(3, b1);
}

std::shared_ptr<BgBaseCard> ReplicatingMenace::summon() {
    auto f = BgCardFactory();
    return f.get_card("Microbot");
}

void ReplicatingMenaceGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(3, b1);
}

std::shared_ptr<BgBaseCard> ReplicatingMenaceGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Microbot (Golden)");
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

void TheBeast::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b2);
}

std::shared_ptr<BgBaseCard> TheBeast::summon() {
    auto f = BgCardFactory();
    return f.get_card("Finkle Einhorn");
}

void TheBeastGolden::do_deathrattle(Board* b1, Board* b2) {
    the_beast.do_deathrattle(b1, b2);
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
