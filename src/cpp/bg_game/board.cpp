#include "board.hpp"

int Board::calculate_damage() {
    int res = 0;
    for (BaseCard card : cards) {
	res += card.get_cost()
    }
    return res
}
