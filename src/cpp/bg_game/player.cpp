#include "player.hpp"

std::ostream& operator<<(std::ostream& os, Player& p) {
    auto cards = p.get_board()->get_cards();
    os << "Player " << p.get_name() << std::endl;
    for (auto c : cards) {
    	os << "(" << c.get_attack() << "/" << c.get_health() << ") ";
    }
    os << std::endl;
    return os;
}
