#include "../test/googletest/include/gtest/gtest.h"

#include "player.hpp"
#include "battler.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
#include "../cards/bgs/BgCards.hpp"
#include "../utils/pyutils.hpp"

TEST(Player, CanPlayCardFromHandBasic) {
    auto tidecaller = BgCardFactory().get_card("Murloc Tidecaller");
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards { tidecaller };
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)1);
    EXPECT_EQ(hand.get_cards()[0]->get_name(), "Murloc Tidecaller");
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(tidecaller, 0);
    EXPECT_EQ(player.get_hand().size(), (unsigned)0);
    EXPECT_EQ(player.get_board()->size(), (unsigned)1);
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

TEST(Player, GoldenTripleBasicPlaythrough) {
    auto player = Player("Test");
    player.start_turn();
    player.set_gold(9);
    std::vector<std::string> tavern_minions = {"Sellemental", "Sellemental", "Sellemental"};
    player.set_tavern_minions(tavern_minions);
    player.buy_minion(0);
    player.buy_minion(0);
    player.buy_minion(0);
    auto hand_cards = player.get_hand().get_cards();
    EXPECT_EQ(hand_cards.size(), (unsigned)1);
    EXPECT_EQ(hand_cards[0]->get_name(), "Sellemental (Golden)");

    player.play_card(0, 0);
     // Should not affect the fact that gold card was played at tier1
    player.set_gold(10);
    player.tavern_up();
    hand_cards = player.get_hand().get_cards();
    EXPECT_EQ(hand_cards.size(), (unsigned)1);
    auto triple_discover_card = hand_cards[0];
    EXPECT_EQ(triple_discover_card->get_name(), "Triple Discover");
    EXPECT_EQ(triple_discover_card->get_discover_choices().size(), (unsigned)3);
    std::unordered_set<std::string> discover_choices;
    for (auto choice : triple_discover_card->get_discover_choices()) {
        BgCardFactory f;        
        auto discover_choice = f.get_card(choice);
        // Should all be tav 2 since triple was played on tav 1
        EXPECT_EQ(discover_choice->get_tavern_tier(), 2);
        discover_choices.insert(choice);

        // Expect that the corresponding choice index has the same name...
        // TODO: Make a cleaner interface for the RL python/JSON bindings...
        EXPECT_EQ(discover_choice->get_name(), choice);
    }
    // Expect them all to be unique
    EXPECT_EQ(discover_choices.size(), (unsigned)3);

    // Discover the first one (randomly...?)
    player.play_card(0, 0);
    hand_cards = player.get_hand().get_cards();
    EXPECT_EQ(hand_cards.size(), (unsigned)1);
    EXPECT_EQ(hand_cards[0]->get_name(),
              triple_discover_card->get_discover_choices()[0]);
    
    player.end_turn();    
}

TEST(Player, GoldenTripleWhenSummoning) {
    BgCardFactory f;
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
         f.get_card("Tabbycat"),
         f.get_card("Tabbycat")
        };
    std::shared_ptr<Board> board1(new Board(b1_cards));
    auto player = Player(board1, "Test");
    player.set_gold(3);
    std::vector<std::string> tavern_minions = {"Alleycat"};
    player.set_tavern_minions(tavern_minions);
    player.buy_minion(0);
    player.play_card(0, 0);
    auto hand_cards = player.get_hand().get_cards();
    auto board_cards = player.get_board()->get_cards();
    EXPECT_EQ(hand_cards.size(), (unsigned)1);
    EXPECT_EQ(board_cards.size(), (unsigned)1);
    EXPECT_EQ(hand_cards[0]->get_name(), "Tabbycat (Golden)");
    EXPECT_EQ(board_cards[0]->get_name(), "Alleycat");
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)2);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Triple discover card created
    EXPECT_EQ(player.get_board()->size(), (unsigned)4);

    // TODO: Fix ordering
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

TEST(Player, AnnoyoModuleMagnetic) {
    auto f = BgCardFactory();
    auto micro_mummy = f.get_card("Micro Mummy");
    auto annoyo = f.get_card("Annoy-o-Module");
    auto annoyo_gold = f.get_card("Annoy-o-Module (Golden)");
    EXPECT_EQ(micro_mummy->is_magnetic(), false);
    EXPECT_EQ(annoyo->is_magnetic(), true);
    EXPECT_EQ(annoyo_gold->is_magnetic(), true);
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         micro_mummy,
         annoyo,
         annoyo_gold
        };
    auto in_hand = Hand(hand_cards);
    std::unique_ptr<Player> p1(new Player(in_hand, "Eudora"));

    p1->play_card(1, 0); // Place annoyo
    p1->play_card(0, 1); // Place micro mummy
    p1->play_card(0, 1); // Create magnetic annoyo gold

    // Assert magnetization
    micro_mummy = p1->get_board()->get_cards()[1];
    EXPECT_EQ(p1->get_board()->get_cards().size(), (unsigned)2); // One should have been magneticized
    EXPECT_EQ(micro_mummy->get_attack(), 1 + 4); // 1 base, 4 from golden annoyo
    EXPECT_EQ(micro_mummy->get_health(), 2 + 8); // 1 base, 8 from replicating
    EXPECT_TRUE(micro_mummy->has_reborn());
    EXPECT_TRUE(micro_mummy->has_taunt());
    EXPECT_TRUE(micro_mummy->has_divine_shield());
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

