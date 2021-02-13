/** Random spot checks for the card factory. Not all of the cases are tested. **/
#include <fstream>

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

// This is not an all inclusive test for the edge cases
// If bugs crop up, we'll write a test to fix and build from there
TEST(BgBaseCard, CanBeSerializedToJsonAndDeserializedAgain) {
    BgCardFactory f;
    auto original_card = f.get_card("Amalgadon");
    original_card->set_poison();
    original_card->set_divine_shield();
    original_card->set_taunt();
    std::string json_file = "amalgadon.json";
    auto json_to_dump = original_card->to_json();
    std::ofstream out(json_file);
    out << json_to_dump.dump(4);
    out.close();
    auto desialized = std::make_unique<BgBaseCard>(BgBaseCard::from_json(json_file));

    EXPECT_EQ(original_card->get_attack(), desialized->get_attack());
    EXPECT_EQ(original_card->get_base_attack(), desialized->get_base_attack());
    EXPECT_EQ(original_card->get_card_class(), desialized->get_card_class());
    EXPECT_EQ(original_card->get_cost(), desialized->get_cost());
    EXPECT_EQ(original_card->get_health(), desialized->get_health());
    EXPECT_EQ(original_card->get_base_health(), desialized->get_base_health());
    EXPECT_EQ(original_card->get_mechanics(), desialized->get_mechanics());
    EXPECT_EQ(original_card->get_name(), desialized->get_name());
    EXPECT_EQ(original_card->get_race(), desialized->get_race());
    EXPECT_EQ(original_card->get_rarity(), desialized->get_rarity());
    EXPECT_EQ(original_card->get_tavern_tier(), desialized->get_tavern_tier());
    EXPECT_EQ(original_card->get_type(), desialized->get_type());

    EXPECT_EQ(original_card->has_battlecry(), desialized->has_battlecry());
    EXPECT_EQ(original_card->has_cleave(), desialized->has_cleave());
    EXPECT_EQ(original_card->has_deathrattle(), desialized->has_deathrattle());
    EXPECT_EQ(original_card->has_divine_shield(), desialized->has_divine_shield());
    EXPECT_EQ(original_card->has_poison(), desialized->has_poison());
    EXPECT_EQ(original_card->has_reborn(), desialized->has_reborn());
    EXPECT_EQ(original_card->has_taunt(), desialized->has_reborn());  
    EXPECT_EQ(original_card->has_windfury(), desialized->has_windfury());
    EXPECT_EQ(original_card->has_windfury_active(), desialized->has_windfury_active());
}
