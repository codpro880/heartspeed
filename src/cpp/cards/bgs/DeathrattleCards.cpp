#include <iostream>

#include "DeathrattleCards.hpp"
#include "BgCardFactory.hpp"
#include "../../bg_game/board.hpp"
#include "../../bg_game/battler.hpp"

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

void SelflessHero::do_deathrattle(int pos, Board* b1, Board*b2) {
    // Cards w/o divine shield
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    for (auto card : b1->get_cards()) {
	if (!card->has_divine_shield()) {
	    cards.push_back(card);
	}
    }
    if (cards.size() > 0) {
	auto buffed_pos = rand() % cards.size();
	auto card = cards[buffed_pos];
	card->set_divine_shield();
	b1->set_card(buffed_pos, card);
    }
}

void Scallywag::do_deathrattle(int pos, Board* b1, Board* b2) {
    auto f = BgCardFactory();
    auto joebot = f.get_card("Jo-E Bot"); // TODO: Replace w/ pirate token
    b1->insert_card(pos, joebot);
    BoardBattler().battle_boards(pos, b1, b2); // Modifies b1/b2
    // std::vector<std::shared_ptr<BgBaseCard> > cards;
    // for (auto card : b1->get_cards()) {
    // 	if (!card->has_divine_shield()) {
    // 	    cards.push_back(card);
    // 	}
    // }
    // if (cards.size() > 0) {
    // 	auto buffed_pos = rand() % cards.size();
    // 	auto card = cards[buffed_pos];
    // 	card->set_divine_shield();
    // 	b1->set_card(buffed_pos, card);
    // }
}