TEST(Player, BigfernalReactsToDemonCards) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Bigfernal"),
         f.get_card("Bigfernal (Golden)"),
         f.get_card("Imprisoner"),
        };
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.start_turn();
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 0);    

    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Imprisoner");
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_attack(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_health(), 3);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Bigfernal");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_attack(), 4 + 2);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_health(), 4 + 2);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Bigfernal (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 8 + 2);
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_health(), 8 + 2);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)5);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    player.play_card(0, 3);
    player.play_card(0, 4);
    EXPECT_EQ(player.get_board()->size(), (unsigned)5);
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

TEST(Player, ChampionOfYShaarjRetainsStats) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > board_cards
        {
         f.get_card("Champion of Y'Shaarj"),
         f.get_card("Champion of Y'Shaarj (Golden)"),
         f.get_card("Dragonspawn Lieutenant")
        };
    std::shared_ptr<Board> board1(new Board(board_cards));
    std::unique_ptr<Player> p1(new Player(board1, "p1"));
    p1->start_turn();

    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
        {
         f.get_card("Murloc Tidehunter"),
         f.get_card("Murloc Scout"),
         f.get_card("Murloc Scout"),
         f.get_card("Murloc Scout")
        };
    std::shared_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p2(new Player(board2, "p2"));
    p1->end_turn();
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "p1");
    EXPECT_EQ(p1->get_board()->get_cards().size(), 2);
    auto yshaarj = p1->get_board()->get_cards()[0];
    auto yshaarj_gold = p1->get_board()->get_cards()[1];
    // Tokens would have attacked twice, each yshaarj killed one token took one dmg
    EXPECT_EQ(yshaarj->get_attack(), 2 + 2);
    EXPECT_EQ(yshaarj->get_base_attack(), 2 + 2);
    EXPECT_EQ(yshaarj->get_health(), 2 + 1);
    EXPECT_EQ(yshaarj->get_base_health(), 2 + 2);
    EXPECT_EQ(yshaarj_gold->get_attack(), 4 + 4);
    EXPECT_EQ(yshaarj_gold->get_base_attack(), 4 + 4);
    EXPECT_EQ(yshaarj_gold->get_health(), 4 + 3);
    EXPECT_EQ(yshaarj_gold->get_base_health(), 4 + 4);

    // Should now be full health
    p1->start_turn();
    // TODO: Major Bugfix required...
    EXPECT_EQ(p1->get_board()->get_cards().size(), 3);
    yshaarj = p1->get_board()->get_cards()[0];
    yshaarj_gold = p1->get_board()->get_cards()[1];
    EXPECT_EQ(yshaarj->get_attack(), 2 + 2);
    EXPECT_EQ(yshaarj->get_health(), 2 + 2);
    EXPECT_EQ(yshaarj_gold->get_attack(), 4 + 4);
    EXPECT_EQ(yshaarj_gold->get_health(), 4 + 4);
    p1->end_turn();
}

TEST(Player, ChampionOfYShaarjRetainsStatsIfDeathDuringBattle) {
    auto f = BgCardFactory();
    auto lt = f.get_card("Dragonspawn Lieutenant");
    lt->set_health(10);
    lt->set_attack(1);
    std::vector<std::shared_ptr<BgBaseCard> > board_cards
        {
         lt,
         f.get_card("Champion of Y'Shaarj"),
        };
    std::shared_ptr<Board> board1(new Board(board_cards));
    std::unique_ptr<Player> p1(new Player(board1, "p1"));
    p1->start_turn();

    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(9);
    th->set_health(2);

    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
        {
         th
        };
    std::shared_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p2(new Player(board2, "p2"));
    p1->end_turn();
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "p1");
    EXPECT_EQ(p1->get_board()->get_cards().size(), 1);

    // Should now be full health
    p1->start_turn();
    // TODO: Major Bugfix required...
    EXPECT_EQ(p1->get_board()->get_cards().size(), 2);
    auto yshaarj = p1->get_board()->get_cards()[1];
    EXPECT_EQ(yshaarj->get_attack(), 1 + 2);
    EXPECT_EQ(yshaarj->get_health(), 1 + 2);
    p1->end_turn();
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)5);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2); 
    player.play_card(0, 1); // Give mama and egg taunt
    player.play_card(0, 4); // Give tidecaller taunt
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Triple discover card created
    EXPECT_EQ(player.get_board()->size(), (unsigned)5);
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

TEST(Player, FacelessTaverngoerTargettedBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Faceless Taverngoer"),
         f.get_card("Faceless Taverngoer (Golden)")
        };
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    auto hand = player.get_hand();
    std::vector<std::string> bobs_minions
        {
         "Bronze Warden",
         "Bronze Warden (Golden)"
        };
    player.set_tavern_minions(bobs_minions);;

    // It's pretty rare to have a gold minion in bobs tav,
    // but it can happen w/ the new darkmoon prizes.
    player.play_card(0, 1, 0); // Non golden faceless targets golden warden, but should stay non golden...
    player.play_card(0, 0, 1); // Golden faceless targets non-golden warden, but should turn golden
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Gold card played, triple discover card created
    EXPECT_EQ(player.get_board()->size(), (unsigned)2);
    auto bronze_warden = player.get_board()->get_cards()[0];
    EXPECT_EQ(bronze_warden->get_name(), "Bronze Warden");
    EXPECT_EQ(bronze_warden->get_attack(), 2);
    EXPECT_EQ(bronze_warden->get_health(), 1);
    EXPECT_EQ(bronze_warden->get_tavern_tier(), 3);
    EXPECT_TRUE(bronze_warden->has_reborn());
    EXPECT_TRUE(bronze_warden->has_divine_shield());
    auto bronze_warden_golden = player.get_board()->get_cards()[1];
    EXPECT_EQ(bronze_warden_golden->get_name(), "Bronze Warden (Golden)");
    EXPECT_EQ(bronze_warden_golden->get_attack(), 4);
    EXPECT_EQ(bronze_warden_golden->get_health(), 2);
    EXPECT_EQ(bronze_warden_golden->get_tavern_tier(), 3);
    EXPECT_TRUE(bronze_warden_golden->has_reborn());
    EXPECT_TRUE(bronze_warden_golden->has_divine_shield());
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

TEST(Player, HangryDragonStartOfTurnMechanic) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Hangry Dragon"),
         f.get_card("Hangry Dragon (Golden)"),
        };
    auto in_hand = Hand(hand_cards);
    std::unique_ptr<Player> p1(new Player(in_hand, "Eudora"));
    
    p1->start_turn();
    p1->play_card(0, 0);
    p1->play_card(0, 1);
    p1->end_turn();

    // If p1 wins, hangries should get buffed
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
        {
         f.get_card("Murloc Tidehunter")
        };
    std::shared_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p2(new Player(board2, "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Eudora");

    p1->start_turn();
    auto hangry = p1->get_board()->get_cards()[0];
    auto hangry_gold = p1->get_board()->get_cards()[1];
    EXPECT_EQ(hangry->get_attack(), 4 + 2);
    EXPECT_EQ(hangry->get_health(), 4 + 2);
    EXPECT_EQ(hangry_gold->get_attack(), 8 + 4);
    EXPECT_EQ(hangry_gold->get_health(), 8 + 4);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)2);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);

    // Should be nothing buffed
    player.play_card(0, 0);
    player.play_card(0, 1);
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Triple discover card created
    EXPECT_EQ(player.get_board()->size(), (unsigned)2);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)5);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);

    // Play cards in hand order, only Harvest Golem gets buffed
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    player.play_card(0, 4);
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Triple Discover card created
    EXPECT_EQ(player.get_board()->size(), (unsigned)5);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)3);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Triple discover card created (Gold card played)
    EXPECT_EQ(player.get_board()->size(), (unsigned)3);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)3);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 0);
    player.play_card(0, 3);
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Triple discover card created (Gold card played)
    EXPECT_EQ(player.get_board()->size(), (unsigned)5);
    EXPECT_EQ(player.get_board()->get_cards()[0]->get_name(), "Murloc Tidehunter");
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Murloc Scout");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_name(), "Murloc Tidecaller");
    EXPECT_EQ(player.get_board()->get_cards()[2]->get_attack(), 5);
    EXPECT_EQ(player.get_board()->get_cards()[3]->get_name(), "Murloc Tidehunter (Golden)");
    EXPECT_EQ(player.get_board()->get_cards()[4]->get_name(), "Murloc Scout (Golden)");
}

