#include <cstdlib>
// #include <execution>
#include <stdexcept>
#include <stdlib.h>

#include <iostream>

#include "battler.hpp"
#include "rng_singleton.hpp"

BattleResult Battler::sim_battle(std::string goes_first) {
    auto res = sim_battle(p1, p2, goes_first);
    if (res.who_won == p1->get_name()) {
	p1->set_won_last_turn();
    }
    else if (res.who_won == p2->get_name()) {
	p2->set_won_last_turn();
    }
    return res;
}

BattleResults Battler::sim_battles_par(int num_battles) {    
    std::vector<std::pair<std::shared_ptr<Player>, std::shared_ptr<Player>>> players(num_battles);
    std::cerr << "Copying..." << std::endl;
    for (int i = 0; i < num_battles; i++) {
	players[i].first = std::make_shared<Player>(p1);
	players[i].second = std::make_shared<Player>(p2);
    }
    std::cerr << "Done copying" << std::endl;
    std::vector<int> results(num_battles);

    // TODO: Make thread_local seeding for reproducibility
    #pragma omp parallel for
    for(size_t i = 0; i < players.size(); ++i) {
	auto player_pair = players[i];
	auto player1 = player_pair.first;
	auto player2 = player_pair.second;
	auto b = Battler(player1.get(), player2.get());
	auto res = b.sim_battle();
	if (res.who_won == player1->get_name()) {
	    results[i] = -1;
	}
	else if (res.who_won == player2->get_name()) {
	    results[i] = 1;
	}
	else {
	    results[i] = 0;
	}
    }
    
    int total_p1_win = 0;
    int total_draw = 0;
    int total_p2_win = 0;
    for (auto i : results) {
	if (i == -1) total_p1_win++;
	else if (i == 1) total_p2_win++;
	else total_draw++;
    }

    BattleResults full_res = BattleResults();
    full_res.p1_win = total_p1_win / double(num_battles);
    full_res.draw = total_draw / double(num_battles);
    full_res.p2_win = total_p2_win / double(num_battles);
    
    return full_res;
}

BattleResults Battler::sim_battles(int num_battles) {
    int total_p1_win = 0;
    int total_draw = 0;
    int total_p2_win = 0;
    for (int bnum = 0; bnum < num_battles; bnum++) {
	auto player1 = std::make_shared<Player>(p1);
	auto player2 = std::make_shared<Player>(p2);
	auto res = sim_battle(player1.get(), player2.get(), "null");

	if (res.who_won == player1->get_name()) {
	    total_p1_win++;
	}
	else if (res.who_won == player2->get_name()) {
	    total_p2_win++;
	}
	else {
	    total_draw++;
	}
    }

    BattleResults full_res = BattleResults();
    full_res.p1_win = total_p1_win / double(num_battles);
    full_res.draw = total_draw / double(num_battles);
    full_res.p2_win = total_p2_win / double(num_battles);
    
    return full_res;
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
    // Precondition: p1 goes first
    bool p1_turn = true;
    auto b1 = p1->get_board();
    auto b2 = p2->get_board();
    BattleResult res = BattleResult();
    while(!b1->empty() && !b2->empty()) {
	res.frames.push_back(std::make_pair(Board(b1), Board(b2)));
	if (debug) {
	    std::cout << "P1 (before): " << std::endl;
	    std::cout << (*p1) << std::endl;
	    // std::cout << "Attacker pos: " << b1->get_attacker_pos() << std::endl;
	    std::cout << "P2 (before): " << std::endl;
	    std::cout << (*p2) << std::endl;
	}
	
	// b1 always goes first here
	b1->clip_attacker_pos();
	b2->clip_attacker_pos();

	std::tuple<bool, bool, int, int> battle_res;
	if (p1_turn) {
	    battle_res = board_battler.battle_boards(b1->get_attacker_pos(), p1, p2); // Modifies b1/b2
	}
	else {
	    battle_res = board_battler.battle_boards(b2->get_attacker_pos(), p2, p1); // Modifies b1/b2
	}
	res.attacker_pos.push_back(std::get<2>(battle_res));
	res.defender_pos.push_back(std::get<3>(battle_res));
	bool attacker_is_dead = std::get<0>(battle_res);
	bool defender_is_dead = std::get<1>(battle_res);
	
	if (!attacker_is_dead) {
	    if (p1_turn) b1->increment_attacker_pos(); //p1_counter++;
	    else b2->increment_attacker_pos(); //p2_counter++;
	}
	
	if (debug) {
	    std::cout << "P1: " << (*p1) << std::endl;
	    // std::cout << "Attacker pos: " << p1_counter << std::endl;
	    std::cout << "P2: " << (*p2) << std::endl;
	}
	res.b1_turn.push_back(p1_turn);
	p1_turn = !p1_turn;
    }
    res.frames.push_back(std::make_pair(Board(b1), Board(b2)));

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
    else { // b2 empty
	res.who_won = p1->get_name();
	res.damage_taken = p1->calculate_damage();
	return res;
    }

    // return battle(p2, p1, p2_counter, p1_counter);
}

