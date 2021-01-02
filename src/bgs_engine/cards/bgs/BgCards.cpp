#include <algorithm>
#include <iostream>
#include <memory>

#include "BgCards.hpp"
#include "BgCardFactory.hpp"
#include "../../bg_game/board.hpp"
#include "../../bg_game/battler.hpp"
#include "../../bg_game/rng_singleton.hpp"

// Some reusable funcs
void buff_attack_end_turn(Player* p1, int attack_buff, BgBaseCard* this_) {
    if (p1->get_board()->get_cards().size() < 1) return;
    auto cards = p1->get_board()->get_cards();
    auto card_to_buff =  cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    while (card_to_buff.get() == this_) {
	card_to_buff =  cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    }
    card_to_buff->set_attack(card_to_buff->get_attack() + attack_buff);
}

// Generic Classes
// TODO: Efficiency
void DeathrattleCard::deathrattle(Board* b1, Board* b2) {
    if (b1->contains("Baron Rivendare (Golden)")) {
	do_deathrattle(b1, b2);
	do_deathrattle(b1, b2);
	do_deathrattle(b1, b2);
    }
    else if (b1->contains("Baron Rivendare")) {
	do_deathrattle(b1, b2);
	do_deathrattle(b1, b2);
    }
    else {
	do_deathrattle(b1, b2);
    }
}

// TODO: Efficiency
void BattlecryCard::battlecry(Player* p1) {
    // kalecgos_mods(p1, this);
    if (p1->get_board()->contains("Brann Bronzebeard (Golden)")) {
	do_battlecry(p1);
	do_battlecry(p1);
	do_battlecry(p1);
    }
    else if (p1->get_board()->contains("Brann Bronzebeard")) {
	do_battlecry(p1);
	do_battlecry(p1);
    }
    else {
	do_battlecry(p1);
    }
}

// TODO: Efficiency...and design issues...
void TargetedBattlecryCard::targeted_battlecry(std::shared_ptr<BgBaseCard> c, Player* p1) {
    // kalecgos_mods(p1, this);
    if (p1->get_board()->contains("Brann Bronzebeard (Golden)")) {
	do_targeted_battlecry(c);
	do_targeted_battlecry(c);
	do_targeted_battlecry(c);
    }
    else if (p1->get_board()->contains("Brann Bronzebeard")) {
	do_targeted_battlecry(c);
	do_targeted_battlecry(c);
    }
    else {
	do_targeted_battlecry(c);
    }
}


void PirateCard::do_preattack(std::shared_ptr<BgBaseCard> defender,
			      Board* b1,
			      Board* b2) {
    int num_eliza = 0;
    int num_eliza_gold = 0;
    for (auto c : b1->get_cards()) {
	if (c->get_name() == "Dread Admiral Eliza") {
	    num_eliza += 1;
	}
	else if (c->get_name() == "Dread Admiral Eliza (Golden)") {
	    num_eliza_gold += 1;
	}
    }
    for (auto c : b1->get_cards()) {
	if (c->get_name() == "Ripsnarl Captain" && c.get() != this) {
	    set_attack(get_attack() + 2);
	    set_health(get_health() + 2);
	}
	else if (c->get_name() == "Ripsnarl Captain (Golden)" && c.get() != this) {
	    set_attack(get_attack() + 4);
	    set_health(get_health() + 4);
	}
    }
    auto total_buff = 2 * num_eliza_gold + num_eliza;
    for (auto c : b1->get_cards()) {
	c->set_attack(c->get_attack() + total_buff);
	c->set_health(c->get_health() + total_buff);
    }
}

// Specific Cards

void Alleycat::do_battlecry(Player* p1) {
    basic_summon(p1, true);
}

std::shared_ptr<BgBaseCard> Alleycat::summon() {
    auto f = BgCardFactory();
    return f.get_card("Tabbycat");
}

void AlleycatGolden::do_battlecry(Player* p1) {
    basic_summon(p1, true);
}

std::shared_ptr<BgBaseCard> AlleycatGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Tabbycat (Golden)");
}

void Amalgadon::do_battlecry(Player* p1) {
    std::unordered_set<std::string> races;
    for (auto c : p1->get_board()->get_cards()) {
	if (c.get() == this) continue;
	races.insert(c->get_race());
    }
    races.erase("");
    for (size_t i = 0; i < races.size(); ++i) {	
	adapt();
    }
}

void AmalgadonGolden::do_battlecry(Player* p1) {
    am.do_battlecry(p1);
    am.do_battlecry(p1);
    adapt_count += am.get_adapt_count();
}

void AnnihilanBattlemaster::do_battlecry(Player* p1) {
    set_health(get_health() + p1->get_damage_taken());
}

void AnnihilanBattlemasterGolden::do_battlecry(Player* p1) {
    set_health(get_health() + p1->get_damage_taken() * 2);
}

void ArcaneAssistant::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "ELEMENTAL") {
	    card->set_attack(card->get_attack() + 1);
	    card->set_health(card->get_health() + 1);
	}
    }
}

void ArcaneAssistantGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "ELEMENTAL") {
	    card->set_attack(card->get_attack() + 2);
	    card->set_health(card->get_health() + 2);
	}
    }
}

void BloodsailCannoneer::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "PIRATE") {
	    card->set_attack(card->get_attack() + 3);
	}
    }
}

void BloodsailCannoneerGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "PIRATE") {
	    card->set_attack(card->get_attack() + 6);
	}
    }
}

void CobaltScalebane::end_turn_mechanic(Player* p1) {
    buff_attack_end_turn(p1, 3, this);
}

