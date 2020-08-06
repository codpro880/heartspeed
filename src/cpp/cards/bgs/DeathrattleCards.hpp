#include "BgBaseCard.hpp"

class FiendishServant : public BgBaseCard {
public:
    FiendishServant() : BgBaseCard(2, "WARLOCK", 1, 1, "Fiendish Servant",
				   "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<FiendishServant>(*this); } // boilerplate that every drattle needs...
};

class FiendishServantGolden : public BgBaseCard {
public:
    FiendishServantGolden() : BgBaseCard(4, "WARLOCK", 1, 2, "Fiendish Servant (Golden)",
				   "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<FiendishServantGolden>(*this); } // boilerplate that every drattle needs...
private:
    FiendishServant serv;
};

class HarvestGolem : public BgBaseCard {
public:
    HarvestGolem() : BgBaseCard(2, "NEUTRAL", 3, 3, "Harvest Golem",
				"['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<HarvestGolem>(*this); } // boilerplate that every drattle needs...
};

class HarvestGolemGolden : public BgBaseCard {
public:
    HarvestGolemGolden() : BgBaseCard(4, "NEUTRAL", 3, 6, "Harvest Golem (Golden)",
				"['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<HarvestGolemGolden>(*this); } // boilerplate that every drattle needs...
};

class Imprisoner : public BgBaseCard {
public:
    Imprisoner() : BgBaseCard(3, "NEUTRAL", 3, 3, "Imprisoner",
			      "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Imprisoner>(*this); } // boilerplate that every drattle needs...
};

class KaboomBot : public BgBaseCard {
public:
    KaboomBot() : BgBaseCard(2, "NEUTRAL", 3, 2, "Kaboom Bot",
			     "['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KaboomBot>(*this); } // boilerplate that every drattle needs...
};

class KaboomBotGolden : public BgBaseCard {
public:
    KaboomBotGolden() : BgBaseCard(4, "NEUTRAL", 3, 4, "Kaboom Bot (Golden)",
			     "['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KaboomBotGolden>(*this); } // boilerplate that every drattle needs...
private:
    KaboomBot kbot;
};



class KindlyGrandmother : public BgBaseCard {
public:
    KindlyGrandmother() : BgBaseCard(1, "HUNTER", 2, 1, "Kindly Grandmother",
				     "['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KindlyGrandmother>(*this); } // boilerplate that every drattle needs...
};

class Mecharoo : public BgBaseCard {
public:
    Mecharoo() : BgBaseCard(1, "NEUTRAL", 1, 1, "Mecharoo",
			    "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Mecharoo>(*this); } // boilerplate that every drattle needs...
};

class Scallywag : public BgBaseCard {
public:
    Scallywag() : BgBaseCard(2, "NEUTRAL", 1, 1, "Scallywag",
			     "['DEATHRATTLE']", "PIRATE", "", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Scallywag>(*this); } // boilerplate that every drattle needs...
};

class RatPack : public BgBaseCard {
public:
    RatPack() : BgBaseCard(2, "HUNTER", 3, 2, "Rat Pack",
			   "['DEATHRATTLE']", "BEAST", "EPIC", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RatPack>(*this); } // boilerplate that every drattle needs...
};

class SelflessHero : public BgBaseCard {
public:
    SelflessHero() : BgBaseCard(2, "PALADIN", 1, 1, "Selfless Hero",
				"['DEATHRATTLE']", "", "RARE", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SelflessHero>(*this); } // boilerplate that every drattle needs...
};

class SpawnOfNzoth : public BgBaseCard {
public:
    SpawnOfNzoth() : BgBaseCard(2, "NEUTRAL", 3, 2, "Spawn of N'Zoth", "['DEATHRATTLE']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SpawnOfNzoth>(*this); } // boilerplate that every drattle needs...
};

class UnstableGhoul : public BgBaseCard {
public:
    UnstableGhoul() : BgBaseCard(1, "NEUTRAL", 2, 3, "Unstable Ghoul",	"['DEATHRATTLE', 'TAUNT']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<UnstableGhoul>(*this); } // boilerplate that every drattle needs...
};
