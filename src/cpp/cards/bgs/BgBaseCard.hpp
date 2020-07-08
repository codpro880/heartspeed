#pragma once

#include <string>

class BgBaseCard {
public:
    BgBaseCard(int attack,
	     std::string card_class,
	     int cost,
	     int health,
	     std::string id,
	     std::string name,
	     std::string mechanics,
	     std::string race,
	     std::string rarity,
	     int tech_level,
	     std::string text,
	     std::string type) : attack(attack),
				 card_class(card_class),
				 cost(cost),
				 health(health),
				 id(id),
				 mechanics(mechanics),
				 name(name),
				 race(race),
				 rarity(rarity),
				 tech_level(tech_level),
				 text(text),
				 type(type) {}
    int get_attack() { return attack; }
    std::string get_card_class() { return card_class; }
    int get_cost() { return cost; }
    int get_health() { return health; }
    std::string get_id() { return id; }
    std::string get_mechanics() { return mechanics; }
    std::string get_name() { return name; }
    std::string get_race() { return race; }
    std::string get_rarity() { return rarity; }
    int get_tech_level() { return tech_level; }
    std::string get_text() { return text; }
    std::string get_type() { return type; }
private:
    int attack;
    std::string card_class;
    int cost;
    int health;
    std::string id;
    std::string mechanics;
    std::string name;
    std::string race;
    std::string rarity;
    int tech_level;
    std::string text;
    std::string type;
};
