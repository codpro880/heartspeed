#include "BgBaseCard.hpp"

std::ostream& operator<<(std::ostream& os, BgBaseCard& card) {
    os << "(" << card.get_attack() << "/" << card.get_health() << ") ";
    return os;
}

void BgBaseCard::take_damage(int damage) {
    if (has_divine_shield) {
	has_divine_shield = false;
    }
    else {
	health -= damage;
    }
}
