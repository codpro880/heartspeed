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

TEST(Battler, Baron) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Spawn of Nzoth"),
	 f.get_card("Baron")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(8); // Two attacks for baron
    th->set_health(2 + 3 + 3); // 2 from spawn, 3 and 3 from baron
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}


TEST(Battler, Bolvar) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Deflect-o-Bot"),
	 f.get_card("Bolvar")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_health(9); // 6 from deflecto, 3 from bolvar
    th->set_attack(100);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}


TEST(Battler, BronzeWarden) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Bronze Warden")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

TEST(Battler, CaveHydra) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Cave Hydra")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

TEST(Battler, DrakonidEnforcer) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Deflect-o-Bot"),
	 f.get_card("Drakonid Enforcer")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_health(16); // 3 and 3 from bot, 5 and 5 from enforcer
    th->set_attack(7); // Should take two hits from both bot and enforcer
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
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
    BoardBattler b;
    // who_from (fiendish) doesn't matter here
    b.take_dmg_simul(fiendish, "", 10, board1.get(), board2.get());
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

TEST(Battler, GhastcoilerDrattle) {
    auto f = BgCardFactory();
    auto gc = f.get_card("Ghastcoiler");
    gc->set_attack(100);
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 gc
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Ghastcoiler (Golden)")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    BoardBattler().battle_boards(0, board1.get(), board2.get());

    auto b1_cards = board1->get_cards();
    auto b2_cards = board2->get_cards();
    EXPECT_EQ(b1_cards.size(), (unsigned)2);
    EXPECT_EQ(b2_cards.size(), (unsigned)4);
    for (auto c : b1_cards) {
	EXPECT_EQ(c->has_deathrattle(), true);
    }
    for (auto c : b2_cards) {
	EXPECT_EQ(c->has_deathrattle(), true);
    }
}

TEST(Battler, GlyphGuardian) {
    auto f = BgCardFactory();
    auto th = f.get_card("Murloc Tidehunter");
    th->set_health(4 + 8);
    th->set_attack(1);
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Glyph Guardian")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Alleycat"),
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Pyramad"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Murgle"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
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

TEST(Battler, HeraldOfFlame) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Herald Of Flame")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // Herald will chain overkill all the tidehunters
    EXPECT_EQ(res.who_won, "Tess");
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

TEST(Battler, ImpGangBoss) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Imp Gang Boss")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Tess");
    auto p1_res_cards = p1->get_board()->get_cards();
    // Should summon an imp
    EXPECT_EQ(p1_res_cards.size(), (unsigned)2);
}

TEST(Battler, IronhideDirehorn) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Ironhide Direhorn")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(board1->get_cards().size(), (unsigned)2);
    EXPECT_EQ(res.who_won, "Tess");
}


// So similar to ratpack we skip it for now...
// TEST(Battler, InfestedWolfDrattle) {
// }

// So similar to ratpack we skip it for now...
// TEST(Battler, InfestedWolfGoldenDrattle) {
// }

TEST(Battler, Junkbot) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Kaboom Bot"),
	 f.get_card("Junkbot")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(6); // Should attack junkbot twice
    th->set_health(6 + 3 + 3); // 6 from kaboom, 3 and 3 from junkbot
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
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

TEST(Battler, Khadgar) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Imp Gang Boss"),
	 f.get_card("Khadgar")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Tess");
    auto p1_res_cards = p1->get_board()->get_cards();
    // Should summon two imps b/c of khadgar
    EXPECT_EQ(p1_res_cards.size(), (unsigned)4);
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

TEST(Battler, KangorNoMechDeathsDeathrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Kangor")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("King Bagurgle")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

TEST(Battler, KangorOneMechDeathrattle) {
    // Boombot goes first, hits 9 health menace, deals 6 total damage.
    // Menace attacks into Kangor, they both die. Kangor summons bomb,
    // menace summons tokens, which tie.
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Kaboom Bot"),
	 f.get_card("Kangor")
	};
    auto replicating_menace = f.get_card("Replicating Menace");
    replicating_menace->set_attack(6);
    replicating_menace->set_health(9);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 replicating_menace
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

bool compareCards(std::shared_ptr<BgBaseCard> c1,
		  std::shared_ptr<BgBaseCard> c2) {
    return c1->get_name() < c2->get_name();
}

TEST(Battler, KangorTwoMechDeathrattle) {
    // Should summon the harvest golem and damaged golem
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Harvest Golem"),
	 f.get_card("Kangor")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(3);
    th->set_health(10);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Edwin");
    auto b1_cards = p1->get_board()->get_cards();
    EXPECT_EQ(b1_cards.size(), (unsigned)2);
    std::sort(b1_cards.begin(), b1_cards.end(), compareCards);
    EXPECT_TRUE(b1_cards[0]->get_name() == "Damaged Golem");
    EXPECT_TRUE(b1_cards[1]->get_name() == "Harvest Golem");
}

TEST(Battler, KingBagurgleDeathrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("King Bagurgle"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Alleycat"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Mechano-Egg")
	};
    auto high_attack = f.get_card("Murloc Tidehunter");
    high_attack->set_attack(100);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 high_attack
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    auto battled_p1_cards = p1->get_board()->get_cards();
    EXPECT_EQ(battled_p1_cards.size(), (unsigned)5);
    for (auto c : battled_p1_cards) {
	if (c->get_name() == "Murloc Tidehunter") {
	    // Check that the murlocs got buffed. All 2/1 -> 4/3    
	    EXPECT_EQ(c->get_attack(), 4);
	    EXPECT_EQ(c->get_health(), 3);
	}
	else {
	    // Make sure non-murlocs didn't get buffed
	    auto copy = f.get_card(c->get_name());
	    EXPECT_EQ(copy->get_attack(), c->get_attack());
	    EXPECT_EQ(copy->get_health(), c->get_health());
	}
    }
}

TEST(Battler, KingBagurgleGoldenDeathrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("King Bagurgle (Golden)"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Alleycat"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Mechano-Egg")
	};
    auto high_attack = f.get_card("Murloc Tidehunter");
    high_attack->set_attack(100);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 high_attack
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    auto battled_p1_cards = p1->get_board()->get_cards();
    EXPECT_EQ(battled_p1_cards.size(), (unsigned)5);
    for (auto c : battled_p1_cards) {
	if (c->get_name() == "Murloc Tidehunter") {
	    // Check that the murlocs got buffed. All 2/1 -> 6/5
	    EXPECT_EQ(c->get_attack(), 6);
	    EXPECT_EQ(c->get_health(), 5);
	}
	else {
	    // Make sure non-murlocs didn't get buffed
	    auto copy = f.get_card(c->get_name());
	    EXPECT_EQ(copy->get_attack(), c->get_attack());
	    EXPECT_EQ(copy->get_health(), c->get_health());
	}
    }
}

// So similar to MurlocWarleader not going to test
// TEST(Battler, MalGanis) {
// }

TEST(Battler, MamaBear) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Rat Pack"),
	 f.get_card("Mama Bear")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(4);
    // 2 from rp, 5 from each re-sum rat, and each attacks twice (5*2*2)
    // Then 4 from mama bear
    th->set_health(2 + 5*2*2 + 4);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}


// So similar to other basic drattle summons not going to test
// TEST(Battler, MechanoEggDrattle) {
// }

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

TEST(Battler, MecharooGoldenDrattle) {
    auto f = BgCardFactory();
    auto mecharoo = f.get_card("Mecharoo (Golden)");
    auto tidehunter = f.get_card("Murloc Tidehunter"); // No battlecry
    auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
    // These should draw since mecharoo summons a 2/2 token as a drattle
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

TEST(Battler, MonstrousMacaw) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Monstrous Macaw"),
	 f.get_card("Spawn of Nzoth")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_health(7); // 4 from macaw, 3 from spawn that had drattle trig
    th->set_attack(7);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

TEST(Battler, MurlocWarleader) {
    auto f = BgCardFactory();
    auto gambler = f.get_card("Freedealing Gambler");
    gambler->set_health(6 + 5 + 3); // Tidehunter deals 6, next warleader 5, last 3
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Warleader"),
	 f.get_card("Murloc Warleader")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 gambler
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, OldMurkey) {
    auto f = BgCardFactory();
    auto gambler = f.get_card("Freedealing Gambler");
    gambler->set_health(4 + 3 + 2); // As murkeyes die, they deal less dmg
    gambler->set_attack(4);
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Old Murkeye"),
	 f.get_card("Old Murkeye"),
	 f.get_card("Old Murkeye")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 gambler
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, OldMurkeyProperDamage) {
    // Bug where last murkey didn't have damage reduced
    auto f = BgCardFactory();
    auto gambler = f.get_card("Freedealing Gambler");
    // As murkeyes die, they deal less dmg.
    // Added one extra health so gambler/p2 will win.
    gambler->set_health(4 + 3 + 2 + 1);
    gambler->set_attack(4);
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Old Murkeye"),
	 f.get_card("Old Murkeye"),
	 f.get_card("Old Murkeye")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 gambler
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Edwin");
    EXPECT_EQ(res.damage_taken, 3);
}

TEST(Battler, NadinaDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Nadina"),
	 f.get_card("Razorgore")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(4);
    th->set_health(11); // 7 nadina, 2 razor div shield, then 2 razorgore
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, PackLeader) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Rat Pack"),
	 f.get_card("Pack Leader")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(3);
    th->set_health(2 + 3*2 + 3); // 2 from rp, 3x2=6 from the two summoned rats, 3 from packleader
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
}

