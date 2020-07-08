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
    EXPECT_EQ(board.calculate_damage(), 5);
}

// TEST(Board, CanCalculateDamageWithFullBoard) {
//     auto f = BgCardFactory();
//     auto goldrinn = f.get_card("Alleycat");
//     std::vector<BgBaseCard> cards{ goldrinn };
//     auto board = Board(cards);
//     EXPECT_EQ(board.calculate_damage(), 5);
// }
