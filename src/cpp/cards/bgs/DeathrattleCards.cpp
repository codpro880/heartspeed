#include <iostream>

#include "DeathrattleCards.hpp"
#include "../../bg_game/board.hpp"

void FiendishServant::do_deathrattle(int pos, Board* b1, Board*b2) {
    auto card = b1->get_cards()[0];
    card->set_attack(card->get_attack() + attack);
    std::cout << "Card during drattl: " << card << std::endl;
    b1->set_card(0, card);
    std::cout << "b1 during drattl: " << (*b1) << std::endl;
    std::cout << "b2 during drattl: " << (*b2) << std::endl;
}    
