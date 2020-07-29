#include <iostream>

#include "DeathrattleCards.hpp"
#include "../../bg_game/board.hpp"

void FiendishServant::do_deathrattle(int pos, Board* b1, Board*b2) {
    auto card = b1->get_cards()[0];
    card->set_attack(card->get_attack() + attack);
    b1->set_card(0, card);
}
