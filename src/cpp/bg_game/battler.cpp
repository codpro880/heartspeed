#include "battler.hpp"

#include <stdlib.h>
#include <cstdlib>
#include <stdexcept>

#include <iostream>

BattleResult Battler::sim_battle(std::string goes_first) {
    return sim_battle(p1, p2, goes_first);
}

BattleResult Battler::sim_battle(Player* p1, Player* p2, std::string goes_first) {
    auto b1 = p1->get_board();
    auto b2 = p2->get_board();
    BattleResult res = BattleResult();
    if (b1->empty() && b2->empty()) {
	res.who_won = "draw";
	res.damage_taken = 0;
    }
    else if (b1->empty()) {
	res.who_won = p2->get_name();
	res.damage_taken = p2->calculate_damage();
    }
    else if (b2->empty()) {
	res.who_won = p1->get_name();
	res.damage_taken = p1->calculate_damage();
    }
    else {
	auto first_player = decide_who_goes_first(b1, b2);
	if (goes_first != "null") {
	    first_player = goes_first;
	}
	if (first_player == "p1") {
	    res = battle(p1, p2);
	}
	else {
	    res = battle(p2, p1);
	}
    }
    return res;
}

BattleResult Battler::battle(Player* p1,
			     Player* p2,
			     int p1_counter,
			     int p2_counter) {
    // base case
    auto b1 = p1->get_board();
    auto b2 = p2->get_board();
    std::cout << "P1 (before): " << std::endl;
    std::cout << (*p1) << std::endl;
    std::cout << "Attacker pos: " << p1_counter << std::endl;
    std::cout << "P2 (before): " << std::endl;
    std::cout << (*p2) << std::endl;
    BattleResult res = BattleResult();
    if (b1->empty() && b2->empty()) {
	res.who_won = "draw";
	res.damage_taken = 0;
	return res;
    }
    else if (b1->empty()) {
	res.who_won = p2->get_name();
	res.damage_taken = p2->calculate_damage();
	return res;
    }
    else if (b2->empty()) {
	res.who_won = p1->get_name();
	res.damage_taken = p1->calculate_damage();
	return res;
    }    
    
    // b1 always goes first here
    if (p1_counter >= b1->length()) {
	p1_counter = 0;
    }

    //p1->set_board(b1);
    //p2->set_board(b2);
    auto attacker_is_dead = board_battler.battle_boards(p1_counter, b1, b2); // Modifies b1/b2
    if (!attacker_is_dead) {
	p1_counter++;
    }
    std::cout << "P1: " << (*p1) << std::endl;
    std::cout << "Attacker pos: " << p1_counter << std::endl;
    std::cout << "P2: " << (*p2) << std::endl;
    return battle(p2, p1, p2_counter, p1_counter);
}

void BoardBattler::take_dmg_simul(std::shared_ptr<BgBaseCard> attacker,
				  std::shared_ptr<BgBaseCard> defender,
				  Board* b1,
				  Board* b2) {
    attacker->do_preattack(defender, b1, b2);
    std::vector<int> dmg;
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    if (attacker->has_cleave() && b2->get_cards().size() > (unsigned)1) {
	auto def_pos = b2->get_pos(defender);
	if (def_pos == 0) {
	    dmg = {defender->get_attack(), attacker->get_attack(), attacker->get_attack()};
	    cards = {attacker, defender, b2->get_cards()[def_pos+1]};
	}
	else if (def_pos == b2->get_cards().size()) {
	    dmg = {defender->get_attack(), attacker->get_attack(), attacker->get_attack()};
	    cards = {attacker, defender, b2->get_cards()[def_pos-1]};
	}
	else if (b2->get_cards().size() == (unsigned)2) {
	    dmg = {defender->get_attack(), attacker->get_attack(), attacker->get_attack()};
	    cards = {attacker, b2->get_cards()[0], b2->get_cards()[1]};
	}
	else { // More than 2 and not on ends
	    dmg = {defender->get_attack(), attacker->get_attack(), attacker->get_attack(), attacker->get_attack()};
	    cards = {attacker, defender, b2->get_cards()[def_pos+1], b2->get_cards()[def_pos-1]};
	}
    }
    else {
	dmg = {defender->get_attack(), attacker->get_attack()};
	cards = {attacker, defender};
    }
    std::vector<std::string> who_from_race = {defender->get_race(), attacker->get_race()};
    take_dmg_simul(cards, who_from_race, dmg, b1, b2);
    attacker->do_postattack(defender, b1, b2);
    defender->do_postdefense(attacker, b2, b1);
}

void BoardBattler::take_dmg_simul(std::shared_ptr<BgBaseCard> card,
				  std::string who_from_race,
				  int dmg,
				  Board* b1,
				  Board* b2) {
    auto cards = {card};
    auto who_from_races = {who_from_race};
    take_dmg_simul(cards, who_from_races, dmg, b1, b2);
}

void BoardBattler::take_dmg_simul(std::vector<std::shared_ptr<BgBaseCard>> cards,
				  std::vector<std::string> who_from_race,
				  int dmg,
				  Board* b1,
				  Board* b2) {
    for (int i = 0; i < cards.size(); i++) {
	if (b1->contains(cards[i])) {
	    std::cerr << "Card " << cards[i]->get_name() << " in b1";
	    cards[i]->take_damage(dmg, who_from_race[i], b1, b2);
	}
	else {
	    cards[i]->take_damage(dmg, who_from_race[i], b2, b1);
	}
    }
    
    b1->remove_and_mark_dead();
    b2->remove_and_mark_dead();

    // TODO: Ordering seems to be a coin flip?
    b1->do_deathrattles(b2);
    b2->do_deathrattles(b1);
}

