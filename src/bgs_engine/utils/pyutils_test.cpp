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
