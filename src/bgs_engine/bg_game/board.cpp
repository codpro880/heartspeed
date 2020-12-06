#include "board.hpp"
#include "../cards/bgs/BgBaseCard.hpp"

using namespace std;

int Board::calculate_damage() const {
    int res = 0;
    for (auto card : cards) {
	res += card->get_tavern_tier();
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    auto cards = board.get_cards();
    for (auto c : cards) {
	auto card = c.get();
    	os << (*card) << " ";
    }
    os << std::endl;
    return os;
}