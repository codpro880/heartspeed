#include "../test/googletest/include/gtest/gtest.h"

#include "player.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
#include "../cards/bgs/BgCards.hpp"

TEST(Player, CanPlayCardFromHandBasic) {
    auto tidecaller = BgCardFactory().get_card("Murloc Tidecaller");
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards { tidecaller };
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 1);
    EXPECT_EQ(hand.get_cards()[0]->get_name(), "Murloc Tidecaller");
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(tidecaller, 0);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 1);
}

TEST(Player, CanAndUnfreezeFreezeTavern) {
    auto player = Player("Test");

    player.start_turn();
    auto tavern_minions = player.get_tavern_minions();
    player.end_turn();

    // Since we weren't frozen last turn, minions should be different
    player.start_turn();
    auto tavern_minions_next_turn = player.get_tavern_minions();
    EXPECT_NE(tavern_minions, tavern_minions_next_turn);
    player.freeze_tavern(); // Freeze it for following turn
    player.end_turn();

    // Test we get the same as last turn (frozen)
    player.start_turn();
    auto tavern_minions_again = player.get_tavern_minions();
    EXPECT_EQ(tavern_minions_next_turn, tavern_minions_again);
    player.end_turn();

    // Tavern should automatically unfreeze at start of turn
    player.start_turn();
    auto tavern_minions_last_time = player.get_tavern_minions();
    EXPECT_NE(tavern_minions_again, tavern_minions_last_time);
    player.end_turn();    
}

TEST(Player, AlleycatBattlecryBasic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Alleycat"),
	 f.get_card("Alleycat (Golden)"),
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 2);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Alleycat");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Alleycat (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Tabbycat (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Tabbycat");
}

TEST(Player, AmalgadonLoneBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Brann Bronzebeard"),
	 f.get_card("Baron Rivendare"),
	 f.get_card("Amalgadon")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Amalgadon");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_adapt_count(), 0);
}

TEST(Player, AmalgadonMultiBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Murloc Scout"),
	 f.get_card("Deck Swabbie"),
	 f.get_card("Baron Rivendare"),
	 f.get_card("Amalgadon"),
	 f.get_card("Amalgadon (Golden)"),
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);    
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Amalgadon");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_adapt_count(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Amalgadon (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_adapt_count(), 3*2);
}


TEST(Player, AnnihilanBattlemasterBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Annihilan Battlemaster"),
	 f.get_card("Annihilan Battlemaster (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    player.take_damage(30);

    player.play_card(0, 0);
    player.play_card(0, 1);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Annihilan Battlemaster");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 1 + 30);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Annihilan Battlemaster (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 2 + 30 + 30);
} 

TEST(Player, ArcaneAssistantBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Wildfire Elemental"),
	 f.get_card("Harvest Golem"),
	 f.get_card("Arcane Assistant (Golden)"),
	 f.get_card("Arcane Assistant"),
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Wildfire Elemental");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Harvest Golem");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Arcane Assistant (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 7);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 7);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Arcane Assistant");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 3);
}

TEST(Player, BloodsailCannoneerBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Goldgrubber"),
	 f.get_card("Bloodsail Cannoneer (Golden)"),
	 f.get_card("Bloodsail Cannoneer")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Goldgrubber");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 2+3+6);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Bloodsail Cannoneer (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 8+3);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Bloodsail Cannoneer");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 3);
}

TEST(Player, BrannMakesBattlecriesGoOffTwiceAndGoldenThrice) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Murloc Scout"),
	 f.get_card("Brann Bronzebeard"),
	 f.get_card("Rockpool Hunter (Golden)"),
	 f.get_card("Brann Bronzebeard (Golden)"),
	 f.get_card("Felfin Navigator")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 5);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    player.play_card(0, 3);
    player.play_card(0, 4);
    EXPECT_EQ(player.get_board()->size(), 5);
    // 2*2 for golden rockpool and normal brann, 1*3 for felfin and golden brann
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Murloc Scout");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 1 + 2*2 + 1*3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 1 + 2*2 + 1*3);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Brann Bronzebeard");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Rockpool Hunter (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 4 + 1*3);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 6 + 1*3);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Brann Bronzebeard (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Felfin Navigator");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_health(), 4);
}

TEST(Player, CobaltScalebaneEndTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Cobalt Scalebane"),
	 f.get_card("Cobalt Scalebane (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    // Play out a quick turn
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.end_turn();

    // Scalebanes buff each other
    auto scalebane = player.get_board()->get_cards()[0];
    auto scalebane_golden = player.get_board()->get_cards()[1];
    EXPECT_EQ(scalebane->get_attack(), 5+6); // Normally it's 5, but buffed +6
    EXPECT_EQ(scalebane_golden->get_attack(), 10+3); // Normally it's 10, but buffed +3
}

TEST(Player, CrowdFavoriteReactsToBattlecryCards) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Crowd Favorite"),
	 f.get_card("Crowd Favorite (Golden)"),
	 f.get_card("Crystalweaver"),
	 f.get_card("Coldlight Seer")

	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Crowd Favorite");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Crowd Favorite (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 12);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 12);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Crystalweaver");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Coldlight Seer");
}

TEST(Player, CrystalweaverBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Nathrezim Overseer"),
	 f.get_card("Siegebreaker"),
	 f.get_card("Crystalweaver (Golden)"),
	 f.get_card("Coldlight Seer"),
	 f.get_card("Crystalweaver")

	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Nathrezim Overseer");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Siegebreaker");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 11);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Crystalweaver (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Coldlight Seer");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 3);    
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Crystalweaver");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_health(), 4);
}

TEST(Player, ColdlightBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Murloc Scout"),
	 f.get_card("Murloc Scout"),
	 f.get_card("Crystalweaver"),
	 f.get_card("Coldlight Seer (Golden)"),
	 f.get_card("Coldlight Seer")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Murloc Scout");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 1);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 7);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Murloc Scout");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 1);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 7);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Crystalweaver");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Coldlight Seer (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Coldlight Seer");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_health(), 3);
}

TEST(Player, DefenderOfArgusBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Mama Bear"),
	 f.get_card("Mechano-Egg"),
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Defender of Argus"),
	 f.get_card("Defender of Argus (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 5);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2); 
    player.play_card(0, 1); // Give mama and egg taunt
    player.play_card(0, 4); // Give tidecaller taunt
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Mama Bear");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 5);
    EXPECT_TRUE(player.get_board()->get_cards()[0]->has_taunt());
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Defender of Argus");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 3);
    EXPECT_FALSE(player.get_board()->get_cards()[1]->has_taunt());
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Mechano-Egg");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 1);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 6);
    EXPECT_TRUE(player.get_board()->get_cards()[2]->has_taunt());
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Murloc Tidecaller");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 4);
    EXPECT_TRUE(player.get_board()->get_cards()[3]->has_taunt());
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Defender of Argus (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_health(), 6);
    EXPECT_FALSE(player.get_board()->get_cards()[4]->has_taunt());
}

TEST(Player, FelfinNavigatorBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Coldlight Seer"),
	 f.get_card("Mama Bear"),
	 f.get_card("Felfin Navigator"),
	 f.get_card("Felfin Navigator (Golden)"),
	 f.get_card("Coldlight Seer"),
	 f.get_card("Murloc Tidehunter")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    player.play_card(0, 5);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Coldlight Seer");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Mama Bear");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Felfin Navigator");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Felfin Navigator (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Coldlight Seer");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_health(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[5]->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(player.get_board()->get_cards()[5]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[5]->get_health(), 1);
    EXPECT_EQ(player.get_board()->get_cards()[6]->get_name(), "Murloc Scout");
    EXPECT_EQ(player.get_board()->get_cards()[6]->get_attack(), 1);
    EXPECT_EQ(player.get_board()->get_cards()[6]->get_health(), 1);    
}

TEST(Player, FloatingWatcherRespondsToDamageBattlecries) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Floating Watcher"),
	 f.get_card("Floating Watcher (Golden)"),
	 f.get_card("Vulgar Homunculus")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Floating Watcher");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Floating Watcher (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 12);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 12);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Vulgar Homunculus");
}

