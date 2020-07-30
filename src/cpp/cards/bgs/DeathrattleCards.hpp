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

class SelflessHero : public BgBaseCard {
public:
    SelflessHero() : BgBaseCard(2, "PALADIN", 1, 1, "Selfless Hero",
				"['DEATHRATTLE']", "", "RARE", 1, "MINION") {}
    virtual void do_deathrattle(int pos, Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() { return std::make_shared<SelflessHero>(*this); } // boilerplate that every drattle needs...
};

class Scallywag : public BgBaseCard {
public:
    Scallywag() : BgBaseCard(2, "NEUTRAL", 1, 1, "Scallywag",
			     "['DEATHRATTLE']", "PIRATE", "", 1, "MINION") {}
    virtual void do_deathrattle(int pos, Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() { return std::make_shared<Scallywag>(*this); } // boilerplate that every drattle needs...
};
