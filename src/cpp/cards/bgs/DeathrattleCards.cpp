#include <iostream>

#include "DeathrattleCards.hpp"
#include "BgCardFactory.hpp"
#include "../../bg_game/board.hpp"

void FiendishServant::do_deathrattle(int pos, Board* b1, Board*b2) {
    auto buffed_pos = rand() % b1->length();
    auto card = b1->get_cards()[buffed_pos];
    card->set_attack(card->get_attack() + attack);
    b1->set_card(buffed_pos, card);
}

void Mecharoo::do_deathrattle(int pos, Board* our_board, Board* opp_board) {
    auto f = BgCardFactory();
    auto joebot = f.get_card("Jo-E Bot");
    our_board->insert_card(pos, joebot);
}
