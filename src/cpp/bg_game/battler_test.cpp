#include "battler.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
#include "../cards/bgs/BgBaseCard.hpp"

#include "../test/googletest/include/gtest/gtest.h"

#include <memory>

TEST(Battler, CalculatesWinWhenOppEmptyBoard) {
    auto f = BgCardFactory();
    auto tidecaller = f.get_card("Murloc Tidecaller (Golden)");
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { tidecaller };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards;
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p1(new Player(board1.get(), "HookTusk"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Pyramad"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "HookTusk");
    EXPECT_EQ(res.damage_taken, 2);
}

TEST(Battler, CanCalculateDrawWithEmptyBoards) {
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards;
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards;
    //auto board1 = std::unique_ptr<Board(p1_cards)>;
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p1(new Player(board1.get(), "HookTusk"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Pyramad"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, CanCalculateDrawWithCardsThatImmediatelyDieToEachOther) {
    auto f = BgCardFactory();
    auto tidecaller1 = f.get_card("Murloc Tidehunter (Golden)");
    auto tidecaller2 = f.get_card("Murloc Tidehunter (Golden)");
    auto tidecaller3 = f.get_card("Murloc Tidehunter (Golden)");
    auto tidecaller4 = f.get_card("Murloc Tidehunter (Golden)");
    auto tidecaller5 = f.get_card("Murloc Tidehunter (Golden)");
    auto tidecaller6 = f.get_card("Murloc Tidehunter (Golden)");
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { tidecaller1, tidecaller2, tidecaller3 };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { tidecaller4, tidecaller5, tidecaller6 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "HookTusk"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Pyramad"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, WinsIfWinIs100PercentGuaranteed) {
    auto f = BgCardFactory();
    auto tidecaller1 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto tidecaller2 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto tidecaller3 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
    auto gambler2 = f.get_card("Freedealing Gambler (Golden)");
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { tidecaller1, tidecaller2, tidecaller3 };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { gambler1, gambler2 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p1(new Player(board1.get(), "HookTusk"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Pyramad"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Pyramad");
    // One gambler will always live, and it's a tier 2 card
    EXPECT_EQ(res.damage_taken, 1+2); 
}

TEST(Battler, CanHandlePoisonCorrectly) {
    auto f = BgCardFactory();
    auto tidecaller1 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto tidecaller2 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto tidecaller3 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    tidecaller1->set_poison();
    tidecaller2->set_poison();
    tidecaller3->set_poison();
    auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
    auto gambler2 = f.get_card("Freedealing Gambler (Golden)");
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { tidecaller1, tidecaller2, tidecaller3 };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { gambler1, gambler2 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p1(new Player(board1.get(), "HookTusk"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Pyramad"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "HookTusk");
    // One murloc should always live, tier 1
    EXPECT_EQ(res.damage_taken, 1+1); 
}

TEST(Battler, CanHandleDivineShieldCorrectly) {
    auto f = BgCardFactory();
    auto tidecaller1 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto tidecaller2 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto tidecaller3 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    tidecaller1->set_poison();
    tidecaller2->set_poison();
    tidecaller3->set_poison();
    auto deflecto1 = f.get_card("Deflect-o-Bot");
    auto deflecto2 = f.get_card("Deflect-o-Bot");
    // deflecto1->set_divine_shield();
    // deflecto2->set_divine_shield();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { tidecaller1, tidecaller2, tidecaller3 };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { deflecto1, deflecto2 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p1(new Player(board1.get(), "Eudora"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Edwin");
    // One deflecto should always live, it's a tier 3 card.    
    EXPECT_EQ(res.damage_taken, 1+3);
}

TEST(Battler, CanHandleBasicDeathrattles) {
    auto f = BgCardFactory();
    auto fiendish_serv = f.get_card("Fiendish Servant");
    auto micro_machine = f.get_card("Micro Machine (Golden)");
    auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
    // Imply proper deathrattle would be enough to draw it
    EXPECT_EQ(fiendish_serv->get_attack() * 2 + micro_machine->get_attack(),
	      gambler1->get_attack());
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { fiendish_serv, micro_machine };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { gambler1 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0); 
}

TEST(Battler, FiendishServantGoldenDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Fiendish Servant (Golden)"),
	 f.get_card("Micro Machine (Golden)"),
	 f.get_card("Micro Machine (Golden)"),
	 f.get_card("Micro Machine (Golden)"),
	 f.get_card("Micro Machine (Golden)"),
    };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards;
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));    
    auto fiendish = p1_cards[0];
    fiendish->set_attack(10);
    fiendish->take_damage(10, board1.get(), board2.get());
    int total_attack = 0;
    // TODO: Looks like p1_cards getting copied, probably not great for
    // performace...fix when profiler setup
    for (auto c : board1->get_cards()) {
	total_attack += c->get_attack();
    }
    // Make sure total attack is 2*4 (original micro machines)
    // plus 10*2 (fiendish damage)
    EXPECT_EQ(total_attack, 2*4 + 10*2);
}


TEST(Battler, MecharooDrattle) {
    auto f = BgCardFactory();
    auto mecharoo = f.get_card("Mecharoo");
    auto tidehunter = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
    // These should draw since mecharoo summons a 1/1 token as a drattle
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { mecharoo, tidehunter };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { gambler1 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, SelflessHeroDrattle) {
    auto f = BgCardFactory();
    auto selfless = f.get_card("Selfless Hero");
    auto tidehunter = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
    auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
    // These should draw since mecharoo summons a 1/1 token as a drattle
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { selfless, tidehunter };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { gambler1 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // Selfless will die first, gives tidehunter a divine shield, which then kills gambler and lives.
    EXPECT_EQ(res.who_won, "Tess");
    EXPECT_EQ(res.damage_taken, 1+1);
}

TEST(Battler, SelflessHeroDrattleDoesntHelpIfDivineAlreadyPresent) {
    auto f = BgCardFactory();
    auto selfless = f.get_card("Selfless Hero");
    auto deflecto = f.get_card("Deflect-o-Bot"); // No battlecry
    auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
    // These should draw since mecharoo summons a 1/1 token as a drattle
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { selfless, deflecto };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { gambler1 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // Selfless will die first, gives no shield, gambler pops div sheild, then deflecto and gambler kill each other
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, ScallywagDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag") };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { f.get_card("Murloc Tidehunter"),
							 f.get_card("Murloc Scout") };
    // p1 should be left w/ 6 Scallywags (since 2/1 kills either scout or tidehunter, then 1/1 token immediately kills the other
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "p1"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "p2"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "p1");
    auto battled_p1_cards = p1->get_board()->get_cards();
    for (auto c : battled_p1_cards) {
	EXPECT_EQ(c->get_name(), "Scallywag");
    }
    EXPECT_EQ(res.damage_taken, 7);
}

TEST(Battler, HarvestGolemDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { f.get_card("Harvest Golem") };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { f.get_card("Murloc Tidehunter (Golden)") };
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Pyramad"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Murgle"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Pyramad");
    auto battled_p1_cards = p1->get_board()->get_cards();
    for (auto c : battled_p1_cards) {
	EXPECT_EQ(c->get_name(), "Damaged Golem");
    }
    EXPECT_EQ(res.damage_taken, 2);
}

TEST(Battler, HarvestGolemGoldenDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { f.get_card("Harvest Golem (Golden)") };
    auto th = f.get_card("Murloc Tidehunter (Golden)");
    th->set_attack(6); // ENough to kill the golem
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { th };
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Pyramad"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Murgle"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Pyramad");
    auto battled_p1_cards = p1->get_board()->get_cards();
    for (auto c : battled_p1_cards) {
	EXPECT_EQ(c->get_name(), "Damaged Golem (Golden)");
    }
    EXPECT_EQ(res.damage_taken, 2);
}

TEST(Battler, KaboomBotDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Kaboom Bot")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)")
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Pyramad"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Murgle"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // 2/2 kills 4/2, bomb kills other 4/2
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, KaboomBotGoldenDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Kaboom Bot (Golden)")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)")
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Pyramad"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Murgle"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // 4/4 kills 4/2, bombs kills other 4/2s
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}


TEST(Battler, KindlyGrandmotherDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Kindly Grandmother")
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // 1/1 kills 2/1, 3/2 drattle wins
    auto battled_p2_cards = p2->get_board()->get_cards();
    for (auto c : battled_p2_cards) {
	EXPECT_EQ(c->get_name(), "Big Bad Wolf");
    }
    EXPECT_EQ(res.who_won, "Tess");
    EXPECT_EQ(res.damage_taken, 2);
}

TEST(Battler, KindlyGrandmotherGoldenDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Kindly Grandmother (Golden)")
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // 2/2 kills 2/1, 6/4 drattle wins
    auto battled_p2_cards = p2->get_board()->get_cards();
    for (auto c : battled_p2_cards) {
	EXPECT_EQ(c->get_name(), "Big Bad Wolf (Golden)");
    }
    EXPECT_EQ(res.who_won, "Tess");
    EXPECT_EQ(res.damage_taken, 2);
}


TEST(Battler, SpawnOfNzothDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Spawn of Nzoth"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)"),
	 f.get_card("Freedealing Gambler (Golden)")
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Tess");
    // Will take at most 2 * 6 = 12
    // WIll take at least 2 * 3 = 6
    EXPECT_LE(res.damage_taken, 12);
    EXPECT_GE(res.damage_taken, 6);
}

TEST(Battler, UnstableGhoulDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Unstable Ghoul")
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // Tidehunters kill ghoul, it explodes, should be a draw
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_LE(res.damage_taken, 0);
}

TEST(Battler, RatPackDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    auto rp = f.get_card("Rat Pack");
    rp->set_attack(7);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	    rp
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // One rat should always survive
    EXPECT_EQ(res.who_won, "Tess");
    EXPECT_EQ(res.damage_taken, 2);
}

TEST(Battler, RatPackDrattleSummonsCorrectNumOfRats) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards;
    auto rp = f.get_card("Rat Pack");
    rp->set_attack(7);
    auto big_guy = f.get_card("Razorgore");
    big_guy->set_health(100);
    auto big_guy2 = f.get_card("Razorgore");
    big_guy2->set_health(100);	
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 big_guy,
	 rp,
	 big_guy2
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    rp->take_damage(2, board2.get(), board1.get());
    int rat_count = 0;
    int razor_count = 0;
    for (auto c : board2->get_cards()) {
	auto name = c->get_name();
	if (name == "Rat") rat_count++;
	if (name.find("Razorgore") != std::string::npos) razor_count++;
    }
    EXPECT_EQ(rat_count, 5);
    EXPECT_EQ(razor_count, 2);
}

TEST(Battler, ImprisonerDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter (Golden)")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Imprisoner")
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // 3/3 taunt kills 4/2 murloc, 1/1 drattle wins
    auto battled_p2_cards = p2->get_board()->get_cards();
    for (auto c : battled_p2_cards) {
	EXPECT_EQ(c->get_name(), "Imp");
    }
    EXPECT_EQ(res.who_won, "Tess");
    EXPECT_EQ(res.damage_taken, 2);
}

TEST(Battler, ImprisonerGoldenDrattle) {
    auto f = BgCardFactory();
    auto th = f.get_card("Murloc Tidehunter (Golden)");
    th->set_attack(10);
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 th
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Imprisoner (Golden)")
	};
    // Should have the 1 damaged golem left on board
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // 6/6 taunt kills 10/2 murloc, 2/2 drattle wins
    auto battled_p2_cards = p2->get_board()->get_cards();
    for (auto c : battled_p2_cards) {
	EXPECT_EQ(c->get_name(), "Imp (Golden)");
    }
    EXPECT_EQ(res.who_won, "Tess");
    EXPECT_EQ(res.damage_taken, 2);
}

