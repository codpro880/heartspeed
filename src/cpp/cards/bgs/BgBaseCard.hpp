#pragma once

#include <iostream>
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
				 is_poison(mechanics.find("POISON") != std::string::npos),
				 mechanics(mechanics),
				 name(name),
				 race(race),
				 rarity(rarity),
				 tech_level(tech_level),
				 text(text),
				 type(type) {}

    // BgBaseCard(BgBaseCard c, new_health) : attack(c.attack),
    // 				 card_class(c.card_class),
    // 				 cost(c.cost),
    // 				 health(new_health),
    // 				 id(c.id),
    // 				 mechanics(c.mechanics),
    // 				 name(c.name),
    // 				 race(c.race),
    // 				 rarity(c.rarity),
    // 				 tech_level(c.tech_level),
    // 				 text(c.text),
    // 				 type(type) {}
    
    int get_attack() { return is_poison ? 999999 : attack; } // Poison is like 'infinite' attack
    // int get_attack() { return attack; }
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

    void set_attack(int att) { attack = att; }
    void set_health(int hth) { health = hth; }
    void set_poison() { is_poison = true; }

    friend std::ostream& operator<<(std::ostream& os, BgBaseCard& card);
private:
    int attack;
    std::string card_class;
    int cost;
    int health;
    std::string id;
    bool is_poison;
    std::string mechanics;
    std::string name;
    std::string race;
    std::string rarity;
    int tech_level;
    std::string text;
    std::string type;
};
