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

TEST(PyUtils, CanDoSplitLikeFunctionality) {
    auto res = pyutils::get_str_between("PLAY_CARD_FROM_HAND_0_TO_BOARD_0", "HAND_", "_TO_BOARD");
    EXPECT_EQ(res, "0");
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