void BoardBattler::take_dmg_simul(std::vector<std::shared_ptr<BgBaseCard>> cards,
				  std::vector<std::string> who_from_race,
				  std::vector<int> dmg,
				  Board* b1,
				  Board* b2) {
    for (int i = 0; i < cards.size(); i++) {
	if (b1->contains(cards[i])) {
	    std::cerr << "Card " << cards[i]->get_name() << " in b1";
	    cards[i]->take_damage(dmg[i], who_from_race[i], b1, b2);
	}
	else {
	    cards[i]->take_damage(dmg[i], who_from_race[i], b2, b1);
	}
    }
    
    b1->remove_and_mark_dead();
    b2->remove_and_mark_dead();

    // TODO: Ordering seems to be a coin flip?
    b1->do_deathrattles(b2);
    b2->do_deathrattles(b1);
}

void BoardBattler::pre_combat(Board* b1, Board* b2) {
    if (!first_combat) {
	return;
    }
    first_combat = false;
    // TODO: randomize order
    for (auto c : b1->get_cards()) {
	c->do_precombat(b1, b2);
    }
    for (auto c : b2->get_cards()) {
	c->do_precombat(b2, b1);
    }
}

void BoardBattler::post_battle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    if (dead_b1.empty() && dead_b2.empty()) return;
    for (auto c : b1->get_cards()) {
	c->do_postbattle(b1, b2, dead_b1, dead_b2);
    }
}

bool BoardBattler::battle_boards(int attacker_pos, Board* b1, Board* b2) {
    auto pre_precom_b1_dead = b1->has_died();
    auto pre_precom_b2_dead = b2->has_died();
    
    pre_combat(b1, b2); // Special case: Red Whelp start of combat mechanic. Illidan, too.

    auto post_precom_b1_dead = b1->has_died();
    auto post_precom_b2_dead = b2->has_died();

    // TODO: Refactor to have new dead for b1/b2 calculated here
    auto firstb1_precom = post_precom_b1_dead.begin() + pre_precom_b1_dead.size();
    auto lastb1_precom = post_precom_b1_dead.end();
    std::vector<std::shared_ptr<BgBaseCard> > precom_dead_b1(firstb1_precom, lastb1_precom);

    auto firstb2_precom = post_precom_b2_dead.begin() + pre_precom_b2_dead.size();
    auto lastb2_precom = post_precom_b2_dead.end();
    std::vector<std::shared_ptr<BgBaseCard> > precom_dead_b2(firstb2_precom, lastb2_precom);

    // Handles things like Scavenging Hyena
    post_battle(b1, b2, precom_dead_b1, precom_dead_b2);
    post_battle(b2, b1, precom_dead_b2, precom_dead_b1);

    if (b1->length() == 0 || b2->length() == 0) {
	return true;
    }
    
    auto attacker = (*b1)[attacker_pos];
    std::vector<std::shared_ptr<BgBaseCard>> taunts;
    for (auto c : b2->get_cards()) {
	if (c->has_taunt()) {
	    taunts.push_back(c);
	}
    }
    std::shared_ptr<BgBaseCard> defender;
    if (!taunts.empty()) {
	auto defender_pos = rand() % taunts.size();
	defender = taunts[defender_pos];
    }
    else {
	auto defender_pos = rand() % b2->length();
	defender = (*b2)[defender_pos];
    }
    

    auto pre_b1_dead = b1->has_died();
    auto pre_b2_dead = b2->has_died();
    
    // Handles drattles
    take_dmg_simul(attacker, defender, b1, b2);

    auto post_b1_dead = b1->has_died();
    auto post_b2_dead = b2->has_died();

    // TODO: Refactor to have new dead for b1/b2 calculated here
    auto firstb1 = post_b1_dead.begin() + pre_b1_dead.size();
    auto lastb1 = post_b1_dead.end();
    std::vector<std::shared_ptr<BgBaseCard> > new_dead_b1(firstb1, lastb1);

    auto firstb2 = post_b2_dead.begin() + pre_b2_dead.size();
    auto lastb2 = post_b2_dead.end();
    std::vector<std::shared_ptr<BgBaseCard> > new_dead_b2(firstb2, lastb2);

    // Handles things like Scavenging Hyena
    post_battle(b1, b2, new_dead_b1, new_dead_b2);
    post_battle(b2, b1, new_dead_b2, new_dead_b1);

    if (!attacker->is_dead() && attacker->has_windfury_active()) {
	attacker->set_windfury_active(false);
	battle_boards(attacker_pos, b1, b2);	   
    }

    if (attacker->has_windfury()) {
	attacker->set_windfury_active();
    }

    // Handles deathrattles, nothing happens if nothing died
    //attacker->do_deathrattle(b1, b2);
    //defender->do_deathrattle(b2, b1); // May modify b1/b2
    return attacker->is_dead();
}

std::string Battler::decide_who_goes_first(Board* b1, Board* b2) {
    if (b1->length() > b2->length()) {
	return "p1";
    }
    else if (b1->length() < b2->length()) {
	return "p2";
    }
    else {
	auto rng = rand() / RAND_MAX;
	if (rng > .5) {
	    return "p1";
	}
	else {
	    return "p2";
	}
    }
}
