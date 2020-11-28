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
