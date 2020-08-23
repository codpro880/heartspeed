#include "BgBaseCard.hpp"

#include <queue>

/** Class for common deathrattle logic. **/
class DeathrattleCard : public BgBaseCard {
public:
    using BgBaseCard::BgBaseCard;
    virtual void do_deathrattle(Board* b1, Board* b2) override = 0;
    virtual std::shared_ptr<BgBaseCard> get_copy() override = 0; // boilerplate that every card needs...
    void basic_summon(Board* b1);
    void multi_summon(int num_summons, Board* b1);
    // TODO: Move summon mechanic to base class
    // summon() must be overriden if called,
    // but don't want to force it since not all drattles summon
    // Think of it as a mutable callback...
    virtual std::shared_ptr<BgBaseCard> summon() {throw std::runtime_error("summon() not implemented"); }
    virtual std::shared_ptr<BgBaseCard> do_summon(Board* b1);
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

class Ghastcoiler : public DeathrattleCard {
public:
    Ghastcoiler() : DeathrattleCard(7, "PRIEST", 6, 7, "Ghastcoiler",
				    "['DEATHRATTLE']", "BEAST", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Ghastcoiler>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;    
};

class GhastcoilerGolden : public DeathrattleCard {
public:
    GhastcoilerGolden() : DeathrattleCard(14, "PRIEST", 6, 14, "Ghastcoiler (Golden)",
					  "['DEATHRATTLE']", "BEAST", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<GhastcoilerGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    Ghastcoiler coiler;
};

class GlyphGuardian : public BgBaseCard {
public:
    GlyphGuardian() : BgBaseCard(2, "MAGE", 4, 4, "Glyph Guardian",
				    "", "DRAGON", "", 2, "MINION") {}
    virtual void do_preattack(std::shared_ptr<BgBaseCard> defender,
			      Board* b1,
			      Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<GlyphGuardian>(*this); } // boilerplate that every drattle needs...
};

class GlyphGuardianGolden : public BgBaseCard {
public:
    GlyphGuardianGolden() : BgBaseCard(4, "MAGE", 4, 8, "Glyph Guardian (Golden)",
				       "", "DRAGON", "", 2, "MINION") {}
    virtual void do_preattack(std::shared_ptr<BgBaseCard> defender,
			      Board* b1,
			      Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<GlyphGuardianGolden>(*this); } // boilerplate that every drattle needs...
private:
    GlyphGuardian coiler;
};

class Goldrinn : public DeathrattleCard {
public:
    Goldrinn() : DeathrattleCard(4, "NEUTRAL", 8, 4, "Goldrinn",
				 "['DEATHRATTLE']", "BEAST", "LEGENDARY", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Goldrinn>(*this); } // boilerplate that every drattle needs...
};

class GoldrinnGolden : public DeathrattleCard {
public:
    GoldrinnGolden() : DeathrattleCard(8, "NEUTRAL", 8, 8, "Goldrinn (Golden)",
				       "['DEATHRATTLE']", "BEAST", "LEGENDARY", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<GoldrinnGolden>(*this); } // boilerplate that every drattle needs...
private:
    Goldrinn bag;
};


class HarvestGolem : public DeathrattleCard {
public:
    HarvestGolem() : DeathrattleCard(2, "NEUTRAL", 3, 3, "Harvest Golem",
				"['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<HarvestGolem>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class HarvestGolemGolden : public DeathrattleCard {
public:
    HarvestGolemGolden() : DeathrattleCard(4, "NEUTRAL", 3, 6, "Harvest Golem (Golden)",
				"['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<HarvestGolemGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class Imprisoner : public DeathrattleCard {
public:
    Imprisoner() : DeathrattleCard(3, "NEUTRAL", 3, 3, "Imprisoner",
			      "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Imprisoner>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class ImprisonerGolden : public DeathrattleCard {
public:
    ImprisonerGolden() : DeathrattleCard(6, "NEUTRAL", 3, 6, "ImprisonerGolden",
			      "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ImprisonerGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class InfestedWolf : public DeathrattleCard {
public:
    InfestedWolf() : DeathrattleCard(6, "HUNTER", 4, 6, "Infested Wolf",
				"['DEATHRATTLE']", "BEAST", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<InfestedWolf>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class InfestedWolfGolden : public DeathrattleCard {
public:
    InfestedWolfGolden() : DeathrattleCard(6, "HUNTER", 4, 6, "Infested Wolf (Golden)",
				      "['DEATHRATTLE']", "BEAST", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<InfestedWolfGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
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

class Kangor : public DeathrattleCard {
public:
    Kangor() : DeathrattleCard(3, "NEUTRAL", 9, 6, "Kangor",
			       "['DEATHRATTLE']", "", "EPIC", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Kangor>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
    void reset_mech_queue(Board* b);
    std::queue<std::string> mech_queue;
};

class KangorGolden : public DeathrattleCard {
public:
    KangorGolden() : DeathrattleCard(6, "NEUTRAL", 9, 12, "Kangor (Golden)",
				     "['DEATHRATTLE']", "", "EPIC", 6,"MINION") {}
				    
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KangorGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    Kangor kang;
};

class KindlyGrandmother : public DeathrattleCard {
public:
    KindlyGrandmother() : DeathrattleCard(1, "HUNTER", 2, 1, "Kindly Grandmother (Golden)",
					  "['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KindlyGrandmother>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class KindlyGrandmotherGolden : public DeathrattleCard {
public:
    KindlyGrandmotherGolden() : DeathrattleCard(1, "HUNTER", 2, 1, "Kindly Grandmother (Golden)",
						"['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KindlyGrandmotherGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class KingBagurgle : public DeathrattleCard {
public:
    KingBagurgle() : DeathrattleCard(6, "NEUTRAL", 6, 3, "King Bagurgle",
				     "['BATTLECRY', 'DEATHRATTLE']", "MURLOC", "LEGENDARY", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KingBagurgle>(*this); } // boilerplate that every drattle needs...
};

class KingBagurgleGolden : public DeathrattleCard {
public:
    KingBagurgleGolden() : DeathrattleCard(12, "NEUTRAL", 6, 6, "King Bagurgle (Golden)",
					   "['BATTLECRY', 'DEATHRATTLE']", "MURLOC", "LEGENDARY", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<KingBagurgleGolden>(*this); } // boilerplate that every drattle needs...
private:
    KingBagurgle bag;
};

class MechanoEgg : public DeathrattleCard {
public:
    MechanoEgg() : DeathrattleCard(0, "NEUTRAL", 5, 5, "Mechano-Egg",
				   "['DEATHRATTLE']", "MECHANICAL", "COMMON", 4, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<MechanoEgg>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class MechanoEggGolden : public DeathrattleCard {
public:
    MechanoEggGolden() : DeathrattleCard(0, "PALADIN", 5, 10, "Mechano-Egg (Golden)",
				 "['DEATHRATTLE']", "MECHANICAL", "COMMON", 4, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<MechanoEggGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class Mecharoo : public DeathrattleCard {
public:
    Mecharoo() : DeathrattleCard(1, "NEUTRAL", 1, 1, "Mecharoo",
				       "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Mecharoo>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class MecharooGolden : public DeathrattleCard {
public:
    MecharooGolden() : DeathrattleCard(2, "NEUTRAL", 1, 2, "Mecharoo (Golden)",
				       "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<MecharooGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class MurlocWarleader : public BgBaseCard {
public:
    MurlocWarleader() : BgBaseCard(3, "NEUTRAL", 3, 3, "Murloc Warleader",
				   "['AURA']", "MURLOC", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<MurlocWarleader>(*this); } // boilerplate that every drattle needs...
};

class MurlocWarleaderGolden : public BgBaseCard {
public:
    MurlocWarleaderGolden() : BgBaseCard(6, "NEUTRAL", 3, 6, "Murloc Warleader (Golden)",
					    "['AURA']", "MURLOC", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<MurlocWarleaderGolden>(*this); } // boilerplate that every drattle needs...
private:
    MurlocWarleader rw;
};

class Nadina : public DeathrattleCard {
public:
    Nadina() : DeathrattleCard(7, "NEUTRAL", 6, 4, "Nadina",
			       "['DEATHRATTLE']", "", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Nadina>(*this); } // boilerplate that every drattle needs...
};

class NadinaGolden : public DeathrattleCard {
public:
    NadinaGolden() : DeathrattleCard(14, "NEUTRAL", 6, 8, "Nadina (Golden)",
				     "['DEATHRATTLE']", "", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<NadinaGolden>(*this); } // boilerplate that every drattle needs...
private:
    Nadina bag;
};

class OldMurkeye : public BgBaseCard {
public:
    OldMurkeye() : BgBaseCard(2, "NEUTRAL", 4, 4, "Old Murk-Eye",
				 "['CHARGE']", "MURLOC", "LEGENDARY", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_postbattle(Board* b1,
			       std::vector<std::shared_ptr<BgBaseCard> > new_dead) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<OldMurkeye>(*this); } // boilerplate that every drattle needs...
};

class OldMurkeyeGolden : public BgBaseCard {
public:
    OldMurkeyeGolden() : BgBaseCard(4, "NEUTRAL", 4, 8, "Old Murk-Eye (Golden)",
				       "['CHARGE']", "MURLOC", "LEGENDARY", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_postbattle(Board* b1,
			       std::vector<std::shared_ptr<BgBaseCard> > new_dead) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<OldMurkeyeGolden>(*this); } // boilerplate that every drattle needs...
private:
    OldMurkeye rw;
};

class PackLeader : public BgBaseCard {
public:
    PackLeader() : BgBaseCard(3, "NEUTRAL", 3, 3, "Pack Leader",
			      "['TRIGGER_VISUAL']", "", "RARE", 2, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<PackLeader>(*this); } // boilerplate that every drattle needs...
    void mod_summoned(std::shared_ptr<BgBaseCard> card) override;
};

class PackLeaderGolden : public BgBaseCard {
public:
    PackLeaderGolden() : BgBaseCard(6, "NEUTRAL", 3, 6, "Pack Leader (Golden)",
				    "['TRIGGER_VISUAL']", "", "RARE", 2, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<PackLeaderGolden>(*this); } // boilerplate that every drattle needs...
    void mod_summoned(std::shared_ptr<BgBaseCard> card) override;
private:
    PackLeader pl;
};


class PilotedShredder : public DeathrattleCard {
public:
    PilotedShredder() : DeathrattleCard(4, "NEUTRAL", 4, 3, "Piloted Shredder",
					"['DEATHRATTLE']", "MECHANICAL", "COMMON", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<PilotedShredder>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class PilotedShredderGolden : public DeathrattleCard {
public:
    PilotedShredderGolden() : DeathrattleCard(8, "NEUTRAL", 4, 6, "Piloted Shredder (Golden)",
					      "['DEATHRATTLE']", "MECHANICAL", "COMMON", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<PilotedShredderGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    PilotedShredder shredder;
};

class RatPack : public DeathrattleCard {
public:
    RatPack() : DeathrattleCard(2, "HUNTER", 3, 2, "Rat Pack",
			   "['DEATHRATTLE']", "BEAST", "EPIC", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RatPack>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class RatPackGolden : public DeathrattleCard {
public:
    RatPackGolden() : DeathrattleCard(4, "HUNTER", 3, 4, "Rat Pack (Golden)",
			   "['DEATHRATTLE']", "BEAST", "EPIC", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RatPackGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class RedWhelp : public BgBaseCard {
public:
    RedWhelp() : BgBaseCard(1, "NEUTRAL", 1, 2, "Red Whelp",
			       "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RedWhelp>(*this); } // boilerplate that every drattle needs...
};

class RedWhelpGolden : public BgBaseCard {
public:
    RedWhelpGolden() : BgBaseCard(1, "NEUTRAL", 1, 2, "Red Whelp",
			       "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RedWhelpGolden>(*this); } // boilerplate that every drattle needs...
private:
    RedWhelp rw;
};

class ReplicatingMenace : public DeathrattleCard {
public:
    ReplicatingMenace() : DeathrattleCard(3, "NEUTRAL", 4, 1, "Replicating Menace",
					  "['DEATHRATTLE', 'MODULAR']", "MECHANICAL", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ReplicatingMenace>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class ReplicatingMenaceGolden : public DeathrattleCard {
public:
    ReplicatingMenaceGolden() : DeathrattleCard(6, "NEUTRAL", 4, 2, "Replicating Menace (Golden)",
						"['DEATHRATTLE', 'MODULAR']", "MECHANICAL", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ReplicatingMenaceGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class SavannahHighmane : public DeathrattleCard {
public:
    SavannahHighmane() : DeathrattleCard(6, "HUNTER", 6, 5, "Savannah Highmane",
					 "['DEATHRATTLE']", "BEAST", "RARE", 4,"MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SavannahHighmane>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class SavannahHighmaneGolden : public DeathrattleCard {
public:
    SavannahHighmaneGolden() : DeathrattleCard(12, "HUNTER", 6, 10, "Savannah Highmane (Golden)",
					       "['DEATHRATTLE']", "BEAST", "RARE", 4,"MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SavannahHighmaneGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
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

class ScavagingHyena : public BgBaseCard {
public:
    ScavagingHyena() : BgBaseCard(2, "HUNTER", 2, 2, "Scavenging Hyena",
				  "['TRIGGER_VISUAL']", "BEAST", "COMMON", 1, "MINION") {}
    virtual void do_postbattle(Board* b1, std::vector<std::shared_ptr<BgBaseCard> > new_dead) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ScavagingHyena>(*this); } // boilerplate that every drattle needs...
};

class ScavagingHyenaGolden : public BgBaseCard {
public:
    ScavagingHyenaGolden() : BgBaseCard(4, "HUNTER", 2, 4, "Scavenging Hyena (Golden)",
					"['TRIGGER_VISUAL']", "BEAST", "COMMON", 1, "MINION") {}
    virtual void do_postbattle(Board* b1, std::vector<std::shared_ptr<BgBaseCard> > new_dead) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ScavagingHyenaGolden>(*this); } // boilerplate that every drattle needs...
private:
    ScavagingHyena sh;
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

class SneedsOldShredder : public DeathrattleCard {
public:
    SneedsOldShredder() : DeathrattleCard(5, "NEUTRAL", 8, 7,"Sneed's Old Shredder",
					  "['DEATHRATTLE']", "MECHANICAL", "LEGENDARY", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SneedsOldShredder>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;    
};

class SneedsOldShredderGolden : public DeathrattleCard {
public:
    SneedsOldShredderGolden() : DeathrattleCard(10, "NEUTRAL", 8, 14, "Sneed's Old Shredder (Golden)",
						"['DEATHRATTLE']", "MECHANICAL", "LEGENDARY", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SneedsOldShredderGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    SneedsOldShredder shredder;
};

class SouthseaCaptain : public BgBaseCard {
public:
    SouthseaCaptain() : BgBaseCard(3, "NEUTRAL", 3, 3, "Southsea Captain",
				      "['AURA']", "PIRATE", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SouthseaCaptain>(*this); } // boilerplate that every drattle needs...
};

class SouthseaCaptainGolden : public BgBaseCard {
public:
    SouthseaCaptainGolden() : BgBaseCard(6, "NEUTRAL", 3, 6, "Southsea Captain (Golden)",
					    "['AURA']", "PIRATE", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SouthseaCaptainGolden>(*this); } // boilerplate that every drattle needs...
private:
    SouthseaCaptain rw;
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

class TheTideRazor : public DeathrattleCard {
public:
    TheTideRazor() : DeathrattleCard(6, "NEUTRAL", 7, 4, "The Tide Razor",
				     "['DEATHRATTLE']", "", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<TheTideRazor>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class TheTideRazorGolden : public DeathrattleCard {
public:
    TheTideRazorGolden() : DeathrattleCard(12, "NEUTRAL", 7, 8, "The Tide Razor",
					   "['DEATHRATTLE']", "", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<TheTideRazorGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    TheTideRazor ttr;
};

class TheBeast : public DeathrattleCard {
public:
    TheBeast() : DeathrattleCard(9, "NEUTRAL", 6, 7, "The Beast",
				 "['DEATHRATTLE']", "BEAST", "LEGENDARY", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<TheBeast>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class TheBeastGolden : public DeathrattleCard {
public:
    TheBeastGolden() : DeathrattleCard(18, "NEUTRAL", 6, 14, "The Beast (Golden)",
				       "['DEATHRATTLE']", "BEAST", "LEGENDARY", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<TheBeastGolden>(*this); } // boilerplate that every drattle needs...
private:
    TheBeast the_beast;
};

class Voidlord : public DeathrattleCard {
public:
    Voidlord() : DeathrattleCard(3, "WARLOCK", 9, 9, "Voidlord",
				 "['DEATHRATTLE', 'TAUNT']", "DEMON", "EPIC", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<Voidlord>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class VoidlordGolden : public DeathrattleCard {
public:
    VoidlordGolden() : DeathrattleCard(6, "WARLOCK", 9, 18, "VoidlordGolden (Golden)",
				 "['DEATHRATTLE', 'TAUNT']", "DEMON", "EPIC", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<VoidlordGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
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
