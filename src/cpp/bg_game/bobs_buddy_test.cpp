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

    // Turn 3, can't see all off opp board from screenshot
    auto our_third_board = battle_boards[2].first;
    // auto their_third_board = battle_boards[2].second;
    EXPECT_EQ(our_third_board->length(), 5);
    auto our_third_rockpool1 = our_third_board->get_cards()[0];
    auto our_third_rockpool2 = our_third_board->get_cards()[1];
    auto our_third_murlocTH1 = our_third_board->get_cards()[2];
    auto our_third_murlocTH2 = our_third_board->get_cards()[3];
    auto our_third_drag_lt = our_third_board->get_cards()[4];
    EXPECT_EQ(our_third_rockpool1->get_name(), "Rockpool Hunter");
    EXPECT_EQ(our_third_rockpool2->get_name(), "Rockpool Hunter");
    EXPECT_EQ(our_third_murlocTH1->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_third_murlocTH2->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_third_drag_lt->get_name(), "Dragonspawn Lieutenant");
    // Attack and def
    EXPECT_EQ(our_third_rockpool1->get_attack(), 3);
    EXPECT_EQ(our_third_rockpool1->get_health(), 4);
    EXPECT_EQ(our_third_rockpool2->get_attack(), 2);
    EXPECT_EQ(our_third_rockpool2->get_health(), 3);
    EXPECT_EQ(our_third_murlocTH1->get_attack(), 2);
    EXPECT_EQ(our_third_murlocTH1->get_health(), 1);
    EXPECT_EQ(our_third_murlocTH2->get_attack(), 2);
    EXPECT_EQ(our_third_murlocTH2->get_health(), 1);
    EXPECT_EQ(our_third_drag_lt->get_attack(), 2);
    EXPECT_EQ(our_third_drag_lt->get_health(), 3);

    // Turn 4
    auto our_fourth_board = battle_boards[3].first;
    auto their_fourth_board = battle_boards[3].second;
    EXPECT_EQ(our_fourth_board->length(), 6);
    EXPECT_EQ(their_fourth_board->length(), 7);
    // ours
    auto our_fourth_rockpool = our_fourth_board->get_cards()[0];
    auto our_fourth_stew = our_fourth_board->get_cards()[1];
    auto our_fourth_murlocTH1 = our_fourth_board->get_cards()[2];
    auto our_fourth_murlocTH2 = our_fourth_board->get_cards()[3];
    auto our_fourth_salty_looter = our_fourth_board->get_cards()[4];
    auto our_fourth_drag_lt = our_fourth_board->get_cards()[5];
    EXPECT_EQ(our_fourth_rockpool->get_name(), "Rockpool Hunter (Golden)");
    EXPECT_EQ(our_fourth_stew->get_name(), "Steward of Time");
    EXPECT_EQ(our_fourth_murlocTH1->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_fourth_murlocTH2->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_fourth_salty_looter->get_name(), "Salty Looter");
    EXPECT_EQ(our_fourth_drag_lt->get_name(), "Dragonspawn Lieutenant");
    EXPECT_EQ(our_fourth_rockpool->get_attack(), 5);
    EXPECT_EQ(our_fourth_rockpool->get_health(), 7);
    EXPECT_EQ(our_fourth_stew->get_attack(), 3);
    EXPECT_EQ(our_fourth_stew->get_health(), 4);
    EXPECT_EQ(our_fourth_murlocTH1->get_attack(), 4);
    EXPECT_EQ(our_fourth_murlocTH1->get_health(), 3);
    EXPECT_EQ(our_fourth_murlocTH2->get_attack(), 2);
    EXPECT_EQ(our_fourth_murlocTH2->get_health(), 1);
    EXPECT_EQ(our_fourth_salty_looter->get_attack(), 3);
    EXPECT_EQ(our_fourth_salty_looter->get_health(), 3);
    EXPECT_EQ(our_fourth_drag_lt->get_attack(), 2);
    EXPECT_EQ(our_fourth_drag_lt->get_health(), 3);
    // theirs
    auto their_fourth_tabby = their_fourth_board->get_cards()[0];
    auto their_fourth_selfless = their_fourth_board->get_cards()[1];
    auto their_fourth_scav1 = their_fourth_board->get_cards()[2];
    auto their_fourth_scav2 = their_fourth_board->get_cards()[3];
    auto their_fourth_ref_anom = their_fourth_board->get_cards()[4];
    auto their_fourth_drag_lt = their_fourth_board->get_cards()[5];
    auto their_fourth_right_prot = their_fourth_board->get_cards()[6];
    EXPECT_EQ(their_fourth_tabby->get_name(), "Tabbycat");
    EXPECT_EQ(their_fourth_selfless->get_name(), "Selfless Hero");
    EXPECT_EQ(their_fourth_scav1->get_name(), "Scavenging Hyena");
    EXPECT_EQ(their_fourth_scav2->get_name(), "Scavenging Hyena");
    EXPECT_EQ(their_fourth_ref_anom->get_name(), "Refreshing Anomaly");
    EXPECT_EQ(their_fourth_drag_lt->get_name(), "Dragonspawn Lieutenant");
    EXPECT_EQ(their_fourth_right_prot->get_name(), "Righteous Protector");
    EXPECT_EQ(their_fourth_tabby->get_attack(), 1);
    EXPECT_EQ(their_fourth_tabby->get_health(), 1);
    EXPECT_EQ(their_fourth_selfless->get_attack(), 2);
    EXPECT_EQ(their_fourth_selfless->get_health(), 1);
    EXPECT_EQ(their_fourth_scav1->get_attack(), 2);
    EXPECT_EQ(their_fourth_scav1->get_health(), 2);
    EXPECT_EQ(their_fourth_scav2->get_attack(), 2);
    EXPECT_EQ(their_fourth_scav2->get_health(), 2);
    EXPECT_EQ(their_fourth_ref_anom->get_attack(), 1);
    EXPECT_EQ(their_fourth_ref_anom->get_health(), 3);
    EXPECT_EQ(their_fourth_drag_lt->get_attack(), 2);
    EXPECT_EQ(their_fourth_drag_lt->get_health(), 3);
    EXPECT_EQ(their_fourth_right_prot->get_attack(), 1);
    EXPECT_EQ(their_fourth_right_prot->get_health(), 1);
}
