#include "../test/googletest/include/gtest/gtest.h"

#include "player.hpp"
#include "../cards/bgs/BgCardFactory.hpp"

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
    // player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    // player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), 0);
    EXPECT_EQ(player.get_board()->size(), 1);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Vulgar Homunculus");
    EXPECT_EQ(player.get_health(), 34);
    // EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 6);
    // EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 5);
    // EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Rockpool Hunter");
    // EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Rockpool Hunter (Golden)");
}
