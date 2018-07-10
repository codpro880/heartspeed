#pragma once

#include <string>
#include <unordered_map>

class BaseCard {
public:
    BaseCard(std::string name,
	     int cost,
	     std::string id,
	     std::string text) : name(name), cost(cost), id(id), text(text) {}
    //int get_damage() = 0;
    //int get_health() = 0;
    std::string get_name() { return name; }
    //HsClass get_class() = 0;
    int get_cost() { return cost; }
    std::string get_id() { return id; }
    //HsSet get_set() = 0;
    std::string get_text() { return text; }
    //HsType get_type() = 0;  
    //PlayReqs get_play_requirements() = 0;
private:
    std::string name;
    int cost;
    std::string id;
    std::string text;
};

// class PlayReqs {
// public:
//     PlayReqs(std::unordered_map);
// };
