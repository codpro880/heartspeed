#pragma once

#include <unordered_map>

#include "player.hpp"

// TODO: Make this work properly multithreaded (i.e. make minion buy/sells atomic)
//       For now, to simulate, we can let players buy in a random order
class BobsTavern {
public:
    BobsTavern();
    std::vector<std::string> refresh_minions(Player*); // Reduce players gold by one
    std::vector<std::string> get_current_minions(Player*); // Player arg not necessary for now, but probably will be in future
    void buy_minion(std::string minion, Player*); // Reduces players gold by three, adds minion to hand
private:
    std::unordered_map<int, std::unordered_map<std::string, int>> card_pool;
    std::unordered_map<int, std::vector<std::string>> tier_to_cards_map;
    std::vector<std::string> current_minions;
    std::vector<std::string> begin_turn;
    void init_card_pool();
    void _refresh_minions();
    int tier;
};