void BoardBattler::take_dmg_simul(std::shared_ptr<BgBaseCard> attacker,
				  std::shared_ptr<BgBaseCard> defender,
				  Player* p1,
				  Player* p2) {
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    attacker->do_preattack(defender, p1, p2);
    defender->do_predefense(attacker, p2, p1);
    std::vector<int> dmg;
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    auto def_pos = b2->get_pos(defender);
    if (attacker->has_cleave() && b2->get_cards().size() > (unsigned)1) {
	if (def_pos == 0) {
	    dmg = {defender->get_attack(), attacker->get_attack(), attacker->get_attack()};
	    cards = {attacker, defender, b2->get_cards()[def_pos+1]};
	}
	else if (def_pos == b2->get_cards().size() - 1) {
	    dmg = {defender->get_attack(), attacker->get_attack(), attacker->get_attack()};
	    cards = {attacker, defender, b2->get_cards()[def_pos-1]};
	}
	else { // More than 2 and not on ends
	    dmg = {defender->get_attack(), attacker->get_attack(), attacker->get_attack(), attacker->get_attack()};
	    cards = {attacker, defender, b2->get_cards()[def_pos+1], b2->get_cards()[def_pos-1]};
	}
    }
    else { // One defender
	dmg = {defender->get_attack(), attacker->get_attack()};
	cards = {attacker, defender};
    }
    std::vector<std::string> who_from_race = {defender->get_race(), attacker->get_race()};
    auto races_size = who_from_race.size();
    for (size_t i = 0; i < cards.size() - races_size; i++) {
	who_from_race.push_back(attacker->get_race());
    }
    take_dmg_simul(cards, who_from_race, dmg, p1, p2);
    attacker->do_postattack(defender, def_pos, p1, p2);
    defender->do_postdefense(attacker, p2, p1);
}

void BoardBattler::take_dmg_simul(std::shared_ptr<BgBaseCard> card,
				  std::string who_from_race,
				  int dmg,
				  Player* p1,
				  Player* p2) {
    auto cards = {card};
    auto who_from_races = {who_from_race};
    take_dmg_simul(cards, who_from_races, dmg, p1, p2);
}

void BoardBattler::take_dmg_simul(std::vector<std::shared_ptr<BgBaseCard>> cards,
				  std::vector<std::string> who_from_race,
				  int dmg,
				  Player* p1,
				  Player* p2) {
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    for (int i = 0; i < cards.size(); i++) {
	if (b1->contains(cards[i])) {
	    cards[i]->take_damage(dmg, who_from_race[i], p1, p2);
	}
	else {
	    cards[i]->take_damage(dmg, who_from_race[i], p2, p1);
	}
    }
    
    b1->remove_and_mark_dead();
    b2->remove_and_mark_dead();

    // TODO: Ordering seems to be a coin flip?
    b1->do_deathrattles(p1, p2, b2);
    b2->do_deathrattles(p2, p1, b1);
}

void BoardBattler::take_dmg_simul(std::vector<std::shared_ptr<BgBaseCard>> cards,
				  std::vector<std::string> who_from_race,
				  std::vector<int> dmg,
				  Player* p1,
				  Player* p2) {
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    for (int i = 0; i < cards.size(); i++) {
	if (b1->contains(cards[i])) {
	    cards[i]->take_damage(dmg[i], who_from_race[i], p1, p2);
	}
	else {
	    cards[i]->take_damage(dmg[i], who_from_race[i], p2, p1);
	}
    }    

    b1->remove_and_mark_dead();
    b2->remove_and_mark_dead();

    // TODO: Ordering seems to be a coin flip?
    b1->do_deathrattles(p1, p2, b2);
    b2->do_deathrattles(p2, p1, b1);
}

void BoardBattler::pre_combat(Player* p1, Player* p2) {
    if (!first_combat) {
	return;
    }
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    first_combat = false;
    // TODO: randomize order
    for (auto c : b1->get_cards()) {
	c->do_precombat(p1, p2);
    }
    for (auto c : b2->get_cards()) {
	c->do_precombat(p2, p1);
    }
}

void BoardBattler::post_battle(Player* p1,
			       Player* p2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    if (dead_b1.empty() && dead_b2.empty()) return;
    Board* b1 = p1->get_board().get();
    for (auto c : b1->get_cards()) {
	c->do_postbattle(p1, p2, dead_b1, dead_b2);
    }
}

