#include "BgBaseCard.hpp"

class FiendishServant : public BgBaseCard {
public:
    FiendishServant() : BgBaseCard(2, "WARLOCK", 1, 1, "Fiendish Servant",
				   "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(int pos, Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() { return std::make_shared<FiendishServant>(*this); } // boilerplate that every drattle needs...
};

class Mecharoo : public BgBaseCard {
public:
    Mecharoo() : BgBaseCard(1, "NEUTRAL", 1, 1, "Mecharoo",
			    "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(int pos, Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() { return std::make_shared<Mecharoo>(*this); } // boilerplate that every drattle needs...
};
