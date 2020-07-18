#include "board.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
#include "../cards/bgs/BgBaseCard.hpp"
#include "../test/googletest/include/gtest/gtest.h"

TEST(Board, CanCalculateDamageEmptyBoard) {
    auto f = BgCardFactory();
    std::vector<BgBaseCard> cards;
    auto board = Board(cards);
    EXPECT_EQ(board.calculate_damage(), 0);
}

TEST(Board, CanCalculateDamageOneCard) {
    auto f = BgCardFactory();
    auto goldrinn = f.get_card("Goldrinn, the Great Wolf");
    std::vector<BgBaseCard> cards{ goldrinn };
    auto board = Board(cards);
    EXPECT_EQ(board.calculate_damage(), 6);
}

TEST(Board, CanCalculateDamageWithFullBoard) {
    auto f = BgCardFactory();
    std::vector<BgBaseCard> cards{
	f.get_card("Alleycat (Golden)"), // Tier 1
	    f.get_card("Imprisoner (Golden)"), // Tier 2
	    f.get_card("Houndmaster (Golden)"), // Tier 3
	    f.get_card("Goldgrubber (Golden)"), // Tier 4
	    f.get_card("Mal'Ganis (Golden)"), // Tier 5
	    f.get_card("Foe Reaper 4000"), // Tier 6
	};
    auto board = Board(cards);
    EXPECT_EQ(board.calculate_damage(), 1+2+3+4+5+6);
}
