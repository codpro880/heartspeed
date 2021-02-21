#include "../test/googletest/include/gtest/gtest.h"

#include "pyutils.hpp"

TEST(PyUtils, CanDoVectorContainmentStr) {
    std::vector<std::string> test({"one", "two", "three", "four"});
    EXPECT_TRUE(pyutils::contains(test, "one"));
    EXPECT_FALSE(pyutils::contains(test, "five"));
}

TEST(PyUtils, CanDoVectorContainmentInt) {
    std::vector<int> test({1, 2, 3, 4});
    EXPECT_TRUE(pyutils::contains(test, 3));
    EXPECT_FALSE(pyutils::contains(test, 8));
}