TEST(Player, MurozondBattlecry) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Murozond"),
         f.get_card("Murozond (Golden)")
        };
    auto in_hand = Hand(hand_cards);
    std::unique_ptr<Player> p1(new Player(in_hand, "p1"));
    p1->start_turn();
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
        {
         f.get_card("Murloc Tidehunter"),
         f.get_card("Red Whelp"),
         f.get_card("Red Whelp (Golden)"),
         f.get_card("Bronze Warden"),
         f.get_card("Bronze Warden (Golden)"),
         f.get_card("Harvest Golem (Golden)")
        };
    std::shared_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p2(new Player(board2, "p2"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "p2");
    p1->end_turn();
    
    p1->start_turn();
    p1->play_card(0, 0);
    p1->play_card(0, 1);
    std::vector<std::string> valid_cards =
        {
         "Murloc Tidehunter",
         "Red Whelp",
         "Bronze Warden",
         "Harvest Golem"
        };
    auto valid_cards_gold(valid_cards);
    auto append_gold = [](auto x)->std::string { return x += " (Golden)"; };
    std::transform(valid_cards.begin(),
                   valid_cards.end(),
                   valid_cards_gold.begin(),
                   append_gold);

    // First card in hand should be non-golden
    EXPECT_TRUE(pyutils::in(p1->get_hand().get_cards()[0]->get_name(), valid_cards));
    // Second card in hand should be golden
    EXPECT_TRUE(pyutils::in(p1->get_hand().get_cards()[1]->get_name(), valid_cards_gold));
    p1->end_turn();
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

TEST(Player, NomiUpdatesBobsTavernWhenElementalsArePlayed) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Nomi, Kitchen Nightmare"),
         f.get_card("Nomi, Kitchen Nightmare (Golden)"),
         f.get_card("Sellemental")
        };
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");

    player.start_turn();

    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2); // Buffs tavern elementals +3/+3 (sellemental played)
    std::vector<std::string> tavern_minions = {"Crackling Cyclone",
                                               "Mal'Ganis",
                                               "Sellemental"};
    player.set_tavern_minions(tavern_minions);
    player.set_gold(10);
    player.buy_minion(0);
    player.buy_minion(0);
    player.buy_minion(0);

    // 0th position card is the discover triple card
    player.play_card(1, 3);
    player.play_card(1, 4);
    player.play_card(1, 5);

    // Assert elementals on board unchanged
    auto sellemental = player.get_board()->get_cards()[2];
    EXPECT_EQ(sellemental->get_name(), "Sellemental");
    EXPECT_EQ(sellemental->get_attack(), 2);
    EXPECT_EQ(sellemental->get_health(), 2);

    // Assert elementals from tavern got +3/+3
    auto crackling = player.get_board()->get_cards()[3];
    EXPECT_EQ(crackling->get_name(), "Crackling Cyclone");
    EXPECT_EQ(crackling->get_attack(), 4 + 3);
    EXPECT_EQ(crackling->get_health(), 1 + 3);

    // Malganis should not be buffed, not an elemental
    auto malganis = player.get_board()->get_cards()[4];
    EXPECT_EQ(malganis->get_name(), "Mal'Ganis");
    EXPECT_EQ(malganis->get_attack(), 9);
    EXPECT_EQ(malganis->get_health(), 7);

    // Sellemental from tavern should be buffed +3/+3
    auto selle_tav = player.get_board()->get_cards()[5];
    EXPECT_EQ(selle_tav->get_name(), "Sellemental");
    EXPECT_EQ(selle_tav->get_attack(), 2 + 3);
    EXPECT_EQ(selle_tav->get_health(), 2 + 3);
    
    player.end_turn();
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

