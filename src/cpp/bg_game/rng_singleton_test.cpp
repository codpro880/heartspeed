#include <iostream>

#include "../test/googletest/include/gtest/gtest.h"

#include "rng_singleton.hpp"

TEST(RngSingleton, CannotResetSeedAfterInit) {
    RngSingleton& rng = RngSingleton::getInstance(123, true);
    EXPECT_EQ(rng.get_seed(), 123);
    RngSingleton& rng_again = RngSingleton::getInstance(999);
    EXPECT_EQ(rng.get_seed(), 123);
}
