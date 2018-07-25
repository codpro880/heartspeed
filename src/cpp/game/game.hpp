/**
 * Class to represent a hearthstone game
 **/
#pragma once

#include <string>
#include <vector>

#include "../cards/BaseCard.hpp"

class Deck {
public:
    Deck(std::vector<BaseCard*> cards) : cards(cards), hsclass("") {}
    BaseCard* get_top_card();
    std::string get_class();
private:
    std::vector<BaseCard*> cards;
    std::string hsclass;
};

// class Player {
// public:
//     Player(Deck* d) : d(d) {}
//     Board* get_board() const;
//     uint8_t get_mana() const;
//     uint8_t get_health() const;
//     std::string get_class() const;
//     void take_damage(int damage);
//     void heal(int damage);
// private:
//     Deck* deck;
//     Board* board;
//     uint8_t mana;
// };

// class HearthstoneGame {
// public:
//     HearthstoneGame(Player* player_one, Player* player_two);
//     Player current_turn() const;
//     void end_current_turn();
//     bool is_over();
// private:
//     void initialize();
// };
