/* Class to represent the player */
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "board.hpp"
#include "hand.hpp"
#include "bobs_tavern.hpp"

#include "../cards/bgs/BgCardFactory.hpp"

class Player {
public:
    Player(std::shared_ptr<Board> board_, std::string name) : board(board_),
                                                              elementals_played_this_turn(0),
                                                              gold(3),
                                                              health(40),
                                                              max_gold(3),
                                                              max_health(40),
                                                              name(name),
                                                              num_free_refreshes(0),
                                                              original_board(std::make_shared<Board>(board_)),
                                                              pirates_bought_this_turn(0),
                                                              tavern_tier(1),
                                                              tavern(std::make_shared<BobsTavern>(this)),
                                                              tavern_is_frozen(false),
                                                              turns_at_current_tier(0) { }

    Player(std::string name) : board(new Board()),
                               elementals_played_this_turn(0),
                               gold(3),
                               health(40),
                               max_gold(3),
                               max_health(40),
                               name(name),
                               num_free_refreshes(0),
                               original_board(new Board()),
                               pirates_bought_this_turn(0),
                               tavern_tier(1),
                               tavern(std::make_shared<BobsTavern>(this)),
                               tavern_is_frozen(false),
                               turns_at_current_tier(0) { }

    Player(Hand hand, std::string name) : board(new Board()),
                                          elementals_played_this_turn(0),
                                          gold(3),
                                          hand(hand),
                                          health(40),
                                          max_gold(3),
                                          max_health(40),
                                          name(name),
                                          num_free_refreshes(0),
                                          original_board(new Board()),
                                          pirates_bought_this_turn(0),
                                          tavern_tier(1),
                                          tavern(std::make_shared<BobsTavern>(this)),
                                          tavern_is_frozen(false),
                                          turns_at_current_tier(0) { }
    
    Player(Player* player) {
        board = std::make_shared<Board>(player->get_original_board());
        hand = player->get_hand();
        health = player->get_health();
        max_health = player->get_max_health();
        name = player->get_name();
        original_board = std::make_shared<Board>(player->get_original_board());
        tavern_tier = player->get_tavern_tier();
        turns_at_current_tier = player->get_turns_at_current_tier();
    }

    // TODO: Impl bobs tav
    int calculate_damage() const { return tavern_tier + board->calculate_damage(); }
    void inc_tavern_tier() { tavern_tier += 1; }
    std::shared_ptr<Board> get_board() const { return board; }
    Hand get_hand() const { return hand; }
    std::shared_ptr<Board> get_original_board() const { return original_board; }
    void set_board(std::shared_ptr<Board> b) { board = b; }
    friend std::ostream& operator<<(std::ostream& os, const Player& p);
    int get_health() const { return health; }
    int get_max_health() const { return max_health; }
    int get_damage_taken() const { return max_health - health; }
    std::string get_name() const { return name; }
    int get_tavern_tier() const { return tavern_tier; }
    void set_tavern_tier(int tav_tier) { tavern_tier = tav_tier; }
    int get_turns_at_current_tier() const { return turns_at_current_tier; }
    void set_free_refreshes(int num_free) { num_free_refreshes = num_free; }

    // Used by steward of time, syndragosa, etc
    void buff_tav_till_refresh(int attack_buff, int health_buff) {
        return tavern->buff_tav_till_refresh(attack_buff, health_buff);
    }

    // Should only be invoked by the nomi card
    void add_to_nomi_tav_count() {
        tavern->inc_nomi_counter();
    }

    void add_card_to_hand(std::shared_ptr<BgBaseCard> card) {
        hand.add_card(card);
    }

    std::shared_ptr<BgBaseCard> remove_card_from_board(int pos) {
        auto card = board->get_cards()[pos];
        board->remove(pos);
        return card;
    }
    
    void play_card(std::shared_ptr<BgBaseCard> card, uint8_t board_pos) {
        auto pos = hand.get_pos(card);
        play_card(pos, board_pos);
    }