TEST(Player, GoldgrubberEndTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Goldgrubber"),
	 f.get_card("Goldgrubber (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    // Play out a quick turn
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.end_turn();

    // Scalebanes buff each other
    auto goldgrub = player.get_board()->get_cards()[0];
    auto goldgrub_golden = player.get_board()->get_cards()[1];
    EXPECT_EQ(goldgrub->get_attack(), 4); // Normally it's 2/2, but buffed +2/+2
    EXPECT_EQ(goldgrub->get_health(), 4);
    EXPECT_EQ(goldgrub_golden->get_attack(), 8); // Normally it's 4/4, but buffed +4/+4
    EXPECT_EQ(goldgrub_golden->get_health(), 8);
}


TEST(Player, HoundmasterBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Mama Bear"),
	 f.get_card("Houndmaster (Golden)"),
	 f.get_card("Houndmaster")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 0, 1);
    player.play_card(0, 0, 2);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Mama Bear");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 10);
    EXPECT_TRUE(player.get_board()->get_cards()[0]->has_taunt());
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Houndmaster (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Houndmaster");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 3);
}

TEST(Player, IronSenseiEndTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Iron Sensei"),
	 f.get_card("Iron Sensei (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.end_turn();

    // Assert sensei's buff each other
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Iron Sensei");
    // Originally a 2/2, now should have +4/+4
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Iron Sensei (Golden)");
    // Originally a 4/4, now should have +2/+2
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 6);
}


TEST(Player, KalecgosBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Kalecgos, Arcane Aspect"),
	 f.get_card("Kalecgos, Arcane Aspect (Golden)"),
	 f.get_card("Cobalt Scalebane"),
	 f.get_card("Red Whelp"),
	 f.get_card("Brann Bronzebeard"),
	 f.get_card("Coldlight Seer"),
	 f.get_card("Twilight Emissary")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    player.play_card(0, 5);
    player.play_card(0, 0, 6);
    // Assert dragons got +6/+6
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Kalecgos, Arcane Aspect");    
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 4+6+2*2);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 12+6+2*2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Kalecgos, Arcane Aspect (Golden)");    
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 8+6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 24+6);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Cobalt Scalebane");    
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 5+6);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 5+6);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Red Whelp");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 1+6);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 2+6);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Brann Bronzebeard");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_health(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[5]->get_name(), "Coldlight Seer");
    EXPECT_EQ(player.get_board()->get_cards()[5]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[5]->get_health(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[6]->get_name(), "Twilight Emissary");
    EXPECT_EQ(player.get_board()->get_cards()[6]->get_attack(), 4+3);
    EXPECT_EQ(player.get_board()->get_cards()[6]->get_health(), 4+3);
}

TEST(Player, KingBagurgleBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Murloc Scout"),
	 f.get_card("King Bagurgle (Golden)"),
	 f.get_card("King Bagurgle")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Murloc Scout");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 1 + 2 + 4);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 1 + 2 + 4);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "King Bagurgle (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 12 + 2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 6 + 2);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "King Bagurgle");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 3);
} 

TEST(Player, LieutenantGarrBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Lieutenant Garr (Golden)"),
	 f.get_card("Lieutenant Garr"),
	 f.get_card("Cobalt Scalebane"),
	 f.get_card("Sellemental"),
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Lieutenant Garr (Golden)");    
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 2 + 2*2 + 2*3);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Lieutenant Garr");    
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 1+3);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Cobalt Scalebane");    
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Sellemental");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 2);
}

TEST(Player, LightfangMix) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Foe Reaper 4000"),
	 f.get_card("Harvest Golem"),
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Deck Swabbie"),
	 f.get_card("Imp Gang Boss"),
	 f.get_card("Lightfang Enforcer"),
	 f.get_card("Lightfang Enforcer (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    int total_hand_attack = 0;
    int total_hand_health = 0;
    for (auto card : in_hand.get_cards()) {
	total_hand_attack += card->get_attack();
	total_hand_health += card->get_health();
    }
    
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    player.play_card(0, 5);
    player.play_card(0, 6);
    player.end_turn();
    
    int total_board_attack = 0;
    int total_board_health = 0;
    for (auto card : player.get_board()->get_cards()) {
	total_board_attack += card->get_attack();
	total_board_health += card->get_health();
    }

    // Should see +8/+8 and +16/+16 (golden) total stat change, 4 different types
    EXPECT_EQ(total_hand_attack + 8 + 16, total_board_attack);
    EXPECT_EQ(total_hand_health + 8 + 16, total_board_health);
}

