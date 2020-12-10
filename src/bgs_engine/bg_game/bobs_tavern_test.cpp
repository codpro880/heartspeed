#include "../test/googletest/include/gtest/gtest.h"

#include "player.hpp"
#include "bobs_tavern.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
// #include "../cards/bgs/BgCards.hpp"

//  expect_all_of_tier(std::vector<std::string>> minions, int tier) {
// }

TEST(BobsTav, AllowsPlayerToSeeMinionsAtStart) {
    auto player = std::make_unique<Player>("Test");
    // auto tav = player.get_tavern();
    auto avail_minions = player->get_tavern_minions();
    EXPECT_EQ(avail_minions.size(), (unsigned)3);
    BgCardFactory f;
    for (auto minion_name : avail_minions) {
	EXPECT_EQ(f.get_card(minion_name)->get_tavern_tier(), 1);
    }
}

TEST(BobsTav, AllowsPlayerToRefreshRepeatedly) {
    auto player = std::make_unique<Player>("Test");
    player->set_gold(2);
    
    // Can refresh and manually get
    player->refresh_tavern_minions();
    EXPECT_EQ(player->get_gold(), 1);
    auto first_refresh_minions = player->get_tavern_minions();
    BgCardFactory f;
    for (auto minion_name : first_refresh_minions) {
	EXPECT_EQ(f.get_card(minion_name)->get_tavern_tier(), 1);
    }

    // Can get minion shop from refresh directly
    auto second_refresh_minions = player->refresh_tavern_minions();
    EXPECT_EQ(player->get_gold(), 0);

    // RNG Seed set to 0 by default, this should always pass
    EXPECT_TRUE(first_refresh_minions != second_refresh_minions);
}

TEST(BobsTav, WontRefreshWithZeroGold) {
    auto player = std::make_unique<Player>("Test");
    player->set_gold(0);

    auto avail_minions = player->get_tavern_minions();    
    
    // Should be a no-op
    auto refreshed_minions = player->refresh_tavern_minions();

    EXPECT_EQ(avail_minions, refreshed_minions);
}

TEST(BobsTav, AllowsPlayerToBuyMinionInShop) {
    // Note: Players start w/ 3 gold
    auto player = std::make_unique<Player>("Test");
    auto avail_minions = player->get_tavern_minions();
    auto minion = avail_minions[0];
    EXPECT_EQ(player->get_gold(), 3);
    player->buy_minion(minion);
    EXPECT_EQ(player->get_gold(), 0);
    EXPECT_EQ(player->get_hand().size(), 1);
    auto player_cards_in_hand = player->get_hand().get_cards();
    EXPECT_EQ(player_cards_in_hand[0]->get_name(), minion);
}

TEST(BobsTav, AllowsPlayerToBuyMinionByPosition) { // Probably useful for RL
    // Note: Players start w/ 3 gold
    auto player = std::make_unique<Player>("Test");
    auto avail_minions = player->get_tavern_minions();
    auto minion = avail_minions[0];
    player->buy_minion(0);
    EXPECT_EQ(player->get_gold(), 0);
    EXPECT_EQ(player->get_hand().size(), 1);
    auto player_cards_in_hand = player->get_hand().get_cards();
    EXPECT_EQ(player_cards_in_hand[0]->get_name(), minion);
}

TEST(BobsTav, ShowsPlayerHigherTierMinions) {
    auto player = std::make_unique<Player>("Test");
    player->set_tavern_tier(3);
    player->refresh_tavern_minions();
    auto avail_minions = player->get_tavern_minions();
    EXPECT_EQ(avail_minions.size(), (unsigned)5);
    
    // Expect that at least one of them is not t1 and none over t3
    bool all_tier1 = true;
    bool any_greater_than_t3 = false;
    bool any_golden = false;
    BgCardFactory f;
    for (auto minion_name : avail_minions) {
	auto card = f.get_card(minion_name);
	auto tav_tier = card->get_tavern_tier();
	if (tav_tier != 1) {
	    all_tier1 = false;
	}
	if (tav_tier > 3) {
	    any_greater_than_t3 = true;
	}
	if (card->is_golden()) {
	    any_golden = true;
	}
    }
    EXPECT_TRUE(!all_tier1);
    EXPECT_TRUE(!any_greater_than_t3);
    EXPECT_TRUE(!any_golden);
}

