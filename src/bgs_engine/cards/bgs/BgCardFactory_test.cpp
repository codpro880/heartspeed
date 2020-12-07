/** Random spot checks for the card factory. Not all of the cases are tested. **/

#include "BgCardFactory.hpp"
#include "../../test/googletest/include/gtest/gtest.h"

TEST(BgCardFactory, CanGenerateGoldrinn) {
    auto f = BgCardFactory();
    auto goldrinn = f.get_card("Goldrinn");
    EXPECT_EQ(goldrinn->get_name(), "Goldrinn");
    EXPECT_EQ(goldrinn->get_health(), 4);
    EXPECT_EQ(goldrinn->get_attack(), 4);
    EXPECT_EQ(goldrinn->get_mechanics(), "['DEATHRATTLE']");
    EXPECT_EQ(goldrinn->get_race(), "BEAST");
    EXPECT_EQ(goldrinn->get_tavern_tier(), 6);
}

TEST(BgCardFactory, CanGenerateParrot) {
    auto f = BgCardFactory();
    auto parrot = f.get_card("Monstrous Macaw");
    EXPECT_EQ(parrot->get_name(), "Monstrous Macaw");
    EXPECT_EQ(parrot->get_race(), "BEAST");
    EXPECT_EQ(parrot->get_tavern_tier(), 3);
}

bool contains(std::vector<std::string> v, std::string item) {
    return std::find(v.begin(), v.end(), item) != v.end();
}

TEST(BgCardFactory, CanListCardsAtEachTier) {
    auto f = BgCardFactory();
    auto tier_to_card_name_map = f.get_card_names_by_tier();

    // Spot check containment
    auto tier1 = tier_to_card_name_map[1];
    EXPECT_TRUE(contains(tier1, "Deck Swabbie"));
    EXPECT_TRUE(contains(tier1, "Sellemental"));
    EXPECT_TRUE(contains(tier1, "Alleycat"));

    auto tier2 = tier_to_card_name_map[2];
    EXPECT_TRUE(contains(tier2, "Kaboom Bot"));
    EXPECT_TRUE(contains(tier2, "Unstable Ghoul"));
    EXPECT_TRUE(contains(tier2, "Steward of Time"));

    auto tier3 = tier_to_card_name_map[3];
    EXPECT_TRUE(contains(tier3, "Coldlight Seer"));
    EXPECT_TRUE(contains(tier3, "Felfin Navigator"));
    EXPECT_TRUE(contains(tier3, "The Beast"));

    auto tier4 = tier_to_card_name_map[4];
    EXPECT_TRUE(contains(tier4, "Cave Hydra"));
    EXPECT_TRUE(contains(tier4, "Toxfin"));
    EXPECT_TRUE(contains(tier4, "Goldgrubber"));

    auto tier5 = tier_to_card_name_map[5];
    EXPECT_TRUE(contains(tier5, "Voidlord"));
    EXPECT_TRUE(contains(tier5, "Murozond"));
    EXPECT_TRUE(contains(tier5, "Mama Bear"));

    auto tier6 = tier_to_card_name_map[6];
    EXPECT_TRUE(contains(tier6, "Amalgadon"));
    EXPECT_TRUE(contains(tier6, "Imp Mama"));
    EXPECT_TRUE(contains(tier6, "The Tide Razor"));
}
