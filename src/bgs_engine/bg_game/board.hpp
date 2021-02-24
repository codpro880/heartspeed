/* Data Class to represent the cards a player currently has in play */
#pragma once

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_set>

#include "../cards/bgs/BgBaseCard.hpp"
#include "../utils/pyutils.hpp"
#include "../third_party/json.hpp"

class Player;

class Board {
public:
    Board(std::vector<std::shared_ptr<BgBaseCard> > cards) : cards(cards), attacker_pos(0) {
        for (auto c : cards) {
            card_names.insert(c->get_name());
        }
    }
    
    Board(Board* b) : attacker_pos(0) {
        cards.clear();
        card_names.clear();
        for (auto c : b->get_cards()) {
            cards.push_back(c->get_copy());
            card_names.insert(c->get_name());
        }
    }

    Board(std::shared_ptr<Board> b) : Board(b.get()) {}

    Board() {}
    
    int calculate_damage() const;
    
    auto empty() const { return cards.empty(); }
    
    auto length() const { return cards.size(); }

    auto size() const { return cards.size(); }

    void swap(int pos1, int pos2) { std::iter_swap(cards.begin() + pos1, cards.begin() + pos2); }
    
    auto operator[](const int& i) { return cards[i]; }
    
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
    
    void remove(BgBaseCard* c) {
        int pos = 0;
        for (auto card : cards) {
            if (c == card.get()) {
                remove(pos);
                return;
            }
            pos++;
        }
    }
    
    void remove(std::shared_ptr<BgBaseCard> c) {
        auto it = std::find(cards.begin(), cards.end(), c);
        cards.erase(it);
        card_names.erase(c->get_name());
    }
    
    void remove(const int& i) {
        if (cards.size() > (unsigned)i) {
            cards.erase(cards.begin() + i);
            card_names.erase(cards[i]->get_name());
        }
    }
    
    auto get_pos(std::shared_ptr<BgBaseCard> c) const {
        auto it = std::find(cards.begin(), cards.end(), c);
        return std::distance(cards.begin(), it);
    }
    
    auto get_pos(BgBaseCard* c) const {
        int pos = 0;
        for (auto card : cards) {
            if (card.get() == c) {
                return pos;
            }
            pos++;
        }
        return -1;
    }
    
    bool contains(std::shared_ptr<BgBaseCard> c) const {
        auto pos = get_pos(c);
        return pos != -1 && (unsigned)pos != cards.size();
    }
    
    void remove_and_mark_dead(Player* p1) {
        std::queue<std::shared_ptr<BgBaseCard> > to_remove;
        for (auto c : cards) {
            if (c->is_dead()) {
                auto death_pos = this->get_pos(c);
                c->set_death_pos(death_pos);
                deathrattle_q.push(c);
                to_remove.push(c);
            }
        }
        while (!to_remove.empty()) {
            auto front = to_remove.front();
            auto death_pos = this->get_pos(front);
            if (death_pos < attacker_pos) {
                attacker_pos--;
            }
            this->remove(front);
            if (front->has_reborn()) {
                front->reborn_self(p1);
                attacker_pos++;
            }
            _has_died.push_back(front);
            to_remove.pop();
        }
    }
    
    void do_deathrattles(Player* p1, Player* p2, Board* b2) {
        bool at_least_one_dead = false;
        while (!deathrattle_q.empty()) {
            at_least_one_dead = true;
            auto card = deathrattle_q.front();
            deathrattle_q.pop();
            card->deathrattle(p1, p2);
        }
        if (at_least_one_dead) {
            // Deathrattles can cause other deaths to occur
            remove_and_mark_dead(p1);
            b2->remove_and_mark_dead(p2);
            do_deathrattles(p1, p2, b2);
            b2->do_deathrattles(p2, p1, this);
        }
    }
    
    int insert_card(int pos,
                    std::shared_ptr<BgBaseCard> c,
                    Player* player,
                    bool from_hand=false);
    
    std::vector<std::shared_ptr<BgBaseCard>> const get_cards() const { return cards;  } // TODO: Make this an iterator
    
    std::vector<std::shared_ptr<BgBaseCard>> has_died() const { return _has_died; }
    
    bool contains(std::string card_name) const { return pyutils::in(card_name, card_names); }

    int get_attacker_pos() { return attacker_pos; }

    void increment_attacker_pos() {
        attacker_pos++;
        clip_attacker_pos();
    }

    void clip_attacker_pos() {
        if ((unsigned)attacker_pos >= length()) {
            attacker_pos = 0;
        }
    }

    std::shared_ptr<BgBaseCard> get_card_by_id(int id) {
        for (auto c : cards) {
            if (c->get_id() == id) {
                return c;
            }
        }
        throw std::logic_error("Card with id " + std::to_string(id) + " not found.");
    }

    nlohmann::json to_json() {
        nlohmann::json j;
        for (int i = 0; (unsigned)i < cards.size(); i++) {
            j["cards"][i] = cards[i]->to_json();
        }
        // I don't think any of the other attrs are ever useful to serialize
        // They're used in battle and cleared at end of battle, or can be calculated
        return j;
    }

    static Board from_json(std::string infile) {
        std::ifstream i(infile);
        nlohmann::json j;
        i >> j;
        return Board::from_json(j);
    }

    static Board from_json(nlohmann::json j) {
        std::vector<std::shared_ptr<BgBaseCard> > deserialized_cards;
        for (auto card_json : j["cards"]) {
            auto card = BgBaseCard::from_json(card_json);
            deserialized_cards.push_back(card);
        }
        auto res_board = Board(deserialized_cards);
        return res_board;
    }
    
private:
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    std::queue<std::shared_ptr<BgBaseCard> > deathrattle_q;
    std::vector<std::shared_ptr<BgBaseCard> > _has_died;
    std::unordered_set<std::string> card_names;
    int attacker_pos;
};
