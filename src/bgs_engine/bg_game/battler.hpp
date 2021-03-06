/* Battler takes two players and simulates the result of the battle */
#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "../third_party/json.hpp"

#include "player.hpp"
#include "board.hpp"

struct BattleResult {
    std::string who_won; // player1 or player2 or draw
    int damage_taken;
    std::vector<std::pair<Board, Board>> frames;
    std::vector<int> attacker_pos;
    std::vector<int> defender_pos;
    std::vector<int> b1_turn;
};

struct BattleResults {
    double p1_win;
    double draw;
    double p2_win;
};

void to_json(nlohmann::json& j, const BattleResult& br);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BattleResults, p1_win, draw, p2_win)

class BoardBattler {
public:
    BoardBattler() : first_combat(true) {}
    std::tuple<bool, bool, int, int> battle_boards(int attacker_pos, Player*, Player*);
    std::tuple<bool, bool, int, int> battle_boards(int attacker_pos, std::shared_ptr<Player>, std::shared_ptr<Player>);
    void pre_combat(Player* p1, Player* p2);
    void post_battle(Player*, Player*, std::vector<std::shared_ptr<BgBaseCard> >, std::vector<std::shared_ptr<BgBaseCard> >);
    void take_dmg_simul(std::shared_ptr<BgBaseCard> card, std::string who_from_race, int dmg, Player*, Player*);
    void take_dmg_simul(std::shared_ptr<BgBaseCard> attacker, std::shared_ptr<BgBaseCard> defender, Player*, Player*);
    void take_dmg_simul(std::vector<std::shared_ptr<BgBaseCard>> cards, std::vector<std::string> who_from_race, int dmg, Player*, Player*);
    void take_dmg_simul(std::vector<std::shared_ptr<BgBaseCard>> cards, std::vector<std::string> who_from_race,  std::vector<int> dmg, Player*, Player*);
private:
    bool first_combat;
};

// Takes two players. Simulates one attack. Two new players -> recursively battle
class Battler {
public:
    Battler(Player* p1, Player* p2, bool debug=false) : p1(p1), p2(p2), debug(debug), owns(false) {}
    Battler(Player _p1, Player _p2, bool debug=false) : p1(new Player(_p1)), p2(new Player(_p2)), debug(debug), owns(true) {}
    ~Battler() {
        if (owns) {
            delete p1;
            delete p2;
        }
    }
    BattleResult sim_battle(std::string goes_first="null"); // specify "p1" or "p2" for goes_first
    BattleResults sim_battles(int num_battles=1000);
    BattleResults sim_battles_par(int num_battles=1000);
private:
    BattleResult sim_battle(Player* p1, Player* p2, std::string goes_first);
    std::string decide_who_goes_first(Board* b1, Board* b2);
    std::string decide_who_goes_first(std::shared_ptr<Board> b1, std::shared_ptr<Board> b2);
    BattleResult battle(Player* p1,
                        Player* p2,
                        int p1_counter=0,
                        int p2_counter=0);
    Player* p1;
    Player* p2;
    BoardBattler board_battler;
    bool debug;
    bool owns; // Whether or not we own the player pointers....
};

class BattleFrameJsonDump {
public:
    void dump_to_json(BattleResult bres, std::string filename) {
        nlohmann::json j;
        /**
           [
               {
                   "b1": {
                         "MurlocTH": {
                              "attack": 1,
                              "health": 1
                             }
                         }
               }
           ]
         **/
        std::vector<std::pair<Board, Board>> frames(bres.frames);
        int frame_ind = 0;
        for (auto frame : frames) {
            auto board1 = frame.first;
            int card_ind = 0;
            j[frame_ind]["b1_turn"] = bres.b1_turn[frame_ind];
            j[frame_ind]["attacker_pos"] = bres.attacker_pos[frame_ind];
            j[frame_ind]["defender_pos"] = bres.defender_pos[frame_ind];
            for (auto card : board1.get_cards()) {
                j[frame_ind]["b1"][card_ind] = card->to_json();
                card_ind++;
            }
            auto board2 = frame.second;
            card_ind = 0;
            for (auto card : board2.get_cards()) {
                j[frame_ind]["b2"][card_ind] = card->to_json();
                card_ind++;
            }
            frame_ind++;
        }
        std::ofstream out(filename);
        out << j.dump(4);
    }
};
