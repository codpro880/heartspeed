#include "BgBaseCard.hpp"

std::ostream& operator<<(std::ostream& os, BgBaseCard& card) {
    os << "(" << card.get_attack() << "/" << card.get_health() << ") ";
    return os;
}

void BgBaseCard::take_damage(int damage) {
    if (divine_shield) {
	divine_shield = false;
    }
    else {
	health -= damage;
    }
}

std::shared_ptr<BgBaseCard> BgBaseCard::get_copy() {
    return std::make_shared<BgBaseCard>(*this);
}
