#include "BgBaseCard.hpp"

class DeathrattleCard : public BgBaseCard {
public:
    using BgBaseCard::BgBaseCard;
    virtual void do_deathrattle(Board* b1, Board* b2) override = 0;
    virtual std::shared_ptr<BgBaseCard> get_copy() override = 0; // boilerplate that every drattle needs...
    void basic_summon(std::string cardname, Board* b1, Board* b2);
    void multi_summon(std::string cardname, int num_summons, Board* b1, Board* b2);
};

class FiendishServant : public DeathrattleCard {
public:
    FiendishServant() : DeathrattleCard(2, "WARLOCK", 1, 1, "Fiendish Servant",
				   "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<FiendishServant>(*this); } // boilerplate that every drattle needs...
};

class FiendishServantGolden : public DeathrattleCard {
public:
    FiendishServantGolden() : DeathrattleCard(4, "WARLOCK", 1, 2, "Fiendish Servant (Golden)",
					      "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<FiendishServantGolden>(*this); } // boilerplate that every drattle needs...
private:
    FiendishServant serv;
};

class HarvestGolem : public DeathrattleCard {
public:
    HarvestGolem() : DeathrattleCard(2, "NEUTRAL", 3, 3, "Harvest Golem",
				"['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<HarvestGolem>(*this); } // boilerplate that every drattle needs...
};

class HarvestGolemGolden : public DeathrattleCard {
public:
    HarvestGolemGolden() : DeathrattleCard(4, "NEUTRAL", 3, 6, "Harvest Golem (Golden)",
				"['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<HarvestGolemGolden>(*this); } // boilerplate that every drattle needs...
};

class Imprisoner : public DeathrattleCard {
public:
    Imprisoner() : DeathrattleCard(3, "NEUTRAL", 3, 3, "Imprisoner",
			      "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Imprisoner>(*this); } // boilerplate that every drattle needs...
};

class ImprisonerGolden : public DeathrattleCard {
public:
    ImprisonerGolden() : DeathrattleCard(6, "NEUTRAL", 3, 6, "ImprisonerGolden",
			      "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ImprisonerGolden>(*this); } // boilerplate that every drattle needs...
};

class InfestedWolf : public DeathrattleCard {
public:
    InfestedWolf() : DeathrattleCard(6, "HUNTER", 4, 6, "Infested Wolf",
				"['DEATHRATTLE']", "BEAST", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<InfestedWolf>(*this); } // boilerplate that every drattle needs...
};

class InfestedWolfGolden : public DeathrattleCard {
public:
    InfestedWolfGolden() : DeathrattleCard(6, "HUNTER", 4, 6, "Infested Wolf (Golden)",
				      "['DEATHRATTLE']", "BEAST", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<InfestedWolfGolden>(*this); } // boilerplate that every drattle needs...
};


class KaboomBot : public DeathrattleCard {
public:
    KaboomBot() : DeathrattleCard(2, "NEUTRAL", 3, 2, "Kaboom Bot",
			     "['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KaboomBot>(*this); } // boilerplate that every drattle needs...
};

class KaboomBotGolden : public DeathrattleCard {
public:
    KaboomBotGolden() : DeathrattleCard(4, "NEUTRAL", 3, 4, "Kaboom Bot (Golden)",
			     "['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KaboomBotGolden>(*this); } // boilerplate that every drattle needs...
private:
    KaboomBot kbot;
};

class KindlyGrandmother : public DeathrattleCard {
public:
    KindlyGrandmother() : DeathrattleCard(1, "HUNTER", 2, 1, "Kindly Grandmother (Golden)",
				     "['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KindlyGrandmother>(*this); } // boilerplate that every drattle needs...
};

class KindlyGrandmotherGolden : public DeathrattleCard {
public:
    KindlyGrandmotherGolden() : DeathrattleCard(1, "HUNTER", 2, 1, "Kindly Grandmother (Golden)",
				     "['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KindlyGrandmotherGolden>(*this); } // boilerplate that every drattle needs...
};

class Mecharoo : public DeathrattleCard {
public:
    Mecharoo() : DeathrattleCard(1, "NEUTRAL", 1, 1, "Mecharoo",
			    "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Mecharoo>(*this); } // boilerplate that every drattle needs...
};

class MecharooGolden : public DeathrattleCard {
public:
    MecharooGolden() : DeathrattleCard(2, "NEUTRAL", 1, 2, "Mecharoo (Golden)",
			    "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<MecharooGolden>(*this); } // boilerplate that every drattle needs...
};

class RatPack : public DeathrattleCard {
public:
    RatPack() : DeathrattleCard(2, "HUNTER", 3, 2, "Rat Pack",
			   "['DEATHRATTLE']", "BEAST", "EPIC", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RatPack>(*this); } // boilerplate that every drattle needs...
};

class RatPackGolden : public DeathrattleCard {
public:
    RatPackGolden() : DeathrattleCard(4, "HUNTER", 3, 4, "Rat Pack (Golden)",
			   "['DEATHRATTLE']", "BEAST", "EPIC", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RatPackGolden>(*this); } // boilerplate that every drattle needs...
};

class Scallywag : public DeathrattleCard {
public:
    Scallywag() : DeathrattleCard(2, "NEUTRAL", 1, 1, "Scallywag",
			     "['DEATHRATTLE']", "PIRATE", "", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Scallywag>(*this); } // boilerplate that every drattle needs...
};

class ScallywagGolden : public DeathrattleCard {
public:
    ScallywagGolden() : DeathrattleCard(4, "NEUTRAL", 2, 1, "Scallywag (Golden)",
			     "['DEATHRATTLE']", "PIRATE", "", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ScallywagGolden>(*this); } // boilerplate that every drattle needs...
};


class SelflessHero : public DeathrattleCard {
public:
    SelflessHero() : DeathrattleCard(2, "PALADIN", 1, 1, "Selfless Hero",
				"['DEATHRATTLE']", "", "RARE", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SelflessHero>(*this); } // boilerplate that every drattle needs...
};

class SelflessHeroGolden : public DeathrattleCard {
public:
    SelflessHeroGolden() : DeathrattleCard(4, "PALADIN", 1, 2, "Selfless Hero (Golden)",
				      "['DEATHRATTLE']", "", "RARE", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SelflessHeroGolden>(*this); } // boilerplate that every drattle needs...
private:
    SelflessHero hero;
};

class SpawnOfNzoth : public DeathrattleCard {
public:
    SpawnOfNzoth() : DeathrattleCard(2, "NEUTRAL", 3, 2, "Spawn of N'Zoth", "['DEATHRATTLE']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SpawnOfNzoth>(*this); } // boilerplate that every drattle needs...
};

class SpawnOfNzothGolden : public DeathrattleCard {
public:
    SpawnOfNzothGolden() : DeathrattleCard(4, "NEUTRAL", 4, 2, "Spawn of N'Zoth (Golden)", "['DEATHRATTLE']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SpawnOfNzothGolden>(*this); } // boilerplate that every drattle needs...
};

class UnstableGhoul : public DeathrattleCard {
public:
    UnstableGhoul() : DeathrattleCard(1, "NEUTRAL", 2, 3, "Unstable Ghoul",	"['DEATHRATTLE', 'TAUNT']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<UnstableGhoul>(*this); } // boilerplate that every drattle needs...
};

class UnstableGhoulGolden : public DeathrattleCard {
public:
    UnstableGhoulGolden() : DeathrattleCard(2, "NEUTRAL", 2, 6, "Unstable Ghoul (Golden)",	"['DEATHRATTLE', 'TAUNT']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<UnstableGhoulGolden>(*this); } // boilerplate that every drattle needs...
private:
    UnstableGhoul ghoul;
};
