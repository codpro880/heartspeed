#include "board.hpp"
#include "hero.hpp"

#include <stdexcept>

void AlAkir::apply_hero_power(std::shared_ptr<Board> board, int pos) {
    if (board->length() == 0) return;
    auto cards = board->get_cards();
    cards[0]->set_divine_shield();
    cards[0]->set_taunt();
    cards[0]->set_windfury();
}

void LichKing::apply_hero_power(std::shared_ptr<Board> board, int pos) {
    if (board->length() == 0) return;
    if (pos < 0 || pos >= board->length()) {
        throw std::runtime_error("Pos must be between 0 and length of board.");
    }   
    auto cards = board->get_cards();
    cards[pos]->set_reborn();
}