TEST(Battler, PilotedShredderDrattle) {
    auto f = BgCardFactory();
    auto ps = f.get_card("Piloted Shredder");
    ps->set_attack(6);
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 ps
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Piloted Shredder (Golden)")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    BoardBattler().battle_boards(0, board1.get(), board2.get());

    auto b1_cards = board1->get_cards();
    auto b2_cards = board2->get_cards();
    EXPECT_EQ(b1_cards.size(), (unsigned)1);
    EXPECT_EQ(b2_cards.size(), (unsigned)2);
    for (auto c : b1_cards) {
	EXPECT_EQ(c->get_cost(), 2);
    }
    for (auto c : b2_cards) {
	EXPECT_EQ(c->get_cost(), 2);
    }
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

TEST(Battler, RatPackGoldenDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)")
	};
    auto rp = f.get_card("Rat Pack (Golden)");
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
    BoardBattler b;
    b.take_dmg_simul(rp, "", 2, board2.get(), board1.get());
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

TEST(Battler, RedWhelpPreBattleCondition) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Red Whelp")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "p1"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "p2"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

TEST(Battler, RedWhelpGoldenPreBattleCondition) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Red Whelp (Golden)")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "p1"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "p2"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

TEST(Battler, RipsnarlWithScallywag) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Scallywag"),
	 f.get_card("Ripsnarl Captain")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(3); // Want to kill scallywag, but not ripsnarl
    th->set_health(3 + 3 + 3 + 3); // 3 and 3 from scally, ripsnarl takes two attacks as well
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}


TEST(Battler, ScavengingHyenaIfBeastDies) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Alleycat"),
	 f.get_card("Scavaging Hyena")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(3);
    th->set_health(5);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "p1"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "p2"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

TEST(Battler, ScavengingHyenaIfBeastDiesGolden) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Alleycat"),
	 f.get_card("Scavaging Hyena (Golden)")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(3); // Not enough health to kill golden hyena
    th->set_health(7);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "p1"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "p2"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "p1");
}


// So similar to ratpack we skip it for now...
// TEST(Battler, ReplicatingMenaceDrattle) {
// }

// So similar to ratpack we skip it for now...
// TEST(Battler, ReplicatingMenaceGoldenDrattle) {
// }

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

