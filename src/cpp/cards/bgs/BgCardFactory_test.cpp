/** Random spot checks for the card factory. Not all of the cases are tested. **/

#include "BgCardFactory.hpp"
#include "../../test/googletest/include/gtest/gtest.h"

TEST(BgCardFactory, CanGenerateGoldrinn) {
    auto f = BgCardFactory();
    auto goldrinn = f.get_card("Goldrinn, the Great Wolf");
    EXPECT_EQ(goldrinn.get_name(), "Goldrinn, the Great Wolf");
    EXPECT_EQ(goldrinn.get_health(), 4);
    EXPECT_EQ(goldrinn.get_attack(), 4);
    EXPECT_EQ(goldrinn.get_mechanics(), "['DEATHRATTLE']");
    EXPECT_EQ(goldrinn.get_race(), "BEAST");
    EXPECT_EQ(goldrinn.get_tech_level(), 5);
}

// TEST(CardFactory, CanGenRamWrangler) {
//     auto f = CardFactory();
//     auto ram_wrangler = f.get_card("RamWrangler");
//     EXPECT_EQ(ram_wrangler.get_name(), "RamWrangler");
//     EXPECT_EQ(ram_wrangler.get_class(), "HUNTER");
// }
