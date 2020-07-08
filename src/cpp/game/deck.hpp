/**
 * Class to represent a hearthstone game
 **/
#pragma once

#include <string>
#include <vector>

#include "../cards/standard/BaseCard.hpp"

class Deck {
public:
    Deck(std::vector<BaseCard> cards, std::string hsclass) : cards(cards), hsclass(hsclass) {}
    BaseCard get_top_card();
    std::string get_class() const { return hsclass; }
    std::vector<BaseCard> mulligan(std::vector<BaseCard> put_back);
private:
    std::vector<BaseCard> cards;
    std::string hsclass;
};

class DeckFactory {
public:
    DeckFactory() {}
    Deck get_deck(std::string name, bool shuffle=true);
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