#include "player.hpp"

std::ostream& operator<<(std::ostream& os, Player& p) {
    auto board = p.get_board();
    os << "Player " << p.get_name() << std::endl;
    os << (*board) << std::endl;
    return os;
}
