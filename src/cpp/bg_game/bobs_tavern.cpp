#include "bobs_tavern.hpp"
#include "../cards/bgs/BgCardFactory.hpp"

BobsTavern::BobsTavern() : tier(1) {
    init_card_pool();
    _refresh_minions();
    // current_minions = begin_turn();
}

// std::vector<std::string> BobsTavern::begin_turn() {
//     auto num_minions = tier + 2;    
// }

std::vector<std::string> BobsTavern::get_current_minions(Player* p1) {
    // TODO: Make this work on a per-player basis
    return current_minions;
}

std::vector<std::string> BobsTavern::refresh_minions(Player* p1) {
    if (p1->get_gold() == 0) return current_minions;
    p1->lose_gold(1);
    _refresh_minions();
    return current_minions;
}

// TODO: Somehow I doubt this is the most efficient way to do this
void BobsTavern::_refresh_minions() {
    // Return current minions to card pool
    const int MAX_TIER = 6;
    for (int i = 0; i < MAX_TIER; i++) {
	for (auto minion : current_minions) {
	    if (card_pool[i].find(minion) != card_pool[i].end()) {
		card_pool[i][minion] += 1;
	    }
	}
    }
    current_minions.clear();

    // Refresh
    std::vector<std::string> cards;
    for(auto const& imap: card_pool[1]) {
	for (int i = 0; i < imap.second; i++) {
	    cards.push_back(imap.first);
	}
    }
    std::unordered_set<int> indexes;
    while (indexes.size() != tier + 2) {
	auto card_ind = RngSingleton::getInstance().get_rand_int() % cards.size();
	indexes.insert(card_ind);
    }
    for (auto const& idx : indexes) {
	current_minions.push_back(cards[idx]);
    }
    for (auto const& cur_minion : current_minions) {
	card_pool[1][cur_minion] -= 1;
    }
}

void BobsTavern::init_card_pool() {
    BgCardFactory f;
    tier_to_cards_map = f.get_card_names_by_tier();
    for (auto card_name : tier_to_cards_map[1]) {
	card_pool[1][card_name] = 16;
    }
    for (auto card_name : tier_to_cards_map[2]) {
	card_pool[2][card_name] = 15;
    }
    for (auto card_name : tier_to_cards_map[3]) {
	card_pool[3][card_name] = 13;
    }
    for (auto card_name : tier_to_cards_map[4]) {
	card_pool[4][card_name] = 11;
    }
    for (auto card_name : tier_to_cards_map[5]) {
	card_pool[5][card_name] = 9;
    }
    for (auto card_name : tier_to_cards_map[6]) {
	card_pool[6][card_name] = 7;
    }
}

