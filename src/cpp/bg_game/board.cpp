#include "board.hpp"
#include "../cards/bgs/BgBaseCard.hpp"

using namespace std;

int Board::calculate_damage() {
    int res = 0;
    for (BgBaseCard card : cards) {
	res += card.get_tech_level();
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    os << "card" << std::endl;
    return os;
    // for (auto c : board.get_cards()) {
    // 	os << "(" << c.get_attack() << "/" << c.get_health() << ") ";
    // }
    // os << "\n";
    // return os;
}
