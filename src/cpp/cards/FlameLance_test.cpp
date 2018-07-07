#include "FlameLance.hpp"
#include "../test/googletest/include/gtest/gtest.h"

TEST(FlameLance, HasCorrectName) {
    auto f = FlameLance();
    EXPECT_EQ(f.get_name(), "Flame Lance");
}