void CobaltScalebaneGolden::end_turn_mechanic(Player* p1) {
    buff_attack_end_turn(p1, 6, this);
}


int CrowdFavorite::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) {
    if (card->has_battlecry()) {
	set_attack(get_attack() + 1);
	set_health(get_health() + 1);
    }
    return 0;
}

int CrowdFavoriteGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) {
    if (card->has_battlecry()) {
	set_attack(get_attack() + 2);
	set_health(get_health() + 2);
    }
    return 0;
}

void Crystalweaver::do_battlecry(Player* p1) {
    for (auto c : p1->get_board()->get_cards()) {
	if (c->get_race() == "DEMON") {
	    c->set_attack(c->get_attack() + 1);
	    c->set_health(c->get_health() + 1);
	}
    }

}

void CrystalweaverGolden::do_battlecry(Player* p1) {
    cw.do_battlecry(p1);
    cw.do_battlecry(p1);
}

void ColdlightSeer::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "MURLOC") {
	    card->set_health(card->get_health() + 2);
	}
    }
}

void ColdlightSeerGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "MURLOC") {
	    card->set_health(card->get_health() + 4);
	}
    }
}

// <<<<<<< Updated upstream
// =======
void defender_of_argus_mod(std::shared_ptr<BgBaseCard> card, uint8_t buff) {
    card->set_attack(card->get_attack() + buff);
    card->set_health(card->get_health() + buff);
    card->set_taunt();
}

void DefenderOfArgus::do_battlecry(Player* p1) {
    int pos = 0;
    const uint8_t buff = 1;
    auto cards = p1->get_board()->get_cards();
    for (auto c : cards) {
	if (c.get() == this) {
	    if (pos == 0) {
		defender_of_argus_mod(cards[1], buff);
	    }
	    else if ((unsigned)pos == cards.size() - 1) {
		defender_of_argus_mod(cards[cards.size() - 2], buff);
	    }
	    else {
		defender_of_argus_mod(cards[pos-1], buff);
		defender_of_argus_mod(cards[pos+1], buff);
	    }
	}
	pos++;
    }
}

void DefenderOfArgusGolden::do_battlecry(Player* p1) {
    int pos = 0;
    const uint8_t buff = 2;
    auto cards = p1->get_board()->get_cards();
    for (auto c : cards) {
	if (c.get() == this) {
	    if (pos == 0) {
		defender_of_argus_mod(cards[1], buff);
	    }
	    else if ((unsigned)pos == cards.size() - 1) {
		defender_of_argus_mod(cards[cards.size() - 2], buff);
	    }
	    else {
		defender_of_argus_mod(cards[pos-1], buff);
		defender_of_argus_mod(cards[pos+1], buff);
	    }
	}
	pos++;
    }
    //doa.do_battlecry(p1, board_pos);
    //doa.do_battlecry(p1, board_pos);
}

// >>>>>>> Stashed changes
void Djinni::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> Djinni::summon() {
    auto f = BgCardFactory();
    auto cards = f.get_cards_of_race("ELEMENTAL");
    // TOOD: Use a map or something more efficient
    int pos = 0;
    for (auto c : cards) {
	if (c->get_name() == "Djinni") {
	    break;
	}
	pos++;
    }
    cards.erase(cards.begin() + pos);
    auto card = cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    return card;
}

void DjinniGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> DjinniGolden::summon() {
    return dj.summon();
}

void FelfinNavigator::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "MURLOC") {
	    card->set_attack(card->get_attack() + 1);
	    card->set_health(card->get_health() + 1);
	}
    }
}

void FelfinNavigatorGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "MURLOC") {
	    card->set_attack(card->get_attack() + 2);
	    card->set_health(card->get_health() + 2);
	}
    }
}

void FiendishServant::do_deathrattle(Board* b1, Board*b2) {
    auto buffed_pos = RngSingleton::getInstance().get_rand_int() % b1->length();
    auto card = b1->get_cards()[buffed_pos];
    card->set_attack(card->get_attack() + attack);
}

void FiendishServantGolden::do_deathrattle(Board* b1, Board*b2) {
    serv.set_attack(this->get_attack());
    for (int i = 0; i < 2; i++) {
	serv.do_deathrattle(b1, b2);
    }
}

void FreedealingGambler::on_sell(Player* p1) {
    p1->add_gold(3);
}

void FreedealingGamblerGolden::on_sell(Player* p1) {
    p1->add_gold(6);
}

void Ghastcoiler::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> Ghastcoiler::summon() {
    auto f = BgCardFactory();
    auto cards = f.get_cards_with_deathrattle();
    auto card = cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    return card;
}

void GhastcoilerGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(4, b1);
}

std::shared_ptr<BgBaseCard> GhastcoilerGolden::summon() {
    return coiler.summon();
}

void GlyphGuardian::do_preattack(std::shared_ptr<BgBaseCard> defender,
				 Board* b1,
				 Board* b2) {
    set_attack(get_attack() * 2);
}

void GlyphGuardianGolden::do_preattack(std::shared_ptr<BgBaseCard> defender,
				       Board* b1,
				       Board* b2) {
    set_attack(get_attack() * 3);
}

void Goldgrubber::end_turn_mechanic(Player* p1) {
    int num_gold_cards = 0;
    for (auto c : p1->get_board()->get_cards()) {
	if (c->is_golden()) {
	    num_gold_cards++;
	}
    }
    set_attack(get_attack() + num_gold_cards*2);
    set_health(get_health() + num_gold_cards*2);
}

