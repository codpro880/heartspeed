#include "board.hpp"
#include "../cards/bgs/BgBaseCard.hpp"

using namespace std;

int Board::calculate_damage() {
    int res = 0;
    for (auto card : cards) {
	res += card->get_tech_level();
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, Board& board) {
    auto cards = board.get_cards();
    for (auto c : cards) {
	auto card = c.get();
    	os << (*card) << " ";
    }
    os << std::endl;
    return os;
}
