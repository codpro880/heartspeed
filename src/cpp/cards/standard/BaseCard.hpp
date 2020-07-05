#pragma once

#include <string>
#include <unordered_map>

class BaseCard {
public:
    BaseCard(std::string name,
	     int cost,
	     std::string id,
	     std::string text,
	     std::string hsclass) : name(name),
				    cost(cost),
				    id(id),
				    text(text),
				    hsclass(hsclass) {}
    //int get_damage() = 0;
    //int get_health() = 0;
    std::string get_name() { return name; }
    int get_cost() { return cost; }
    std::string get_id() { return id; }
    //HsSet get_set() = 0;
    std::string get_text() { return text; }
    std::string get_class() { return hsclass; }
    //HsType get_type() = 0;  
    //PlayReqs get_play_requirements() = 0;
private:
    std::string name;
    int cost;
    std::string id;
    std::string text;
    std::string hsclass;
};

// class PlayReqs {
// public:
//     PlayReqs(std::unordered_map);
// };
