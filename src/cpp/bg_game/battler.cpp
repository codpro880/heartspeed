#include "battler.hpp"

#include <stdexcept>

BattleResult Battler::sim_battle() {
    return sim_battle(p1->get_board(), p2->get_board());
}

BattleResult Battler::sim_battle(Board* b1, Board* b2) {
    if (b1->empty() && b2->empty()) {
	auto res = BattleResult();
	res.who_won = "draw";
	res.damage_taken = 0;
    }
    else if (b1->empty()) {
	auto res = BattleResult();
	res.who_won = "p2";
	auto player = Player(b2);
	res.damage_taken = player.calculate_damage();
    }
    else if (b2->empty()) {
	auto res = BattleResult();
	res.who_won = "p1";
	auto player = Player(b1);
	res.damage_taken = player.calculate_damage();
    }
    else {
	// Not impl error
	std::logic_error("Not yet impl...");
    }
}
