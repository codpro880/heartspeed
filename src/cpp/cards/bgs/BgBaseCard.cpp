#include "BgBaseCard.hpp"

#include "../../bg_game/board.hpp"

std::ostream& operator<<(std::ostream& os, BgBaseCard& card) {
    os << "(" << card.get_attack() << "/" << card.get_health() << ")";
    if (card.has_divine_shield()) {
	os << "D";
    }
    os << " ";
    return os;
}

void BgBaseCard::take_damage(int damage, Board* b1, Board* b2) {
    if (divine_shield) {
	divine_shield = false;
    }
    else {
	health -= damage;
    }
    //deal_with_death(b1, b2);
}

std::shared_ptr<BgBaseCard> BgBaseCard::get_copy() {
    return std::make_shared<BgBaseCard>(*this);
}

void BgBaseCard::deal_with_death(Board* b1, Board* b2) {
    // std::cerr << "Checking if dead." << std::endl;
    // if (this->is_dead()) {
    // 	death_pos = b1->get_pos(this);
    // 	std::cerr << "Dead." << std::endl;
    // 	b1->remove(this);
    // 	do_deathrattle(b1, b2);
    // }
}

void BgBaseCard::do_deathrattle(Board* b1, Board* b2) {
    
}