std::tuple<bool, bool, int, int> BoardBattler::battle_boards(int attacker_pos, std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) {
    return battle_boards(attacker_pos, p1.get(), p2.get());
}

std::tuple<bool, bool, int, int> BoardBattler::battle_boards(int attacker_pos, Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    auto pre_precom_b1_dead = b1->has_died();
    auto pre_precom_b2_dead = b2->has_died();
    
    pre_combat(p1, p2); // Special case: Red Whelp start of combat mechanic. Illidan, too.

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
    post_battle(p1, p2, precom_dead_b1, precom_dead_b2);
    post_battle(p2, p1, precom_dead_b2, precom_dead_b1);

    if (b1->length() == 0 || b2->length() == 0) {
	// TODO: Need to figure out how to show pre_combat victories in the UI
	return std::make_tuple(false, false, -1, -1);
    }
    
    auto attacker = (*b1)[attacker_pos];
    std::vector<std::shared_ptr<BgBaseCard>> taunts;
    for (auto c : b2->get_cards()) {
	if (c->has_taunt()) {
	    taunts.push_back(c);
	}
    }
    
    std::shared_ptr<BgBaseCard> defender;
    size_t defender_pos;
    if (!taunts.empty()) {
	defender_pos = RngSingleton::getInstance().get_rand_int() % taunts.size();
	defender = taunts[defender_pos];
	// important to have global board position (this value is returned)
	defender_pos = b2->get_pos(defender);
    }
    else {
	defender_pos = RngSingleton::getInstance().get_rand_int() % b2->length();
	defender = (*b2)[defender_pos];
    }

    // Zapp special case, ignores taunts
    if (attacker->get_name() == "Zapp" || attacker->get_name() == "Zapp (Golden)") {
	// No Idea why this isn't working, tried w/ and w/o const& in lambda
	// auto min_elt_it = std::min_element(b2->get_cards().begin(),
	// 				   b2->get_cards().end(),
	// 				   [](std::shared_ptr<BgBaseCard> a, std::shared_ptr<BgBaseCard> b)
	// 				   {
	// 				       return a->get_attack() < b->get_attack();
	// 				   }
	// 				   );
	// std::cerr << "GOt here..." << std::endl;
	// auto idx = std::distance(b2->get_cards().begin(), min_elt_it);
	// std::cerr << "Index: " << idx << std::endl;
	// defender = b2->get_cards()[idx];
	int min_attack = b2->get_cards()[0]->get_attack();
	std::vector<std::shared_ptr<BgBaseCard>> defenders;
	for (auto c : b2->get_cards()) {
	    if (c->get_attack() < min_attack) {
		min_attack = c->get_attack();
	    }
	}
	for (auto c : b2->get_cards()) {
	    if (c->get_attack() == min_attack) {
		defenders.push_back(c);
	    }
	}
	defender = defenders[RngSingleton::getInstance().get_rand_int() % defenders.size()];
	defender_pos = b2->get_pos(defender);
    }
    

    auto pre_b1_dead = b1->has_died();
    auto pre_b2_dead = b2->has_died();
    
    // Handles drattles
    take_dmg_simul(attacker, defender, p1, p2);

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
    post_battle(p1, p2, new_dead_b1, new_dead_b2);
    post_battle(p2, p1, new_dead_b2, new_dead_b1);

    if (!attacker->is_dead() && attacker->has_windfury_active()) {
	// Need to turn off windfury, or we'll infinitely recurse
	attacker->set_windfury_active(false);
	battle_boards(attacker_pos, p1, p2);
	if (b1->contains("Whirlwind Tempest")) {
	    if (!attacker->is_dead()) {
		attacker->set_windfury_active(false);
		battle_boards(attacker_pos, p1, p2);
	    }
	    if (!attacker->is_dead()) {
		attacker->set_windfury_active(false);
		battle_boards(attacker_pos, p1, p2);
	    }
	}
    }

    if (attacker->has_windfury()) {
	attacker->set_windfury_active();
    }

    // Handles deathrattles, nothing happens if nothing died
    return std::make_tuple(attacker->is_dead(), defender->is_dead(), attacker_pos, (int)defender_pos);
}

std::string Battler::decide_who_goes_first(std::shared_ptr<Board> b1, std::shared_ptr<Board> b2) {
    return decide_who_goes_first(b1.get(), b2.get());
}

std::string Battler::decide_who_goes_first(Board* b1, Board* b2) {
    if (b1->length() > b2->length()) {
	return "p1";
    }
    else if (b1->length() < b2->length()) {
	return "p2";
    }
    else {
	auto rng = RngSingleton::getInstance().get_rand_int(0, 1);
	if (rng == 0) {
	    return "p1";
	}
	else {
	    return "p2";
	}
    }
}
