#include <algorithm>
#include <iostream>
#include <memory>

#include "PrebattleCards.hpp"
#include "BgCardFactory.hpp"
#include "../../bg_game/board.hpp"
#include "../../bg_game/battler.hpp"

void RedWhelp::do_precombat(Board* b1, Board*b2) {
    int drag_count = 0;
    for (auto card : b1->get_cards()) {
	if (card->get_race() == "DRAGON") {
	    drag_count++;
	}
    }
    auto defender_pos = rand() % b2->length();
    auto defender = (*b2)[defender_pos];
    BoardBattler b;
    b.take_dmg_simul(defender, drag_count, b2, b1);
}

void RedWhelpGolden::do_precombat(Board* b1, Board*b2) {
    for (int i = 0; i < 2; i++) {
	rw.do_prebattle(b1, b2);
    }
}
