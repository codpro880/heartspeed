#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <vector>

#include "../test/googletest/include/gtest/gtest.h"

#include "bobs_buddy.hpp"
#include "player.hpp"
#include "battler.hpp"
#include "rng_singleton.hpp"

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

class BobsReader : public ::testing::Test {
protected:
    void SetUp() override {
        if (battle_boards_p == nullptr) {
            std::filesystem::path power_log = std::filesystem::current_path() / "test_data" / "Power.log";
            auto bb = BobsBuddy(power_log.string());
            auto battle_boards = bb.parse_full_log();
            battle_boards_p = new std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>>(battle_boards);
        }
    }

    static std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>>* battle_boards_p;
};

std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>>* BobsReader::battle_boards_p = nullptr;

TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn1) {
    // Turn 1
    auto battle_boards = *battle_boards_p;
    auto our_first_board = battle_boards[0].first;
    auto their_first_board = battle_boards[0].second;
    EXPECT_EQ(our_first_board->length(), (unsigned)1);
    EXPECT_EQ(their_first_board->length(), (unsigned)1);
    auto our_rockpool = our_first_board->get_cards()[0];
    auto their_rockpool = their_first_board->get_cards()[0];
    EXPECT_EQ(our_rockpool->get_name(), "Rockpool Hunter");
    EXPECT_EQ(their_rockpool->get_name(), "Rockpool Hunter");
    EXPECT_EQ(our_rockpool->get_attack(), 4);
    EXPECT_EQ(their_rockpool->get_attack(), 4);
}

TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn2) {
    // Turn 2
    auto battle_boards = *battle_boards_p;
    auto our_second_board = battle_boards[1].first;
    auto their_second_board = battle_boards[1].second;
    EXPECT_EQ(our_second_board->length(), (unsigned)3);
    EXPECT_EQ(their_second_board->length(), (unsigned)1);
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

TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn3) {
    // Turn 3, can't see all off opp board from screenshot
    auto battle_boards = *battle_boards_p;
    auto our_third_board = battle_boards[2].first;
    // auto their_third_board = battle_boards[2].second;
    EXPECT_EQ(our_third_board->length(), (unsigned)5);
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
}

