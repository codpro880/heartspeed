/* Battler takes two players and simulates the result of the battle */
#pragma once

#include <string>
#include <vector>

#include "player.hpp"

struct BattleResult {
    std::string who_won; // player1 or player2 or draw
    int damage_taken;
};

// Takes two players. Simulates one attack. Two new players -> recursively battle
class Battler {
public:
    Battler(Player* player1, Player* player2) : player1(player1), player2(player2) {}
    BattleResult sim_battle();
private:
    BattleResult sim_battle(Board* b1, Board* b2);
    Player* player1, player2;
}
