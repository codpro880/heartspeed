#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Board; // Forward declare for circular dep between cards and board
class Player; // Forward declare for circular dep between cards and player
// TODO: Move all boards to be players for consistent-interface purposes

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
				   _has_reborn(false),
				   _has_windfury(false),
				   _has_windfury_active(false),
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
					  _has_reborn(other._has_reborn),
					  _has_windfury(other._has_windfury),
					  _has_windfury_active(other._has_windfury_active),
					  health(other.health),
					  is_poison(other.is_poison),
					  mechanics(other.mechanics),
					  name(other.name),
					  race(other.race),
					  rarity(other.rarity),
					  tech_level(other.tech_level),
					  type(other.type) {}

    virtual void do_battlecry(Player*) {}
    virtual void targeted_battlecry(std::shared_ptr<BgBaseCard>) {}
    // Triggered on death
    // (ex: stat-buffs that die)
    // Note: Actual deathrattle cards handled by DeathrattleCard class
    virtual void do_deathrattle(Board*, Board*) {}
    virtual void deathrattle(Board* b1, Board* b2) { do_deathrattle(b1, b2); }
    
    // Triggered before every attack (ex: glyph gaurdian mechanic)
    virtual void do_preattack(std::shared_ptr<BgBaseCard>,
			      Board*,
			      Board*) {}
    // Triggered once before combat starts (ex: red whelp, stat-buffs)
    virtual void do_precombat(Board*, Board*) {}
    // Triggered after attack (ex: overkill mechanic)
    virtual void do_postattack(std::shared_ptr<BgBaseCard>,
			       int,
			       Board*,
			       Board*) {}
    // Triggered after attack for defender (ex: yo-ho-ogre)
    virtual void do_postdefense(std::shared_ptr<BgBaseCard>,
				Board*,
				Board*) {}
    // Triggered after deaths of each dmg exchange (ex: scavaging hyena)
    virtual void do_postbattle(Board*, Board*, std::vector<std::shared_ptr<BgBaseCard>>, std::vector<std::shared_ptr<BgBaseCard>>) {}

    // Triggered after a summon occurs
    virtual void mod_summoned(std::shared_ptr<BgBaseCard>) { }
    
    virtual std::shared_ptr<BgBaseCard> get_copy() const;
    
    int get_attack() const { return is_poison ? 999999 : attack; } // Poison is like 'infinite' attack
    std::string get_card_class() const { return card_class; }
    int get_cost() const { return cost; }
    // int get_death_pos() { return death_pos; }
    int get_health() const { return health; }
    std::string get_mechanics() const { return mechanics; }
    std::string get_name() const { return name; }
    std::string get_race() const { return race; }
    std::string get_rarity() const { return rarity; }
    int get_tech_level() const { return tech_level; }
    std::string get_type() const { return type; }

    bool has_divine_shield() const { return divine_shield; }
    bool has_deathrattle() const { return mechanics.find("DEATHRATTLE") != std::string::npos; }
    bool has_poison() const { return is_poison; }
    bool has_taunt() const {
	return _has_taunt || mechanics.find("TAUNT") != std::string::npos;
    }
    bool has_reborn() const {
	return _has_reborn;
    }
    bool has_cleave() const {
	return mechanics.find("CLEAVE") != std::string::npos;
    }
    bool has_windfury() const { return _has_windfury; }
    bool has_windfury_active() const { return _has_windfury_active; }

    bool is_dead() const { return health <= 0; }
    bool is_golden() const { return name.find("Golden") != std::string::npos; }

    void reborn_self(Board* b1);

    void set_attack(int att) { attack = att; }
    void set_death_pos(int dp) { death_pos = dp; }
    void set_health(int hth) { health = hth; }
    void set_poison() { is_poison = true; }
    void set_divine_shield() { divine_shield = true; }
    void set_taunt() { _has_taunt = true; }
    void set_reborn(bool rb = true) {_has_reborn = rb; }
    void set_windfury(bool wf = true) { _has_windfury = wf; _has_windfury_active = wf; }
    void set_windfury_active(bool wfa = true) { _has_windfury_active = wfa; }

    virtual std::shared_ptr<BgBaseCard> summon() {throw std::runtime_error("summon() not implemented");}
    virtual std::shared_ptr<BgBaseCard> do_summon(Board* b1);
    void basic_summon(Board* b1);
    void basic_summon(Player* p1);
    void multi_summon(int num_summons, Board* b1);
    void multi_summon(int num_summons, Player* p1);

    virtual void take_damage(int damage, std::string who_from_race, Board* b1, Board* b2);

    std::string who_killed_race() const {
	if (is_dead()) {
	    return last_dmg_race;
	}
	else {
	    throw std::runtime_error("Card isn't dead");
	}
    }
    friend std::ostream& operator<<(std::ostream& os, const BgBaseCard& card);
    virtual ~BgBaseCard() {}
    
protected:
    int attack;
    std::string card_class;
    int cost;
    bool divine_shield;
    bool _has_taunt = false;
    bool _has_reborn = false;
    bool _has_windfury = false; // attribute
    bool _has_windfury_active = false; // Whether or not we can attack again
    int health;
    bool is_poison;
    std::string mechanics;
    std::string name;
    std::string race;
    std::string rarity;
    int tech_level;
    std::string type;
    int death_pos = -2;
    std::string last_dmg_race;
};
