/* Battler takes two players and simulates the result of the battle */
#pragma once

#include <memory>
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
    Battler(Player* p1, Player* p2) : p1(p1), p2(p2) {}
    BattleResult sim_battle();
private:
    BattleResult sim_battle(Player* p1, Player* p2);
    std::string decide_who_goes_first(Board* b1, Board* b2);
    BattleResult battle(Player* p1,
			Player* p2,
			int p1_counter=0,
			int p2_counter=0);
    Player* p1;
    Player* p2;
};

class BoardBattler {
public:
    void battle_boards(int attacker_pos, Board* b1, Board* b2);
};