void GoldgrubberGolden::end_turn_mechanic(Player* p1) {
    int num_gold_cards = 0;
    for (auto c : p1->get_board()->get_cards()) {
	if (c->is_golden()) {
	    num_gold_cards++;
	}
    }
    set_attack(get_attack() + num_gold_cards*4);
    set_health(get_health() + num_gold_cards*4);
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

void HeraldOfFlame::do_postattack(std::shared_ptr<BgBaseCard> defender,
				  int def_pos,
				  Board* b1,
				  Board* b2) {
    if (defender->get_health() < 0) {
	auto b2_cards = b2->get_cards();
	if (!b2_cards.empty()) {
	    auto new_defender = b2_cards[0];
	    auto f = BgCardFactory();
	    auto hof = f.get_card("Herald Of Flame");
	    hof->set_attack(3);
	    BoardBattler().take_dmg_simul(hof, new_defender, b1, b2);
	}
    }
}

void HeraldOfFlameGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
					int def_pos,
					Board* b1,
					Board* b2) {
    if (defender->get_health() < 0) {
	auto b2_cards = b2->get_cards();
	if (!b2_cards.empty()) {
	    auto new_defender = b2_cards[0];
	    auto f = BgCardFactory();
	    auto hof = f.get_card("Herald Of Flame (Golden)");
	    hof->set_attack(6);
	    BoardBattler().take_dmg_simul(hof, new_defender, b1, b2);
	}
    }
}

void Houndmaster::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "BEAST") {
	c->set_attack(c->get_attack() + 2);
	c->set_health(c->get_health() + 2);
	c->set_taunt();
    }
}

void HoundmasterGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "BEAST") {
	c->set_attack(c->get_attack() + 4);
	c->set_health(c->get_health() + 4);
	c->set_taunt();
    }
}


void Imprisoner::do_deathrattle(Board* b1, Board* b2) {
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> Imprisoner::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp");
}

void ImpGangBoss::take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) {
    BgBaseCard::take_damage(damage, who_from_race, b1, b2);
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> ImpGangBoss::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp");
}

void ImpGangBossGolden::take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) {
    BgBaseCard::take_damage(damage, who_from_race, b1, b2);
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> ImpGangBossGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp (Golden)");
}

void ImpMama::take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) {
    BgBaseCard::take_damage(damage, who_from_race, b1, b2);
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> ImpMama::summon() {
    auto f = BgCardFactory();
    auto demons = f.get_cards_of_race("DEMON");    
    auto demon = demons[RngSingleton::getInstance().get_rand_int() % demons.size()];
    demon->set_taunt();
    return demon;
}

void ImpMamaGolden::take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) {
    BgBaseCard::take_damage(damage, who_from_race, b1, b2);
    multi_summon(2, b1);    
}

std::shared_ptr<BgBaseCard> ImpMamaGolden::summon() {
    return imp_mama.summon();
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

void IronhideDirehorn::do_postattack(std::shared_ptr<BgBaseCard> defender,
				     int def_pos,
				     Board* b1,
				     Board* b2) {
    if (defender->get_health() < 0) {
	basic_summon(b1);
    }
}

std::shared_ptr<BgBaseCard> IronhideDirehorn::summon() {
    auto f = BgCardFactory();
    return f.get_card("Ironhide Runt");
}


void IronhideDirehornGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
					   int def_pos,
					   Board* b1,
					   Board* b2) {
    if (defender->get_health() < 0) {
	basic_summon(b1);
    }
}

std::shared_ptr<BgBaseCard> IronhideDirehornGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Ironhide Runt (Golden)");
}

void iron_sensei_end_turn(Player* p1, int buff, BgBaseCard* this_) {
    std::vector<std::shared_ptr<BgBaseCard>> mech_cards;
    for (auto c : p1->get_board()->get_cards()) {
	if (c.get() == this_) continue;
	if (c->get_race() == "MECHANICAL") {
	    mech_cards.push_back(c);
	}
    }
    if (mech_cards.size() == 0) return;
    auto card_to_buff =  mech_cards[RngSingleton::getInstance().get_rand_int() % mech_cards.size()];
    card_to_buff->set_attack(card_to_buff->get_attack() + buff);
    card_to_buff->set_health(card_to_buff->get_health() + buff);
}

void IronSensei::end_turn_mechanic(Player* p1) {
    iron_sensei_end_turn(p1, 2, this);
}

void IronSenseiGolden::end_turn_mechanic(Player* p1) {
    iron_sensei_end_turn(p1, 4, this);
}


void Junkbot::do_postbattle(Board* b1,
			    Board* b2,
			    std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			    std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    int dead_mech_count = 0;
    for (auto c : dead_b1) {
	if (c->get_race() == "MECHANICAL") {
	    dead_mech_count++;
	}
    }
    for (int i = 0; i < dead_mech_count; i++) {
	set_attack(get_attack() + 2);
	set_health(get_health() + 2);
    }
}

void JunkbotGolden::do_postbattle(Board* b1,
				     Board* b2,
				     std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
				     std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (int i = 0; i < 2; i++) {
	junk_bot.do_postbattle(b1, b2, dead_b1, dead_b2);
    }
}