TEST(Battler, ScallywagGoldenDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards { f.get_card("Scallywag (Golden)"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag"),
							 f.get_card("Scallywag") };
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards { f.get_card("Murloc Tidehunter (Golden)"),
							 f.get_card("Murloc Tidehunter (Golden)") };
    // p1 should be left w/ 6 Scallywags AND (since 4/2 kills either tidehunter, then 2/2 token immediately kills the other
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

TEST(Battler, SecurityRover) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Security Rover")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Tess");
    auto p1_res_cards = p1->get_board()->get_cards();
    // Should summon a guard bot
    EXPECT_EQ(p1_res_cards.size(), (unsigned)2);
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

TEST(Battler, SelflessHeroGoldenDrattle) {
    auto f = BgCardFactory();
    auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
    gambler1->set_health(20); // 4x5 (two divine shields) should give a draw
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Selfless Hero (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)"),
	 f.get_card("Murloc Tidehunter (Golden)")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 gambler1
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_EQ(res.damage_taken, 0);
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

TEST(Battler, SneedsOldShredderDrattle) {
    auto f = BgCardFactory();
    auto sos = f.get_card("Sneed's Old Shredder");
    sos->set_attack(100);
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 sos
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Sneed's Old Shredder (Golden)")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    BoardBattler().battle_boards(0, board1.get(), board2.get());

    auto b1_cards = board1->get_cards();
    auto b2_cards = board2->get_cards();
    EXPECT_EQ(b1_cards.size(), (unsigned)1);
    EXPECT_EQ(b2_cards.size(), (unsigned)2);
    for (auto c : b1_cards) {
	EXPECT_EQ(c->get_rarity(), "LEGENDARY");
    }
    for (auto c : b2_cards) {
	EXPECT_EQ(c->get_rarity(), "LEGENDARY");
    }
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

TEST(Battler, SpawnOfNzothGoldenDrattle) {
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
	 f.get_card("Spawn of Nzoth (Golden)"),
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

TEST(Battler, SoulJuggler) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Imp"),
	 f.get_card("Soul Juggler")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_health(7); // 1 imp, 3 juggle, 3 from juggler attack
    th->set_attack(3);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "draw");
}

TEST(Battler, Taunt) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Voidlord"),
	 f.get_card("Murloc Tidehunter")
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(4);
    th->set_health(4);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Tess");
    auto p1_res_cards = p1->get_board()->get_cards();
    // Both voidwalker and tidehunter should always survive
    EXPECT_EQ(p1_res_cards.size(), (unsigned)2);
}

TEST(Battler, TheBeastDrattle) {
    auto f = BgCardFactory();
    auto card = f.get_card("Murloc Tidehunter");
    card->set_attack(100);
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 card
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("The Beast (Golden)")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    BoardBattler().battle_boards(0, board1.get(), board2.get());

    auto b1_cards = board1->get_cards();
    auto b2_cards = board2->get_cards();
    EXPECT_EQ(b1_cards.size(), (unsigned)1);
    EXPECT_EQ(b2_cards.size(), (unsigned)0);
    for (auto c : b1_cards) {
	EXPECT_EQ(c->get_name(), "Finkle Einhorn");
    }
}

TEST(Battler, TheTideRazorDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("The Tide Razor"),
	};
    auto th = f.get_card("Murloc Tidehunter");
    th->set_attack(4);
    th->set_health(4);
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 th
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Tess");
    EXPECT_GT(res.damage_taken, 3); // 3 one drops and 1 for tav tier
    auto p1_res_cards = p1->get_board()->get_cards();
    EXPECT_EQ(p1_res_cards.size(), (unsigned)3);
    for (auto c : p1_res_cards) {
	EXPECT_EQ(c->get_race(), "PIRATE");
    }
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

TEST(Battler, UnstableGhoulGoldenDrattle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Mecharoo"),
	 f.get_card("Mecharoo"),
	 f.get_card("Mecharoo"),
	 f.get_card("Mecharoo")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Unstable Ghoul (Golden)")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Edwin"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Tess"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // Ghoul goes off twice, so kills mecharoos, then all jo-e-bots
    EXPECT_EQ(res.who_won, "draw");
    EXPECT_LE(res.damage_taken, 0);
}

TEST(Battler, WaxriderTogwaggle) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Red Whelp"),
	 f.get_card("Waxrider Togwaggle")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Alleycat")
	 };
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Pyramad"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Murgle"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    EXPECT_EQ(res.who_won, "Pyramad");
    auto battled_p1_cards = p1->get_board()->get_cards();
    for (auto c : battled_p1_cards) {
	if (c->get_name() == "Red Whelp") {
	}
	else if (c->get_name() == "Waxrider Togwaggle") {
	    EXPECT_EQ(c->get_attack(), 3);
	    EXPECT_EQ(c->get_health(), 4);
	}
	else {
	    // fail
	    EXPECT_EQ(true, false);
	}
    }
}

TEST(Battler, YoHoOgre) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > p1_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Yo-Ho-Ogre")
	};
    std::vector<std::shared_ptr<BgBaseCard> > p2_cards
	{
	 f.get_card("Murloc Tidehunter"),
	 f.get_card("Murloc Tidehunter")
	};
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));
    std::unique_ptr<Player> p1(new Player(board1.get(), "Tess"));
    std::unique_ptr<Player> p2(new Player(board2.get(), "Edwin"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle("p2");
    EXPECT_EQ(res.who_won, "Tess");
    // Murloc tidehunter never attacks since yo-ho attacks back immediately
    EXPECT_EQ(board1->get_cards().size(), (unsigned)2);
}
