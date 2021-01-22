#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../../bg_game/rng_singleton.hpp"

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
	       int tavern_tier,
	       std::string type) : attack(attack),
				   base_attack(attack),
				   card_class(card_class),
				   cost(cost),
				   divine_shield(false),
				   _has_reborn(false),
				   _has_windfury(false),
				   _has_windfury_active(false),
				   health(health),
				   base_health(health),
				   is_poison(false),
				   mechanics(mechanics),
				   name(name),
				   race(race),
				   rarity(rarity),
				   tavern_tier(tavern_tier),
				   type(type),
				   adapt_count(0) {}
    
    BgBaseCard(const BgBaseCard& other) : attack(other.attack),
					  base_attack(other.attack),
					  card_class(other.card_class),
					  cost(other.cost),
					  divine_shield(other.divine_shield),
					  _has_reborn(other._has_reborn),
					  _has_windfury(other._has_windfury),
					  _has_windfury_active(other._has_windfury_active),
					  health(other.health),
					  base_health(other.health),
					  is_poison(other.is_poison),
					  mechanics(other.mechanics),
					  name(other.name),
					  race(other.race),
					  rarity(other.rarity),
					  tavern_tier(other.tavern_tier),
					  type(other.type),
					  adapt_count(0) {}

    void adapt(std::string _test_adapt="None");

    virtual void do_battlecry(Player*) {}
    virtual void battlecry(Player*) {}
    virtual void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) {}
    virtual void targeted_battlecry(std::shared_ptr<BgBaseCard>, Player*) {}
    // Triggered on death
    // (ex: stat-buffs that die)
    // Note: Actual deathrattle cards handled by DeathrattleCard class
    virtual void do_deathrattle(Player*, Player*);
    virtual void deathrattle(Player*, Player*);
    
    // Triggered before every attack (ex: glyph gaurdian mechanic)
    virtual void do_preattack(std::shared_ptr<BgBaseCard>,
			      Player*,
			      Player*) {}
    // Triggered before defense (Tormented Ritualist)
    virtual void do_predefense(std::shared_ptr<BgBaseCard>,
			       Player*,
			       Player*);
    // Triggered once before combat starts (ex: red whelp, stat-buffs)
    virtual void do_precombat(Player*, Player*) {}
    // Triggered after attack (ex: overkill mechanic)
    virtual void do_postattack(std::shared_ptr<BgBaseCard>,
			       int,
			       Player*,
			       Player*) {}
    // Triggered after attack for defender (ex: yo-ho-ogre)
    virtual void do_postdefense(std::shared_ptr<BgBaseCard>,
				Player*,
				Player*) {}
    // Triggered after deaths of each dmg exchange (ex: scavaging hyena)
    virtual void do_postbattle(Player*, Player*, std::vector<std::shared_ptr<BgBaseCard>>, std::vector<std::shared_ptr<BgBaseCard>>) {}

    // Triggered after a summon occurs, returns damage taken (wrathweave)
    virtual int mod_summoned(std::shared_ptr<BgBaseCard>, Board*, bool) { return 0; }

    // Triggered at start of turn
    virtual void start_turn_mechanic(Player*) {}

    // Triggered at end of turn
    virtual void end_turn_mechanic(Player*) {}
    
    virtual std::shared_ptr<BgBaseCard> get_copy() const;

    int get_adapt_count() const { return adapt_count; }
    int get_attack() const { return is_poison ? 999999 : attack; } // Poison is like 'infinite' attack
    int get_base_attack() const { return base_attack; }
    std::string get_card_class() const { return card_class; }
    int get_cost() const { return cost; }
    // int get_death_pos() { return death_pos; }
    int get_health() const { return health; }
    int get_base_health() const { return base_health; }
    std::string get_mechanics() const { return mechanics; }
    std::string get_name() const { return name; }
    std::string get_race() const { return race; }
    std::string get_rarity() const { return rarity; }
    int get_tavern_tier() const { return tavern_tier; }
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
    bool has_battlecry() const {
	return mechanics.find("BATTLECRY") != std::string::npos;
    }
    bool has_windfury() const { return _has_windfury; }
    bool has_windfury_active() const { return _has_windfury_active; }

    bool is_dead() const { return health <= 0; }
    bool is_golden() const { return name.find("Golden") != std::string::npos; }
    bool is_magnetic() const { return mechanics.find("MODULAR") != std::string::npos; }

    void reborn_self(Board*);

    void set_attack(int att) { attack = att; }
    // Base stats can't be affected during combat
    void set_base_attack(int att) { base_attack = att; attack = att; }
    void set_death_pos(int dp) { death_pos = dp; }
    void set_health(int hth) { health = hth; }
    // Base stats can't be affected during combat
    void set_base_health(int hth) {
	auto added_health = hth - base_health;
	health += added_health;
	base_health = hth;
    }
    void set_poison(bool p = true) { is_poison = p; }
    void set_divine_shield(bool s = true) { divine_shield = s; }
    void set_taunt(bool t = true) { _has_taunt = t; }
    void set_reborn(bool rb = true) {_has_reborn = rb; }
    void set_windfury(bool wf = true) { _has_windfury = wf; _has_windfury_active = wf; }
    void set_windfury_active(bool wfa = true) { _has_windfury_active = wfa; }

    virtual std::shared_ptr<BgBaseCard> summon() {throw std::runtime_error("summon() not implemented");}
    virtual std::shared_ptr<BgBaseCard> do_summon(Player*, bool from_hand);
    // void basic_summon(Board* b1, bool from_hand = false);
    void basic_summon(Player* p1, bool from_hand = false);
    // void multi_summon(int num_summons, Board* b1, bool from_hand = false);
    void multi_summon(int num_summons, Player* p1, bool from_hand = false);
    virtual void on_sell(Player* p1);
    virtual void card_bought_trigger(Player*, std::shared_ptr<BgBaseCard>) { }
    void add_to_deathrattle_cards(std::shared_ptr<BgBaseCard> c) {
	deathrattle_cards.push_back(c);
    }

    virtual void take_damage(int damage, std::string who_from_race, Player*, Player*);

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
    std::vector<std::shared_ptr<BgBaseCard>> deathrattle_cards; // Used for magnetic effects or other deathrattle stacking
    int attack;
    int base_attack;
    std::string card_class;
    int cost;
    bool divine_shield;
    bool _has_taunt = false;
    bool _has_reborn = false;
    bool _has_windfury = false; // attribute
    bool _has_windfury_active = false; // Whether or not we can attack again
    int health;
    int base_health;
    bool is_poison;
    std::string mechanics;
    std::string name;
    std::string race;
    std::string rarity;
    int tavern_tier;
    std::string type;
    int death_pos = -2;
    std::string last_dmg_race;
    // int adapt_count = 0; // Mostly used for testing only
    int adapt_count; // Mostly used for testing only
};
