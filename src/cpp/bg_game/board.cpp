#include "board.hpp"
#include "../cards/bgs/BgBaseCard.hpp"

int Board::calculate_damage() {
    int res = 0;
    for (BgBaseCard card : cards) {
	res += card.get_tech_level();
    }
    return res;
}