TEST(Player, LilRagBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Lil' Rag"),
	 f.get_card("Lil' Rag (Golden)"), // +6/+6 total
	 f.get_card("Sellemental"), // +3/+3 total
	 f.get_card("Cave Hydra"), // does nothing
	 f.get_card("Lieutenant Garr") // +18/+18 total
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    int hand_total_attack = 0;
    int hand_total_health = 0;
    for (auto c : player.get_hand().get_cards()) {
	hand_total_attack += c->get_attack();
	hand_total_health += c->get_health();
    }

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    
    // Assert cave hydra the same
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Cave Hydra");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 4);

    // Assert stats changed by 6+3+18
    int board_total_attack = 0;
    int board_total_health = 0;
    for (auto c : player.get_board()->get_cards()) {
	board_total_attack += c->get_attack();
	board_total_health += c->get_health();
    }
    const int expected_buff = 6 + 3 + 18;
    EXPECT_EQ(board_total_attack, hand_total_attack + expected_buff);
    EXPECT_EQ(board_total_health, hand_total_health + expected_buff);
}

TEST(Player, MajordomoExecutusEndTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Crackling Cyclone"),
	 f.get_card("Crackling Cyclone"),
	 f.get_card("Majordomo Executus"),
	 f.get_card("Majordomo Executus (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    // Play out a quick turn
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.end_turn();

    // Assert left most cracking gets buffed
    auto crackling = player.get_board()->get_cards()[0];
     // Normally it's 4/1, but buffed +3/+3 from majordomo and +6/+6 from golden 
    EXPECT_EQ(crackling->get_attack(), 4 + 3 + 6);
    EXPECT_EQ(crackling->get_health(), 1 + 3 + 6);
    EXPECT_TRUE(crackling->has_divine_shield());
    EXPECT_TRUE(crackling->has_windfury());

    // Play another turn...
    player.start_turn();
    player.end_turn();

    // Make sure crackling got buffed again, but no elems played
    crackling = player.get_board()->get_cards()[0];
     // Buffed +1/+1 from majordomo and +2/+2 from gold version
    EXPECT_EQ(crackling->get_attack(), 4 + 3 + 6 + 1 + 2);
    EXPECT_EQ(crackling->get_health(), 1 + 3 + 6 + 1 + 2);
    EXPECT_TRUE(crackling->has_divine_shield());
    EXPECT_TRUE(crackling->has_windfury());
}


// Some extra tests around the mug since it's sort of complicated.
// This level of testing not necessary for the Jug (since logic will be reused)
TEST(Player, MenagerieMugBattlecryEmpty) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Menagerie Mug"),
	 f.get_card("Menagerie Mug (Golden)"),
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 2);
    EXPECT_EQ(player.get_board()->size(), 0);

    // Should be nothing buffed
    player.play_card(0, 0);
    player.play_card(0, 1);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Menagerie Mug");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Menagerie Mug (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 4);
}

TEST(Player, MenagerieMugBattlecryOne) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Foe Reaper 4000"),
	 f.get_card("Menagerie Mug"),
	 f.get_card("Menagerie Mug (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    int total_hand_attack = 0;
    int total_hand_health = 0;
    for (auto card : in_hand.get_cards()) {
	total_hand_attack += card->get_attack();
	total_hand_health += card->get_health();
    }

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    int total_board_attack = 0;
    int total_board_health = 0;
    for (auto card : player.get_board()->get_cards()) {
	total_board_attack += card->get_attack();
	total_board_health += card->get_health();
    }

    // Should see +3/+3 total stat change
    EXPECT_EQ(total_hand_attack + 3, total_board_attack);
    EXPECT_EQ(total_hand_health + 3, total_board_health);
}

TEST(Player, MenagerieMugBattlecryThreeWithTwoSameType) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Foe Reaper 4000"),
	 f.get_card("Harvest Golem"),
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Menagerie Mug"),
	 f.get_card("Menagerie Mug (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    int total_hand_attack = 0;
    int total_hand_health = 0;
    for (auto card : in_hand.get_cards()) {
	total_hand_attack += card->get_attack();
	total_hand_health += card->get_health();
    }

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    int total_board_attack = 0;
    int total_board_health = 0;
    for (auto card : player.get_board()->get_cards()) {
	total_board_attack += card->get_attack();
	total_board_health += card->get_health();
    }

    // Should see +6/+6 total stat change
    EXPECT_EQ(total_hand_attack + 6, total_board_attack);
    EXPECT_EQ(total_hand_health + 6, total_board_health);
}

