#include <algorithm>
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

void Ghastcoiler::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> Ghastcoiler::summon() {
    auto f = BgCardFactory();
    auto cards = f.get_cards_with_deathrattle();
    auto card = cards[rand() % cards.size()];
    return card;
}

void GhastcoilerGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(4, b1);
}

std::shared_ptr<BgBaseCard> GhastcoilerGolden::summon() {
    return coiler.summon();
}

void Goldrinn::do_deathrattle(Board* b1, Board* b2) {
    auto cards = b1->get_cards();
    for (auto c : cards) {
	if (c->get_race() == "BEAST") {
	    c->set_health(c->get_health() + 4);
	    c->set_attack(c->get_attack() + 4);
	}
    }
}

void GoldrinnGolden::do_deathrattle(Board* b1, Board* b2) {
    for (int i = 0; i < 2; i++) {
	bag.do_deathrattle(b1, b2);
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
    BoardBattler b;
    b.take_dmg_simul(bombed_card, 4, b2, b1);
    // bombed_card->take_damage(4, b2, b1);
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

void Kangor::do_deathrattle(Board* b1, Board* b2) {
    reset_mech_queue(b1);
    //auto available_mechs = std::min(mech_queue.size(), (unsigned)2);
    auto available_mechs = mech_queue.size() < unsigned(2) ? mech_queue.size() : unsigned(2);
    std::cerr << "Avail mechs: " << available_mechs << std::endl;
    multi_summon(available_mechs, b1);
}

std::shared_ptr<BgBaseCard> Kangor::summon() {
    auto card_name = mech_queue.front();
    mech_queue.pop();
    auto f = BgCardFactory();
    return f.get_card(card_name);
}

void Kangor::reset_mech_queue(Board* b) {
    std::queue<std::string>().swap(mech_queue); // clear current queue
    for (auto c : b->has_died()) {
	if (c->get_race() == "MECHANICAL") {
	    mech_queue.push(c->get_name());
	}
    }
}

void KangorGolden::do_deathrattle(Board* b1, Board* b2) {
    kang.reset_mech_queue(b1);
    auto available_mechs = kang.mech_queue.size() < unsigned(4) ? kang.mech_queue.size() : unsigned(4);
    multi_summon(available_mechs, b1);
}

std::shared_ptr<BgBaseCard> KangorGolden::summon() {
    return kang.summon();
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

void KingBagurgle::do_deathrattle(Board* b1, Board* b2) {
    auto cards = b1->get_cards();
    for (auto c : cards) {
	if (c->get_race() == "MURLOC") {
	    c->set_health(c->get_health() + 2);
	    c->set_attack(c->get_attack() + 2);
	}
    }
}

void KingBagurgleGolden::do_deathrattle(Board* b1, Board* b2) {
    for (int i = 0; i < 2; i++) {
	bag.do_deathrattle(b1, b2);
    }
}

void MechanoEgg::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> MechanoEgg::summon() {
    auto f = BgCardFactory();
    return f.get_card("Robosaur");
}

void MechanoEggGolden::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> MechanoEggGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Robosaur (Golden)");
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

void Nadina::do_deathrattle(Board* b1, Board* b2) {
    auto cards = b1->get_cards();
    for (auto c : cards) {
	if (c->get_race() == "DRAGON") {
	    c->set_divine_shield();
	}
    }
}

void NadinaGolden::do_deathrattle(Board* b1, Board* b2) {
    bag.do_deathrattle(b1, b2);
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

void SavannahHighmane::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> SavannahHighmane::summon() {
    auto f = BgCardFactory();
    return f.get_card("Hyena");
}

void SavannahHighmaneGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> SavannahHighmaneGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Hyena (Golden)");
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

void SneedsOldShredder::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(1, b1);
}

std::shared_ptr<BgBaseCard> SneedsOldShredder::summon() {
    auto f = BgCardFactory();
    auto legendary_cards = f.get_cards_of_rarity("LEGENDARY");
    auto card = legendary_cards[rand() % legendary_cards.size()];
    return card;
}

void SneedsOldShredderGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> SneedsOldShredderGolden::summon() {
    return shredder.summon();
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

void TheTideRazor::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(3, b1);
}

std::shared_ptr<BgBaseCard> TheTideRazor::summon() {
    auto f = BgCardFactory();
    auto cards = f.get_cards_of_race("PIRATE");
    auto card = cards[rand() % cards.size()];
    return card;
}

void TheTideRazorGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(6, b1);
}

std::shared_ptr<BgBaseCard> TheTideRazorGolden::summon() {
    return ttr.summon();
}

void Voidlord::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(3, b1);
}

std::shared_ptr<BgBaseCard> Voidlord::summon() {
    auto f = BgCardFactory();
    return f.get_card("Voidwalker");
}

void VoidlordGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(3, b1);
}

std::shared_ptr<BgBaseCard> VoidlordGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Voidwalker (Golden)");
}

void UnstableGhoul::do_deathrattle(Board* b1, Board* b2) {
    std::cerr << "Doin ghoul drattle" << std::endl;
    auto b1_cards = b1->get_cards();
    auto b2_cards = b2->get_cards();
    std::cerr << "Copied" << std::endl;
    b1_cards.insert(b1_cards.end(), b2_cards.begin(), b2_cards.end());
    std::cerr << "Concatted" << std::endl;
    BoardBattler b;
    std::cerr << "Dmg simul." << std::endl;
    b.take_dmg_simul(b1_cards, 1, b1, b2);
}

void UnstableGhoulGolden::do_deathrattle(Board* b1, Board* b2) {
    for (int i = 0; i < 2; i++) {
	std::cerr << "Doin comp drattle" << std::endl;
	ghoul.do_deathrattle(b1, b2);
    }
}
