#include <iostream>

#include "../test/googletest/include/gtest/gtest.h"

#include "rng_singleton.hpp"

TEST(RngSingleton, CannotResetSeedAfterInit) {
    RngSingleton& rng = RngSingleton::getInstance(123, true);
    EXPECT_EQ(rng.get_seed(), 123);
    RngSingleton& rng_again = RngSingleton::getInstance(999);
    EXPECT_EQ(rng.get_seed(), 123);
    EXPECT_EQ(rng_again.get_seed(), 123);
}

TEST(RngSingleton, GetRandIntWithbounds) {
    RngSingleton& rng = RngSingleton::getInstance();
    bool one_hit = false;
    bool zero_hit = false;
    for (int i = 0; i < 100; i++) {
        auto res = rng.get_rand_int(0, 1);
        if (res == 1) one_hit = true;
        if (res == 0) zero_hit = true;
        EXPECT_TRUE(res == 0 || res == 1);
    }
    EXPECT_TRUE(one_hit);
    EXPECT_TRUE(zero_hit);
}
