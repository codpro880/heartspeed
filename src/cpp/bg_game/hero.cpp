#include "board.hpp"
#include "hero.hpp"

void AlAkir::apply_hero_power(std::shared_ptr<Board> board) {
    if (board->length() == 0) return;
    auto cards = board->get_cards();
    cards[0]->set_divine_shield();
    cards[0]->set_taunt();
    cards[0]->set_windfury();
}
