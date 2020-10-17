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

    // Turn 5
    auto our_fifth_board = battle_boards[4].first;
    auto their_fifth_board = battle_boards[4].second;
    EXPECT_EQ(our_fifth_board->length(), 7);
    EXPECT_EQ(their_fifth_board->length(), 6);
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

    // Turn 6
    // Not totally sure what board state was, bad screenshots

    // Turn 7
    auto our_seventh_board = battle_boards[6].first;
    auto their_seventh_board = battle_boards[6].second;
    EXPECT_EQ(our_seventh_board->length(), 7);
    EXPECT_EQ(their_seventh_board->length(), 7);
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
    std::cerr << "Past theirs ours" << std::endl;
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

    // Turn 8, 10 gold
    auto our_eighth_board = battle_boards[7].first;
    auto their_eighth_board = battle_boards[7].second;
    EXPECT_EQ(our_eighth_board->length(), 7);
    EXPECT_EQ(their_eighth_board->length(), 7);
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
    EXPECT_EQ(our_eighth_salty_looter1->get_name(), "Salty Looter");
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

}
