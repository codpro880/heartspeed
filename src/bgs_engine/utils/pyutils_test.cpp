#include "../test/googletest/include/gtest/gtest.h"

#include "pyutils.hpp"

TEST(PyUtils, CanDoVectorContainmentStr) {
    std::vector<std::string> test({"one", "two", "three", "four"});
    EXPECT_TRUE(pyutils::in("one", test));
    EXPECT_FALSE(pyutils::in("five", test));
}

TEST(PyUtils, CanDoVectorContainmentInt) {
    std::vector<int> test({1, 2, 3, 4});
    EXPECT_TRUE(pyutils::in(3, test));
    EXPECT_FALSE(pyutils::in(8, test));
}

TEST(PyUtils, CanDoVectorContainmentSetInt) {
    std::set<int> test({1, 2, 3, 4});
    EXPECT_TRUE(pyutils::in(3, test));
    EXPECT_FALSE(pyutils::in(8, test));
}

TEST(PyUtils, CanDoVectorContainmentUnorderedSetInt) {
    std::unordered_set<int> test({1, 2, 3, 4});
    EXPECT_TRUE(pyutils::in(3, test));
    EXPECT_FALSE(pyutils::in(8, test));
}

TEST(PyUtils, CanDoStringContainment) {
    std::string test("SOME LONG STRING");
    std::string another("LONG");
    EXPECT_TRUE(pyutils::in("SOME", test));
    EXPECT_TRUE(pyutils::in(another, test));
    EXPECT_FALSE(pyutils::in("SOMEWHERE", test));
}

TEST(PyUtils, CanGetStringInBetweenTwoOtherStrings) {
    auto res = pyutils::get_str_between("PLAY_CARD_FROM_HAND_0_TO_BOARD_0", "HAND_", "_TO_BOARD");
    EXPECT_EQ(res, "0");
}

TEST(PyUtils, CanGetStringInBetweenWithEmptyStringOnEnd) {
    auto res = pyutils::get_str_between("PLAY_CARD_FROM_HAND_0_TO_BOARD_0", "TO_BOARD_", "");
    EXPECT_EQ(res, "0");
}

TEST(BobsBuddy, PyLikeStringParsing) {
    std::string item = "D 08:11:01.0627100 PowerTaskList.DebugPrintPower() - BLOCK_START BlockType=TRIGGER Entity=[entityName=BaconShop8PlayerEnchant id=71 zone=PLAY zonePos=0 cardId=TB_BaconShop_8P_PlayerE player=8] EffectCardId=System.Collections.Generic.List`1[System.String] EffectIndex=9 Target=0 SubOption=-1 TriggerKeyword=0";
    std::string start = "id=";
    std::string end = " zone=";
    auto id = pyutils::get_str_between(item, start, end);
    EXPECT_EQ(id, "71");
}

TEST(BobsBuddy, PyLikeStringParsingWorksWithEmptyEndStr) {
    std::string item = "D 08:20:54.1034250 PowerTaskList.DebugPrintPower() -     TAG_CHANGE Entity=[entityName=Rockpool Hunter id=3180 zone=SETASIDE zonePos=0 cardId=TB_BaconUps_061 player=8] tag=HEALTH value=10";
    std::string start = "value=";
    std::string end = "";
    auto id = pyutils::get_str_between(item, start, end);
    EXPECT_EQ(id, "10");
}

TEST(BobsBuddy, PyLikeStringParsingWorksWithWhitespace) {
    std::string item = "D 08:20:54.1034250 PowerTaskList.DebugPrintPower() -     TAG_CHANGE Entity=[entityName=Rockpool Hunter id=3180 zone=SETASIDE zonePos=0 cardId=TB_BaconUps_061 player=8] tag=HEALTH value=10 ";
    std::string start = "value=";
    std::string end = " ";
    auto id = pyutils::get_str_between(item, start, end);
    EXPECT_EQ(atoi(id.c_str()), 10);
}


// TEST(PyUtils, CanDoNegativeIndexingOnArrays) {
//     std::string test("SOME LONG STRING");
//     auto test_pystr = pyutils::PyStr(test);
//     std::string another("LONG");
//     auto another_pystr = pyutils::PyStr(another);
//     EXPECT_TRUE(pyutils::in("SOME", test_pystr));
//     EXPECT_TRUE(pyutils::in(another_pystr, test_pystr));
//     EXPECT_FALSE(pyutils::in("SOMEWHERE", test_pystr));
//     EXPECT_EQ(test_pystr[-1], "G");
// }
