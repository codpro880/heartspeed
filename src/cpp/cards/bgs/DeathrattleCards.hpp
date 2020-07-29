#include "BgBaseCard.hpp"

class FiendishServant : public BgBaseCard {
public:
    FiendishServant() : BgBaseCard(2, "WARLOCK", 1, 1, "Fiendish Servant",
				   "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION") {}
				// 
    //void do_deathrattle(int pos, Board* b1, Board* b2) override {std::cout << "OVERRIDDEN" << std::endl; }
    virtual void do_deathrattle(int pos, Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() { return std::make_shared<FiendishServant>(*this); }
    // FiendishServant(const FiendishServant& other) : attack(other.attack),
    // 						    card_class(other.card_class),
    // 						    cost(other.cost),
    // 						    has_divine_shield(other.has_divine_shield),
    // 						    health(other.health),
    // 						    is_poison(other.is_poison),
    // 						    mechanics(other.mechanics),
    // 						    name(other.name),
    // 						    race(other.race),
    // 						    rarity(other.rarity),
    // 						    tech_level(other.tech_level),
    // 						    type(other.type) {}

};
