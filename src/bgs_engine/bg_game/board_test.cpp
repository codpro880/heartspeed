#include "board.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
#include "../cards/bgs/BgBaseCard.hpp"
#include "../test/googletest/include/gtest/gtest.h"

TEST(Board, CanCalculateDamageEmptyBoard) {
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    auto board = Board(cards);
    EXPECT_EQ(board.calculate_damage(), 0);
}

TEST(Board, CanCalculateDamageOneCard) {
    auto f = BgCardFactory();
    auto goldrinn = f.get_card("Goldrinn");
    std::vector<std::shared_ptr<BgBaseCard> > cards{ goldrinn };
    auto board = Board(cards);
    EXPECT_EQ(board.calculate_damage(), 6);
}

TEST(Board, CanCalculateDamageWithFullBoard) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > cards{
        f.get_card("Alleycat (Golden)"), // Tier 1
            f.get_card("Imprisoner (Golden)"), // Tier 2
            f.get_card("Houndmaster (Golden)"), // Tier 3
            f.get_card("Goldgrubber (Golden)"), // Tier 4
            f.get_card("Mal'Ganis (Golden)"), // Tier 5
            f.get_card("Foe Reaper 4000"), // Tier 6
        };
    auto board = Board(cards);
    EXPECT_EQ(board.calculate_damage(), 1+2+3+4+5+6);
}

TEST(Board, CanSerializeAndDeserializeBoard) {
    auto f = BgCardFactory();
    std::vector<std::shared_ptr<BgBaseCard> > cards
        {
         f.get_card("Alleycat (Golden)"), // Tier 1
         f.get_card("Imprisoner (Golden)"), // Tier 2
         f.get_card("Houndmaster (Golden)"), // Tier 3
         f.get_card("Goldgrubber (Golden)"), // Tier 4
         f.get_card("Mal'Ganis (Golden)"), // Tier 5
         f.get_card("Foe Reaper 4000"), // Tier 6
        };
    auto board = Board(cards);

    // Dump to json
    std::string json_file = "board.json";
    auto json_to_dump = board.to_json();
    std::ofstream out(json_file);
    out << json_to_dump.dump(4);
    out.close();
    auto desialized = std::make_unique<Board>(Board::from_json(json_file));

    auto board_cards = board.get_cards();
    auto desialized_cards = desialized->get_cards();
    EXPECT_EQ(board_cards.size(), desialized_cards.size());
    for (int i = 0; (unsigned)i < board_cards.size(); i++) {
        EXPECT_EQ(board_cards[i]->get_name(), desialized_cards[i]->get_name());
    }
}
