#pragma once

#include <iostream>
#include <string>
#include <vector>

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
				   divine_shield(false),
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
					  divine_shield(other.divine_shield),
					  health(other.health),
					  is_poison(other.is_poison),
					  mechanics(other.mechanics),
					  name(other.name),
					  race(other.race),
					  rarity(other.rarity),
					  tech_level(other.tech_level),
					  type(other.type) {}

    virtual void do_deathrattle(Board* b1, Board* b2) {}
    virtual void do_precombat(Board* b1, Board* b2) {}
    virtual void do_postbattle(Board* b1, std::vector<std::shared_ptr<BgBaseCard> > new_dead) {}
    
    virtual std::shared_ptr<BgBaseCard> get_copy();
    
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

    bool has_divine_shield() { return divine_shield; }
    bool has_deathrattle() { return mechanics.find("DEATHRATTLE") != std::string::npos; }

    bool is_dead() { return health <= 0; }

    void set_attack(int att) { attack = att; }
    void set_death_pos(int dp) { death_pos = dp; }
    void set_health(int hth) { health = hth; }
    void set_poison() { is_poison = true; }
    void set_divine_shield() { divine_shield = true; }

    void take_damage(int damage);

    friend std::ostream& operator<<(std::ostream& os, BgBaseCard& card);
    virtual ~BgBaseCard() {}
    
protected:
    int attack;
    std::string card_class;
    int cost;
    bool divine_shield;
    int health;
    bool is_poison;
    std::string mechanics;
    std::string name;
    std::string race;
    std::string rarity;
    int tech_level;
    std::string type;
    int death_pos = -2;
private:
    void deal_with_death(Board* b1, Board* b2);
};

