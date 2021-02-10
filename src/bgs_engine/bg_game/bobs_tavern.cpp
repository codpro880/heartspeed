#include "bobs_tavern.hpp"
#include "player.hpp"
#include "../cards/bgs/BgCardFactory.hpp"

BobsTavern::BobsTavern(Player* player) : player(player),
                                         nomi_counter(0),
                                         till_refresh_attack_buff(0),
                                         till_refresh_health_buff(0) {
    init_card_pool();
    init_tav_tier_cost();
    _refresh_minions();
}

std::vector<std::string> BobsTavern::get_current_minions() {
    // TODO: Make this work on a per-player basis
    return current_minions;
}

std::vector<std::string> BobsTavern::refresh_minions(bool is_free) {
    if (player->get_gold() == 0) return current_minions;
    
    if (!is_free) {
        player->lose_gold(1);
    }
    _refresh_minions();
    return current_minions;
}

void BobsTavern::buff_tav_till_refresh(int attack_buff, int health_buff) {
    till_refresh_attack_buff += attack_buff;
    till_refresh_health_buff += health_buff;
}

void BobsTavern::buy_minion(int pos) {
    auto minion = current_minions[pos];
    buy_minion(minion);
}

void BobsTavern::buy_minion(std::string minion) {
    if (player->get_gold() < 3) return;
    auto it = std::find(current_minions.begin(), current_minions.end(), minion);
    if (it != current_minions.end()) {
        current_minions.erase(it);
    }
    else {
        return;
    }
    BgCardFactory f;
    auto card = f.get_card(minion);
    card->set_attack(card->get_attack() + till_refresh_attack_buff);
    card->set_health(card->get_health() + till_refresh_health_buff);
    if (card->get_race() == "ELEMENTAL") {
        card->set_attack(card->get_attack() + nomi_counter);
        card->set_health(card->get_health() + nomi_counter);
    }
    player->add_card_to_hand(card);
    if (card->get_race() == "PIRATE") {
        player->inc_pirates_bought_this_turn();
    }
    // TODO: Add special buy mechanics like hogger
    player->lose_gold(3);
    for (auto c : player->get_board()->get_cards()) {
        c->card_bought_trigger(player, card);
    }
}

bool BobsTavern::can_tavern_up(int turns_at_current_tier) {
    auto current_tier = player->get_tavern_tier();
    if (current_tier == 6) return false;
    int player_gold = player->get_gold();
    int upgrade_cost = tav_tier_cost[current_tier + 1] - turns_at_current_tier;
    return player_gold >= upgrade_cost;
}

bool BobsTavern::tavern_up(int turns_at_current_tier) {
    int current_tier = player->get_tavern_tier();
    if (current_tier < 6) {
        int player_gold = player->get_gold();
        int upgrade_cost = tav_tier_cost[current_tier + 1] - turns_at_current_tier;
        if (player_gold >= upgrade_cost) {
            player->lose_gold(upgrade_cost);
            player->inc_tavern_tier();
            return true;
        } else {
            return false;
        }
    } else {
        return false;
  }
}

void BobsTavern::sell_minion(int pos) {
    auto card = player->remove_card_from_board(pos);
    card->on_sell(player);
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
    till_refresh_health_buff = 0;
    till_refresh_attack_buff = 0;

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

void BobsTavern::init_tav_tier_cost() {
    // source: https://hearthstone.gamepedia.com/Battlegrounds
    tav_tier_cost[2] = 5;
    tav_tier_cost[3] = 7;
    tav_tier_cost[4] = 8;
    tav_tier_cost[5] = 9;
    tav_tier_cost[6] = 10;
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

// Should be used for testing only
void BobsTavern::set_current_minions(std::vector<std::string> current_minions) {
    this->current_minions = current_minions;
}

void BobsTavern::inc_nomi_counter() {
    nomi_counter++;
}
