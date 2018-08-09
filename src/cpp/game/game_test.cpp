
#include "game.hpp"
#include "deck.hpp"

#include "../test/googletest/include/gtest/gtest.h"

TEST(GameObj, CanStart) {
    auto df = DeckFactory();
    auto p1_deck = df.get_deck("Mage", false);
    auto p2_deck = df.get_deck("Mage", false);
    // TODO: Wrap in smart ptr
    auto p1 = new Player(p1_deck);
    auto p2 = new Player(p2_deck);

    auto game = Game(p1, p2);
    game.start_game();

    auto p1_hand = p1->get_hand();
    auto p2_hand = p2->get_hand();

    EXPECT_EQ(p1_hand.size(), (size_t)3);
    EXPECT_EQ(p2_hand.size(), (size_t)5);

    // Make sure coin there
    bool found = false;
    for (auto card : p2_hand) {
	if (card.get_name() == "TheCoin") {
	    found = true;
	}
    }

    EXPECT_EQ(found, true);
}