    void play_card(uint8_t hand_pos, uint8_t board_pos_or_discover_choice) {
        auto card = hand.get_cards()[hand_pos];
        if (card->is_minion()) {
            play_minion_card(hand_pos, board_pos_or_discover_choice);
        }
        else {
            play_spell_card(hand_pos, board_pos_or_discover_choice);
        }
        if (card->is_golden()) {
            BgCardFactory f;
            auto triple_discover = f.get_card("Triple Discover");
            triple_discover->set_tavern_tier(get_tavern_tier());
            add_card_to_hand(triple_discover);
        }
    }

    void play_card(uint8_t hand_pos, uint8_t target_pos, uint8_t board_pos) {
        if (get_board()->get_cards().size() == (unsigned)7) {
            std::cerr << "WARNING: Board is full" << std::endl;
            return;
        }
        auto card = hand.get_cards()[hand_pos];
        if (card->get_race() == "ELEMENTAL") {
            elementals_played_this_turn += 1;
        }
        // TODO: Enforce valid targets (e.g. MUST pick valid target if available)
        std::shared_ptr<BgBaseCard> target;
        if (card->get_name() == "Faceless Taverngoer" || card->get_name() == "Faceless Taverngoer (Golden)") {
            auto minions = tavern->get_current_minions();
            BgCardFactory f;
            target = f.get_card(minions[target_pos]);
        }
        else {
            target = board->get_cards()[target_pos];
        }
        auto dmg_taken = board->insert_card(board_pos, card, this, true);
        take_damage(dmg_taken);
        card->targeted_battlecry(target, this);
        hand.remove(card);
        if (card->is_golden()) {
            BgCardFactory f;
            auto triple_discover = f.get_card("Triple Discover");
            triple_discover->set_tavern_tier(get_tavern_tier());
            add_card_to_hand(triple_discover);
        }
    }

    void take_damage(int dmg, bool our_turn=false) {
        health -= dmg;
        if (our_turn) {
            floating_watcher_hook(get_board().get(), dmg);
        }
    }

    void freeze_tavern() {
        tavern_is_frozen = true;
    }

    void unfreeze_tavern() {
        tavern_is_frozen = false;
    }

    void add_gold(int g) { set_gold(get_gold() + g); }
    void lose_gold(int g) { set_gold(get_gold() - g); }
    void set_gold(int g) {
        if (g < 0) gold = 0;
        else if (g > 10) gold = 10;
        else gold = g;
    }
    int get_gold() { return gold; }

    // Tavern wrappers
    std::vector<std::string> refresh_tavern_minions() {
        if (num_free_refreshes > 0) {
            num_free_refreshes -= 1;
            return tavern->refresh_minions(true);
        }
        else {
            return tavern->refresh_minions();
        }
    }

    std::vector<std::string> get_tavern_minions() {
        return tavern->get_current_minions();
    }

    void set_tavern_minions(std::vector<std::string> minions) {
        return tavern->set_current_minions(minions);
    }

    void buy_minion(std::string minion) {
        tavern->buy_minion(minion);
        check_for_triples();
    }
    
    void buy_minion(int pos) {
        tavern->buy_minion(pos);
        check_for_triples();
    }
    
    void sell_minion(int board_bos) {
        return tavern->sell_minion(board_bos);
    }
    
    bool tavern_up() {
        auto res =  tavern->tavern_up(turns_at_current_tier);
        turns_at_current_tier = 0;
        return res;
    }

    void start_turn() {
        for (auto c : board->get_cards()) {
            c->set_health(c->get_base_health());
        }
                                           
        for (auto c : board->get_cards()) {
            c->start_turn_mechanic(this);
        }
        // Effects like Djini can cause tripling
        check_for_triples();
        gold = max_gold;
        pirates_bought_this_turn = 0;
        elementals_played_this_turn = 0;
        if (!tavern_is_frozen) {
            tavern->refresh_minions(true); // Free refresh at start of turn, unless frozen
        }
        if (frozen_minions.size() != 0 && !tavern_is_frozen) {
            auto minions = tavern->refresh_minions(true);
            minions.insert(minions.end(), frozen_minions.begin(), frozen_minions.end());
            while (minions.size() > (unsigned)7) {
                minions.erase(minions.begin());
            }
            tavern->set_current_minions(minions);
        }
        tavern_is_frozen = false;
        frozen_minions.clear();
    }
    
