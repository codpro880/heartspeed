#include <string>
#include <vector>

#include "game.hpp"

//#include "../cards/CardFactory.hpp"
#include "../test/googletest/include/gtest/gtest.h"

TEST(Deck, HasBasicFunctionality) {
    auto df = DeckFactory();
    auto deck = df.get_deck("Mage", false);

    EXPECT_EQ(deck.get_class(), "MAGE");

    auto card = deck.get_top_card();
    EXPECT_EQ(card.get_name(), "BoulderfistOgre");

    auto card2 = deck.get_top_card();
    EXPECT_EQ(card2.get_name(), "BoulderfistOgre");

    auto card3 = deck.get_top_card();
    EXPECT_EQ(card3.get_name(), "Nightblade");
}

TEST(Deck, CanBeShuffled) {
    auto df = DeckFactory();
    auto deck = df.get_deck("Mage");

    EXPECT_EQ(deck.get_class(), "MAGE");

//     auto card = deck.get_top_card();
//     EXPECT_EQ(card.get_name(), "BoulderfistOgre");

//     auto card2 = deck.get_top_card();
//     EXPECT_EQ(card2.get_name(), "BoulderfistOgre");

//     auto card3 = deck.get_top_card();
//     EXPECT_EQ(card3.get_name(), "Nightblade");
}