TEST(Player, MenagerieMugBattlecryMix) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Foe Reaper 4000"),
	 f.get_card("Harvest Golem"),
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Deck Swabbie"),
	 f.get_card("Imp Gang Boss"),
	 f.get_card("Menagerie Mug"),
	 f.get_card("Menagerie Mug (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    int total_hand_attack = 0;
    int total_hand_health = 0;
    for (auto card : in_hand.get_cards()) {
	total_hand_attack += card->get_attack();
	total_hand_health += card->get_health();
    }

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    player.play_card(0, 5);
    player.play_card(0, 6);
    int total_board_attack = 0;
    int total_board_health = 0;
    for (auto card : player.get_board()->get_cards()) {
	total_board_attack += card->get_attack();
	total_board_health += card->get_health();
    }

    // Should see +9/+9 total stat change
    EXPECT_EQ(total_hand_attack + 9, total_board_attack);
    EXPECT_EQ(total_hand_health + 9, total_board_health);
}

TEST(Player, MicroMachineStartTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Micro Machine"),
	 f.get_card("Micro Machine (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    // Play out a quick turn
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.end_turn();

    // Start next turn
    player.start_turn();

    // Micro Mummies buff each other
    auto micro_machine = player.get_board()->get_cards()[0];
    auto micro_machine_golden = player.get_board()->get_cards()[1];
    EXPECT_EQ(micro_machine->get_attack(), 2); // Normally it's 1, but buffed +1
    EXPECT_EQ(micro_machine_golden->get_attack(), 4); // Normally it's 2, but buffed +2
}


TEST(Player, MicroMummyEndTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Micro Mummy"),
	 f.get_card("Micro Mummy (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    // Play out a quick turn
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.end_turn();

    // Micro Mummies buff each other
    auto micro_mummy = player.get_board()->get_cards()[0];
    auto micro_mummy_golden = player.get_board()->get_cards()[1];
    EXPECT_EQ(micro_mummy->get_attack(), 3); // Normally it's 1, but buffed +2
    EXPECT_EQ(micro_mummy_golden->get_attack(), 3); // Normally it's 2, but buffed +1
    EXPECT_TRUE(micro_mummy->has_reborn());
    EXPECT_TRUE(micro_mummy_golden->has_reborn());
}

TEST(Player, MetaltoothLeaperBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Harvest Golem"),
	 f.get_card("Metaltooth Leaper"),
	 f.get_card("Metaltooth Leaper (Golden)"),
	 f.get_card("Kindly Grandmother"),
	 f.get_card("Kaboom Bot")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 5);
    EXPECT_EQ(player.get_board()->size(), 0);

    // Play cards in hand order, only Harvest Golem gets buffed
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Harvest Golem");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Metaltooth Leaper");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 7);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Metaltooth Leaper (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Kindly Grandmother");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 1);
    // Kaboom bot was played after leapers
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Kaboom Bot");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_attack(), 2);
}


// TOOD: Impl bobs tav
// TEST(Player, DeckSwabbieBattlecry) {
//     auto f = BgCardFactory();
//     std::vector<std::shared_ptr<BgBaseCard> > hand_cards
// 	{
// 	 f.get_card("Deck Swabbie"),
// 	 f.get_card("Deck Swabbie (Golden)")
// 	};
//     auto in_hand = Hand(hand_cards);
//     auto player = Player(in_hand, "Test");
//     EXPECT_EQ(player.get_board()->size(), 0);
    
//     player.play_card(0, 0);
//     EXPECT_EQ(player.get_board()->size(), 1);
//     // Turn 1 tav up costs 5.
//     // Golden swabbie takes away 2, normal swabbie takes away 1
//     EXPECT_EQ(player->get_bobs_tavern().cost() == 5-2-1);
// }

TEST(Player, MoltenRockReactsToElementalCards) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Molten Rock"),
	 f.get_card("Molten Rock (Golden)"),
	 f.get_card("Sellemental"),
	 f.get_card("Coldlight Seer")

	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    
    auto original_health = f.get_card("Molten Rock")->get_health();
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Molten Rock");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), original_health + 2); // Gets +2 (from golden and selle)
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Molten Rock (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 2 * original_health + 2); // Gets +2 (from selle)
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Sellemental");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Coldlight Seer");
}

