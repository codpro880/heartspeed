/* Class to represent the player */
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "board.hpp"
#include "hand.hpp"
#include "bobs_tavern.hpp"

#include "../cards/bgs/BgCardFactory.hpp"
#include "../third_party/json.hpp"

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
                                                              opponents_last_board(std::make_shared<Board>()),
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
                               original_board(std::make_shared<Board>()),
                               opponents_last_board(std::make_shared<Board>()),
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
                                          original_board(std::make_shared<Board>()),
                                          opponents_last_board(std::make_shared<Board>()),
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
    void set_health(int _health) { health = _health; }
    int get_max_health() const { return max_health; }
    void set_max_health(int _max_health) { max_health = _max_health; }
    int get_damage_taken() const { return max_health - health; }
    std::string get_name() const { return name; }
    int get_tavern_tier() const { return tavern_tier; }
    void set_tavern_tier(int tav_tier) { tavern_tier = tav_tier; }
    int get_turns_at_current_tier() const { return turns_at_current_tier; }
    void set_turns_at_current_tier(int t) { turns_at_current_tier = t; }
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

    bool is_tavern_frozen() const {
        return tavern_is_frozen;
    }

    void set_tavern_frozen(bool t) {
        tavern_is_frozen = t;
    }

    void add_gold(int g) { set_gold(get_gold() + g); }
    void lose_gold(int g) { set_gold(get_gold() - g); }
    void set_gold(int g) {
        if (g < 0) gold = 0;
        else if (g > 10) gold = 10;
        else gold = g;
    }
    void set_max_gold(int g) {
        if (g < 0) max_gold = 0;
        else if (g > 10) max_gold = 10;
        else max_gold = g;
    }
    int get_gold() const { return gold; }
    int get_max_gold() const { return max_gold; }

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

    int get_pirates_bought_this_turn() const {
        return pirates_bought_this_turn;
    }

    void set_pirates_bought_this_turn(int p) {
        pirates_bought_this_turn = p;
    }

    int get_elementals_played_this_turn() const {
        return elementals_played_this_turn;
    }

    void set_elementals_played_this_turn(int e) {
        elementals_played_this_turn = e;
    }

    void add_to_frozen_minions(std::string minion) {
        frozen_minions.push_back(minion);
    }

    std::vector<std::string> get_frozen_minions() const {
        return frozen_minions;
    }

    void set_frozen_minions(std::vector<std::string> minions) {
        frozen_minions = minions;
    }

    void set_won_last_turn(bool wlt = true) {
        _won_last_turn = wlt;
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
        // One day we'll have "extend" like python...until then...
        for (auto action : list_board_reposition_actions()) res.push_back(action);
        for (auto action : list_buy_actions()) res.push_back(action);
        for (auto action : list_freeze_actions()) res.push_back(action);
        for (auto action : list_play_from_hand_actions()) res.push_back(action);
        for (auto action : list_roll_actions()) res.push_back(action);
        for (auto action : list_sell_actions()) res.push_back(action);
        for (auto action : list_tavern_up_actions()) res.push_back(action);
        
        // TODO
        // _list_hero_power()
        return res;
    }

    std::vector<std::string> list_tavern_up_actions() {
        std::vector<std::string> res;
        if (tavern->can_tavern_up(turns_at_current_tier)) {
            res.push_back("TAVERN_UP");
        }
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
            res.push_back("SELL_" + std::to_string(i));
        }
        return res;
    }

    std::vector<std::string> list_board_reposition_actions() {
        std::vector<std::string> res;
        auto cards = board->get_cards();
        for (size_t x = 0; x < cards.size(); x++) {
            for (size_t y = 0; y < cards.size(); y++) {
                if (x == y) continue;
                res.push_back("REPOSITION_FROM_"
                              + std::to_string(x)
                              + "_TO_"
                              + std::to_string(y));
            }
        }
        return res;
    }


    std::vector<std::string> list_play_from_hand_actions() {
        std::vector<std::string> res;
        if (board->get_cards().size() == (unsigned)7) {
            return res;
        }
        auto hand_cards = hand.get_cards();
        for (size_t hand_ind = 0; hand_ind <  hand_cards.size(); hand_ind++) {
            for (size_t board_ind = 0; board_ind <= board->get_cards().size(); board_ind++) {
                auto target_indexes = hand_cards[hand_ind]->get_valid_target_indexes(this);
                if (target_indexes.size() > 0) {
                    for (const auto& target_index : target_indexes) {
                        res.push_back("PLAY_CARD_FROM_HAND_"
                                      + std::to_string(hand_ind)
                                      + "_TO_BOARD_"
                                      + std::to_string(board_ind)
                                      + "_TARGET_"
                                      + std::to_string(target_index));
                    }
                }
                else {
                    res.push_back("PLAY_CARD_FROM_HAND_"
                                  + std::to_string(hand_ind)
                                  + "_TO_BOARD_"
                                  + std::to_string(board_ind));
                }
            }
        }
        return res;
    }

    std::vector<std::string> list_buy_actions() {
        std::vector<std::string> res;
        if (hand.get_cards().size() == (unsigned)10) return res;
        for (int i = 0; (unsigned)i < get_tavern_minions().size(); i++) {
            res.push_back("BUY_" + std::to_string(i));
        }
        return res;
    }

    void dump_all_possible_actions_json() {
        nlohmann::json j;
        auto all_possible_actions = list_all_possible_actions();
        for (size_t action_ind = 0; action_ind < all_possible_actions.size(); action_ind++) {
            j["all_possible_actions"][action_ind] = all_possible_actions[action_ind];
        }
        std::cout << j.dump(4);
    }

    std::vector<std::string> list_all_possible_actions() {
        std::vector<std::string> res =
            {
             "FREEZE",
             "UNFREEZE",
             "ROLL",
             "BUY_0",
             "BUY_1",
             "BUY_2",
             "BUY_3",
             "BUY_4",
             "BUY_5",
             "BUY_6",
             "SELL_0",
             "SELL_1",
             "SELL_2",
             "SELL_3",
             "SELL_4",
             "SELL_5",
             "SELL_6",
             "TAVERN_UP",
            };
        
        // Generates all actions of the form "PLAY_FROM_HAND_<X>_TO_BOARD_<Y>"
        for (int x = 0; x < 7; x++) {
            for (int y = 0; y < 7; y++) {
                res.push_back("PLAY_CARD_FROM_HAND_"
                              + std::to_string(x)
                              + "_TO_BOARD_"
                              + std::to_string(y));
            }
        }
        
        // Generates all actions of the form "PLAY_FROM_HAND_<X>_TO_BOARD_<Y>_TARGET_<Z>"
        for (int x = 0; x < 7; x++) {
            for (int y = 0; y < 7; y++) {
                for (int z = 0; z < 7; z++) {
                    res.push_back("PLAY_CARD_FROM_HAND_"
                                  + std::to_string(x)
                                  + "_TO_BOARD_"
                                  + std::to_string(y)
                                  + "_TARGET_"
                                  + std::to_string(z));
                }
            }
        }

        // Generates all actions of the form "REPOSITION_FROM_<X>_TO_<Y>"
        for (int x = 0; x < 7; x++) {
            for (int y = 0; y < 7; y++) {
                if (x == y) continue;
                res.push_back("REPOSITION_FROM_"
                              + std::to_string(x)
                              + "_TO_"
                              + std::to_string(y));
            }
        }
        
        return res;
    }
    

    std::shared_ptr<Board> get_opponents_last_board() const { return opponents_last_board; }
    void set_opponents_last_board(std::shared_ptr<Board> b) { opponents_last_board = b; }
    

    int get_num_free_refreshes() const {
        return num_free_refreshes;
    }

    void set_num_free_refreshes(int n) {
        num_free_refreshes = n;
    }

    nlohmann::json to_json() {
    //         std::shared_ptr<Board> board;
    // int elementals_played_this_turn;
    // std::vector<std::string> frozen_minions;
    // int gold;
    // Hand hand;
    // int health;
    // int max_gold;
    // int max_health;    
    // std::string name;
    // int num_free_refreshes;
    // std::shared_ptr<Board> original_board; // Read-only board
    // std::shared_ptr<Board> opponents_last_board; // Read-only board
    // int pirates_bought_this_turn;
    // int tavern_tier;
    // std::shared_ptr<BobsTavern> tavern;
    // bool tavern_is_frozen;
    // int turns_at_current_tier;
    // bool _won_last_turn;
        nlohmann::json j;
        j["board"] = board->to_json();
        j["hand"] = hand.to_json();
        j["elementals_played_this_turn"] = elementals_played_this_turn;
        j["frozen_minions"] = frozen_minions;
        j["gold"] = gold;
        j["max_gold"] = max_gold;
        j["health"] = health;
        j["max_health"] = max_health;
        j["name"] = name;
        j["num_free_refreshes"] = num_free_refreshes;
        j["opponents_last_board"] = opponents_last_board->to_json();
        j["pirates_bought_this_turn"] = pirates_bought_this_turn;
        j["tavern_tier"] = tavern_tier;
        j["tavern_minions"] = get_tavern_minions();
        j["tavern_is_frozen"] = tavern_is_frozen;
        j["turns_at_current_tier"] = turns_at_current_tier;
        j["_won_last_turn"] = _won_last_turn;
        return j;
    }

    static Player from_json(std::string infile) {
        std::ifstream i(infile);
        nlohmann::json j;
        i >> j;
        return Player::from_json(j);
    }

    static Player from_json(nlohmann::json j) {
        auto board = std::make_shared<Board>(Board::from_json(j["board"]));
        auto hand = Hand::from_json(j["hand"]);
        auto elementals_played_this_turn = j["elementals_played_this_turn"];
        auto frozen_minions = j["frozen_minions"];
        auto gold = j["gold"];
        auto max_gold = j["max_gold"];
        auto health = j["health"];
        auto max_health = j["max_health"];
        auto name = j["name"];
        auto num_free_refreshes = j["num_free_refreshes"];
        auto opponents_last_board = std::make_shared<Board>(Board::from_json(j["opponents_last_board"]));
        auto pirates_bought_this_turn = j["pirates_bought_this_turn"];
        auto tavern_tier = j["tavern_tier"];
        auto tavern_minions = j["tavern_minions"];
        auto tavern_is_frozen = j["tavern_is_frozen"];
        auto turns_at_current_tier = j["turns_at_current_tier"];
        auto _won_last_turn = j["_won_last_turn"];

        auto player = Player(hand, name);
        player.set_board(board);
        player.set_elementals_played_this_turn(elementals_played_this_turn);
        player.set_frozen_minions(frozen_minions);
        player.set_gold(gold);
        player.set_max_gold(max_gold);
        player.set_health(health);
        player.set_max_health(max_health);
        player.set_num_free_refreshes(num_free_refreshes);
        player.set_opponents_last_board(opponents_last_board);
        player.set_pirates_bought_this_turn(pirates_bought_this_turn);
        player.set_tavern_tier(tavern_tier);
        player.set_tavern_minions(tavern_minions);
        player.set_tavern_frozen(tavern_is_frozen);
        player.set_turns_at_current_tier(turns_at_current_tier);
        player.set_won_last_turn(_won_last_turn);

        return player;
    }
    

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
