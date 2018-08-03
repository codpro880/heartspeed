#include "player.hpp"

void Player::take_damage(int damage) { // I feel as if this will become complicated
    health -= damage;
}

void Player::heal(int damage) {
    health += damage;
}