TEST(Player, MurlocTidecallerAfterMurlocSummoned) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Murloc Tidecaller (Golden)"),
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Murloc Tidecaller")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 3);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Murloc Tidecaller (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Murloc Tidecaller");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Murloc Tidecaller");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 1);
}

TEST(Player, MurlocTidehunterBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 3);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 0);
    player.play_card(0, 3);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Murloc Scout");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Murloc Tidecaller");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Murloc Tidehunter (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Murloc Scout (Golden)");
}

TEST(Player, MythraxTheUnravelerEndOfTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Foe Reaper 4000"),
	 f.get_card("Harvest Golem"),
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Deck Swabbie"),
	 f.get_card("Imp Gang Boss"),
	 f.get_card("Mythrax the Unraveler"),
	 f.get_card("Mythrax the Unraveler (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    player.play_card(0, 5);
    player.play_card(0, 6);
    player.end_turn();
    
    auto mythrax = player.get_board()->get_cards()[5];
    auto mythrax_golden = player.get_board()->get_cards()[6];    

    // 4 different types, so non gold gets a 4/8 buff, golden gets double that
    EXPECT_EQ(mythrax->get_attack(), 4 + 4); // Starts as a 4/4
    EXPECT_EQ(mythrax->get_health(), 4 + 8);
    EXPECT_EQ(mythrax_golden->get_attack(), 8 + 8); // Starts as a 8/8
    EXPECT_EQ(mythrax_golden->get_health(), 8 + 16);
}

TEST(Player, PartyElementalReactsToElementalCards) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Party Elemental"),
	 f.get_card("Party Elemental (Golden)"),
	 f.get_card("Sellemental"),
	 f.get_card("Coldlight Seer"),
	 f.get_card("Molten Rock")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");    

    int total_hand_attack = 0;
    int total_hand_health = 0;
    for (auto card : in_hand.get_cards()) {
	total_hand_attack += card->get_attack();
	total_hand_health += card->get_health();
    }

    player.play_card(0, 0); // Nothing
    player.play_card(0, 1); // +1/+1 buff
    player.play_card(0, 2); // +3/+3 buff
    player.play_card(0, 3); // Nothing
    player.play_card(0, 4); // +3/+3 buff
                            // +7/+7 buff total

    // Assert total of +7/+7 buff
    int total_board_attack = 0;
    int total_board_health = 0;
    for (auto card : player.get_board()->get_cards()) {
	total_board_attack += card->get_attack();
	total_board_health += card->get_health();
    }
    EXPECT_EQ(total_hand_attack + 7, total_board_attack);
    EXPECT_EQ(total_hand_health + 7, total_board_health);
    
    // Coldlight should not have gotten any buffs
    auto coldlight = player.get_board()->get_cards()[3];
    EXPECT_EQ(coldlight->get_name(), "Coldlight Seer");
    EXPECT_EQ(coldlight->get_attack(), 2);
    EXPECT_EQ(coldlight->get_health(), 3);
}

TEST(Player, RabidSauroliskReactsToDeathrattleCards) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Rabid Saurolisk"),
	 f.get_card("Rabid Saurolisk (Golden)"),
	 f.get_card("Harvest Golem"), // drattle
	 f.get_card("Foe Reaper 4000"),
	 f.get_card("Molten Rock"),
	 f.get_card("Fiendish Servant"), // drattle
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");    

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    player.play_card(0, 5);

    // Nongold should have +2/+2, gold should have +4/+4
    auto sauro = player.get_board()->get_cards()[0];
    auto sauro_gold = player.get_board()->get_cards()[1];
    auto original_card = f.get_card("Rabid Saurolisk");
    auto original_attack = original_card->get_attack();
    auto original_health = original_card->get_health();
    EXPECT_EQ(sauro->get_attack(), original_attack + 2);
    EXPECT_EQ(sauro->get_health(), original_health + 2);
    EXPECT_EQ(sauro_gold->get_attack(), 2 * original_attack + 4);
    EXPECT_EQ(sauro_gold->get_health(), 2 * original_health + 4);   
}

