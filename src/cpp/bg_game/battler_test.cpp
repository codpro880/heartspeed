#include "battler.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
#include "../cards/bgs/BgBaseCard.hpp"

#include "../test/googletest/include/gtest/gtest.h"

#include <memory>

TEST(Battler, CalculatesWinWhenOppEmptyBoard) {
    auto f = BgCardFactory();
    auto tidecaller = f.get_card("Murloc Tidecaller (Golden)");
    std::vector<BgBaseCard> p1_cards { tidecaller };
    std::vector<BgBaseCard> p2_cards;
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
    auto f = BgCardFactory();
    std::vector<BgBaseCard> p1_cards;
    std::vector<BgBaseCard> p2_cards;
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
    std::vector<BgBaseCard> p1_cards { tidecaller1, tidecaller2, tidecaller3 };
    std::vector<BgBaseCard> p2_cards { tidecaller1, tidecaller2, tidecaller3 };
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
    std::vector<BgBaseCard> p1_cards { tidecaller1, tidecaller2, tidecaller3 };
    std::vector<BgBaseCard> p2_cards { gambler1, gambler2 };
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

// TEST(Battler, CanHandlePoisonCorrectly) {
//     auto f = BgCardFactory();
//     auto tidecaller1 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
//     auto tidecaller2 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
//     auto tidecaller3 = f.get_card("Murloc Tidehunter (Golden)"); // No battlecry
//     // tidecaller1.set_poison();
//     // tidecaller2.set_poison();
//     // tidecaller3.set_poison();
//     auto gambler1 = f.get_card("Freedealing Gambler (Golden)");
//     auto gambler2 = f.get_card("Freedealing Gambler (Golden)");
//     std::vector<BgBaseCard> p1_cards { tidecaller1, tidecaller2, tidecaller3 };
//     std::vector<BgBaseCard> p2_cards { gambler1, gambler2 };
//     std::unique_ptr<Board> board1(new Board(p1_cards));
//     std::unique_ptr<Board> board2(new Board(p2_cards));    
//     std::unique_ptr<Player> p1(new Player(board1.get(), "HookTusk"));
//     std::unique_ptr<Player> p2(new Player(board2.get(), "Pyramad"));
//     auto battler = Battler(p1.get(), p2.get());
//     auto res = battler.sim_battle();
//     EXPECT_EQ(res.who_won, "HookTusk");
//     // One murloc should always live, tier 1
//     EXPECT_EQ(res.damage_taken, 1+1); 
// }