TEST(Player, ReplicatingMenaceMagnetic) {
    auto f = BgCardFactory();
    auto golem = f.get_card("Harvest Golem");
    auto repl_men = f.get_card("Replicating Menace");
    auto repl_men_gold = f.get_card("Replicating Menace (Golden)");
    EXPECT_EQ(golem->is_magnetic(), false);
    EXPECT_EQ(repl_men->is_magnetic(), true);
    EXPECT_EQ(repl_men_gold->is_magnetic(), true);
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         golem,
         repl_men,
         repl_men_gold
        };
    auto in_hand = Hand(hand_cards);
    std::unique_ptr<Player> p1(new Player(in_hand, "Eudora"));

    p1->play_card(0, 0);
    p1->play_card(0, 0);
    p1->play_card(0, 2);

    // Assert magnetization
    golem = p1->get_board()->get_cards()[0];
    EXPECT_EQ(p1->get_board()->get_cards().size(), (unsigned)2); // One should have been magneticized
    EXPECT_EQ(golem->get_attack(), 2 + 3); // 2 base, 3 from replicating
    EXPECT_EQ(golem->get_health(), 3 + 1); // 3 base, 1 from replicating

    // Make sure deathrattle goes off properly
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(10);
    th->set_health(2 + 3 + 6 + 1*1 + 1*3 + 2*3); // base attacks for all, plus one 2/1, 3 1/1s, and 3 2/2s
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { th };
    std::shared_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p2(new Player(board2, "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)3);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Gold card played, discover triple card added to hand
    EXPECT_EQ(player.get_board()->size(), (unsigned)3);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)3);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Gold card played
    EXPECT_EQ(player.get_board()->size(), (unsigned)3);
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
    player.set_gold(10);
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
    EXPECT_EQ(player.get_hand().size(), (unsigned)2);
    player.play_card(0, 1);
    EXPECT_EQ(player.get_board()->get_cards()[1]->get_name(), "Tavern Tempest (Golden)");
    EXPECT_EQ(player.get_hand().size(), (unsigned)4);
    for (auto c : player.get_hand().get_cards()) {
        EXPECT_TRUE(c->get_race() == "ELEMENTAL" || c->get_race() == "");
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)3);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    player.play_card(0, 1, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Gold card played
    EXPECT_EQ(player.get_board()->size(), (unsigned)3);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)3);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Gold card played
    EXPECT_EQ(player.get_board()->size(), (unsigned)3);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)3);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 0, 1); // Hand pos, target pos, board pos
    player.play_card(0, 0, 2); // Hand pos, target pos, board pos
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Gold card played
    EXPECT_EQ(player.get_board()->size(), (unsigned)3);
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
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 0);
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Gold card played
    EXPECT_EQ(player.get_board()->size(), (unsigned)2);
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
    auto hand = player.get_hand();
    EXPECT_EQ(hand.size(), (unsigned)4);
    EXPECT_EQ(player.get_board()->size(), (unsigned)0);
    
    player.play_card(0, 0);
    player.play_card(0, 1);
    player.play_card(0, 2);
    player.play_card(0, 3);
    EXPECT_EQ(player.get_hand().size(), (unsigned)1); // Gold card played
    EXPECT_EQ(player.get_board()->size(), (unsigned)4);
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

TEST(Player, CanListAllPossibleActions) {
    // Just does some spot checking...
    std::vector<std::string> spot_checks =
        {
         "BUY_3", // Buy the 4th minion in bobs tav
         "SELL_6", // Sell the 7th minion on board
         "PLAY_CARD_FROM_HAND_0_TO_BOARD_0", // Play first card in hand to first board slot
         "PLAY_CARD_FROM_HAND_5_TO_BOARD_2", // Play fourth card in hand to third board slot
         "PLAY_CARD_FROM_HAND_6_TO_BOARD_6", // Play seventh card in hand to seventh board slot
         "PLAY_CARD_FROM_HAND_0_TO_BOARD_0_TARGET_0", // Play first card in hand to first board slot target first
         "PLAY_CARD_FROM_HAND_5_TO_BOARD_2_TARGET_3", // Play fourth card in hand to third board slot target third
         "PLAY_CARD_FROM_HAND_6_TO_BOARD_6_TARGET_6", // Play seventh card in hand to seventh board slot target 7th
         "REPOSITION_FROM_0_TO_1", // Flip first and second card
         "REPOSITION_FROM_3_TO_5", // Reposition card 3 into slot 5
         "REPOSITION_FROM_6_TO_5", // Reposition card 3 into slot 5
         // TODO: spells
         // "SPELL_NO_TARGET",
         // "SPELL_BOARD_TARGET_0",
         // "SPELL_TAVERN_TARGET_0",
         // TODO: Hero powers
         // "HERO_POWER_PYRAMAD",
         // "HERO_POWER_LICH_KING",
        };    

    // Can't reposition to original position, this is a no-op
    std::vector<std::string> invalids =
        {
         "REPOSITION_FROM_0_TO_0",
         "REPOSITION_FROM_1_TO_1",
         "REPOSITION_FROM_2_TO_2",
         "REPOSITION_FROM_3_TO_3",
         "REPOSITION_FROM_4_TO_4",
         "REPOSITION_FROM_5_TO_5",
         "REPOSITION_FROM_6_TO_6"
        };

    auto player = Player("Test");
    auto all_actions = player.list_all_possible_actions();
    for (auto spot_check : spot_checks) {
        EXPECT_TRUE(pyutils::in(spot_check, all_actions));
    }

    for (auto invalid : invalids) {
        EXPECT_FALSE(pyutils::in(invalid, all_actions));
    }
}

TEST(Player, CanListFreezeActions) {
    auto player = Player("Test");

    // Tavern not frozen by default
    auto can_freeze = player.list_freeze_actions();
    EXPECT_EQ(can_freeze.size(), (unsigned)1);
    EXPECT_EQ(can_freeze[0], "FREEZE");

    // Freeze it, check we can unfreeze
    player.freeze_tavern();
    auto can_unfreeze = player.list_freeze_actions();
    EXPECT_EQ(can_unfreeze.size(), (unsigned)1);
    EXPECT_EQ(can_unfreeze[0], "UNFREEZE");

    // Unfreeze it, check we can freeze
    player.unfreeze_tavern();
    auto can_freeze_again = player.list_freeze_actions();
    EXPECT_EQ(can_freeze_again.size(), (unsigned)1);
    EXPECT_EQ(can_freeze_again[0], "FREEZE");
}