void KaboomBot::do_deathrattle(Board* b1, Board* b2) {    
    if (b2->length() == 0) {
	return;
    }
    auto bombed_pos = RngSingleton::getInstance().get_rand_int() % b2->length();
    auto bombed_card = b2->get_cards()[bombed_pos];
    BoardBattler b;
    b.take_dmg_simul(bombed_card, this->get_race(), 4, b2, b1);
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

int Kalecgos::mod_summoned(std::shared_ptr<BgBaseCard> c, Board* b1, bool from_hand) {
    if (!from_hand) return 0;
    if (c->has_battlecry()) {
	if (c->get_race() == "DRAGON") {
	    c->set_attack(c->get_attack() + 1);
	    c->set_health(c->get_health() + 1);
	}
	for (auto card : b1->get_cards()) {
	    if (card->get_race() == "DRAGON") {
		card->set_attack(card->get_attack() + 1);
		card->set_health(card->get_health() + 1);
	    }
	}
    }
    return 0;
}

int KalecgosGolden::mod_summoned(std::shared_ptr<BgBaseCard> c, Board* b1, bool from_hand) {
    k.mod_summoned(c, b1, from_hand);
    k.mod_summoned(c, b1, from_hand);
    return 0;
}

void Kangor::do_deathrattle(Board* b1, Board* b2) {
    reset_mech_queue(b1);
    //auto available_mechs = std::min(mech_queue.size(), (unsigned)2);
    auto available_mechs = mech_queue.size() < unsigned(2) ? mech_queue.size() : unsigned(2);
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

void KingBagurgle::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "MURLOC") {
	    card->set_attack(card->get_attack() + 2);
	    card->set_health(card->get_health() + 2);
	}
    }
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

void KingBagurgleGolden::do_battlecry(Player* p1) {
        for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->get_race() == "MURLOC") {
	    card->set_attack(card->get_attack() + 4);
	    card->set_health(card->get_health() + 4);
	}
    }
}

void KingBagurgleGolden::do_deathrattle(Board* b1, Board* b2) {
    for (int i = 0; i < 2; i++) {
	bag.do_deathrattle(b1, b2);
    }
}

void lt_gar_mod_sum(std::shared_ptr<BgBaseCard> card, Board* b1, bool from_hand, int buff_factor, BgBaseCard* to_buff) {
    if (card->get_race() == "ELEMENTAL" && from_hand) {
	int num_elementals = 1; // Already have one that we're playing
	for (auto c : b1->get_cards()) {
	    if (c->get_race() == "ELEMENTAL") num_elementals++;
	}
	to_buff->set_health(to_buff->get_health() + num_elementals * buff_factor);
    }
}

int LieutenantGarr::mod_summoned(std::shared_ptr<BgBaseCard> card, Board* b1, bool from_hand) {
    lt_gar_mod_sum(card, b1, from_hand, 1, this);
    return 0;
}

int LieutenantGarrGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Board* b1, bool from_hand) {
    lt_gar_mod_sum(card, b1, from_hand, 2, this);
    // lt.mod_summoned(card, b1, from_hand);
    // lt.mod_summoned(card, b1, from_hand);
    return 0;
}

int LilRag::mod_summoned(std::shared_ptr<BgBaseCard> card, Board* b1, bool from_hand) {
    if (card->get_race() == "ELEMENTAL" && from_hand) {
	auto all_elem_cards = b1->get_cards();
	all_elem_cards.clear();
	all_elem_cards.push_back(card);
	for (auto c : b1->get_cards()) {
	    if (c->get_race() == "ELEMENTAL") all_elem_cards.push_back(c);
	}
	auto card_to_buff =  all_elem_cards[RngSingleton::getInstance().get_rand_int() % all_elem_cards.size()];
	card_to_buff->set_attack(card_to_buff->get_attack() + card->get_tavern_tier());
	card_to_buff->set_health(card_to_buff->get_health() + card->get_tavern_tier());
    }
    return 0;
}

int LilRagGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Board* b1, bool from_hand) {
    lr.mod_summoned(card, b1, from_hand);
    lr.mod_summoned(card, b1, from_hand);
    return 0;
}


void MalGanis::do_precombat(Board* b1, Board*b2) {
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "DEMON") {
	    card->set_attack(card->get_attack() + 2);
	    card->set_health(card->get_health() + 2);
	}
    }
    set_attack(get_attack() - 2); // MalGanis doesn't apply to itself
    set_health(get_health() - 2); // MalGanis doesn't apply to itself
}

void MalGanis::do_deathrattle(Board* b1, Board*b2) {
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "DEMON") {
	    card->set_attack(card->get_attack() - 2);
	    card->set_health(card->get_health() - 2);
	}
    }
    set_attack(get_attack() + 2); // Malganis doesn't apply to itself
    set_health(get_health() + 2); // Malganis doesn't apply to itself
}

void MalGanisGolden::do_precombat(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_precombat(b1, b2);
    }
}

void MalGanisGolden::do_deathrattle(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_deathrattle(b1, b2);
    }
}

int MamaBear::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool) {
    if (card->get_race() == "BEAST") {
	card->set_attack(card->get_attack() + 4);
	card->set_health(card->get_health() + 4);
    }
    return 0;
}

int MamaBearGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Board* b1, bool) {
    for (int i = 0; i < 2; i++) {
	pl.mod_summoned(card, b1, false); // from hand doesn't matter
    }
    return 0;
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

void menagerie_bcry(Player* p1, uint8_t buff_amount) {
    auto board = p1->get_board();
    std::unordered_set<std::string> races;
    std::vector<std::shared_ptr<BgBaseCard>> cards_to_buff;
    auto cards = board->get_cards();
    while (!cards.empty()) {
	auto rand_pos = RngSingleton::getInstance().get_rand_int() % cards.size();
	auto card = cards[rand_pos];
	cards.erase(cards.begin() + rand_pos);
	auto race = card->get_race();
	if (races.find(race) == races.end() && race != "") {
	    races.insert(race);
	    cards_to_buff.push_back(card);
	}
	if (races.size() == (unsigned)3) {
	    break;
	}
    }

    for (auto c : cards_to_buff) {
	c->set_attack(c->get_attack() + buff_amount);
	c->set_health(c->get_health() + buff_amount);
    }
}

void MenagerieJug::do_battlecry(Player* p1) {
    menagerie_bcry(p1, 2);
}

void MenagerieJugGolden::do_battlecry(Player* p1) {
    menagerie_bcry(p1, 4);
}

void MenagerieMug::do_battlecry(Player* p1) {
    menagerie_bcry(p1, 1);
}

void MenagerieMugGolden::do_battlecry(Player* p1) {
    menagerie_bcry(p1, 2);
}


void MetaltoothLeaper::do_battlecry(Player* p1) {
    auto board = p1->get_board();
    for (auto card : board->get_cards()) {
	if (card.get() == this) {
	    continue;
	}
	if (card->get_race() == "MECHANICAL") {
	    card->set_attack(card->get_attack() + 2);
	}
    }
}

void MetaltoothLeaperGolden::do_battlecry(Player* p1) {
    auto board = p1->get_board();
    for (auto card : board->get_cards()) {
	if (card.get() == this) {
	    continue;
	}
	if (card->get_race() == "MECHANICAL") {
	    card->set_attack(card->get_attack() + 4);
	}
    }
    // Just do it twice
    // leaper.do_battlecry(p1);
    // leaper.do_battlecry(p1);
}

void micro_machine_start_turn(Player*, int attack_buff, BgBaseCard* this_) {
    this_->set_attack(this_->get_attack() + attack_buff);
}

void MicroMachine::start_turn_mechanic(Player* p1) {
    micro_machine_start_turn(p1, 1, this);
}

void MicroMachineGolden::start_turn_mechanic(Player* p1) {
    micro_machine_start_turn(p1, 2, this);
}

void MicroMummy::end_turn_mechanic(Player* p1) {
    buff_attack_end_turn(p1, 1, this);
}

void MicroMummyGolden::end_turn_mechanic(Player* p1) {
    buff_attack_end_turn(p1, 2, this);
}

void MonstrousMacaw::do_preattack(std::shared_ptr<BgBaseCard> defender,
				  Board* b1,
				  Board* b2) {
    std::vector<std::shared_ptr<BgBaseCard>> drattles;
    for (auto c : b1->get_cards()) {
	if (c->has_deathrattle()) {
	    drattles.push_back(c);
	}
    }
    if (!drattles.empty()) {
	auto random_drattle = drattles[RngSingleton::getInstance().get_rand_int() % drattles.size()];
	random_drattle->do_deathrattle(b1, b2);
    }
}

void MonstrousMacawGolden::do_preattack(std::shared_ptr<BgBaseCard> defender,
					Board* b1,
					Board* b2) {
    macaw.do_preattack(defender, b1, b2);
    macaw.do_preattack(defender, b1, b2);
}

int MurlocTidecaller::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) {
    if (card->get_race() == "MURLOC" && from_hand) {
	set_attack(get_attack() + 1);
    }
    return 0;
}

int MurlocTidecallerGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) {
    if (card->get_race() == "MURLOC" && from_hand) {
	set_attack(get_attack() + 2);
    }
    return 0;
}

void MurlocTidehunter::do_battlecry(Player* p1) {
    basic_summon(p1, true);
}

std::shared_ptr<BgBaseCard> MurlocTidehunter::summon() {
    auto f = BgCardFactory();
    return f.get_card("Murloc Scout");
}

void MurlocTidehunterGolden::do_battlecry(Player* p1) {
    basic_summon(p1, true);
}

std::shared_ptr<BgBaseCard> MurlocTidehunterGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Murloc Scout (Golden)");
}

void MurlocWarleader::do_precombat(Board* b1, Board*b2) {
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "MURLOC") {
	    card->set_attack(card->get_attack() + 2);
	}
    }
    set_attack(get_attack() - 2); // Warleader doesn't apply to itself
}

void MurlocWarleader::do_deathrattle(Board* b1, Board*b2) {
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "MURLOC") {
	    card->set_attack(card->get_attack() - 2);
	}
    }
    set_attack(get_attack() + 2); // Warleader doesn't apply to itself
}

void MurlocWarleaderGolden::do_precombat(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_precombat(b1, b2);
    }
}

void MurlocWarleaderGolden::do_deathrattle(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_deathrattle(b1, b2);
    }
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

void NathrezimOverseer::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "DEMON") {
	c->set_attack(c->get_attack() + 2);
	c->set_health(c->get_health() + 2);
    }
}

void NathrezimOverseerGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "DEMON") {
	c->set_attack(c->get_attack() + 4);
	c->set_health(c->get_health() + 4);
    }
}

void OldMurkeye::do_precombat(Board* b1, Board*b2) {
    int murloc_count = 0;
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "MURLOC") {
	    murloc_count++;
	}
    }
    set_attack(get_attack() + murloc_count - 1); // Old Murkeye doesn't apply to itself
}

void OldMurkeyeGolden::do_precombat(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_precombat(b1, b2);
    }
}

void OldMurkeye::do_postbattle(Board* b1,
				     Board* b2,
				     std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
				     std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    int dead_murloc_count = 0;
    for (auto c : dead_b1) {
	if (c->get_race() == "MURLOC") {
	    dead_murloc_count++;
	}
    }
    set_attack(get_attack() - dead_murloc_count);
}

void OldMurkeyeGolden::do_postbattle(Board* b1,
				     Board* b2,
				     std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
				     std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_postbattle(b1, b2, dead_b1, dead_b2);
    }
}

int PackLeader::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool) {
    if (card->get_race() == "BEAST") {
	card->set_attack(card->get_attack() + 3);
    }
    return 0;
}

int PackLeaderGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Board* b1, bool) {
    for (int i = 0; i < 2; i++) {
	pl.mod_summoned(card, b1, false); // from_hand doesn't matter
    }
    return 0;
}

void PilotedShredder::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(1, b1);
}

std::shared_ptr<BgBaseCard> PilotedShredder::summon() {
    auto f = BgCardFactory();
    auto two_cost_cards = f.get_cards_of_cost(2);
    auto card = two_cost_cards[RngSingleton::getInstance().get_rand_int() % two_cost_cards.size()];
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

void RedWhelp::do_precombat(Board* b1, Board*b2) {
    int drag_count = 0;
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "DRAGON") {
	    drag_count++;
	}
    }
    auto defender_pos = RngSingleton::getInstance().get_rand_int() % b2->length();
    auto defender = (*b2)[defender_pos];
    BoardBattler b;
    b.take_dmg_simul(defender, this->get_race(), drag_count, b2, b1);
}

void RedWhelpGolden::do_precombat(Board* b1, Board* b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_precombat(b1, b2);
    }
}

void RefreshingAnomaly::do_battlecry(Player* p1) {
    p1->set_free_refreshes(1);
}

void RefreshingAnomalyGolden::do_battlecry(Player* p1) {
    p1->set_free_refreshes(2);
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

void RockpoolHunter::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "MURLOC") {
	c->set_attack(c->get_attack() + 1);
	c->set_health(c->get_health() + 1);
    }
}

void RockpoolHunterGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "MURLOC") {
	c->set_attack(c->get_attack() + 2);
	c->set_health(c->get_health() + 2);
    }
}

int SaltyLooter::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) {
    if (card->get_race() == "PIRATE" && from_hand) {
	set_attack(get_attack() + 1);
	set_health(get_health() + 1);
    }
    return 0;
}

int SaltyLooterGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) {
    if (card->get_race() == "PIRATE" && from_hand) {
	set_attack(get_attack() + 2);
	set_health(get_health() + 2);
    }
    return 0;
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

void ScavengingHyena::do_postbattle(Board* b1,
				   Board* b2,
				   std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
				   std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (auto c : dead_b1) {
	if (c->get_race() == "BEAST") {
	    set_health(get_health() + 1);
	    set_attack(get_attack() + 2);
	}
    }
}

void ScavengingHyenaGolden::do_postbattle(Board* b1,
					 Board* b2,
					 std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
					 std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (auto c : dead_b1) {
	if (c->get_race() == "BEAST") {
	    set_health(get_health() + 2);
	    set_attack(get_attack() + 4);
	}
    }
}

void ScrewjankClunker::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "MECHANICAL") {
	c->set_attack(c->get_attack() + 2);
	c->set_health(c->get_health() + 2);
    }
}

void ScrewjankClunkerGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "MECHANICAL") {
	c->set_attack(c->get_attack() + 4);
	c->set_health(c->get_health() + 4);
    }
}

void SeabreakerGoliath::do_postattack(std::shared_ptr<BgBaseCard> defender,
				      int def_pos,
				      Board* b1,
				      Board* b2) {
    if (defender->get_health() < 0) {
	for (auto c : b1->get_cards()) {
	    if (c->get_race() == "PIRATE" && c.get() != this) {
		c->set_attack(c->get_attack() + 2);
		c->set_health(c->get_health() + 2);
	    }
	}
    }
}

void SeabreakerGoliathGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
					    int def_pos,
					    Board* b1,
					    Board* b2) {
    for (int i = 0; i < 2; i++) {
	sbg.do_postattack(defender, def_pos, b1, b2);
    }
}


void SecurityRover::take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) {
    BgBaseCard::take_damage(damage, who_from_race, b1, b2);
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> SecurityRover::summon() {
    auto f = BgCardFactory();
    return f.get_card("Guard Bot");
}

void SecurityRoverGolden::take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) {
    BgBaseCard::take_damage(damage, who_from_race, b1, b2);
    basic_summon(b1);
}

std::shared_ptr<BgBaseCard> SecurityRoverGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Guard Bot (Golden)");
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
	auto buffed_pos = RngSingleton::getInstance().get_rand_int() % cards.size();
	auto card = cards[buffed_pos];
	card->set_divine_shield();
    }
}

void SelflessHeroGolden::do_deathrattle(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	hero.do_deathrattle(b1, b2);
    }
}

void Sellemental::on_sell(Player* p1) {
    BgCardFactory f;
    auto wd = f.get_card("Water Droplet");
    p1->add_card_to_hand(wd);
    p1->add_gold(1);
}

void SellementalGolden::on_sell(Player* p1) {
    BgCardFactory f;
    auto wd = f.get_card("Water Droplet (Golden)");
    p1->add_card_to_hand(wd);
    p1->add_gold(1);
}

void Siegebreaker::do_precombat(Board* b1, Board*b2) {
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "DEMON") {
	    card->set_attack(card->get_attack() + 1);
	}
    }
    set_attack(get_attack() - 1); // Siegbreaker doesn't apply to itself
}

void Siegebreaker::do_deathrattle(Board* b1, Board*b2) {
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "DEMON") {
	    card->set_attack(card->get_attack() - 1);
	}
    }
    set_attack(get_attack() + 1); // Siegebreaker doesn't apply to itself
}

void SiegebreakerGolden::do_precombat(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	sb.do_precombat(b1, b2);
    }
}

void SiegebreakerGolden::do_deathrattle(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	sb.do_deathrattle(b1, b2);
    }
}

void SneedsOldShredder::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(1, b1);
}

