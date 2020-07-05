/** Random spot checks for the card factory. Not all of the cases are tested. **/

#include "CardFactory.hpp"
#include "../../test/googletest/include/gtest/gtest.h"

TEST(CardFactory, CanGenerateFlameLance) {
    auto f = CardFactory();
    auto flameLance = f.get_card("FlameLance");
    EXPECT_EQ(flameLance.get_name(), "FlameLance");
    EXPECT_EQ(flameLance.get_class(), "MAGE");
}

TEST(CardFactory, CanGenRamWrangler) {
    auto f = CardFactory();
    auto ram_wrangler = f.get_card("RamWrangler");
    EXPECT_EQ(ram_wrangler.get_name(), "RamWrangler");
    EXPECT_EQ(ram_wrangler.get_class(), "HUNTER");
}