    void end_turn() {
        for (auto c : board->get_cards()) {
            c->end_turn_mechanic(this);
        }
        turns_at_current_tier += 1;
        if (max_gold < 10) max_gold++;
        _won_last_turn = false;
    }

    void inc_pirates_bought_this_turn() {
        pirates_bought_this_turn++;
    }

    int get_pirates_bought_this_turn() {
        return pirates_bought_this_turn;
    }

    int get_elementals_played_this_turn() {
        return elementals_played_this_turn;
    }

    void add_to_frozen_minions(std::string minion) {
        frozen_minions.push_back(minion);
    }

    void set_won_last_turn() {
        _won_last_turn = true;
    }

    bool won_last_turn() {
        return _won_last_turn;
    }

    void do_deathrattles(std::shared_ptr<Player> p) {
        return do_deathrattles(p.get());
    }
    
    void do_deathrattles(Player* other) {
        Board* b1 = get_board().get();
        b1->do_deathrattles(this, other, other->get_board().get());
    }

    std::vector<std::shared_ptr<BgBaseCard>> get_board_and_hand(bool golden=false) {
        auto board_and_hand = get_board()->get_cards();
        auto hand_cards = get_hand().get_cards();
        // Concat board/hand
        board_and_hand.insert(board_and_hand.end(),
                              hand_cards.begin(),
                              hand_cards.end());
        if (golden) return board_and_hand;
        
        for (int i = board_and_hand.size() - 1; i >= 0; i--) {
            auto is_golden = board_and_hand[i]->get_name().find("(Golden)") != std::string::npos;
            if (is_golden) {
                board_and_hand.erase(board_and_hand.begin() + i);
            }
        }
        return board_and_hand;
    }

    void check_for_triples() {
        std::unordered_map<std::string, int> minions_counts;
        auto board_and_hand = get_board_and_hand();
        for (auto c : board_and_hand) {
            auto it = minions_counts.find(c->get_name());
            if (it == minions_counts.end()) {
                minions_counts[c->get_name()] = 1;
            }
            else {
                minions_counts[c->get_name()] += 1;
            }
        }

        for (auto p : minions_counts) {
            if (p.second == 3) {
                _triple_minion(p.first);
            }
        }
    }

    std::vector<std::string> list_available_actions() {
        // There are 'basic' actions like FREEZE, TAVERNUP
        // There are positional actions like SELL<CARDNAME>_BOARD<POSITION> or PLAY<CARDNAME>_HAND<POSITION>_BOARD<POSITION>
        // There are targetted actions like PLAY<CARDNAME>_HAND<POSITION>_BOARD<POSITION>_TARGET<POSITION>
        std::vector<std::string> res;        
        // list_sell_actions();
        // list_buy_actions();
        // list_roll_actions();
        // list_freeze_actions();
        // list_play_from_hand_actions();
        // list_board_reposition_actions();
        // list_tavern_up_actions();
        
        // TODO
        // _list_hero_power()
        return res;
    }

    std::vector<std::string> list_freeze_actions() {
        std::vector<std::string> res;
        if (tavern_is_frozen) {
            res.push_back("UNFREEZE");
        }
        else {
            res.push_back("FREEZE");
        }
        return res;
    }

    std::vector<std::string> list_roll_actions() {
        std::vector<std::string> res;
        if (gold > 0 || num_free_refreshes > 0) {
            res.push_back("ROLL");
        }
        return res;
    }

