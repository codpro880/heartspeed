#include <vector>

#include "../cards/standard/CardFactory.hpp"
#include "game.hpp"

void Game::start_game() {
    auto p1_start_cards = std::vector<BaseCard>();
    p1_start_cards.push_back(p1->draw_card());
    p1_start_cards.push_back(p1->draw_card());
    p1_start_cards.push_back(p1->draw_card());
	
    p1->init_hand(p1_start_cards);

    auto p2_start_cards = std::vector<BaseCard>();
    p2_start_cards.push_back(p2->draw_card());
    p2_start_cards.push_back(p2->draw_card());
    p2_start_cards.push_back(p2->draw_card());
    p2_start_cards.push_back(p2->draw_card());
    auto fac = CardFactory();
    p2_start_cards.push_back(fac.get_card("TheCoin"));

    p2->init_hand(p2_start_cards);
}
