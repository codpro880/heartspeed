#include <iostream>

#include "DeathrattleCards.hpp"
#include "../../bg_game/board.hpp"

void FiendishServant::do_deathrattle(int pos, Board* b1, Board*b2) {
    auto buffed_pos = rand() % b1->length();
    auto card = b1->get_cards()[buffed_pos];
    card->set_attack(card->get_attack() + attack);
    b1->set_card(buffed_pos, card);
}
