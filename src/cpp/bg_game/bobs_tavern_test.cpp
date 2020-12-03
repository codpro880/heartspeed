#include "../test/googletest/include/gtest/gtest.h"

#include "player.hpp"
#include "bobs_tavern.hpp"
// #include "../cards/bgs/BgCardFactory.hpp"
// #include "../cards/bgs/BgCards.hpp"

TEST(BobsTav, AllowsPlayerToSeeMinions) {
    auto player = std::make_unique<Player>("Test");
    auto tav = BobsTavern();
    auto avail_minions = tav.get_current_minions(player.get());
    // TODO: Fix
    // EXPECT_EQ(avail_minions.size(), (unsigned)3);
}
