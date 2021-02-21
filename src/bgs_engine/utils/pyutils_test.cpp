#include "pyutils.hpp"

TEST(PyUtils, CanDoVectorContainment) {
    std::vector<std::string> test({"one", "two", "three", "four"});
    EXPECT_EQ(pyutils::contains(test, "one"));
}