// Righteous protector removed from game
// TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn4) {
//     // Turn 4
//     auto battle_boards = *battle_boards_p;
//     auto our_fourth_board = battle_boards[3].first;
//     auto their_fourth_board = battle_boards[3].second;
//     EXPECT_EQ(our_fourth_board->length(), (unsigned)6);
//     EXPECT_EQ(their_fourth_board->length(), (unsigned)7);
//     // ours
//     auto our_fourth_rockpool = our_fourth_board->get_cards()[0];
//     auto our_fourth_stew = our_fourth_board->get_cards()[1];
//     auto our_fourth_murlocTH1 = our_fourth_board->get_cards()[2];
//     auto our_fourth_murlocTH2 = our_fourth_board->get_cards()[3];
//     auto our_fourth_salty_looter = our_fourth_board->get_cards()[4];
//     auto our_fourth_drag_lt = our_fourth_board->get_cards()[5];
//     EXPECT_EQ(our_fourth_rockpool->get_name(), "Rockpool Hunter (Golden)");
//     EXPECT_EQ(our_fourth_stew->get_name(), "Steward of Time");
//     EXPECT_EQ(our_fourth_murlocTH1->get_name(), "Murloc Tidehunter");
//     EXPECT_EQ(our_fourth_murlocTH2->get_name(), "Murloc Tidehunter");
//     EXPECT_EQ(our_fourth_salty_looter->get_name(), "Salty Looter");
//     EXPECT_EQ(our_fourth_drag_lt->get_name(), "Dragonspawn Lieutenant");
//     EXPECT_EQ(our_fourth_rockpool->get_attack(), 5);
//     EXPECT_EQ(our_fourth_rockpool->get_health(), 7);
//     EXPECT_EQ(our_fourth_stew->get_attack(), 3);
//     EXPECT_EQ(our_fourth_stew->get_health(), 4);
//     EXPECT_EQ(our_fourth_murlocTH1->get_attack(), 4);
//     EXPECT_EQ(our_fourth_murlocTH1->get_health(), 3);
//     EXPECT_EQ(our_fourth_murlocTH2->get_attack(), 2);
//     EXPECT_EQ(our_fourth_murlocTH2->get_health(), 1);
//     // Default has changed...TODO: Low prio, fix this issue
//     EXPECT_EQ(our_fourth_salty_looter->get_attack(), 4);
//     EXPECT_EQ(our_fourth_salty_looter->get_health(), 4);
//     EXPECT_EQ(our_fourth_drag_lt->get_attack(), 2);
//     EXPECT_EQ(our_fourth_drag_lt->get_health(), 3);
//     // theirs
//     auto their_fourth_tabby = their_fourth_board->get_cards()[0];
//     auto their_fourth_selfless = their_fourth_board->get_cards()[1];
//     auto their_fourth_scav1 = their_fourth_board->get_cards()[2];
//     auto their_fourth_scav2 = their_fourth_board->get_cards()[3];
//     auto their_fourth_ref_anom = their_fourth_board->get_cards()[4];
//     auto their_fourth_drag_lt = their_fourth_board->get_cards()[5];
//     auto their_fourth_right_prot = their_fourth_board->get_cards()[6];
//     EXPECT_EQ(their_fourth_tabby->get_name(), "Tabbycat");
//     EXPECT_EQ(their_fourth_selfless->get_name(), "Selfless Hero");
//     EXPECT_EQ(their_fourth_scav1->get_name(), "Scavenging Hyena");
//     EXPECT_EQ(their_fourth_scav2->get_name(), "Scavenging Hyena");
//     EXPECT_EQ(their_fourth_ref_anom->get_name(), "Refreshing Anomaly");
//     EXPECT_EQ(their_fourth_drag_lt->get_name(), "Dragonspawn Lieutenant");
//     EXPECT_EQ(their_fourth_right_prot->get_name(), "Righteous Protector");
//     EXPECT_EQ(their_fourth_tabby->get_attack(), 1);
//     EXPECT_EQ(their_fourth_tabby->get_health(), 1);
//     EXPECT_EQ(their_fourth_selfless->get_attack(), 2);
//     EXPECT_EQ(their_fourth_selfless->get_health(), 1);
//     EXPECT_EQ(their_fourth_scav1->get_attack(), 2);
//     EXPECT_EQ(their_fourth_scav1->get_health(), 2);
//     EXPECT_EQ(their_fourth_scav2->get_attack(), 2);
//     EXPECT_EQ(their_fourth_scav2->get_health(), 2);
//     EXPECT_EQ(their_fourth_ref_anom->get_attack(), 1);
//     EXPECT_EQ(their_fourth_ref_anom->get_health(), 3);
//     EXPECT_EQ(their_fourth_drag_lt->get_attack(), 2);
//     EXPECT_EQ(their_fourth_drag_lt->get_health(), 3);
//     EXPECT_EQ(their_fourth_right_prot->get_attack(), 1);
//     EXPECT_EQ(their_fourth_right_prot->get_health(), 1);
// }

 TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn5) {
    // Turn 5
    auto battle_boards = *battle_boards_p;
    auto our_fifth_board = battle_boards[4].first;
    auto their_fifth_board = battle_boards[4].second;
    EXPECT_EQ(our_fifth_board->length(), (unsigned)7);
    EXPECT_EQ(their_fifth_board->length(), (unsigned)6);
    // ours
    auto our_fifth_rockpool = our_fifth_board->get_cards()[0];
    auto our_fifth_salty_looter = our_fifth_board->get_cards()[1];
    auto our_fifth_scally = our_fifth_board->get_cards()[2];
    auto our_fifth_stew = our_fifth_board->get_cards()[3];
    auto our_fifth_murlocTH1 = our_fifth_board->get_cards()[4];
    auto our_fifth_murlocTH2 = our_fifth_board->get_cards()[5];
    auto our_fifth_drag_lt = our_fifth_board->get_cards()[6];
    EXPECT_EQ(our_fifth_rockpool->get_name(), "Rockpool Hunter (Golden)");
    EXPECT_EQ(our_fifth_salty_looter->get_name(), "Salty Looter");
    EXPECT_EQ(our_fifth_scally->get_name(), "Scallywag");
    EXPECT_EQ(our_fifth_stew->get_name(), "Steward of Time");
    EXPECT_EQ(our_fifth_murlocTH1->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_fifth_murlocTH2->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_fifth_drag_lt->get_name(), "Dragonspawn Lieutenant");
    EXPECT_EQ(our_fifth_rockpool->get_attack(), 6);
    EXPECT_EQ(our_fifth_rockpool->get_health(), 8);
    EXPECT_EQ(our_fifth_salty_looter->get_attack(), 5);
    EXPECT_EQ(our_fifth_salty_looter->get_health(), 5);
    EXPECT_EQ(our_fifth_scally->get_attack(), 2);
    EXPECT_EQ(our_fifth_scally->get_health(), 1);
    EXPECT_EQ(our_fifth_stew->get_attack(), 3);
    EXPECT_EQ(our_fifth_stew->get_health(), 4);
    EXPECT_EQ(our_fifth_murlocTH1->get_attack(), 4);
    EXPECT_EQ(our_fifth_murlocTH1->get_health(), 3);
    EXPECT_EQ(our_fifth_murlocTH2->get_attack(), 2);
    EXPECT_EQ(our_fifth_murlocTH2->get_health(), 1);
    EXPECT_EQ(our_fifth_drag_lt->get_attack(), 3);
    EXPECT_EQ(our_fifth_drag_lt->get_health(), 4);
    // theirs
    auto their_fifth_macaw = their_fifth_board->get_cards()[0];
    auto their_fifth_spawn = their_fifth_board->get_cards()[1];
    auto their_fifth_sellemental = their_fifth_board->get_cards()[2];
    auto their_fifth_deck_swab = their_fifth_board->get_cards()[3];
    auto their_fifth_alley = their_fifth_board->get_cards()[4];
    auto their_fifth_molten_rock = their_fifth_board->get_cards()[5];
    EXPECT_EQ(their_fifth_macaw->get_name(), "Monstrous Macaw");
    // Special checks for al-akir
    EXPECT_EQ(their_fifth_macaw->has_divine_shield(), true);
    EXPECT_EQ(their_fifth_macaw->has_taunt(), true);
    EXPECT_EQ(their_fifth_macaw->has_windfury(), true);
    EXPECT_EQ(their_fifth_macaw->get_name(), "Monstrous Macaw");
    EXPECT_EQ(their_fifth_spawn->get_name(), "Spawn of N'Zoth");
    EXPECT_EQ(their_fifth_sellemental->get_name(), "Sellemental");
    EXPECT_EQ(their_fifth_deck_swab->get_name(), "Deck Swabbie");
    EXPECT_EQ(their_fifth_alley->get_name(), "Alleycat");
    EXPECT_EQ(their_fifth_molten_rock->get_name(), "Molten Rock");
    EXPECT_EQ(their_fifth_macaw->get_attack(), 4);
    EXPECT_EQ(their_fifth_macaw->get_health(), 3);
    EXPECT_EQ(their_fifth_spawn->get_attack(), 2);
    EXPECT_EQ(their_fifth_spawn->get_health(), 2);
    EXPECT_EQ(their_fifth_sellemental->get_attack(), 2);
    EXPECT_EQ(their_fifth_sellemental->get_health(), 2);
    EXPECT_EQ(their_fifth_deck_swab->get_attack(), 2);
    EXPECT_EQ(their_fifth_deck_swab->get_health(), 2);
    EXPECT_EQ(their_fifth_alley->get_attack(), 1);
    EXPECT_EQ(their_fifth_alley->get_health(), 1);
    EXPECT_EQ(their_fifth_molten_rock->get_attack(), 2);
    EXPECT_EQ(their_fifth_molten_rock->get_health(), 3);
}

TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn7) {
    // Turn 7
    auto battle_boards = *battle_boards_p;
    auto our_seventh_board = battle_boards[6].first;
    auto their_seventh_board = battle_boards[6].second;
    EXPECT_EQ(our_seventh_board->length(), (unsigned)7);
    EXPECT_EQ(their_seventh_board->length(), (unsigned)7);
    // ours
    auto our_seventh_rockpool = our_seventh_board->get_cards()[0];
    auto our_seventh_salty_looter = our_seventh_board->get_cards()[1];
    auto our_seventh_bronzew1 = our_seventh_board->get_cards()[2];
    auto our_seventh_bronzew2 = our_seventh_board->get_cards()[3];
    auto our_seventh_murlocTH1 = our_seventh_board->get_cards()[4];
    auto our_seventh_murlocTH2 = our_seventh_board->get_cards()[5];
    auto our_seventh_stasis = our_seventh_board->get_cards()[6];
    EXPECT_EQ(our_seventh_rockpool->get_name(), "Rockpool Hunter (Golden)");
    EXPECT_EQ(our_seventh_salty_looter->get_name(), "Salty Looter");
    EXPECT_EQ(our_seventh_bronzew1->get_name(), "Bronze Warden");
    EXPECT_EQ(our_seventh_bronzew2->get_name(), "Bronze Warden");
    EXPECT_EQ(our_seventh_murlocTH1->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_seventh_murlocTH2->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(our_seventh_stasis->get_name(), "Stasis Elemental");
    EXPECT_EQ(our_seventh_rockpool->get_attack(), 6);
    EXPECT_EQ(our_seventh_rockpool->get_health(), 10);
    EXPECT_EQ(our_seventh_salty_looter->get_attack(), 6);
    EXPECT_EQ(our_seventh_salty_looter->get_health(), 6);
    EXPECT_EQ(our_seventh_bronzew1->get_attack(), 4);
    EXPECT_EQ(our_seventh_bronzew1->get_health(), 3);
    EXPECT_EQ(our_seventh_bronzew2->get_attack(), 4);
    EXPECT_EQ(our_seventh_bronzew2->get_health(), 3);
    EXPECT_EQ(our_seventh_murlocTH1->get_attack(), 4);
    EXPECT_EQ(our_seventh_murlocTH1->get_health(), 5);
    EXPECT_EQ(our_seventh_murlocTH2->get_attack(), 2);
    EXPECT_EQ(our_seventh_murlocTH2->get_health(), 3);
    EXPECT_EQ(our_seventh_stasis->get_attack(), 6);
    EXPECT_EQ(our_seventh_stasis->get_health(), 6);
    // theirs
    auto their_seventh_glyph = their_seventh_board->get_cards()[0];
    auto their_seventh_selfless1 = their_seventh_board->get_cards()[1];
    auto their_seventh_selfless2 = their_seventh_board->get_cards()[2];
    auto their_seventh_stew1 = their_seventh_board->get_cards()[3];
    auto their_seventh_hangry = their_seventh_board->get_cards()[4];
    auto their_seventh_stew2 = their_seventh_board->get_cards()[5];
    auto their_seventh_waxrider = their_seventh_board->get_cards()[6];
    EXPECT_EQ(their_seventh_glyph->get_name(), "Glyph Guardian");
    // Special checks for lich king
    EXPECT_EQ(their_seventh_glyph->has_reborn(), true);
    EXPECT_EQ(their_seventh_selfless1->get_name(), "Selfless Hero");
    EXPECT_EQ(their_seventh_selfless2->get_name(), "Selfless Hero");
    EXPECT_EQ(their_seventh_stew1->get_name(), "Steward of Time");
    EXPECT_EQ(their_seventh_hangry->get_name(), "Hangry Dragon");
    EXPECT_EQ(their_seventh_stew2->get_name(), "Steward of Time");
    EXPECT_EQ(their_seventh_waxrider->get_name(), "Waxrider Togwaggle");
    EXPECT_EQ(their_seventh_glyph->get_attack(), 2);
    EXPECT_EQ(their_seventh_glyph->get_health(), 4);
    EXPECT_EQ(their_seventh_selfless1->get_attack(), 2);
    EXPECT_EQ(their_seventh_selfless1->get_health(), 1);
    EXPECT_EQ(their_seventh_selfless2->get_attack(), 2);
    EXPECT_EQ(their_seventh_selfless2->get_health(), 1);
    EXPECT_EQ(their_seventh_stew1->get_attack(), 3);
    EXPECT_EQ(their_seventh_stew1->get_health(), 4);
    EXPECT_EQ(their_seventh_hangry->get_attack(), 6);
    EXPECT_EQ(their_seventh_hangry->get_health(), 6);
    EXPECT_EQ(their_seventh_stew2->get_attack(), 3);
    EXPECT_EQ(their_seventh_stew2->get_health(), 4);
    EXPECT_EQ(their_seventh_waxrider->get_attack(), 1);
    EXPECT_EQ(their_seventh_waxrider->get_health(), 2);
}

TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn8) {
    // Turn 8, 10 Gold
    auto battle_boards = *battle_boards_p;
    auto our_eighth_board = battle_boards[7].first;
    auto their_eighth_board = battle_boards[7].second;
    EXPECT_EQ(our_eighth_board->length(), (unsigned)7);
    EXPECT_EQ(their_eighth_board->length(), (unsigned)7);
    // ours
    auto our_eighth_rockpool = our_eighth_board->get_cards()[0];
    auto our_eighth_salty_looter1 = our_eighth_board->get_cards()[1];
    auto our_eighth_salty_looter2 = our_eighth_board->get_cards()[2];
    auto our_eighth_bronzew1 = our_eighth_board->get_cards()[3];
    auto our_eighth_bronzew2 = our_eighth_board->get_cards()[4];
    auto our_eighth_stasis = our_eighth_board->get_cards()[5];
    auto our_eighth_yoho = our_eighth_board->get_cards()[6];
    EXPECT_EQ(our_eighth_rockpool->get_name(), "Rockpool Hunter (Golden)");
    EXPECT_EQ(our_eighth_salty_looter1->get_name(), "Salty Looter");
    EXPECT_EQ(our_eighth_salty_looter2->get_name(), "Salty Looter");
    EXPECT_EQ(our_eighth_bronzew1->get_name(), "Bronze Warden");
    EXPECT_EQ(our_eighth_bronzew2->get_name(), "Bronze Warden");
    EXPECT_EQ(our_eighth_stasis->get_name(), "Stasis Elemental");
    EXPECT_EQ(our_eighth_yoho->get_name(), "Yo-Ho-Ogre");
    EXPECT_EQ(our_eighth_rockpool->get_attack(), 6);
    EXPECT_EQ(our_eighth_rockpool->get_health(), 10);
    EXPECT_EQ(our_eighth_salty_looter1->get_attack(), 8);
    EXPECT_EQ(our_eighth_salty_looter1->get_health(), 8);
    EXPECT_EQ(our_eighth_salty_looter2->get_attack(), 4);
    EXPECT_EQ(our_eighth_salty_looter2->get_health(), 4);
    EXPECT_EQ(our_eighth_bronzew1->get_attack(), 4);
    EXPECT_EQ(our_eighth_bronzew1->get_health(), 3);
    EXPECT_EQ(our_eighth_bronzew2->get_attack(), 4);
    EXPECT_EQ(our_eighth_bronzew2->get_health(), 3);
    EXPECT_EQ(our_eighth_stasis->get_attack(), 6);
    EXPECT_EQ(our_eighth_stasis->get_health(), 6);
    EXPECT_EQ(our_eighth_yoho->get_attack(), 2);
    EXPECT_EQ(our_eighth_yoho->get_health(), 8);
    // theirs
    auto their_eighth_spawn = their_eighth_board->get_cards()[0];
    auto their_eighth_tabby1 = their_eighth_board->get_cards()[1];
    auto their_eighth_tabby2 = their_eighth_board->get_cards()[2];
    auto their_eighth_selfless = their_eighth_board->get_cards()[3];
    auto their_eighth_scav = their_eighth_board->get_cards()[4];
    auto their_eighth_crackling = their_eighth_board->get_cards()[5];
    auto their_eighth_baron = their_eighth_board->get_cards()[6];
    EXPECT_EQ(their_eighth_spawn->get_name(), "Spawn of N'Zoth");
    EXPECT_EQ(their_eighth_tabby1->get_name(), "Tabbycat");
    EXPECT_EQ(their_eighth_tabby2->get_name(), "Tabbycat");
    EXPECT_EQ(their_eighth_selfless->get_name(), "Selfless Hero");
    EXPECT_EQ(their_eighth_scav->get_name(), "Scavenging Hyena (Golden)");
    EXPECT_EQ(their_eighth_crackling->get_name(), "Crackling Cyclone");
    EXPECT_EQ(their_eighth_baron->get_name(), "Baron Rivendare");
    EXPECT_EQ(their_eighth_spawn->get_attack(), 2);
    EXPECT_EQ(their_eighth_spawn->get_health(), 2);
    EXPECT_EQ(their_eighth_tabby1->get_attack(), 1);
    EXPECT_EQ(their_eighth_tabby1->get_health(), 1);
    EXPECT_EQ(their_eighth_tabby2->get_attack(), 1);
    EXPECT_EQ(their_eighth_tabby2->get_health(), 1);
    EXPECT_EQ(their_eighth_selfless->get_attack(), 2);
    EXPECT_EQ(their_eighth_selfless->get_health(), 1);
    EXPECT_EQ(their_eighth_scav->get_attack(), 4);
    EXPECT_EQ(their_eighth_scav->get_health(), 4);
    EXPECT_EQ(their_eighth_crackling->get_attack(), 4);
    EXPECT_EQ(their_eighth_crackling->get_health(), 1);
    EXPECT_EQ(their_eighth_baron->get_attack(), 1);
    EXPECT_EQ(their_eighth_baron->get_health(), 7);
    // Give some tolerance, but bobs buddy gave:
    // Lethal: 13.6, Win 99.7, Tie .2, Loss .1, Lethal 0
    std::unique_ptr<Player> p1(new Player(our_eighth_board, "Ours"));
    std::unique_ptr<Player> p2(new Player(their_eighth_board, "Theirs"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battles_par(10000);
    std::cerr << "P1 win: " << res.p1_win << std::endl;
    EXPECT_LT(res.p1_win, 1);
    EXPECT_GT(res.p1_win, .95);
    std::cerr << "draw: " << res.draw << std::endl;
    EXPECT_LT(res.draw, .005);
    EXPECT_GT(res.draw, 0);
    std::cerr << "p2 win: " << res.p2_win << std::endl;
    EXPECT_LT(res.p2_win, .005);
    EXPECT_GT(res.p2_win, 0);
}

// TODO: Debug this. Probably need some visualization tooling
// TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn9) {
//     // Turn 9
//     auto battle_boards = *battle_boards_p;
//     auto our_nineth_board = battle_boards[8].first;
//     auto their_nineth_board = battle_boards[8].second;
//     EXPECT_EQ(our_nineth_board->length(), (unsigned)7);
//     EXPECT_EQ(their_nineth_board->length(), (unsigned)7);
//     // ours
//     auto our_nineth_rockpool = our_nineth_board->get_cards()[0];
//     auto our_nineth_salty_looter1 = our_nineth_board->get_cards()[1];
//     auto our_nineth_bronzew1 = our_nineth_board->get_cards()[2];
//     auto our_nineth_bronzew2 = our_nineth_board->get_cards()[3];
//     auto our_nineth_stasis = our_nineth_board->get_cards()[4];
//     auto our_nineth_salty_looter2 = our_nineth_board->get_cards()[5];
//     auto our_nineth_yoho = our_nineth_board->get_cards()[6];
//     EXPECT_EQ(our_nineth_rockpool->get_name(), "Rockpool Hunter (Golden)");
//     EXPECT_EQ(our_nineth_salty_looter1->get_name(), "Salty Looter");
//     EXPECT_EQ(our_nineth_salty_looter2->get_name(), "Salty Looter");
//     EXPECT_EQ(our_nineth_bronzew1->get_name(), "Bronze Warden");
//     EXPECT_EQ(our_nineth_bronzew2->get_name(), "Bronze Warden");
//     EXPECT_EQ(our_nineth_stasis->get_name(), "Stasis Elemental");
//     EXPECT_EQ(our_nineth_yoho->get_name(), "Yo-Ho-Ogre");
//     EXPECT_EQ(our_nineth_rockpool->get_attack(), 6);
//     EXPECT_EQ(our_nineth_rockpool->get_health(), 10);
//     EXPECT_EQ(our_nineth_salty_looter1->get_attack(), 8);
//     EXPECT_EQ(our_nineth_salty_looter1->get_health(), 8);
//     EXPECT_EQ(our_nineth_salty_looter2->get_attack(), 4);
//     EXPECT_EQ(our_nineth_salty_looter2->get_health(), 4);
//     EXPECT_EQ(our_nineth_bronzew1->get_attack(), 4);
//     EXPECT_EQ(our_nineth_bronzew1->get_health(), 3);
//     EXPECT_EQ(our_nineth_bronzew2->get_attack(), 4);
//     EXPECT_EQ(our_nineth_bronzew2->get_health(), 3);
//     EXPECT_EQ(our_nineth_stasis->get_attack(), 6);
//     EXPECT_EQ(our_nineth_stasis->get_health(), 6);
//     EXPECT_EQ(our_nineth_yoho->get_attack(), 2);
//     EXPECT_EQ(our_nineth_yoho->get_health(), 8);
//     // theirs
//     auto their_nineth_macaw = their_nineth_board->get_cards()[0];
//     auto their_nineth_spawn = their_nineth_board->get_cards()[1];
//     auto their_nineth_crackling = their_nineth_board->get_cards()[2];
//     auto their_nineth_sauro = their_nineth_board->get_cards()[3];
//     auto their_nineth_bronzew = their_nineth_board->get_cards()[4];
//     auto their_nineth_crowdp = their_nineth_board->get_cards()[5];
//     auto their_nineth_drake = their_nineth_board->get_cards()[6];
//     EXPECT_EQ(their_nineth_macaw->get_name(), "Monstrous Macaw");;
//     EXPECT_EQ(their_nineth_spawn->get_name(), "Spawn of N'Zoth (Golden)");
//     EXPECT_EQ(their_nineth_spawn->has_taunt(), true);
//     EXPECT_EQ(their_nineth_crackling->get_name(), "Crackling Cyclone");
//     EXPECT_EQ(their_nineth_sauro->get_name(), "Rabid Saurolisk");
//     EXPECT_EQ(their_nineth_sauro->has_taunt(), true);
//     EXPECT_EQ(their_nineth_bronzew->get_name(), "Bronze Warden");
//     EXPECT_EQ(their_nineth_crowdp->get_name(), "Crowd Favorite");
//     EXPECT_EQ(their_nineth_drake->get_name(), "Twilight Emissary");
//     EXPECT_EQ(their_nineth_drake->has_taunt(), true);
//     EXPECT_EQ(their_nineth_macaw->get_attack(), 4);
//     EXPECT_EQ(their_nineth_macaw->get_health(), 3);    
//     EXPECT_EQ(their_nineth_spawn->get_attack(), 5);
//     EXPECT_EQ(their_nineth_spawn->get_health(), 5);
//     EXPECT_EQ(their_nineth_crackling->get_attack(), 5);
//     EXPECT_EQ(their_nineth_crackling->get_health(), 2);
//     EXPECT_EQ(their_nineth_sauro->get_attack(), 8);
//     EXPECT_EQ(their_nineth_sauro->get_health(), 6);
//     EXPECT_EQ(their_nineth_bronzew->get_attack(), 7);
//     EXPECT_EQ(their_nineth_bronzew->get_health(), 6);
//     EXPECT_EQ(their_nineth_crowdp->get_attack(), 6);
//     EXPECT_EQ(their_nineth_crowdp->get_health(), 6);
//     EXPECT_EQ(their_nineth_drake->get_attack(), 4);
//     EXPECT_EQ(their_nineth_drake->get_health(), 4);
//     // Give some tolerance, but bobs buddy gave:
//     // Lethal: 13.6, Win 99.7, Tie .2, Loss .1, Lethal 0
//     std::unique_ptr<Player> p1(new Player(our_nineth_board.get(), "Ours"));
//     std::unique_ptr<Player> p2(new Player(their_nineth_board.get(), "Theirs"));
//     auto battler = Battler(p1.get(), p2.get());
//     auto res = battler.sim_battles(10000);
//     std::cerr << "P1 win: " << res.p1_win << std::endl;
//     EXPECT_LT(res.p1_win, .01);
//     EXPECT_GT(res.p1_win, 0);
//     // EXPECT_GT(res.p1_lethal, .10);
//     std::cerr << "draw: " << res.draw << std::endl;
//     EXPECT_LT(res.draw, .02);
//     EXPECT_GT(res.draw, .01);
//     std::cerr << "p2 win: " << res.p2_win << std::endl;
//     EXPECT_LT(res.p2_win, .98);
//     EXPECT_GT(res.p2_win, .97);
//     // EXPECT_EQ(res.p2_lethal, 0);    
// }

// TODO: Debug this. Probably need some visualization tooling
// NOTE: Got this under vis, beat out a few bugs, but there's still discrepancies.
//       will come back to this...
// TEST_F(BobsReader, CanGetBattleBoardsFromLogTurn10) {
//     // Turn 10
//     auto battle_boards = *battle_boards_p;
//     auto our_board = battle_boards[9].first;
//     auto their_board = battle_boards[9].second;
//     EXPECT_EQ(our_board->length(), (unsigned)7);
//     EXPECT_EQ(their_board->length(), (unsigned)7);
//     // ours
//     auto our_rockpool = our_board->get_cards()[0];
//     auto our_salty_looter1 = our_board->get_cards()[1];
//     auto our_bronzew1 = our_board->get_cards()[2];
//     auto our_bronzew2 = our_board->get_cards()[3];
//     auto our_salty_looter2 = our_board->get_cards()[4];
//     auto our_lightfang = our_board->get_cards()[5];
//     auto our_yoho = our_board->get_cards()[6];
//     std::cerr << "Our cards..." << std::endl;
//     EXPECT_EQ(our_rockpool->get_name(), "Rockpool Hunter (Golden)");
//     EXPECT_EQ(our_salty_looter1->get_name(), "Salty Looter");
//     EXPECT_EQ(our_salty_looter2->get_name(), "Salty Looter");
//     EXPECT_EQ(our_bronzew1->get_name(), "Bronze Warden");
//     EXPECT_EQ(our_bronzew2->get_name(), "Bronze Warden");
//     EXPECT_EQ(our_lightfang->get_name(), "Lightfang Enforcer");
//     EXPECT_EQ(our_yoho->get_name(), "Yo-Ho-Ogre");
//     EXPECT_EQ(our_rockpool->get_attack(), 8);
//     EXPECT_EQ(our_rockpool->get_health(), 11);
//     EXPECT_EQ(our_salty_looter1->get_attack(), 13);
//     EXPECT_EQ(our_salty_looter1->get_health(), 10);
//     EXPECT_EQ(our_salty_looter2->get_attack(), 9);
//     EXPECT_EQ(our_salty_looter2->get_health(), 6);
//     EXPECT_EQ(our_bronzew1->get_attack(), 6);
//     EXPECT_EQ(our_bronzew1->get_health(), 5);
//     EXPECT_EQ(our_bronzew2->get_attack(), 6);
//     EXPECT_EQ(our_bronzew2->get_health(), 4);
//     EXPECT_EQ(our_lightfang->get_attack(), 2);
//     EXPECT_EQ(our_lightfang->get_health(), 2);
//     EXPECT_EQ(our_yoho->get_attack(), 7);
//     EXPECT_EQ(our_yoho->get_health(), 9);
//     std::cerr << "Our asserts..." << std::endl;
//     // theirs
//     auto their_glyph = their_board->get_cards()[0];
//     auto their_wildfire1 = their_board->get_cards()[1];
//     auto their_wildfire2 = their_board->get_cards()[2];
//     auto their_rag = their_board->get_cards()[3];
//     auto their_bronzew = their_board->get_cards()[4];
//     auto their_nomi = their_board->get_cards()[5];
//     auto their_hangry = their_board->get_cards()[6];
//     std::cerr << "THeir cards..." << std::endl;
//     EXPECT_EQ(their_glyph->get_name(), "Glyph Guardian (Golden)");
//     EXPECT_TRUE(their_glyph->has_reborn());
//     EXPECT_EQ(their_wildfire1->get_name(), "Wildfire Elemental");
//     EXPECT_EQ(their_wildfire2->get_name(), "Wildfire Elemental");
//     EXPECT_EQ(their_rag->get_name(), "Lil' Rag");
//     EXPECT_EQ(their_bronzew->get_name(), "Bronze Warden");
//     EXPECT_EQ(their_nomi->get_name(), "Nomi, Kitchen Nightmare");
//     EXPECT_EQ(their_hangry->get_name(), "Hangry Dragon (Golden)");
//     EXPECT_EQ(their_glyph->get_attack(), 6);
//     EXPECT_EQ(their_glyph->get_health(), 10);
//     EXPECT_EQ(their_wildfire1->get_attack(), 13);
//     EXPECT_EQ(their_wildfire1->get_health(), 9);
//     EXPECT_EQ(their_wildfire2->get_attack(), 7);
//     EXPECT_EQ(their_wildfire2->get_health(), 3);
//     EXPECT_EQ(their_rag->get_attack(), 4);
//     EXPECT_EQ(their_rag->get_health(), 4);
//     EXPECT_EQ(their_bronzew->get_attack(), 6);
//     EXPECT_EQ(their_bronzew->get_health(), 5);
//     EXPECT_EQ(their_nomi->get_attack(), 4);
//     EXPECT_EQ(their_nomi->get_health(), 4);
//     EXPECT_EQ(their_hangry->get_attack(), 10);
//     EXPECT_EQ(their_hangry->get_health(), 10);
//     std::cerr << "THeir asserts..." << std::endl;
//     // Give some tolerance, but bobs buddy gave:
//     // Lethal: 13.6, Win 99.7, Tie .2, Loss .1, Lethal 0
//     std::unique_ptr<Player> p1(new Player(our_board, "Ours"));
//     std::unique_ptr<Player> p2(new Player(their_board, "Theirs"));
//     auto battler = Battler(p1.get(), p2.get());
//     std::cerr << "Battlin..." << std::endl;
    
//     // auto res = battler.sim_battle();
//     //res = battler.sim_battle();
//     // auto bfjd = BattleFrameJsonDump();
//     // std::string filename = "bob_again.json";
//     // bfjd.dump_to_json(res, filename);
//     // std::ifstream ifs(filename);
//     // EXPECT_TRUE(ifs.good());

//     auto res = battler.sim_battles_par(10000);
//     std::cerr << "P1 win: " << res.p1_win << std::endl;
//     EXPECT_GT(res.p1_win, .35);
//     EXPECT_LT(res.p1_win, .4);
//     std::cerr << "draw: " << res.draw << std::endl;
//     EXPECT_GT(res.draw, .15);
//     EXPECT_LT(res.draw, .2);
//     std::cerr << "p2 win: " << res.p2_win << std::endl;
//     EXPECT_GT(res.p2_win, .42);
//     EXPECT_LT(res.p2_win, .48);
// }

TEST_F(BobsReader, IsReasonablyThreadSafe) {
    // Turn 10
    auto battle_boards = *battle_boards_p;
    auto our_board = battle_boards[9].first;
    auto their_board = battle_boards[9].second;
    // EXPECT_EQ(our_board->length(), (unsigned)7);
    // EXPECT_EQ(their_board->length(), (unsigned)7);
    // // ours
    // auto our_rockpool = our_board->get_cards()[0];
    // auto our_salty_looter1 = our_board->get_cards()[1];
    // auto our_bronzew1 = our_board->get_cards()[2];
    // auto our_bronzew2 = our_board->get_cards()[3];
    // auto our_salty_looter2 = our_board->get_cards()[4];
    // auto our_lightfang = our_board->get_cards()[5];
    // auto our_yoho = our_board->get_cards()[6];
    // std::cerr << "Our cards..." << std::endl;
    // EXPECT_EQ(our_rockpool->get_name(), "Rockpool Hunter (Golden)");
    // EXPECT_EQ(our_salty_looter1->get_name(), "Salty Looter");
    // EXPECT_EQ(our_salty_looter2->get_name(), "Salty Looter");
    // EXPECT_EQ(our_bronzew1->get_name(), "Bronze Warden");
    // EXPECT_EQ(our_bronzew2->get_name(), "Bronze Warden");
    // EXPECT_EQ(our_lightfang->get_name(), "Lightfang Enforcer");
    // EXPECT_EQ(our_yoho->get_name(), "Yo-Ho-Ogre");
    // EXPECT_EQ(our_rockpool->get_attack(), 8);
    // EXPECT_EQ(our_rockpool->get_health(), 11);
    // EXPECT_EQ(our_salty_looter1->get_attack(), 13);
    // EXPECT_EQ(our_salty_looter1->get_health(), 10);
    // EXPECT_EQ(our_salty_looter2->get_attack(), 9);
    // EXPECT_EQ(our_salty_looter2->get_health(), 6);
    // EXPECT_EQ(our_bronzew1->get_attack(), 6);
    // EXPECT_EQ(our_bronzew1->get_health(), 5);
    // EXPECT_EQ(our_bronzew2->get_attack(), 6);
    // EXPECT_EQ(our_bronzew2->get_health(), 4);
    // EXPECT_EQ(our_lightfang->get_attack(), 2);
    // EXPECT_EQ(our_lightfang->get_health(), 2);
    // EXPECT_EQ(our_yoho->get_attack(), 7);
    // EXPECT_EQ(our_yoho->get_health(), 9);
    // std::cerr << "Our asserts..." << std::endl;
    // // theirs
    // auto their_glyph = their_board->get_cards()[0];
    // auto their_wildfire1 = their_board->get_cards()[1];
    // auto their_wildfire2 = their_board->get_cards()[2];
    // auto their_rag = their_board->get_cards()[3];
    // auto their_bronzew = their_board->get_cards()[4];
    // auto their_nomi = their_board->get_cards()[5];
    // auto their_hangry = their_board->get_cards()[6];
    // std::cerr << "THeir cards..." << std::endl;
    // EXPECT_EQ(their_glyph->get_name(), "Glyph Guardian (Golden)");
    // EXPECT_TRUE(their_glyph->has_reborn());
    // EXPECT_EQ(their_wildfire1->get_name(), "Wildfire Elemental");
    // EXPECT_EQ(their_wildfire2->get_name(), "Wildfire Elemental");
    // EXPECT_EQ(their_rag->get_name(), "Lil' Rag");
    // EXPECT_EQ(their_bronzew->get_name(), "Bronze Warden");
    // EXPECT_EQ(their_nomi->get_name(), "Nomi, Kitchen Nightmare");
    // EXPECT_EQ(their_hangry->get_name(), "Hangry Dragon (Golden)");
    // EXPECT_EQ(their_glyph->get_attack(), 6);
    // EXPECT_EQ(their_glyph->get_health(), 10);
    // EXPECT_EQ(their_wildfire1->get_attack(), 13);
    // EXPECT_EQ(their_wildfire1->get_health(), 9);
    // EXPECT_EQ(their_wildfire2->get_attack(), 7);
    // EXPECT_EQ(their_wildfire2->get_health(), 3);
    // EXPECT_EQ(their_rag->get_attack(), 4);
    // EXPECT_EQ(their_rag->get_health(), 4);
    // EXPECT_EQ(their_bronzew->get_attack(), 6);
    // EXPECT_EQ(their_bronzew->get_health(), 5);
    // EXPECT_EQ(their_nomi->get_attack(), 4);
    // EXPECT_EQ(their_nomi->get_health(), 4);
    // EXPECT_EQ(their_hangry->get_attack(), 10);
    // EXPECT_EQ(their_hangry->get_health(), 10);


    
    // Give some tolerance, but bobs buddy gave:
    // Lethal: 13.6, Win 99.7, Tie .2, Loss .1, Lethal 0
    std::unique_ptr<Player> p1(new Player(our_board, "Ours"));
    std::unique_ptr<Player> p2(new Player(their_board, "Theirs"));
    auto battler = Battler(p1.get(), p2.get());

    // Want to say parallel and non-parallel versions are more or less the same
    RngSingleton::getInstance(123, true); // this also sets seed...
    auto res_par = battler.sim_battles_par(2000);
    RngSingleton::getInstance(123, true); // this also sets seed...
    auto res = battler.sim_battles(2000);

    // Within 5%, sample size of 1000 has relatively high variance
    // TODO: Make thread_local seeding for reproducibility
    EXPECT_LT(std::fabs(res_par.p1_win - res.p1_win), .05);
    EXPECT_LT(std::fabs(res_par.p2_win - res.p2_win), .05);
    EXPECT_LT(std::fabs(res_par.draw - res.draw), .05);
}
