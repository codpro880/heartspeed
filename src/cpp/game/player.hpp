#pragma once

#include <string>
#include <vector>

#include "../cards/BaseCard.hpp"
#include "deck.hpp"

class Player {
public:
    Player(Deck d) : deck(d) {}
    std::vector<BaseCard> get_board() const;
    std::vector<BaseCard> get_hand() const { return hand; }
    uint8_t get_mana() const { return mana; }
    uint8_t get_health() const { return health; }
    std::string get_class() const { return deck.get_class(); }
    std::vector<std::string> get_allowable_actions() const; // Action class?
    bool is_dead() const { return health <= 0; }
    BaseCard draw_card() { return deck.get_top_card(); }
    void init_hand(std::vector<BaseCard> cards) { hand = cards; }

    void take_damage(int damage);
    void heal(int damage);
    void play_card(std::string card_name, std::string target);
    void attack_with_minion(std::string card_name, std::string target);
private:
    Deck deck;
    std::vector<BaseCard> board;
    std::vector<BaseCard> hand;
    uint8_t mana;
    uint8_t health;
};
