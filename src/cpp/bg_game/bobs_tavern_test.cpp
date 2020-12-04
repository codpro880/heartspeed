#include "../test/googletest/include/gtest/gtest.h"

#include "player.hpp"
#include "bobs_tavern.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
// #include "../cards/bgs/BgCards.hpp"

//  expect_all_of_tier(std::vector<std::string>> minions, int tier) {
// }

TEST(BobsTav, AllowsPlayerToSeeMinionsAtStart) {
    auto player = std::make_unique<Player>("Test");
    auto tav = BobsTavern();
    auto avail_minions = tav.get_current_minions(player.get());    
    EXPECT_EQ(avail_minions.size(), (unsigned)3);
    BgCardFactory f;
    for (auto minion_name : avail_minions) {
	EXPECT_EQ(f.get_card(minion_name)->get_tavern_tier(), 1);
    }
}

TEST(BobsTav, AllowsPlayerToRefreshRepeatedly) {
    auto player = std::make_unique<Player>("Test");
    player->set_gold(2);
    auto tav = BobsTavern();
    
    // Can refresh and manually get
    tav.refresh_minions(player.get());
    EXPECT_EQ(player->get_gold(), 1);
    auto first_refresh_minions = tav.get_current_minions(player.get());
    BgCardFactory f;
    for (auto minion_name : first_refresh_minions) {
	EXPECT_EQ(f.get_card(minion_name)->get_tavern_tier(), 1);
    }

    // Can get minion shop from refresh directly
    auto second_refresh_minions = tav.refresh_minions(player.get());
    EXPECT_EQ(player->get_gold(), 0);

    // RNG Seed set to 0 by default, this should always pass
    EXPECT_TRUE(first_refresh_minions != second_refresh_minions);
}

TEST(BobsTav, WontRefreshWithZeroGold) {
    auto player = std::make_unique<Player>("Test");
    player->set_gold(0);
    auto tav = BobsTavern();

    auto avail_minions = tav.get_current_minions(player.get());    
    
    // Should be a no-op
    auto refreshed_minions = tav.refresh_minions(player.get());

    EXPECT_EQ(avail_minions, refreshed_minions);
}
