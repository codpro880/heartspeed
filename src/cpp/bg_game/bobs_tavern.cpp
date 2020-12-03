#include "bobs_tavern.hpp"

BobsTavern::BobsTavern() : tier(1) {
    init_minions();
    // current_minions = begin_turn();
}

// std::vector<std::string> BobsTavern::begin_turn() {
//     auto num_minions = tier + 2;    
// }

std::vector<std::string> BobsTavern::get_current_minions(Player* p1) {
    return current_minions;
}

void BobsTavern::init_minions() {
}