TEST(BobsTav, AllowsPlayerToSellBack) {
    // Note: Players start w/ 3 gold
    auto tidecaller = BgCardFactory().get_card("Murloc Tidecaller");
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards { tidecaller };
    auto in_hand = Hand(hand_cards);    
    auto player = std::make_unique<Player>(in_hand, "Test");
    player->play_card(0, 0);
    player->sell_minion(0);
    EXPECT_EQ(player->get_gold(), 4);
    EXPECT_EQ(player->get_hand().size(), 0);
}

TEST(BobsTav, TavernUpMechanismWorks) {
    // Note: Players start w/ 3 gold. In this test, however, we give them
    //       enough to test everything successfully with.
    auto player = std::make_unique<Player>("Test");
    player->set_gold(10);
    // NOTE: Players start out at tavern tier 1
    EXPECT_EQ(player->get_tavern_tier(), 1);
    auto tier_1to2 = player->tavern_up();
    player->set_gold(10);
    EXPECT_EQ(player->get_tavern_tier(), 2);
    EXPECT_TRUE(tier_1to2);
    auto tier_2to3 = player->tavern_up(); // Tier 3
    EXPECT_TRUE(tier_2to3);
    player->set_gold(10);
    auto tier_3to4 = player->tavern_up(); // Tier 4
    EXPECT_TRUE(tier_3to4);
    player->set_gold(10);
    auto tier_4to5 = player->tavern_up(); // Tier 5
    EXPECT_TRUE(tier_4to5);
    player->set_gold(10);
    auto tier_5to6 = player->tavern_up(); // Tier 6
    EXPECT_TRUE(tier_5to6);
    player->set_gold(10);
    auto tier_oops = player->tavern_up(); // Oops... Max tier is 6.
    // Check upper bound
    EXPECT_EQ(player->get_tavern_tier(), 6);
    EXPECT_TRUE(!tier_oops);
}

TEST(BobsTavern, TavernUpMechanismPlayerNotEnoughGold) {
    // Note: Players start w/ 3 gold
    auto player = std::make_unique<Player>("Test");
    // Ensure player does not have enough to tavern up
    auto tier_1to2 = player->tavern_up();
    EXPECT_EQ(player->get_tavern_tier(), 1);
    EXPECT_TRUE(!tier_1to2);
    // Now give them just enough gold to upgrade and try again.
    player->add_gold(2);
    auto tier_1to2_again = player->tavern_up();
    EXPECT_EQ(player->get_tavern_tier(), 2);
    EXPECT_TRUE(tier_1to2_again);
}

TEST(BobsTav, GivesPlayerWaterDropletCardInHandWhenSellementalSold) {
    BgCardFactory f;
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Sellemental"),
	 f.get_card("Sellemental (Golden)")	 
	};
    auto in_hand = Hand(hand_cards);
    auto player = std::make_unique<Player>(in_hand, "Test");
    player->play_card(0, 0);
    player->play_card(0, 1);
    EXPECT_EQ(player->get_hand().size(), 0);
    EXPECT_EQ(player->get_board()->size(), 2);
    player->sell_minion(0);
    player->sell_minion(0);
    // Should have non gold and gold water droplet in hand
    EXPECT_EQ(player->get_gold(), 5); // Note: Players start w/ 3 gold
    EXPECT_EQ(player->get_hand().size(), 2);
    EXPECT_EQ(player->get_hand().get_cards()[0]->get_name(), "Water Droplet");
    EXPECT_EQ(player->get_hand().get_cards()[1]->get_name(), "Water Droplet (Golden)");
}

TEST(BobsTav, GivesPlayerALotMoreThanNormalGoldWhenFreedealingGamblerSold) {
    BgCardFactory f;
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Freedealing Gambler"),
	 f.get_card("Freedealing Gambler (Golden)")	 
	};
    auto in_hand = Hand(hand_cards);
    auto player = std::make_unique<Player>(in_hand, "Test");
    player->set_gold(0); // Set to 0 so we can test appropriately (10 gold cap)
    player->play_card(0, 0);
    player->play_card(0, 1);
    EXPECT_EQ(player->get_hand().size(), 0);
    EXPECT_EQ(player->get_board()->size(), 2);
    player->sell_minion(0);
    player->sell_minion(0);
    // 3 from non golden, 6 from golden
    EXPECT_EQ(player->get_gold(), 9);
}
