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
    std::cout << "Boards: " << std::endl;
    std::cout << (*b1) << std::endl;
    std::cout << (*b2) << std::endl;
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
    int attacker_pos = p1_counter;

    auto attacker = (*b1)[attacker_pos];
    auto defender_pos = rand() % b2->length();
    auto defender = (*b2)[defender_pos];
    // TODO: Deal w/ deathrattle
    attacker.take_damage(defender.get_attack());
    defender.take_damage(attacker.get_attack());

    if (attacker.is_dead()) {
	b1->remove(attacker_pos);
    }
    else {
	b1->set_card(attacker_pos, attacker);
    }
    if (defender.is_dead()) {
	b2->remove(defender_pos);
    }
    else {
	b2->set_card(defender_pos, defender);
    }

    p1->set_board(b1);
    p2->set_board(b2);
    std::cout << "P1: " << (*p1) << std::endl;
    std::cout << "P2: " << (*p2) << std::endl;
    return battle(p2, p1, p2_counter+1, p1_counter+1);
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