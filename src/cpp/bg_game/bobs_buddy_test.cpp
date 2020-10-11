#include <filesystem>

#include "../test/googletest/include/gtest/gtest.h"

#include "bobs_buddy.hpp"

TEST(BobsBuddy, PyLikeStringParsing) {
    auto pystr = PyString();
    std::string item = "D 08:11:01.0627100 PowerTaskList.DebugPrintPower() - BLOCK_START BlockType=TRIGGER Entity=[entityName=BaconShop8PlayerEnchant id=71 zone=PLAY zonePos=0 cardId=TB_BaconShop_8P_PlayerE player=8] EffectCardId=System.Collections.Generic.List`1[System.String] EffectIndex=9 Target=0 SubOption=-1 TriggerKeyword=0";
    std::string start = "id=";
    std::string end = " zone=";
    auto id = pystr.get_str_between(item, start, end);
    EXPECT_EQ(id, "71");
}

// TEST(BobsBuddy, CanGetBattleBoardsFromLog) {
//     std::filesystem::path power_log = std::filesystem::current_path() / "test_data" / "Power.log";
//     auto bb = BobsBuddy(power_log.string());
//     auto battle_boards = bb.parse_full_log();
//     std::cerr << "Battlin boards..." << std::endl;
//     ASSERT_GT(battle_boards.size(), 1);
//     std::cerr << "Asserted." << std::endl;

//     // Turn 1
//     auto our_first_board = battle_boards[0].first;
//     auto their_first_board = battle_boards[0].second;
//     EXPECT_EQ(our_first_board->length(), 1);
//     EXPECT_EQ(their_first_board->length(), 1);
//     EXPECT_EQ(our_first_board->get_cards()[0]->get_name(), "Rockpool Hunter");
//     EXPECT_EQ(their_first_board->get_cards()[0]->get_name(), "Rockpool Hunter");
// }
