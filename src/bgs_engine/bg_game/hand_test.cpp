#include "hand.hpp"

#include "../cards/bgs/BgCardFactory.hpp"

#include "../test/googletest/include/gtest/gtest.h"

TEST(Hand, CanSerializeAndDeserialize) {
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
    auto hand = Hand(cards);

    // Dump to json
    std::string json_file = "hand.json";
    auto json_to_dump = hand.to_json();
    std::ofstream out(json_file);
    out << json_to_dump.dump(4);
    out.close();
    auto desialized = Hand::from_json(json_file);

    auto hand_cards = hand.get_cards();
    auto desialized_cards = desialized.get_cards();
    EXPECT_EQ(hand_cards.size(), desialized_cards.size());
    for (int i = 0; (unsigned)i < hand_cards.size(); i++) {
        EXPECT_EQ(hand_cards[i]->get_name(), desialized_cards[i]->get_name());
    }
}