TEST(Player, RazorgoreEndOfTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Red Whelp"),
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Bronze Warden"),
	 f.get_card("Imp Mama"),
	 f.get_card("Razorgore, the Untamed"),
	 f.get_card("Razorgore, the Untamed (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    player.play_card(0, 5);
    player.end_turn();
    
    // Razorgores get buffed +4/+4 and (golden) +8/+8
    auto razorgore = player.get_board()->get_cards()[4];
    auto razorgore_golden = player.get_board()->get_cards()[5];
    auto razorgore_base_attack = f.get_card("Razorgore, the Untamed")->get_attack();
    auto razorgore_base_health = f.get_card("Razorgore, the Untamed")->get_health();
    EXPECT_EQ(razorgore->get_attack(), razorgore_base_attack + 4);
    EXPECT_EQ(razorgore->get_health(), razorgore_base_health + 4);
    EXPECT_EQ(razorgore_golden->get_attack(), 2 * razorgore_base_attack + 8);
    EXPECT_EQ(razorgore_golden->get_health(), 2 * razorgore_base_health + 8);
}

TEST(Player, RefreshingAnomalyWorks) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Refreshing Anomaly"),
	 f.get_card("Refreshing Anomaly (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    // Play out a quick turn
    player.start_turn();
    // Non golden version allows one free refresh
    player.play_card(0, 0);
    EXPECT_EQ(player.get_gold(), 3);
    player.refresh_tavern_minions();
    EXPECT_EQ(player.get_gold(), 3);
    player.refresh_tavern_minions();
    EXPECT_EQ(player.get_gold(), 2);
    // Gold version allows two free refreshes
    player.play_card(0, 1);
    EXPECT_EQ(player.get_gold(), 2);
    player.refresh_tavern_minions();
    EXPECT_EQ(player.get_gold(), 2);
    player.refresh_tavern_minions();
    EXPECT_EQ(player.get_gold(), 2);
    player.refresh_tavern_minions();
    EXPECT_EQ(player.get_gold(), 1);
    player.end_turn();
}


TEST(Player, RockpoolHunterTargettedBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Rockpool Hunter"),
	 f.get_card("Rockpool Hunter (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 3);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Murloc Tidecaller");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Rockpool Hunter");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Rockpool Hunter (Golden)");
}

TEST(Player, SaltyLooterAfterPirateSummoned) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Salty Looter (Golden)"),
	 f.get_card("Salty Looter"),
	 f.get_card("Deck Swabbie")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 3);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Salty Looter (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 12);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 12);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Salty Looter");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Deck Swabbie");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 2);
}

TEST(Player, ScrewjankClunkerBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Foe Reaper 4000"),
	 f.get_card("Screwjank Clunker (Golden)"),
	 f.get_card("Screwjank Clunker")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 0, 1);
    player.play_card(0, 0, 2);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Foe Reaper 4000");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 12);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 15);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Screwjank Clunker (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Screwjank Clunker");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 5);
}

TEST(Player, SouthseaStrongarmBattlecry) {
    auto f = BgCardFactory();
    auto player = Player("Test");
    // Technically can't buy golden minions, this is just a test
    std::vector<std::string> minions({"Salty Looter", "Southsea Strongarm", "Southsea Strongarm (Golden)"});
    player.set_tavern_minions(minions);

    player.buy_minion(0);
    player.play_card(0, 0);
    player.buy_minion(0);
    player.play_card(0, 0, 1);
    player.buy_minion(0);
    player.play_card(0, 0, 2);

    // Salty gets +2/+2 from first southsea, then +6/+6 from second southsea
    // Also gets +2/+2 b/c each southsea is a pirate (salty buffs itself)
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Salty Looter");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 4+2+6+2);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 4+2+6+2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Southsea Strongarm");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Southsea Strongarm (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 8);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 6);
}