std::shared_ptr<BgBaseCard> SneedsOldShredder::summon() {
    auto f = BgCardFactory();
    auto legendary_cards = f.get_cards_of_rarity("LEGENDARY");
    auto card = legendary_cards[RngSingleton::getInstance().get_rand_int() % legendary_cards.size()];
    return card;
}

void SneedsOldShredderGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(2, b1);
}

std::shared_ptr<BgBaseCard> SneedsOldShredderGolden::summon() {
    return shredder.summon();
}

void SoulJuggler::do_postbattle(Board* b1,
				Board* b2,
				std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
				std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    int dead_demon_count = 0;
    for (auto c : dead_b1) {
	if (c->get_race() == "DEMON") {
	    dead_demon_count++;
	}
    }
    auto cards = b2->get_cards();
    if (cards.empty()) {
	return;
    }
    auto battler = BoardBattler();
    for (int i = 0; i < dead_demon_count; i++) {
	auto card = cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
	battler.take_dmg_simul(card, "NEUTRAL", 3, b1, b2);
    }
    
}

void SoulJugglerGolden::do_postbattle(Board* b1,
				     Board* b2,
				     std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
				     std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (int i = 0; i < 2; i++) {
	soul_juggler.do_postbattle(b1, b2, dead_b1, dead_b2);
    }
}

void SouthseaCaptain::do_precombat(Board* b1, Board*b2) {
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "PIRATE") {
	    card->set_attack(card->get_attack() + 1);
	    card->set_health(card->get_health() + 1);
	}
    }
    set_attack(get_attack() - 1); // Southsea doesn't apply to itself
    set_health(get_health() - 1); // Southsea doesn't apply to itself
}

void SouthseaCaptain::do_deathrattle(Board* b1, Board*b2) {
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "PIRATE") {
	    card->set_attack(card->get_attack() - 1);
	    card->set_health(card->get_health() - 1);
	}
    }
    set_attack(get_attack() + 1);  // Southsea doesn't apply to itself
    set_health(get_health() + 1);  // Southsea doesn't apply to itself
}

void SouthseaCaptainGolden::do_precombat(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_precombat(b1, b2);
    }
}

void SouthseaCaptainGolden::do_deathrattle(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_deathrattle(b1, b2);
    }
}

void SouthseaStrongarm::targeted_battlecry(std::shared_ptr<BgBaseCard> c, Player* p1) {
    pirates_bought_this_turn = p1->get_pirates_bought_this_turn();
    TargetedBattlecryCard::targeted_battlecry(c, p1);
}

void SouthseaStrongarm::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "PIRATE") {
	c->set_attack(c->get_attack() + pirates_bought_this_turn);
	c->set_health(c->get_health() + pirates_bought_this_turn);
    }
}

void SouthseaStrongarmGolden::targeted_battlecry(std::shared_ptr<BgBaseCard> c, Player* p1) {
    pirates_bought_this_turn = p1->get_pirates_bought_this_turn();
    TargetedBattlecryCard::targeted_battlecry(c, p1);
}

void SouthseaStrongarmGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "PIRATE") {
	c->set_attack(c->get_attack() + pirates_bought_this_turn * 2);
	c->set_health(c->get_health() + pirates_bought_this_turn * 2);
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

void StasisElemental::do_battlecry(Player* p1) {
    BgCardFactory f;
    auto elementals = f.get_cards_of_race("ELEMENTAL");
    std::vector<std::shared_ptr<BgBaseCard>> elementals_at_or_below_tier;
    for (auto card : elementals) {
	if (card->get_tavern_tier() <= p1->get_tavern_tier()) {
	    elementals_at_or_below_tier.push_back(card);
	}
    }
    auto to_freeze = elementals_at_or_below_tier[RngSingleton::getInstance().get_rand_int() % elementals_at_or_below_tier.size()];
    p1->add_to_frozen_minions(to_freeze->get_name());
}

void StasisElementalGolden::do_battlecry(Player* p1) {
    se.do_battlecry(p1);
    se.do_battlecry(p1);
}

void StewardOfTime::on_sell(Player* p1) {
    p1->buff_tav_till_refresh(1, 1);
}

void StewardOfTimeGolden::on_sell(Player* p1) {
    p1->buff_tav_till_refresh(2, 2);
}

void StrongshellScavenger::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->has_taunt()) {
	    card->set_attack(card->get_attack() + 2);
	    card->set_health(card->get_health() + 2);
	}
    }
}

void StrongshellScavengerGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
	if (card.get() == this) continue;
	if (card->has_taunt()) {
	    card->set_attack(card->get_attack() + 4);
	    card->set_health(card->get_health() + 4);
	}
    }
}

void TavernTempest::do_battlecry(Player* p1) {
    auto f = BgCardFactory();
    auto elementals = f.get_cards_of_race("ELEMENTAL");
    auto card = elementals[RngSingleton::getInstance().get_rand_int() % elementals.size()];
    p1->add_card_to_hand(card);
}

void TavernTempestGolden::do_battlecry(Player* p1) {
    tt.do_battlecry(p1);
    tt.do_battlecry(p1);
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
    auto card = cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    return card;
}

void TheTideRazorGolden::do_deathrattle(Board* b1, Board* b2) {
    multi_summon(6, b1);
}

std::shared_ptr<BgBaseCard> TheTideRazorGolden::summon() {
    return ttr.summon();
}

void Toxfin::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "MURLOC") {
	c->set_poison();
    }
}

void ToxfinGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    tf.do_targeted_battlecry(c);
}

void TwilightEmissary::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "DRAGON") {
	c->set_attack(c->get_attack() + 2);
	c->set_health(c->get_health() + 2);
    }
}

void TwilightEmissaryGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "DRAGON") {
	c->set_attack(c->get_attack() + 4);
	c->set_health(c->get_health() + 4);
    }
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
    auto b1_cards = b1->get_cards();
    auto b2_cards = b2->get_cards();
    b1_cards.insert(b1_cards.end(), b2_cards.begin(), b2_cards.end());
    std::vector<std::string> who_from_race(b1_cards.size(), this->get_race());
    BoardBattler b;
    b.take_dmg_simul(b1_cards, who_from_race, 1, b1, b2);
}

void UnstableGhoulGolden::do_deathrattle(Board* b1, Board* b2) {
    for (int i = 0; i < 2; i++) {
	ghoul.do_deathrattle(b1, b2);
    }
}

void VirmenSensei::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "BEAST") {
	c->set_attack(c->get_attack() + 2);
	c->set_health(c->get_health() + 2);
    }
}

void VirmenSenseiGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "BEAST") {
	c->set_attack(c->get_attack() + 4);
	c->set_health(c->get_health() + 4);
    }
}

void VulgarHomunculus::do_battlecry(Player* p1) {
    p1->take_damage(2, true);
}

void VulgarHomunculusGolden::do_battlecry(Player* p1) {
    p1->take_damage(4, true);
}

void WaxriderTogwaggle::do_postbattle(Board* b1,
				      Board* b2,
				      std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
				      std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (auto c : dead_b2) {
	if (c->who_killed_race() == "DRAGON") {
	    set_health(get_health() + 2);
	    set_attack(get_attack() + 2);
	}
    }
}

void WaxriderTogwaggleGolden::do_postbattle(Board* b1,
					    Board* b2,
					    std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
					    std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (auto c : dead_b2) {
	if (c->who_killed_race() == "DRAGON") {
	    set_health(get_health() + 4);
	    set_attack(get_attack() + 4);
	}
    }
}

void WildfireElemental::do_postattack(std::shared_ptr<BgBaseCard> defender,
				      int def_pos,
				      Board* b1,
				      Board* b2) {
    auto b2_cards = b2->get_cards();
    if (b2_cards.size() == 0) return;
    if (defender->get_health() < 0) {
	auto damage = -1 * defender->get_health();
	int new_defender_pos = 0;
	if (b2_cards.size() > 1) {
	    auto lor = RngSingleton::getInstance().get_rand_int() % 2;
	    if (lor) { // left
		new_defender_pos = std::max(0, def_pos - 1);
	    }
	    else { // right
		// def_pos since defender is dead
		// (everyone shifted left to fill)
		new_defender_pos = std::min(b2_cards.size() - 1, (size_t)def_pos);
	    }
	}
	auto new_defender = b2_cards[new_defender_pos];
	BoardBattler().take_dmg_simul(new_defender,
				      "ELEMENTAL",
				      damage,
				      b1,
				      b2);
    }
}

void WildfireElementalGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
					    int def_pos,
					    Board* b1,
					    Board* b2) {
    auto b2_cards = b2->get_cards();
    if (b2_cards.size() == 0) return;
    if (defender->get_health() < 0) {	
	auto damage = -1 * defender->get_health();
	int new_defender_pos = 0;
	if (b2_cards.size() == 1) {
	    auto new_defender = b2_cards[0];
	    BoardBattler().take_dmg_simul(new_defender,
					  "ELEMENTAL",
					  damage,
					  b1,
					  b2);
	}
	else if ((unsigned)def_pos == b2_cards.size()) { // one already dead
	    // b2_cards.size()-1 b/c end minion already dead
	    auto new_defender = b2_cards[b2_cards.size()-1];
	    BoardBattler().take_dmg_simul(new_defender,
					  "ELEMENTAL",
					  damage,
					  b1,
					  b2);

	}
	else if ((unsigned)def_pos == 0) {
	    // index 1 gauranteed to exist, or first if would be hit
	    auto new_defender = b2_cards[1];
	    BoardBattler().take_dmg_simul(new_defender,
					  "ELEMENTAL",
					  damage,
					  b1,
					  b2);

	}
	else {
	    auto new_defender_left = b2_cards[def_pos - 1];
	    auto new_defender_right = b2_cards[def_pos];
	    std::vector<std::shared_ptr<BgBaseCard>> cards{new_defender_left, new_defender_right};
	    std::vector<int> dmg{damage, damage};
	    std::vector<std::string> race{"ELEMENTAL", "ELEMENTAL"};
	    BoardBattler().take_dmg_simul(cards,
					  race,
					  dmg,
					  b1,
					  b2);
	}
    }
}

int WrathWeaver::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) {
    if (card->get_race() == "DEMON" && from_hand) {
	set_attack(get_attack() + 2);
	set_health(get_health() + 2);
	return 1;
    }
    return 0;
}

int WrathWeaverGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) {
    if (card->get_race() == "DEMON" && from_hand) {
	set_attack(get_attack() + 4);
	set_health(get_health() + 4);
	return 1;
    }    
    return 0;
}

void YoHoOgre::do_postdefense(std::shared_ptr<BgBaseCard> attacker, Board* b1, Board* b2) {
    if (this->is_dead()) {
	return;
    }
    BoardBattler().battle_boards(b1->get_pos(this), b1, b2); // Modifies b1/b2    
}

void YoHoOgreGolden::do_postdefense(std::shared_ptr<BgBaseCard> attacker, Board* b1, Board* b2) {
    if (this->is_dead()) {
	return;
    }
    BoardBattler().battle_boards(b1->get_pos(this), b1, b2); // Modifies b1/b2    
}
