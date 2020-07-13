#include "battler.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
#include "../cards/bgs/BgBaseCard.hpp"

#include "../test/googletest/include/gtest/gtest.h"

#include <memory>

TEST(Battler, CalculatesWinWhenOppEmptyBoard) {
    auto f = BgCardFactory();
    auto tidecaller = f.get_card("Murloc Tidecaller");
    std::vector<BgBaseCard> p1_cards { tidecaller };
    std::vector<BgBaseCard> p2_cards;
    //auto board1 = std::unique_ptr<Board(p1_cards)>;
    std::unique_ptr<Board> board1(new Board(p1_cards));
    std::unique_ptr<Board> board2(new Board(p2_cards));    
    std::unique_ptr<Player> p1(new Player(board1.get()));
    std::unique_ptr<Player> p2(new Player(board2.get()));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battle();
    // EXPECT_EQ(res.who_won, "player1");
    // EXPECT_EQ(res.damage_taken, 3);
}
