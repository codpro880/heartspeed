#include <algorithm>
#include <iostream>
#include <memory>

#include "PreCombatCards.hpp"
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
	rw.do_precombat(b1, b2);
    }
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

void OldMurkeye::do_postbattle(Board* board,
			       std::vector<std::shared_ptr<BgBaseCard> > new_dead) {
    int dead_murloc_count = 0;
    for (auto c : new_dead) {
	if (c->get_race() == "MURLOC") {
	    dead_murloc_count++;
	}
    }
    set_attack(get_attack() - dead_murloc_count);
}

void OldMurkeyeGolden::do_postbattle(Board* board,
				     std::vector<std::shared_ptr<BgBaseCard> > new_dead) {
    for (int i = 0; i < 2; i++) {
	rw.do_postbattle(board, new_dead);
    }
}
