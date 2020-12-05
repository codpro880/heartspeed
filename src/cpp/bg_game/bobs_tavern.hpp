#pragma once

#include <unordered_map>

#include "player.hpp"

// TODO: Make this work properly multithreaded (i.e. make minion buy/sells atomic)
//       For now, to simulate, we can let players buy in a random order
class BobsTavern {
public:
    BobsTavern(Player*);
    std::vector<std::string> refresh_minions(); // Reduce players gold by one
    std::vector<std::string> get_current_minions(); // Player arg not necessary for now, but probably will be in future
    void buy_minion(std::string minion); // Reduces players gold by three, adds minion to hand
    void buy_minion(int pos); // Reduces players gold by three, adds minion to hand
    void sell_minion(int board_bos); // Removes card from board, gives player a gold
private:
    Player* player;
    std::unordered_map<int, std::unordered_map<std::string, int>> card_pool;
    std::unordered_map<int, std::vector<std::string>> tier_to_cards_map;
    std::vector<std::string> current_minions;
    std::vector<std::string> begin_turn;
    void init_card_pool();
    void _refresh_minions();
    // int tier;
};
