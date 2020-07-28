#pragma once

#include <iostream>
#include <string>

class Board; // Forward declare for circular dep between cards and board

class BgBaseCard {
public:
    BgBaseCard(int attack,
	       std::string card_class,
	       int cost,
	       int health,
	       std::string name,
	       std::string mechanics,
	       std::string race,
	       std::string rarity,
	       int tech_level,
	       std::string type) : attack(attack),
				   card_class(card_class),
				   cost(cost),
				   has_divine_shield(false),
				   health(health),
				   is_poison(false),
				   mechanics(mechanics),
				   name(name),
				   race(race),
				   rarity(rarity),
				   tech_level(tech_level),
				   type(type) {}
    
    BgBaseCard(const BgBaseCard& other) : attack(other.attack),
					  card_class(other.card_class),
					  cost(other.cost),
					  has_divine_shield(other.has_divine_shield),
					  is_poison(false),
					  mechanics(mechanics),
					  name(name),
					  race(race),
					  rarity(rarity),
					  tech_level(tech_level),
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

    virtual void do_deathrattle(int pos, Board* b1, Board* b2) {}
    
    int get_attack() { return is_poison ? 999999 : attack; } // Poison is like 'infinite' attack
    std::string get_card_class() { return card_class; }
    int get_cost() { return cost; }
    int get_health() { return health; }
    std::string get_mechanics() { return mechanics; }
    std::string get_name() { return name; }
    std::string get_race() { return race; }
    std::string get_rarity() { return rarity; }
    int get_tech_level() { return tech_level; }
    std::string get_type() { return type; }

    bool is_dead() { if (health < 0) return true; else return false;; }

    void set_attack(int att) { attack = att; }
    void set_health(int hth) { health = hth; }
    void set_poison() { is_poison = true; }
    void set_divine_shield() { has_divine_shield = true; }

    void take_damage(int damage);

    friend std::ostream& operator<<(std::ostream& os, BgBaseCard& card);
protected:
    int attack;
    std::string card_class;
    int cost;
    bool has_divine_shield;
    int health;
    bool is_poison;
    std::string mechanics;
    std::string name;
    std::string race;
    std::string rarity;
    int tech_level;
    std::string type;
};