    std::vector<std::string> list_sell_actions() {
        std::vector<std::string> res;
        auto cards = board->get_cards();
        for (size_t i = 0; i < cards.size(); i++) {
            res.push_back("SELL_" + cards[i]->get_name() + "_BOARDPOS_" + std::to_string(i));
        }
        return res;
    }


    void set_opponents_last_board(std::shared_ptr<Board> b) { opponents_last_board = b; }
    std::shared_ptr<Board> get_opponents_last_board() { return opponents_last_board; }
    

private:    
    std::shared_ptr<Board> board;
    int elementals_played_this_turn;
    std::vector<std::string> frozen_minions;
    int gold;
    Hand hand;
    int health;
    int max_gold;
    int max_health;    
    std::string name;
    int num_free_refreshes;
    std::shared_ptr<Board> original_board; // Read-only board
    std::shared_ptr<Board> opponents_last_board; // Read-only board
    int pirates_bought_this_turn;
    int tavern_tier;
    std::shared_ptr<BobsTavern> tavern;
    bool tavern_is_frozen;
    int turns_at_current_tier;
    bool _won_last_turn;

    void _triple_minion(std::string name) {
        std::vector<int> hand_indexes;
        std::vector<int> board_indexes;
        auto hand_cards = get_hand().get_cards();
        auto board_cards = get_board()->get_cards();
        // Typically don't want to remove from a collection that's being iterated over...
        // so calculate indexes to remove and remove later
        for (int i = 0; (unsigned)i < hand_cards.size(); i++) {
            if (hand_cards[i]->get_name() == name) {
                hand_indexes.push_back(i);
            }
        }
        for (int i = 0; (unsigned)i < board_cards.size(); i++) {
            if (board_cards[i]->get_name() == name) {
                board_indexes.push_back(i);
            }
        }

        // Remove by index, not particularly efficient...
        for (int i = hand_indexes.size() - 1; i >= 0; i--) {
            hand.remove(hand_indexes[i]);
        }
        for (int i = board_indexes.size() - 1; i >= 0; i--) {
            board->remove(board_indexes[i]);
        }

        // Add golden to hand
        BgCardFactory f;
        auto gold_card = f.get_card(name + " (Golden)");
        add_card_to_hand(gold_card);
    }

    // TODO: Make this more efficient
    void floating_watcher_hook(Board* b1, int dmg_taken) {
        bool floating_watcher_on_board = b1->contains("Floating Watcher") || b1->contains("Floating Watcher (Golden)");
        if (floating_watcher_on_board) {
            bool malganis_on_board = b1->contains("Mal'ganis") || b1->contains("Mal'ganis (Golden)");
            if (dmg_taken > 0 && !malganis_on_board) {
                for (auto c : b1->get_cards()) {
                    if (c->get_name() == "Floating Watcher") {
                        c->set_attack(c->get_attack() + 2);
                        c->set_health(c->get_health() + 2);
                    }
                    else if (c->get_name() == "Floating Watcher (Golden)") {
                        c->set_attack(c->get_attack() + 4);
                        c->set_health(c->get_health() + 4);
                    }
                }
            }
        }
    }

        void play_minion_card(uint8_t hand_pos, uint8_t board_pos) {
        auto card = hand.get_cards()[hand_pos];
        if (get_board()->get_cards().size() == (unsigned)7) {
            std::cerr << "WARNING: Board is full" << std::endl;
            return;
        }
        if (card->get_race() == "ELEMENTAL") {
            elementals_played_this_turn += 1;
        }
        auto dmg_taken = board->insert_card(board_pos, card, this, true);       
        // Responsible for floating watcher effects...
        // TODO: Make more efficient, does linear searching
        take_damage(dmg_taken, true);
        card->battlecry(this);
        hand.remove(card);
    }

    void play_spell_card(uint8_t hand_pos, uint8_t discover_choice) {
        auto card = hand.get_cards()[hand_pos];
        hand.remove(card);
        return card->cast(this, discover_choice);
        hand.remove(card);
    }

};
