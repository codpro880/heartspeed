#include "../test/googletest/include/gtest/gtest.h"

#include "bobs_buddy.hpp"

TEST(BobsBuddy, CanGetBattleBoardsFromLog) {
    auto bb = BobsBuddy("Power.log");
    auto battle_boards = bb.parse_full_log();

    ASSERT_GT(battle_boards.size(), 1);

    // Turn 1
    auto our_first_board = battle_boards[0].first;
    auto their_first_board = battle_boards[0].second;
    EXPECT_EQ(our_first_board->length(), 1);
    EXPECT_EQ(their_first_board->length(), 1);
    EXPECT_EQ(our_first_board->get_cards()[0]->get_name(), "Rockpool Hunter");
    EXPECT_EQ(their_first_board->get_cards()[0]->get_name(), "Rockpool Hunter");
}