TEST(Player, StasisElementalBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Stasis Elemental"),
	 f.get_card("Stasis Elemental (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.end_turn();

    // Should see 3 + 1 + 2 total minions in the tavern.
    // 3 normally, plus one frozen from statis plus two from golden version
    player.start_turn();
    auto tav_minions = player.get_tavern_minions();
    EXPECT_EQ(tav_minions.size(), (unsigned)6);
    // Last three should be elementals
    for (size_t i = tav_minions.size() - 3; i < tav_minions.size(); i++) {
	EXPECT_EQ(f.get_card(tav_minions[i])->get_race(), "ELEMENTAL");
    }
    player.end_turn();
}

TEST(Player, StasisElementalAtHigherTavernTierBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Stasis Elemental"),
	 f.get_card("Stasis Elemental (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    player.set_tavern_tier(5);
    player.start_turn();
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.end_turn();

    // Should see 7 total minions in the tavern.
    // 6 normally, plus one frozen from statis plus two from golden version
    // However, tavern capped at 7 minions
    player.start_turn();
    auto tav_minions = player.get_tavern_minions();
    EXPECT_EQ(tav_minions.size(), (unsigned)7);
    // Last three should be elementals
    for (size_t i = tav_minions.size() - 3; i < tav_minions.size(); i++) {
	EXPECT_EQ(f.get_card(tav_minions[i])->get_race(), "ELEMENTAL");
    }
    player.end_turn();
}


TEST(Player, StrongshellScavengerBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Twilight Emissary"),
	 f.get_card("Mama Bear"),
	 f.get_card("Strongshell Scavenger (Golden)"),
	 f.get_card("Strongshell Scavenger")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Twilight Emissary");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 4+2+4);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 4+2+4);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Mama Bear");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Strongshell Scavenger (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 6);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Strongshell Scavenger");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_attack(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_health(), 3);
}

TEST(Player, TavernTempestBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Tavern Tempest"),
	 f.get_card("Tavern Tempest (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.play_card(0, 0);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Tavern Tempest");
    EXPECT_EQ(player.get_hand().size(), 2);
    player.play_card(0, 1);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Tavern Tempest (Golden)");
    EXPECT_EQ(player.get_hand().size(), 3);
    // TODO; expect this to change to deal with playing golden cards and discover mechanic
    // EXPECT_EQ(player.get_hand().size(), 4);
    for (auto c : player.get_hand().get_cards()) {
	EXPECT_EQ(c->get_race(), "ELEMENTAL");
    }
} 

TEST(Player, ToxfinTargettedBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Murloc Tidecaller"),
	 f.get_card("Toxfin"),
	 f.get_card("Toxfin (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 3);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    player.play_card(0, 1, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Murloc Tidecaller");
    EXPECT_TRUE(player.get_board()->get_cards()[0]->has_poison());
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Toxfin");
    EXPECT_TRUE(player.get_board()->get_cards()[1]->has_poison());
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Toxfin (Golden)");
}

TEST(Player, TwilightEmissaryTargettedBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Cobalt Scalebane"),
	 f.get_card("Twilight Emissary"),
	 f.get_card("Twilight Emissary (Golden)")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 3);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Cobalt Scalebane");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 11);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 11);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Twilight Emissary");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Twilight Emissary (Golden)");
}

TEST(Player, VirmenSenseiTargettedBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Mama Bear"),
	 f.get_card("Virmen Sensei (Golden)"),
	 f.get_card("Virmen Sensei")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 3);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Mama Bear");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Virmen Sensei (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Virmen Sensei");
}

TEST(Player, VulgarHomunculusBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Vulgar Homunculus"),
	 f.get_card("Vulgar Homunculus (Golden)"),
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    // auto hand = player.get_hand();
    // EXPECT_EQ(hand.size(), 1);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 0);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 2);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Vulgar Homunculus (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Vulgar Homunculus");
    EXPECT_EQ(player.get_health(), 34);
}

TEST(Player, WrathweaverAfterDemonSummoned) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
	{
	 f.get_card("Wrath Weaver"),
	 f.get_card("Wrath Weaver (Golden)"),
	 f.get_card("Vulgar Homunculus"),
	 f.get_card("Fiendish Servant")
	};
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    //player.buy_card(tidecaller); // TODO: Impl bobs tav
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), 4);
    EXPECT_EQ(player.get_board()->size(), 0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 4);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Wrath Weaver");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 7);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Wrath Weaver (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 10);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 14);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Vulgar Homunculus");
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Fiendish Servant");
    // 1*2 for wrathweaver, 1*2 for golden weaver, and 2 from homunculus
    EXPECT_EQ(player.get_health(), 40 - 1*2 - 1*2 - 2);
}

TEST(Player, CanStartAndEndTurnsAndGoldRefreshesAccordingly) {
    auto player = Player("Goldy Lochs");
    
    // Keep going up to 10 gold...
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 4);
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 5);
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 6);
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 7);
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 8);
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 9);
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 10);

    // Make sure we're maxed at 10
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 10);
    player.end_turn();
    player.start_turn();
    EXPECT_EQ(player.get_gold(), 10);
}
