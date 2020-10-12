#include <cstdlib>
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

TEST(BobsBuddy, PyLikeStringParsingWorksWithEmptyEndStr) {
    auto pystr = PyString();
    std::string item = "D 08:20:54.1034250 PowerTaskList.DebugPrintPower() -     TAG_CHANGE Entity=[entityName=Rockpool Hunter id=3180 zone=SETASIDE zonePos=0 cardId=TB_BaconUps_061 player=8] tag=HEALTH value=10";
    std::string start = "value=";
    std::string end = "";
    auto id = pystr.get_str_between(item, start, end);
    EXPECT_EQ(id, "10");
}

TEST(BobsBuddy, PyLikeStringParsingWorksWithWhitespace) {
    auto pystr = PyString();
    std::string item = "D 08:20:54.1034250 PowerTaskList.DebugPrintPower() -     TAG_CHANGE Entity=[entityName=Rockpool Hunter id=3180 zone=SETASIDE zonePos=0 cardId=TB_BaconUps_061 player=8] tag=HEALTH value=10 ";
    std::string start = "value=";
    std::string end = " ";
    auto id = pystr.get_str_between(item, start, end);
    EXPECT_EQ(atoi(id.c_str()), 10);
}



TEST(BobsBuddy, CanGetBattleBoardsFromLog) {
    std::filesystem::path power_log = std::filesystem::current_path() / "test_data" / "Power.log";
    auto bb = BobsBuddy(power_log.string());
    auto battle_boards = bb.parse_full_log();
    std::cerr << "Battlin boards..." << std::endl;
    // ASSERT_GT(battle_boards.size(), 1);
    std::cerr << "Asserted." << std::endl;

    // Turn 1
    auto our_first_board = battle_boards[0].first;
    auto their_first_board = battle_boards[0].second;
    EXPECT_EQ(our_first_board->length(), 1);
    EXPECT_EQ(their_first_board->length(), 1);
    auto our_rockpool = our_first_board->get_cards()[0];
    auto their_rockpool = their_first_board->get_cards()[0];
    EXPECT_EQ(our_rockpool->get_name(), "Rockpool Hunter");
    EXPECT_EQ(their_rockpool->get_name(), "Rockpool Hunter");
    EXPECT_EQ(our_rockpool->get_attack(), 4);
    EXPECT_EQ(their_rockpool->get_attack(), 4);

    // Turn 2
    auto our_second_board = battle_boards[1].first;
    auto their_second_board = battle_boards[1].second;
    EXPECT_EQ(our_second_board->length(), 3);
    EXPECT_EQ(their_second_board->length(), 1);
    auto our_second_rockpool = our_second_board->get_cards()[0];
    auto our_second_murlocTH = our_second_board->get_cards()[1];
    auto our_second_murlocScout = our_second_board->get_cards()[2];
    auto their_drag_lt = their_second_board->get_cards()[0];
    EXPECT_EQ(our_second_rockpool->get_name(), "Rockpool Hunter");
    EXPECT_EQ(our_second_murlocTH->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_second_murlocScout->get_name(), "Murloc Scout");
    EXPECT_EQ(their_drag_lt->get_name(), "Dragonspawn Lieutenant");
    EXPECT_EQ(our_second_rockpool->get_attack(), 2);
    EXPECT_EQ(our_second_rockpool->get_health(), 3);
    EXPECT_EQ(our_second_murlocTH->get_attack(), 2);
    EXPECT_EQ(our_second_murlocTH->get_health(), 1);
    EXPECT_EQ(our_second_murlocScout->get_attack(), 1);
    EXPECT_EQ(our_second_murlocScout->get_health(), 1);
    EXPECT_EQ(their_drag_lt->get_attack(), 2);
    EXPECT_EQ(their_drag_lt->get_health(), 3);
}
