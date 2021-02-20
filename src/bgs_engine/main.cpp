#include <cstdio>
#include <iostream>

#include <time.h>
#include <stdlib.h>

#include "bg_game/player.hpp"
#include "third_party/json.hpp"

void dump_usage() {
    std::cout
        << "This is a stateful CLI tool for simulating bgs games."  << std::endl
        << "Current impl tracks only two players."  << std::endl
        << "Responses are always JSON."  << std::endl
        << std::endl
        << "Usage: " << " (specify only one):" << std::endl
        << std::endl
        << "--all-possible-actions (dumps all actions as JSON)" << std::endl
        << "\t" << "Dumps all actions" << std::endl
        << "--p1-available-actions" << std::endl
        << "\t" << "Dumps player1 available actions" << std::endl
        << "--p2-available-actions" << std::endl
        << "\t" << "Dumps player2 available actions" << std::endl
        << "--p1-state" << std::endl
        << "\t" << "Dumps player1 current state" << std::endl
        << "--p2-state" << std::endl
        << "\t" << "Dumps player2 current state" << std::endl
        << "--p2-state" << std::endl
        << "\t" << "Dumps player2 current state" << std::endl
        << "--p1-take-action [ACTION]" << std::endl
        << "\t" << "Have p1 take an action in the available actions list" << std::endl
        << "--p2-take-action [ACTION]" << std::endl
        << "\t" << "Have p2 take an action in the available actions list" << std::endl
        << "--reset" << std::endl
        << "\t" << "Resets game" << std::endl
        // TODO:
        // << "--set-seed" << std::endl
        // << "\t" << "Sets seed for RNG" << std::endl
        << std::endl;
}

// TODO: 
// nlohmann::json load_settings() {
//     nlohmann::json j;
//     std::string settings_filename("bgscli_settings.json");
//     std::ifstream settings_file(settings_filename);
//     if (!settings_file.good()) {
//         return j;
//     }
//     settings_file >> j;
//     return j;
// }

// void dump_settings(nlohmann::json j) {
//     std::string settings_filename("bgscli_settings.json");
//     std::ofstream settings_file(settings_filename);
//     settings_file << j;
// }

// void set_setting(std::string key, std::string value) {
//     nlohmann::json j = load_settings();
//     j[key] = value;
//     dump_settings(j);
// }

void validate(int num_args, int argc) {
    if (num_args != argc) {
        dump_usage();
        exit(1);
    }
}

void assert_or_die(bool valid_action, std::string msg) {
    if (valid_action) return;
    std::cerr << msg << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc == 1
        || std::string(argv[1]) == "-h"
        || std::string(argv[1]) == "--help"
        || argc > 3) {
        dump_usage();
        exit(1);
    }

    std::string p1_json_filename("p1.json");
    std::string p2_json_filename("p2.json");
    
    if (std::string(argv[1]) == "--reset") {
        std::remove(p1_json_filename.c_str());
        std::remove(p2_json_filename.c_str());
    }

    srand(time(NULL));
    RngSingleton::getInstance(rand());
    
    std::ifstream p1_json_file(p1_json_filename);
    std::ifstream p2_json_file(p2_json_filename);
    Player p1 = Player("Init1");
    Player p2 = Player("Init2");
    if (!p1_json_file.good()) {
        // TODO: Pass in names/hero
        auto p1_json = p1.to_json();
        std::ofstream out_p1(p1_json_filename);
        out_p1 << p1_json;
        auto p2_json = p2.to_json();
        std::ofstream out_p2(p2_json_filename);
        out_p2 << p2_json;
    }
    else {
        p1 = Player::from_json(p1_json_filename);
        p2 = Player::from_json(p2_json_filename);
    }
    if (std::string(argv[1]) == "--all-possible-actions") {
        // TODO: serialize player, load state
        p1.dump_all_possible_actions_json(4);
        // TODO: Serialize
    }
    else if (std::string(argv[1]) == "--p1-available-actions") {
        p1.dump_available_actions_json(4);
    }
    else if (std::string(argv[1]) == "--p2-available-actions") {
        p2.dump_available_actions_json(4);
    }
    else if (std::string(argv[1]) == "--p1-state") {
        p1.dump_as_json(4);
    }
    else if (std::string(argv[1]) == "--p2-state") {
        p2.dump_as_json(4);
    }
    else if (std::string(argv[1]) == "--p1-take-action") {
        validate(3, argc);
        std::string action = std::string(argv[2]);
        bool valid_action = p1.take_action(action);
        assert_or_die(valid_action, "Action " + action + " not in available actions list for p1");
        
        auto p1_json = p1.to_json();
        std::ofstream out_p1(p1_json_filename);
        out_p1 << p1_json;
    }
    else if (std::string(argv[1]) == "--p2-take-action") {
        validate(3, argc);
        std::string action = std::string(argv[2]);
        bool valid_action = p2.take_action(action);
        assert_or_die(valid_action, "Action " + action + " not in available actions list for p2");

        auto p2_json = p2.to_json();
        std::ofstream out_p2(p2_json_filename);
        out_p2 << p2_json;
    }
    else if (std::string(argv[1]) == "--reset") {
        // Handled above
    }
    else {
        std::cout << "Unknown arg: " << argv[1] << std::endl;
    }
}

