/** Class responsible for enforcing the rules of both the players. **/
#pragma once

#include "player.hpp"

class Game {
public:
    Game(Player* p1, Player* p2) : p1(p1), p2(p2) {}
    bool is_over() const { return p1->is_dead() || p2->is_dead(); }
    void start_game();

private:
    Player* p1;
    Player* p2;
};