TEST(Player, CanListRollActions) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Refreshing Anomaly")
        };
    auto in_hand = Hand(hand_cards);
    auto player = Player(in_hand, "Test");
    player.set_gold(3);

    // Tavern not frozen by default
    auto can_roll = player.list_roll_actions();
    EXPECT_EQ(can_roll.size(), (unsigned)1);
    EXPECT_EQ(can_roll[0], "ROLL");

    // Roll, check we can still roll again
    player.refresh_tavern_minions();
    can_roll = player.list_roll_actions();
    EXPECT_EQ(can_roll.size(), (unsigned)1);
    EXPECT_EQ(can_roll[0], "ROLL");

    // Roll, check we can still roll
    player.refresh_tavern_minions();
    can_roll = player.list_roll_actions();
    EXPECT_EQ(can_roll.size(), (unsigned)1);
    EXPECT_EQ(can_roll[0], "ROLL");

    // Roll one more time, gold should be 0, can't roll
    player.refresh_tavern_minions();
    can_roll = player.list_roll_actions();
    EXPECT_EQ(can_roll.size(), (unsigned)0);

    // Play refreshing anomaly, can roll again
    player.play_card(0, 0);
    can_roll = player.list_roll_actions();
    EXPECT_EQ(can_roll.size(), (unsigned)1);
    EXPECT_EQ(can_roll[0], "ROLL");

    // After this roll, make sure we can't roll again
    player.refresh_tavern_minions();
    can_roll = player.list_roll_actions();
    EXPECT_EQ(can_roll.size(), (unsigned)0);
}

TEST(Player, CanListSellActionsNominalCase) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
         f.get_card("Alleycat"),
         f.get_card("Cave Hydra"),
         f.get_card("Cave Hydra")
        };
    std::shared_ptr<Board> board1(new Board(b1_cards));
    auto player = Player(board1, "Test");
    
    // Assert we can sell any of the minions on our board
    auto sell_actions = player.list_sell_actions();
    EXPECT_EQ(sell_actions.size(), (unsigned)3);
    EXPECT_EQ(sell_actions[0], "SELL_0");
    EXPECT_EQ(sell_actions[1], "SELL_1");
    EXPECT_EQ(sell_actions[2], "SELL_2");

    // All actions should be in the larger pool of possible actions
    auto all_actions = player.list_all_possible_actions();
    for (auto sell_action : sell_actions) {
        EXPECT_TRUE(pyutils::in(sell_action, sell_actions));
    }
}

TEST(Player, CanListSellActionsEmptyCase) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
        };
    std::shared_ptr<Board> board1(new Board(b1_cards));
    auto player = Player(board1, "Test");

    // Assert we can sell any of the minions on our board
    auto sellables = player.list_sell_actions();
    EXPECT_EQ(sellables.size(), (unsigned)0);
}

TEST(Player, CanListPlayFromHandActionsEmpty) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
        };
    std::shared_ptr<Board> board1(new Board(b1_cards));
    auto player = Player(board1, "Test");

    // Assert we can sell any of the minions on our board
    auto play_actions = player.list_play_from_hand_actions();
    EXPECT_EQ(play_actions.size(), (unsigned)0);
}

TEST(Player, CanListPlayFromHandActionsSingleCard) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
         f.get_card("Alleycat")
        };
    Hand hand(b1_cards);
    auto player = Player(hand, "Test");

    // Assert we can sell any of the minions on our board
    auto all_actions = player.list_all_possible_actions();
    auto play_actions = player.list_play_from_hand_actions();
    EXPECT_EQ(play_actions.size(), (unsigned)1);
    EXPECT_EQ(play_actions[0], "PLAY_CARD_FROM_HAND_0_TO_BOARD_0");
    EXPECT_TRUE(pyutils::in(play_actions[0], all_actions));
}

TEST(Player, CanListPlayFromHandActionsFullBoard) {
    // Should be no play actions if board is full, except for spells
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat")
        };
    std::shared_ptr<Board> board1(new Board(b1_cards));

    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Amalgadon"),
         f.get_card("Alleycat"),
         f.get_card("Foe Reaper 4000"),
        };
    auto hand = Hand(hand_cards);
    auto player = Player(hand, "Test");
    player.set_board(board1);

    // Assert we can't play any minions (board is full)
    auto play_actions = player.list_play_from_hand_actions();
    EXPECT_EQ(play_actions.size(), (unsigned)0);
}

