#include "bobs_tavern.hpp"
#include "../cards/bgs/BgCardFactory.hpp"

BobsTavern::BobsTavern(Player* player) : player(player) {
    init_card_pool();
    _refresh_minions();
    // current_minions = begin_turn();
}

// std::vector<std::string> BobsTavern::begin_turn() {
//     auto num_minions = tier + 2;    
// }

std::vector<std::string> BobsTavern::get_current_minions() {
    // TODO: Make this work on a per-player basis
    return current_minions;
}

std::vector<std::string> BobsTavern::refresh_minions() {
    if (player->get_gold() == 0) return current_minions;
    player->lose_gold(1);
    _refresh_minions();
    return current_minions;
}

void BobsTavern::buy_minion(int pos) {
    auto minion = current_minions[pos];
    buy_minion(minion);
}

void BobsTavern::buy_minion(std::string minion) {
    auto it = std::find(current_minions.begin(), current_minions.end(), minion);
    if (it != current_minions.end()) {
	current_minions.erase(it);
    }
    else {
	return;
    }
    BgCardFactory f;
    auto card = f.get_card(minion);
    player->add_card(card);
    // TODO: Add special buy mechanics like hogger
    player->lose_gold(3);
}

void BobsTavern::sell_minion(int pos) {    
    auto card = player->remove_card(pos);
    card->on_sell(player);
    // player->add_gold(1);
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
    for (int tt = 1; tt < player->get_tavern_tier() + 1; tt++) {
	for(auto const& imap: card_pool[tt]) {
	    for (int i = 0; i < imap.second; i++) {
		cards.push_back(imap.first);
	    }
	}
    }
    std::unordered_set<int> indexes;
    while (indexes.size() != player->get_tavern_tier() + 2) {
	auto card_ind = RngSingleton::getInstance().get_rand_int() % cards.size();
	indexes.insert(card_ind);
    }
    for (auto const& idx : indexes) {
	current_minions.push_back(cards[idx]);
    }
    BgCardFactory f; // TODO: Perf issue, maybe make tav hold bg cards, not strings
    for (auto const& cur_minion : current_minions) {
	int tav_tier = f.get_card(cur_minion)->get_tavern_tier();
	card_pool[tav_tier][cur_minion] -= 1;
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

