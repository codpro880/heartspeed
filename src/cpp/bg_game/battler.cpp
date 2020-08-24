#include "battler.hpp"

#include <stdlib.h>
#include <cstdlib>
#include <stdexcept>

#include <iostream>

BattleResult Battler::sim_battle() {
    return sim_battle(p1, p2);
}

BattleResult Battler::sim_battle(Player* p1, Player* p2) {
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
    std::vector<int> dmg = {defender->get_attack(), attacker->get_attack()};
    std::vector<std::shared_ptr<BgBaseCard> > cards = {attacker, defender};
    std::vector<std::string> who_from_race = {defender->get_race(), attacker->get_race()};
    take_dmg_simul(cards, who_from_race, dmg, b1, b2);
    attacker->do_postattack(defender, b1, b2);
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
	cards[i]->take_damage(dmg, who_from_race[i]);
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
	cards[i]->take_damage(dmg[i], who_from_race[i]);
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

void BoardBattler::post_battle(Board* board,
			       std::vector<std::shared_ptr<BgBaseCard> > pre_death,
			       std::vector<std::shared_ptr<BgBaseCard> > post_death) {
    if (pre_death.size() == post_death.size()) return;
    auto first = post_death.begin() + pre_death.size();
    auto last = post_death.end();
    std::vector<std::shared_ptr<BgBaseCard> > new_dead(first, last);
    for (auto c : board->get_cards()) {	
	c->do_postbattle(board, new_dead);
    }
}

bool BoardBattler::battle_boards(int attacker_pos, Board* b1, Board* b2) {
    pre_combat(b1, b2); // Special case: Red Whelp start of combat mechanic. Illidan, too.    
    
    auto attacker = (*b1)[attacker_pos];
    auto defender_pos = rand() % b2->length();
    auto defender = (*b2)[defender_pos];

    auto pre_b1_dead = b1->has_died();
    auto pre_b2_dead = b2->has_died();
    
    // Handles drattles
    take_dmg_simul(attacker, defender, b1, b2);

    auto post_b1_dead = b1->has_died();
    auto post_b2_dead = b2->has_died();

    // Handles things like Scavenging Hyena
    post_battle(b1, pre_b1_dead, post_b1_dead);
    post_battle(b2, pre_b2_dead, post_b2_dead);

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