TEST(Player, CanListPlayActionsAlmostFullBoard) {
    // Board is almost full, so every slot should be available
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat")
        };
    std::shared_ptr<Board> board1(new Board(b1_cards));

    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Amalgadon"),
         f.get_card("Alleycat"),
         f.get_card("Foe Reaper 4000"),
         f.get_card("Mal'Ganis (Golden)")
        };
    auto hand = Hand(hand_cards);
    auto player = Player(hand, "Test");
    player.set_board(board1);

    auto all_actions = player.list_all_possible_actions();
    auto play_actions = player.list_play_from_hand_actions();
    // Four cards, can play them in any slot (e.g. 4 * 7)
    EXPECT_EQ(play_actions.size(), (unsigned)(4*7));
    for (const auto& play_action : play_actions) {
        EXPECT_TRUE(pyutils::in(play_action, all_actions));
    }
}

TEST(Player, CanListPlayActionsForTargettedBattlecriesFullBoard) {
    // Board is almost full, so every slot should be available
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
         f.get_card("Murloc Tidehunter"),
         f.get_card("Alleycat"),
         f.get_card("Vulgar Homunculus"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat"),
         f.get_card("Murloc Tidehunter"),
        };
    std::shared_ptr<Board> board1(new Board(b1_cards));

    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Virmen Sensei"), // Three targets
         f.get_card("Rockpool Hunter"), // Two target
         f.get_card("Nathrezim Overseer"), // One target
         f.get_card("Mal'Ganis (Golden)") // Not targetted
        };
    auto hand = Hand(hand_cards);
    auto player = Player(hand, "Test");
    player.set_board(board1);

    // Assert we can't play any minions (board is full)
    auto all_actions = player.list_all_possible_actions();
    auto play_actions = player.list_play_from_hand_actions();
    // Vermin can go in any slot, has three targets (3 * 7)
    // Rockpool can go in any slot, has two targets (2 * 7)
    // Nathrezim can go in any slot, has one target (1 * 7)
    // Mal'Ganis can go in any slot, no target (1 * 7)
    EXPECT_EQ(play_actions.size(), (unsigned)(3*7 + 2*7 + 1*7 + 7));
    for (const auto& play_action : play_actions) {
        EXPECT_TRUE(pyutils::in(play_action, all_actions));
    }
}

TEST(Player, CanListBuyActionsTavern1Default) {
    auto player = Player("Test");
    auto buy_actions = player.list_buy_actions();

    // Bobs tavern at tier1 should have 3 minions
    EXPECT_EQ(buy_actions.size(), (unsigned)3);
    EXPECT_EQ(buy_actions[0], "BUY_0");
    EXPECT_EQ(buy_actions[1], "BUY_1");
    EXPECT_EQ(buy_actions[2], "BUY_2");

    // Assert we can sell any of the minions on our board
    auto all_actions = player.list_all_possible_actions();
    for (auto buy_action : buy_actions) {
        EXPECT_TRUE(pyutils::in(buy_action, all_actions));
    }
}

TEST(Player, ListsNoBuyActionsWhenHandFull) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > hand_cards;
    for (int i = 0; i < 10; i++) {
        hand_cards.push_back(f.get_card("Alleycat"));
    }
    auto hand = Hand(hand_cards);    
    auto player = Player(hand, "Test");
    auto buy_actions = player.list_buy_actions();

    // Can't buy a card if hand is full
    EXPECT_EQ(buy_actions.size(), (unsigned)0);
}

TEST(Player, ListsNoRepositionActionsWhenBoardEmpty) {
    auto player = Player("Test");
    auto reposition_actions = player.list_board_reposition_actions();

    // Can't reposition if no cards are on board
    EXPECT_EQ(reposition_actions.size(), (unsigned)0);
}

TEST(Player, ListsNoRepositionActionsWhenOnly1CardOnBoard) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > board_cards;
    board_cards.push_back(f.get_card("Alleycat"));
    auto player = Player(std::make_shared<Board>(board_cards), "Test");
    auto reposition_actions = player.list_board_reposition_actions();

    // Can't reposition if only one card are on board
    EXPECT_EQ(reposition_actions.size(), (unsigned)0);
}

TEST(Player, CanListRepositionActionsWhenBoardHas3Minions) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > board_cards;
    for (int i = 0; i < 3; i++) {
        board_cards.push_back(f.get_card("Alleycat"));
    }
    auto player = Player(std::make_shared<Board>(board_cards), "Test");
    auto reposition_actions = player.list_board_reposition_actions();

    // Each of the 3 cards can be repositioned to 2 other positions
    // So, should be 6 actions total
    EXPECT_EQ(reposition_actions.size(), (unsigned)6);
}

TEST(Player, CanListTavernUpActionsStartOfGame) {
    // Assert no tav up action available at start
    auto player = Player("Test");
    auto tav_up_actions = player.list_tavern_up_actions();

    EXPECT_EQ(tav_up_actions.size(), (unsigned)0);
}

TEST(Player, CanListTavernUpActionsTier2) {
    // Assert no tav up action available at start
    auto player = Player("Test");
    player.set_tavern_tier(2);
    player.set_gold(10);
    auto tav_up_actions = player.list_tavern_up_actions();

    EXPECT_EQ(tav_up_actions.size(), (unsigned)1);
    EXPECT_EQ(tav_up_actions[0], "TAVERN_UP");
}

TEST(Player, CanListTavernUpActionsTier6) {
    // Assert no tav up action available at start
    auto player = Player("Test");
    player.set_tavern_tier(6);
    player.set_gold(10);
    auto tav_up_actions = player.list_tavern_up_actions();

    EXPECT_EQ(tav_up_actions.size(), (unsigned)0);
}

TEST(Player, CanSerializeAndDeserialize) {
    auto f = BgCardFactory();
    auto buffed_cat = f.get_card("Alleycat");
    buffed_cat->set_divine_shield();
    buffed_cat->set_base_health(10);
    buffed_cat->set_base_attack(5);
    std::vector<std::shared_ptr<BgBaseCard> > b1_cards
        {
         buffed_cat,
         f.get_card("Faceless Taverngoer"),
         f.get_card("Mama Bear"),
         f.get_card("Red Whelp"),
         f.get_card("Kaboom Bot"),
         f.get_card("Alleycat"),
         f.get_card("Alleycat")
        };
    std::shared_ptr<Board> board1(new Board(b1_cards));

    std::vector<std::shared_ptr<BgBaseCard> > hand_cards
        {
         f.get_card("Amalgadon"),
         f.get_card("Alleycat"),
         f.get_card("Foe Reaper 4000"),
         f.get_card("Houndmaster")
        };
    auto hand = Hand(hand_cards);
    auto player = Player(hand, "Test");
    player.set_board(board1);

    // Alter some of the default values (spot check)
    player.set_gold(4);
    player.set_max_health(55); // Pretend we're patches
    player.set_health(1);
    player.set_tavern_tier(2);

    // Dump to json
    std::string json_file = "player.json";
    auto json_to_dump = player.to_json();
    std::ofstream out(json_file);
    out << json_to_dump.dump(4);
    out.close();
    auto player_deserialized = Player::from_json(json_file);

    // Assert we deserialize properly
    // Expect boards are the same
    auto board1_deserialized = player_deserialized.get_board();
    auto b1_cards_again = board1->get_cards();
    auto b1_cards_deser = board1_deserialized->get_cards();
    EXPECT_EQ(b1_cards_again.size(), b1_cards_deser.size());
    for (int i = 0; (unsigned)i < b1_cards_again.size(); i++) {
        EXPECT_EQ(b1_cards_again[i]->get_name(), b1_cards_deser[i]->get_name());
    }
    
    // Expect hands are the same
    auto hand_deserialized = player_deserialized.get_hand();
    auto hand_cards_again = hand.get_cards();
    auto hand_cards_deser = hand_deserialized.get_cards();
    EXPECT_EQ(hand_cards_again.size(), hand_cards_deser.size());
    for (int i = 0; (unsigned)i < hand_cards_again.size(); i++) {
        EXPECT_EQ(hand_cards_again[i]->get_name(), hand_cards_deser[i]->get_name());
    }

    // Expect tavern minions are the same
    auto tav_minions =  player.get_tavern_minions();
    auto tav_minions_deser = player_deserialized.get_tavern_minions();
    EXPECT_EQ(tav_minions, tav_minions_deser);

    EXPECT_EQ(player.get_elementals_played_this_turn(), player_deserialized.get_elementals_played_this_turn());
    EXPECT_EQ(player.get_frozen_minions(), player_deserialized.get_frozen_minions());
    EXPECT_EQ(player.get_gold(), player_deserialized.get_gold());
    EXPECT_EQ(player.get_max_gold(), player_deserialized.get_max_gold());
    EXPECT_EQ(player.get_health(), player_deserialized.get_health());
    EXPECT_EQ(player.get_max_health(), player_deserialized.get_max_health());
    EXPECT_EQ(player.get_name(), player_deserialized.get_name());
    EXPECT_EQ(player.get_num_free_refreshes(), player_deserialized.get_num_free_refreshes());
    EXPECT_EQ(player.get_pirates_bought_this_turn(), player_deserialized.get_pirates_bought_this_turn());
    EXPECT_EQ(player.get_tavern_tier(), player_deserialized.get_tavern_tier());
    EXPECT_EQ(player.get_tavern_minions(), player_deserialized.get_tavern_minions());
    EXPECT_EQ(player.is_tavern_frozen(), player_deserialized.is_tavern_frozen());
    EXPECT_EQ(player.get_turns_at_current_tier(), player_deserialized.get_turns_at_current_tier());
    EXPECT_EQ(player.won_last_turn(), player_deserialized.won_last_turn());
}
