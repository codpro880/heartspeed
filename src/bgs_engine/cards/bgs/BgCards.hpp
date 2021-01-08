#pragma once
#include "BgBaseCard.hpp"

#include <memory>
#include <queue>

class BattlecryCard : virtual public BgBaseCard {
    virtual void battlecry(Player* p1) override;
    virtual void do_battlecry(Player* p1) override = 0;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override = 0; // boilerplate that every card needs...
};

/** Class for common deathrattle logic. **/
class DeathrattleCard : virtual public BgBaseCard {
public:
    using BgBaseCard::BgBaseCard;
    virtual void deathrattle(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override = 0;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override = 0; // boilerplate that every card needs...
    // TODO: Move summon mechanic to base class
    // summon() must be overriden if called,
    // but don't want to force it since not all drattles summon
    // Think of it as a mutable callback...
    // virtual std::shared_ptr<BgBaseCard> summon() override {throw std::runtime_error("summon() not implemented"); }    
};

// class RebornCard : public BgBaseCard {
// public:
//     using BgBaseCard::BgBaseCard;
//     virtual void do_deathrattle(Board* b1, Board* b2) override;
//     virtual std::shared_ptr<BgBaseCard> get_copy() override = 0; // boilerplate that every card needs...
// }

class PirateCard : virtual public BgBaseCard {
public:
    using BgBaseCard::BgBaseCard;
    void do_preattack(std::shared_ptr<BgBaseCard> defender,
    		      Board* b1,
    		      Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override = 0; // boilerplate that every card needs...
};

class TargetedBattlecryCard : virtual public BgBaseCard {
public:
    virtual void targeted_battlecry(std::shared_ptr<BgBaseCard>, Player*) override;
    virtual void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override = 0;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override = 0; // boilerplate that every card needs...
};

class Alleycat : public BattlecryCard {
public:
    Alleycat() : BgBaseCard(1, "NEUTRAL", 1, 1, "Alleycat",
			  "['BATTLECRY']", "BEAST", "", 1, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Alleycat>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class AlleycatGolden : public BattlecryCard {
public:
    AlleycatGolden() : BgBaseCard(2, "NEUTRAL", 1, 2, "Alleycat (Golden)",
			  "['BATTLECRY']", "BEAST", "", 1, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<AlleycatGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class Amalgadon : public BattlecryCard {
public:
    Amalgadon() : BgBaseCard(6, "NEUTRAL", 8, 6, "Amalgadon",
			     "['BATTLECRY']", "ALL", "", 6, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Amalgadon>(*this); } // boilerplate that every drattle needs...
};

class AmalgadonGolden : public BattlecryCard {
public:
    AmalgadonGolden() : BgBaseCard(12, "NEUTRAL", 8, 12, "Amalgadon (Golden)",
				   "['BATTLECRY']", "ALL", "", 6, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<AmalgadonGolden>(*this); } // boilerplate that every drattle needs...
private:
    Amalgadon am;
};

class AnnihilanBattlemaster : public BattlecryCard {
public:
    AnnihilanBattlemaster() : BgBaseCard(3, "NEUTRAL", 8, 1, "Annihilan Battlemaster",
					 "['BATTLECRY']", "DEMON", "EPIC", 5, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<AnnihilanBattlemaster>(*this); } // boilerplate that every drattle needs...
};

class AnnihilanBattlemasterGolden : public BattlecryCard {
public:
    AnnihilanBattlemasterGolden() : BgBaseCard(6, "NEUTRAL", 8, 2, "Annihilan Battlemaster (Golden)",
					       "['BATTLECRY']", "DEMON", "EPIC", 5, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<AnnihilanBattlemasterGolden>(*this); } // boilerplate that every drattle needs...
};

class ArcaneAssistant : public BattlecryCard {
public:
    ArcaneAssistant() : BgBaseCard(3, "NEUTRAL", -1, 3, "Arcane Assistant",
				   "", "ELEMENTAL", "", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ArcaneAssistant>(*this); } // boilerplate that every drattle needs...
};

class ArcaneAssistantGolden : public BattlecryCard {
public:
    ArcaneAssistantGolden() : BgBaseCard(6, "NEUTRAL", -1, 6, "Arcane Assistant (Golden)",
					 "", "ELEMENTAL", "", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ArcaneAssistantGolden>(*this); } // boilerplate that every drattle needs...
};

class BloodsailCannoneer : public BattlecryCard {
public:
    BloodsailCannoneer() : BgBaseCard(4, "NEUTRAL", 4, 3, "Bloodsail Cannoneer",
				      "", "PIRATE", "", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<BloodsailCannoneer>(*this); } // boilerplate that every drattle needs...
};

class BloodsailCannoneerGolden : public BattlecryCard {
public:
    BloodsailCannoneerGolden() : BgBaseCard(8, "NEUTRAL", 4, 6, "Bloodsail Cannoneer (Golden)",
					    "", "PIRATE", "", 3,  "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<BloodsailCannoneerGolden>(*this); } // boilerplate that every drattle needs...
};

class CobaltScalebane : public BgBaseCard {
public:
    CobaltScalebane() : BgBaseCard(5, "NEUTRAL", 5, 5, "Cobalt Scalebane",
				   "['TRIGGER_VISUAL']", "DRAGON", "COMMON", 4, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<CobaltScalebane>(*this); } // boilerplate that every drattle needs...
};

class CobaltScalebaneGolden : public BgBaseCard {
public:
    CobaltScalebaneGolden() : BgBaseCard(10, "NEUTRAL", 5, 10, "Cobalt Scalebane (Golden)",
					 "['TRIGGER_VISUAL']", "DRAGON", "COMMON", 4, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<CobaltScalebaneGolden>(*this); } // boilerplate that every drattle needs...
};

class ColdlightSeer : public BattlecryCard {
public:
    ColdlightSeer() : BgBaseCard(2, "NEUTRAL", 3, 3, "Coldlight Seer",
				 "['BATTLECRY']", "MURLOC", "RARE", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ColdlightSeer>(*this); } // boilerplate that every drattle needs...
};

class ColdlightSeerGolden : public BattlecryCard {
public:
    ColdlightSeerGolden() : BgBaseCard(4, "NEUTRAL", 3, 6, "Coldlight Seer (Golden)",
				       "['BATTLECRY']", "MURLOC", "RARE", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ColdlightSeerGolden>(*this); } // boilerplate that every drattle needs...
};

class CrowdFavorite : public BgBaseCard {
public:
    CrowdFavorite() : BgBaseCard(4, "NEUTRAL", 4, 4, "Crowd Favorite",
				 "['TRIGGER_VISUAL']", "", "EPIC", 3, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<CrowdFavorite>(*this); } // boilerplate that every drattle needs...
};

class CrowdFavoriteGolden : public BgBaseCard {
public:
    CrowdFavoriteGolden() : BgBaseCard(8, "NEUTRAL", 4, 8, "Crowd Favorite (Golden)",
				       "['TRIGGER_VISUAL']", "", "EPIC", 3, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board* b1, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<CrowdFavoriteGolden>(*this); } // boilerplate that every drattle needs...
private:
    CrowdFavorite cf;
};

class Crystalweaver : public BattlecryCard {
public:
    Crystalweaver() : BgBaseCard(5, "WARLOCK", 4, 4, "Crystalweaver",
				 "['BATTLECRY']", "", "COMMON", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Crystalweaver>(*this); } // boilerplate that every drattle needs...
    virtual void do_battlecry(Player*) override;    
};

class CrystalweaverGolden : public BattlecryCard {
public:
    CrystalweaverGolden() : BgBaseCard(10, "WARLOCK", 4, 8, "Crystalweaver (Golden)",
				       "['BATTLECRY']", "", "COMMON", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<CrystalweaverGolden>(*this); } // boilerplate that every drattle needs...
    virtual void do_battlecry(Player*) override;
private:
    Crystalweaver cw;
};

class DefenderOfArgus : public BattlecryCard {
public:
    DefenderOfArgus() : BgBaseCard(2, "NEUTRAL", 4, 3, "Defender of Argus",
				   "['BATTLECRY']", "", "RARE", 4, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<DefenderOfArgus>(*this); } // boilerplate that every drattle needs...
};

class DefenderOfArgusGolden : public BattlecryCard {
public:
    DefenderOfArgusGolden() : BgBaseCard(4, "NEUTRAL", 4, 6, "Defender of Argus (Golden)",
					 "['BATTLECRY']", "", "RARE", 4, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<DefenderOfArgusGolden>(*this); } // boilerplate that every drattle needs...
};

//>>>>>>> Stashed changes
class Djinni : public DeathrattleCard {
public:
    Djinni() : BgBaseCard(6, "NEUTRAL", 8, 8, "Djinni",
			  "['DEATHRATTLE']", "ELEMENTAL", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Djinni>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class DjinniGolden : public DeathrattleCard {
public:
    DjinniGolden() : BgBaseCard(12, "NEUTRAL", 8, 16, "Djinni (Golden)",
				"['DEATHRATTLE']", "ELEMENTAL", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<DjinniGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    Djinni dj;
};


class DreadAdmiralEliza : public PirateCard {
public:
    DreadAdmiralEliza() : BgBaseCard(6, "NEUTRAL", 6, 7, "Dread Admiral Eliza",
				     "['TRIGGER_VISUAL']", "PIRATE", "", 6, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<DreadAdmiralEliza>(*this); } // boilerplate that every drattle needs...
};

class DreadAdmiralElizaGolden : public PirateCard {
public:
    DreadAdmiralElizaGolden() : BgBaseCard(12, "NEUTRAL", 6, 14, "Dread Admiral Eliza (Golden)",
					   "['TRIGGER_VISUAL']", "PIRATE", "", 6, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<DreadAdmiralElizaGolden>(*this); } // boilerplate that every drattle needs...
};

class FelfinNavigator : public BattlecryCard {
public:
    FelfinNavigator() : BgBaseCard(4, "NEUTRAL", 4, 4, "Felfin Navigator",
				   "['BATTLECRY']", "MURLOC", "COMMON", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<FelfinNavigator>(*this); } // boilerplate that every drattle needs...
};

class FelfinNavigatorGolden : public BattlecryCard {
public:
    FelfinNavigatorGolden() : BgBaseCard(8, "NEUTRAL", 4, 8, "Felfin Navigator (Golden)",
					 "['BATTLECRY']", "MURLOC", "COMMON", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<FelfinNavigatorGolden>(*this); } // boilerplate that every drattle needs...
};


class FiendishServant : public DeathrattleCard {
public:
    FiendishServant() : BgBaseCard(2, "WARLOCK", 1, 1, "Fiendish Servant",
				   "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<FiendishServant>(*this); } // boilerplate that every drattle needs...
};

class FiendishServantGolden : public DeathrattleCard {
public:
    FiendishServantGolden() : BgBaseCard(4, "WARLOCK", 1, 2, "Fiendish Servant (Golden)",
					      "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<FiendishServantGolden>(*this); } // boilerplate that every drattle needs...
private:
    FiendishServant serv;
};

// Floating Watcher handled by player class

class FreedealingGambler : public PirateCard {
public:
    FreedealingGambler() : BgBaseCard(3, "NEUTRAL", 3, 3, "Freedealing Gambler",
				      "", "PIRATE", "", 2, "MINION") {}
    void on_sell(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<FreedealingGambler>(*this); } // boilerplate that every drattle needs...
};

class FreedealingGamblerGolden : public PirateCard {
public:
    FreedealingGamblerGolden() : BgBaseCard(6, "NEUTRAL", 3, 6, "Freedealing Gambler (Golden)",
					    "", "PIRATE", "", 2, "MINION") {}
    void on_sell(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<FreedealingGamblerGolden>(*this); } // boilerplate that every drattle needs...
};

class Ghastcoiler : public DeathrattleCard {
public:
    Ghastcoiler() : BgBaseCard(7, "PRIEST", 6, 7, "Ghastcoiler",
			       "['DEATHRATTLE']", "BEAST", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Ghastcoiler>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;    
};

class GhastcoilerGolden : public DeathrattleCard {
public:
    GhastcoilerGolden() : BgBaseCard(14, "PRIEST", 6, 14, "Ghastcoiler (Golden)",
					  "['DEATHRATTLE']", "BEAST", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<GhastcoilerGolden>(*this); } // boilerplate that every drattle needs...
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
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<GlyphGuardian>(*this); } // boilerplate that every drattle needs...
};

class GlyphGuardianGolden : public BgBaseCard {
public:
    GlyphGuardianGolden() : BgBaseCard(4, "MAGE", 4, 8, "Glyph Guardian (Golden)",
				       "", "DRAGON", "", 2, "MINION") {}
    virtual void do_preattack(std::shared_ptr<BgBaseCard> defender,
			      Board* b1,
			      Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<GlyphGuardianGolden>(*this); } // boilerplate that every drattle needs...
private:
    GlyphGuardian coiler;
};

class Goldgrubber : public BgBaseCard {
public:
    Goldgrubber() : BgBaseCard(2, "NEUTRAL", 5, 2, "Goldgrubber",
			       "['TRIGGER_VISUAL']", "PIRATE", "", 4, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Goldgrubber>(*this); } // boilerplate that every drattle needs...
};

class GoldgrubberGolden : public BgBaseCard {
public:
    GoldgrubberGolden() : BgBaseCard(4, "NEUTRAL", 5, 4, "Goldgrubber (Golden)",
				     "['TRIGGER_VISUAL']", "PIRATE", "", 4, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<GoldgrubberGolden>(*this); } // boilerplate that every drattle needs...
};

class Goldrinn : public DeathrattleCard {
public:
    Goldrinn() : BgBaseCard(4, "NEUTRAL", 8, 4, "Goldrinn",
				 "['DEATHRATTLE']", "BEAST", "LEGENDARY", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Goldrinn>(*this); } // boilerplate that every drattle needs...
};

class GoldrinnGolden : public DeathrattleCard {
public:
    GoldrinnGolden() : BgBaseCard(8, "NEUTRAL", 8, 8, "Goldrinn (Golden)",
				       "['DEATHRATTLE']", "BEAST", "LEGENDARY", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<GoldrinnGolden>(*this); } // boilerplate that every drattle needs...
private:
    Goldrinn bag;
};


class HarvestGolem : public DeathrattleCard {
public:
    HarvestGolem() : BgBaseCard(2, "NEUTRAL", 3, 3, "Harvest Golem",
				"['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<HarvestGolem>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class HarvestGolemGolden : public DeathrattleCard {
public:
    HarvestGolemGolden() : BgBaseCard(4, "NEUTRAL", 3, 6, "Harvest Golem (Golden)",
				"['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<HarvestGolemGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class HeraldOfFlame : public BgBaseCard {
public:
    HeraldOfFlame() : BgBaseCard(5, "WARRIOR", 5, 6, "Herald of Flame",
				 "['OVERKILL']", "DRAGON", "", 4, "MINION") {}
    virtual void do_postattack(std::shared_ptr<BgBaseCard> defender,
			       int def_pos,
			       Board* b1,
			       Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<HeraldOfFlame>(*this); } // boilerplate that every drattle needs...
};

class HeraldOfFlameGolden : public BgBaseCard {
public:
    HeraldOfFlameGolden() : BgBaseCard(10, "WARRIOR", 5, 12, "Herald of Flame (Golden)",
				       "['OVERKILL']", "DRAGON", "", 4, "MINION") {}
    virtual void do_postattack(std::shared_ptr<BgBaseCard> defender,
			       int def_pos,
			       Board* b1,
			       Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<HeraldOfFlameGolden>(*this); } // boilerplate that every drattle needs...
};

class Houndmaster : public TargetedBattlecryCard {
public:
    Houndmaster() : BgBaseCard(4, "HUNTER", 4, 3, "Houndmaster",
			       "['BATTLECRY']", "", "FREE", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Houndmaster>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class HoundmasterGolden : public TargetedBattlecryCard {
public:
    HoundmasterGolden() : BgBaseCard(8, "HUNTER", 4, 6, "Houndmaster (Golden)",
				     "['BATTLECRY']", "", "FREE", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<HoundmasterGolden>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};


class ImpGangBoss : public BgBaseCard {
public:
    ImpGangBoss() : BgBaseCard(2, "WARLOCK", 3, 4, "Imp Gang Boss",
			       "['TRIGGER_VISUAL']", "DEMON", "COMMON", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ImpGangBoss>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
    void take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) override;
};

class ImpGangBossGolden : public BgBaseCard {
public:
    ImpGangBossGolden() : BgBaseCard(4, "WARLOCK", 3, 8, "Imp Gang Boss (Golden)",
				     "['TRIGGER_VISUAL']", "DEMON", "COMMON", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ImpGangBossGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
    void take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) override;
};

class ImpMama : public BgBaseCard {
public:
    ImpMama() : BgBaseCard(6, "WARLOCK", 8, 10, "Imp Mama",
			   "['TRIGGER_VISUAL']", "DEMON", "", 6, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ImpMama>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
    void take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) override;
};

class ImpMamaGolden : public BgBaseCard {
public:
    ImpMamaGolden() : BgBaseCard(12, "WARLOCK", 8, 20, "Imp Mama (Golden)",
				 "['TRIGGER_VISUAL']", "DEMON", "", 6, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ImpMamaGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
    void take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) override;
private:
    ImpMama imp_mama;
};


class Imprisoner : public DeathrattleCard {
public:
    Imprisoner() : BgBaseCard(3, "NEUTRAL", 3, 3, "Imprisoner",
			      "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Imprisoner>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class ImprisonerGolden : public DeathrattleCard {
public:
    ImprisonerGolden() : BgBaseCard(6, "NEUTRAL", 3, 6, "ImprisonerGolden",
			      "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ImprisonerGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class InfestedWolf : public DeathrattleCard {
public:
    InfestedWolf() : BgBaseCard(3, "HUNTER", 4, 3, "Infested Wolf",
				"['DEATHRATTLE']", "BEAST", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<InfestedWolf>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class InfestedWolfGolden : public DeathrattleCard {
public:
    InfestedWolfGolden() : BgBaseCard(6, "HUNTER", 4, 6, "Infested Wolf (Golden)",
				      "['DEATHRATTLE']", "BEAST", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<InfestedWolfGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class IronhideDirehorn : public BgBaseCard {
public:
    IronhideDirehorn() : BgBaseCard(7, "DRUID", 7, 7, "Ironhide Direhorn",
				 "['OVERKILL']", "BEAST", "", 5, "MINION") {}
    virtual void do_postattack(std::shared_ptr<BgBaseCard> defender,
			       int def_pos,
			       Board* b1,
			       Board* b2) override;
    std::shared_ptr<BgBaseCard> summon() override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<IronhideDirehorn>(*this); } // boilerplate that every drattle needs...
};

class IronhideDirehornGolden : public BgBaseCard {
public:
    IronhideDirehornGolden() : BgBaseCard(14, "DRUID", 7, 14, "Ironhide Direhorn (Golden)",
				       "['OVERKILL']", "BEAST", "", 5, "MINION") {}
    virtual void do_postattack(std::shared_ptr<BgBaseCard> defender,
			       int def_pos,
			       Board* b1,
			       Board* b2) override;
    std::shared_ptr<BgBaseCard> summon() override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<IronhideDirehornGolden>(*this); } // boilerplate that every drattle needs...
};

class IronSensei : public BgBaseCard {
public:
    IronSensei() : BgBaseCard(2, "ROGUE", 3, 2, "Iron Sensei",
			      "['TRIGGER_VISUAL']", "MECHANICAL", "RARE", 2, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<IronSensei>(*this); } // boilerplate that every drattle needs...
};

class IronSenseiGolden : public BgBaseCard {
public:
    IronSenseiGolden() : BgBaseCard(4, "ROGUE", 3, 4, "Iron Sensei (Golden)",
				    "['TRIGGER_VISUAL']", "MECHANICAL", "RARE", 3, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<IronSenseiGolden>(*this); } // boilerplate that every drattle needs...
};

class Junkbot : public BgBaseCard {
public:
    Junkbot() : BgBaseCard(1, "NEUTRAL", 5, 5, "Junkbot",
			   "['TRIGGER_VISUAL']", "MECHANICAL", "EPIC", 5, "MINION") {}
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;    
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Junkbot>(*this); } // boilerplate that every drattle needs...
};

class JunkbotGolden : public BgBaseCard {
public:
    JunkbotGolden() : BgBaseCard(2, "NEUTRAL", 5, 10, "Junkbot (Golden)",
				 "['TRIGGER_VISUAL']", "MECHANICAL", "EPIC", 5, "MINION") {}
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<JunkbotGolden>(*this); } // boilerplate that every drattle needs...
private:
    Junkbot junk_bot;
};

class KaboomBot : public DeathrattleCard {
public:
    KaboomBot() : BgBaseCard(2, "NEUTRAL", 3, 2, "Kaboom Bot",
			     "['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<KaboomBot>(*this); } // boilerplate that every drattle needs...
};

class KaboomBotGolden : public DeathrattleCard {
public:
    KaboomBotGolden() : BgBaseCard(4, "NEUTRAL", 3, 4, "Kaboom Bot (Golden)",
			     "['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<KaboomBotGolden>(*this); } // boilerplate that every drattle needs...
private:
    KaboomBot kbot;
};

class Kalecgos : public BgBaseCard {
public:
    Kalecgos() : BgBaseCard(4, "NEUTRAL", 8, 12, "Kalecgos, Arcane Aspect",
			    "['TRIGGER_VISUAL']", "DRAGON", "", 6, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Kalecgos>(*this); } // boilerplate that every drattle needs...
};

class KalecgosGolden : public BgBaseCard {
public:
    KalecgosGolden() : BgBaseCard(8, "NEUTRAL", 8, 24, "Kalecgos, Arcane Aspect (Golden)",
				  "['TRIGGER_VISUAL']", "DRAGON", "", 6, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<KalecgosGolden>(*this); } // boilerplate that every drattle needs...
private:
    Kalecgos k;
};

class Kangor : public DeathrattleCard {
public:
    Kangor() : BgBaseCard(3, "NEUTRAL", 9, 6, "Kangor",
			       "['DEATHRATTLE']", "", "EPIC", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Kangor>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
    void reset_mech_queue(Board* b);
    std::queue<std::string> mech_queue;
};

class KangorGolden : public DeathrattleCard {
public:
    KangorGolden() : BgBaseCard(6, "NEUTRAL", 9, 12, "Kangor (Golden)",
				     "['DEATHRATTLE']", "", "EPIC", 6,"MINION") {}
				    
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<KangorGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    Kangor kang;
};

class KindlyGrandmother : public DeathrattleCard {
public:
    KindlyGrandmother() : BgBaseCard(1, "HUNTER", 2, 1, "Kindly Grandmother",
					  "['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<KindlyGrandmother>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class KindlyGrandmotherGolden : public DeathrattleCard {
public:
    KindlyGrandmotherGolden() : BgBaseCard(1, "HUNTER", 2, 1, "Kindly Grandmother (Golden)",
						"['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<KindlyGrandmotherGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class KingBagurgle : public DeathrattleCard, public BattlecryCard {
public:
    KingBagurgle() : BgBaseCard(6, "NEUTRAL", 6, 3, "King Bagurgle",
				"['BATTLECRY', 'DEATHRATTLE']", "MURLOC", "LEGENDARY", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual void do_battlecry(Player* p1) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<KingBagurgle>(*this); } // boilerplate that every drattle needs...
};

class KingBagurgleGolden : public DeathrattleCard, public BattlecryCard {
public:
    KingBagurgleGolden() : BgBaseCard(12, "NEUTRAL", 6, 6, "King Bagurgle (Golden)",
					   "['BATTLECRY', 'DEATHRATTLE']", "MURLOC", "LEGENDARY", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual void do_battlecry(Player* p1) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<KingBagurgleGolden>(*this); } // boilerplate that every drattle needs...
private:
    KingBagurgle bag;
};

class LieutenantGarr : public BgBaseCard {
public:
    LieutenantGarr() : BgBaseCard(5, "NEUTRAL", 8, 1, "Lieutenant Garr",
				  "", "ELEMENTAL", "", 6, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<LieutenantGarr>(*this); } // boilerplate that every drattle needs...
};

class LieutenantGarrGolden : public BgBaseCard {
public:
    LieutenantGarrGolden() : BgBaseCard(10, "NEUTRAL", 8, 2, "Lieutenant Garr (Golden)",
					"", "ELEMENTAL", "", 6, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<LieutenantGarrGolden>(*this); } // boilerplate that every drattle needs...
private:
    LieutenantGarr lt;
};

class LightfangEnforcer : public BgBaseCard {
public:
    LightfangEnforcer() : BgBaseCard(2, "NEUTRAL", 6, 2, "Lightfang Enforcer",
				     "['TRIGGER_VISUAL']", "", "EPIC", 5, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<LightfangEnforcer>(*this); } // boilerplate that every drattle needs...
};

class LightfangEnforcerGolden : public BgBaseCard {
public:
    LightfangEnforcerGolden() : BgBaseCard(4, "NEUTRAL", 6, 4, "Lightfang Enforcer (Golden)",
					   "['TRIGGER_VISUAL']", "", "EPIC", 5, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<LightfangEnforcerGolden>(*this); } // boilerplate that every drattle needs...
};

class LilRag : public BgBaseCard {
public:
    LilRag() : BgBaseCard(4, "NEUTRAL", 4, 4, "Lil' Rag",
			  "", "ELEMENTAL", "", 6, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<LilRag>(*this); } // boilerplate that every drattle needs...
};

class LilRagGolden : public BgBaseCard {
public:
    LilRagGolden() : BgBaseCard(8, "NEUTRAL", 4, 8, "Lil' Rag (Golden)",
				"", "ELEMENTAL", "", 6, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<LilRagGolden>(*this); } // boilerplate that every drattle needs...
private:
    LilRag lr;
};

// Fake card, but can be added to adapt deathrattle_card list, similar to magnetic mechanic
class LivingSporeDrattle : public DeathrattleCard {
public:
    LivingSporeDrattle() : BgBaseCard(0, "N/A", 0, 0, "Living Spore Drattle",
				      "['DEATHRATTLE']", "", "COMMON", 0, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<LivingSporeDrattle>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class MajordomoExecutus : public BgBaseCard {
public:
    MajordomoExecutus() : BgBaseCard(6, "NEUTRAL", 6, 3, "Majordomo Executus",
				     "", "", "", 4, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MajordomoExecutus>(*this); } // boilerplate that every drattle needs...
};

class MajordomoExecutusGolden : public BgBaseCard {
public:
    MajordomoExecutusGolden() : BgBaseCard(12, "NEUTRAL", 6, 6, "Majordomo Executus (Golden)",
					   "", "", "", 4, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MajordomoExecutusGolden>(*this); } // boilerplate that every drattle needs...
private:
    MajordomoExecutus majordomo;
};

class MalGanis : public BgBaseCard {
public:
    MalGanis() : BgBaseCard(9, "WARLOCK", 9, 7, "Mal'Ganis",
			    "['AURA']", "DEMON", "LEGENDARY", 5, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MalGanis>(*this); } // boilerplate that every drattle needs...
};

class MalGanisGolden : public BgBaseCard {
public:
    MalGanisGolden() : BgBaseCard(18, "WARLOCK", 9, 14, "Mal'Ganis (Golden)",
				  "['AURA']", "DEMON", "LEGENDARY", 5, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MalGanisGolden>(*this); } // boilerplate that every drattle needs...
private:
    MalGanis rw;
};

class MamaBear : public BgBaseCard {
public:
    MamaBear() : BgBaseCard(4, "NEUTRAL", 8, 4, "Mama Bear",
			    "['TRIGGER_VISUAL']", "BEAST", "EPIC", 5, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MamaBear>(*this); } // boilerplate that every drattle needs...
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
};

class MamaBearGolden : public BgBaseCard {
public:
    MamaBearGolden() : BgBaseCard(8, "NEUTRAL", 8, 8, "Mama Bear (Golden)",
				  "['TRIGGER_VISUAL']", "BEAST", "EPIC", 6, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MamaBearGolden>(*this); } // boilerplate that every drattle needs...
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
private:
    MamaBear pl;
};


class MechanoEgg : public DeathrattleCard {
public:
    MechanoEgg() : BgBaseCard(0, "NEUTRAL", 5, 5, "Mechano-Egg",
				   "['DEATHRATTLE']", "MECHANICAL", "COMMON", 4, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MechanoEgg>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class MechanoEggGolden : public DeathrattleCard {
public:
    MechanoEggGolden() : BgBaseCard(0, "PALADIN", 5, 10, "Mechano-Egg (Golden)",
				 "['DEATHRATTLE']", "MECHANICAL", "COMMON", 4, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MechanoEggGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class Mecharoo : public DeathrattleCard {
public:
    Mecharoo() : BgBaseCard(1, "NEUTRAL", 1, 1, "Mecharoo",
				       "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Mecharoo>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class MecharooGolden : public DeathrattleCard {
public:
    MecharooGolden() : BgBaseCard(2, "NEUTRAL", 1, 2, "Mecharoo (Golden)",
				       "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MecharooGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class MenagerieJug : public BattlecryCard {
public:
    MenagerieJug() : BgBaseCard(3, "NEUTRAL", 5, 3, "Menagerie Jug",
				"['BATTLECRY']", "", "COMMON", 2, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MenagerieJug>(*this); } // boilerplate that every drattle needs...
};

class MenagerieJugGolden : public BattlecryCard {
public:
    MenagerieJugGolden() : BgBaseCard(6, "NEUTRAL", 5, 6, "Menagerie Jug (Golden)",
				      "['BATTLECRY']", "", "COMMON", 2, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MenagerieJugGolden>(*this); } // boilerplate that every drattle needs...
};


class MenagerieMug : public BattlecryCard {
public:
    MenagerieMug() : BgBaseCard(2, "NEUTRAL", 3, 2, "Menagerie Mug",
				"['BATTLECRY']", "", "COMMON", 2, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MenagerieMug>(*this); } // boilerplate that every drattle needs...
};

class MenagerieMugGolden : public BattlecryCard {
public:
    MenagerieMugGolden() : BgBaseCard(4, "NEUTRAL", 3, 4, "Menagerie Mug (Golden)",
				      "['BATTLECRY']", "", "COMMON", 2, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MenagerieMugGolden>(*this); } // boilerplate that every drattle needs...
};


class MetaltoothLeaper : public BattlecryCard {
public:
    MetaltoothLeaper() : BgBaseCard(3, "HUNTER", 3, 3, "Metaltooth Leaper",
				    "['BATTLECRY']", "MECHANICAL", "RARE", 2, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MetaltoothLeaper>(*this); } // boilerplate that every drattle needs...
};

class MetaltoothLeaperGolden : public BattlecryCard {
public:
    MetaltoothLeaperGolden() : BgBaseCard(6, "HUNTER", 3, 6, "Metaltooth Leaper (Golden)",
					  "['BATTLECRY']", "MECHANICAL", "RARE", 2, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MetaltoothLeaperGolden>(*this); } // boilerplate that every drattle needs...
private:
    MetaltoothLeaper leaper;
};

class MicroMachine : public BgBaseCard {
public:
    MicroMachine() : BgBaseCard(1, "NEUTRAL", 2, 2, "Micro Machine",
				"['TRIGGER_VISUAL']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void start_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MicroMachine>(*this); } // boilerplate that every drattle needs...
};

class MicroMachineGolden : public BgBaseCard {
public:
    MicroMachineGolden() : BgBaseCard(2, "NEUTRAL", 2, 4, "Micro Machine (Golden)",
				      "['TRIGGER_VISUAL']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void start_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MicroMachineGolden>(*this); } // boilerplate that every drattle needs...
};

class MicroMummy : public BgBaseCard {
public:
    MicroMummy() : BgBaseCard(1, "NEUTRAL", 2, 2, "Micro Mummy",
			      "['TRIGGER_VISUAL']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MicroMummy>(*this); } // boilerplate that every drattle needs...
};

class MicroMummyGolden : public BgBaseCard {
public:
    MicroMummyGolden() : BgBaseCard(2, "NEUTRAL", 2, 4, "Micro Mummy (Golden)",
				    "['TRIGGER_VISUAL']", "MECHANICAL", "COMMON", 1, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MicroMummyGolden>(*this); } // boilerplate that every drattle needs...
};

class MonstrousMacaw : public BgBaseCard {
public:
    MonstrousMacaw() : BgBaseCard(4, "NEUTRAL", 3, 3, "Monstrous Macaw",
				  "['TRIGGER_VISUAL']", "BEAST", "", 3, "MINION") {}
    virtual void do_preattack(std::shared_ptr<BgBaseCard> defender,
			      Board* b1,
			      Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MonstrousMacaw>(*this); } // boilerplate that every drattle needs...
};

class MonstrousMacawGolden : public BgBaseCard {
public:
    MonstrousMacawGolden() : BgBaseCard(8, "NEUTRAL", 3, 6, "Monstrous Macaw (Golden)",
					"['TRIGGER_VISUAL']", "BEAST", "", 3, "MINION") {}
    virtual void do_preattack(std::shared_ptr<BgBaseCard> defender,
			      Board* b1,
			      Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MonstrousMacawGolden>(*this); } // boilerplate that every drattle needs...
private:
    MonstrousMacaw macaw;
};

class MurlocTidecaller : public BgBaseCard {
public:
    MurlocTidecaller() : BgBaseCard(1, "NEUTRAL", 1, 2, "Murloc Tidecaller",
				    "['TRIGGER_VISUAL']", "MURLOC", "RARE", 1, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MurlocTidecaller>(*this); } // boilerplate that every drattle needs...
};

class MurlocTidecallerGolden : public BgBaseCard {
public:
    MurlocTidecallerGolden() : BgBaseCard(2, "NEUTRAL", 1, 4, "Murloc Tidecaller (Golden)",
					  "['TRIGGER_VISUAL']", "MURLOC", "RARE", 1, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MurlocTidecallerGolden>(*this); } // boilerplate that every drattle needs...
};

class MurlocTidehunter : public BattlecryCard {
public:
    MurlocTidehunter() : BgBaseCard(2, "NEUTRAL", 2, 1, "Murloc Tidehunter",
				    "['BATTLECRY']", "MURLOC", "FREE", 1, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MurlocTidehunter>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class MurlocTidehunterGolden : public BattlecryCard {
public:
    MurlocTidehunterGolden() : BgBaseCard(4, "NEUTRAL", 2, 2, "Murloc Tidehunter (Golden)",
					  "['BATTLECRY']", "MURLOC", "FREE", 1, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MurlocTidehunterGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};


class MurlocWarleader : public BgBaseCard {
public:
    MurlocWarleader() : BgBaseCard(3, "NEUTRAL", 3, 3, "Murloc Warleader",
				   "['AURA']", "MURLOC", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MurlocWarleader>(*this); } // boilerplate that every drattle needs...
};

class MurlocWarleaderGolden : public BgBaseCard {
public:
    MurlocWarleaderGolden() : BgBaseCard(6, "NEUTRAL", 3, 6, "Murloc Warleader (Golden)",
					    "['AURA']", "MURLOC", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MurlocWarleaderGolden>(*this); } // boilerplate that every drattle needs...
private:
    MurlocWarleader rw;
};

class MythraxTheUnraveler : public BgBaseCard {
public:
    MythraxTheUnraveler() : BgBaseCard(4, "NEUTRAL", 5, 4, "Mythrax the Unraveler",
				     "['TRIGGER_VISUAL']", "", "COMMON", 5, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MythraxTheUnraveler>(*this); } // boilerplate that every drattle needs...
};

class MythraxTheUnravelerGolden : public BgBaseCard {
public:
    MythraxTheUnravelerGolden() : BgBaseCard(8, "NEUTRAL", 5, 8, "Mythrax the Unraveler (Golden)",
					     "['TRIGGER_VISUAL']", "", "COMMON", 5, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<MythraxTheUnravelerGolden>(*this); } // boilerplate that every drattle needs...
};

class Nadina : public DeathrattleCard {
public:
    Nadina() : BgBaseCard(7, "NEUTRAL", 6, 4, "Nadina",
			       "['DEATHRATTLE']", "", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Nadina>(*this); } // boilerplate that every drattle needs...
};

class NadinaGolden : public DeathrattleCard {
public:
    NadinaGolden() : BgBaseCard(14, "NEUTRAL", 6, 8, "Nadina (Golden)",
				     "['DEATHRATTLE']", "", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<NadinaGolden>(*this); } // boilerplate that every drattle needs...
private:
    Nadina bag;
};

class NathrezimOverseer : public TargetedBattlecryCard {
public:
    NathrezimOverseer() : BgBaseCard(2, "NEUTRAL", 3, 3, "Nathrezim Overseer",
				     "['BATTLECRY']", "DEMON", "RARE", 2, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<NathrezimOverseer>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class NathrezimOverseerGolden : public TargetedBattlecryCard {
public:
    NathrezimOverseerGolden() : BgBaseCard(4, "NEUTRAL", 3, 6, "Nathrezim Overseer (Golden)",
					   "['BATTLECRY']", "DEMON", "RARE", 2, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<NathrezimOverseerGolden>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};


class OldMurkeye : public BgBaseCard {
public:
    OldMurkeye() : BgBaseCard(2, "NEUTRAL", 4, 4, "Old Murk-Eye",
				 "['CHARGE']", "MURLOC", "LEGENDARY", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;    
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<OldMurkeye>(*this); } // boilerplate that every drattle needs...
};

class OldMurkeyeGolden : public BgBaseCard {
public:
    OldMurkeyeGolden() : BgBaseCard(4, "NEUTRAL", 4, 8, "Old Murk-Eye (Golden)",
				       "['CHARGE']", "MURLOC", "LEGENDARY", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;    
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<OldMurkeyeGolden>(*this); } // boilerplate that every drattle needs...
private:
    OldMurkeye rw;
};

class PackLeader : public BgBaseCard {
public:
    PackLeader() : BgBaseCard(3, "NEUTRAL", 3, 3, "Pack Leader",
			      "['TRIGGER_VISUAL']", "", "RARE", 2, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<PackLeader>(*this); } // boilerplate that every drattle needs...
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
};

class PackLeaderGolden : public BgBaseCard {
public:
    PackLeaderGolden() : BgBaseCard(6, "NEUTRAL", 3, 6, "Pack Leader (Golden)",
				    "['TRIGGER_VISUAL']", "", "RARE", 2, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<PackLeaderGolden>(*this); } // boilerplate that every drattle needs...
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
private:
    PackLeader pl;
};


class PilotedShredder : public DeathrattleCard {
public:
    PilotedShredder() : BgBaseCard(4, "NEUTRAL", 4, 3, "Piloted Shredder",
					"['DEATHRATTLE']", "MECHANICAL", "COMMON", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<PilotedShredder>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class PilotedShredderGolden : public DeathrattleCard {
public:
    PilotedShredderGolden() : BgBaseCard(8, "NEUTRAL", 4, 6, "Piloted Shredder (Golden)",
					      "['DEATHRATTLE']", "MECHANICAL", "COMMON", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<PilotedShredderGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    PilotedShredder shredder;
};

class RatPack : public DeathrattleCard {
public:
    RatPack() : BgBaseCard(2, "HUNTER", 3, 2, "Rat Pack",
			   "['DEATHRATTLE']", "BEAST", "EPIC", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RatPack>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class RatPackGolden : public DeathrattleCard {
public:
    RatPackGolden() : BgBaseCard(4, "HUNTER", 3, 4, "Rat Pack (Golden)",
			   "['DEATHRATTLE']", "BEAST", "EPIC", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RatPackGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class Razorgore : public BgBaseCard {
public:
    Razorgore() : BgBaseCard(2, "NEUTRAL", 8, 4, "Razorgore, the Untamed",
				   "['TRIGGER_VISUAL']", "DRAGON", "", 5, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Razorgore>(*this); } // boilerplate that every drattle needs...
};

class RazorgoreGolden : public BgBaseCard {
public:
    RazorgoreGolden() : BgBaseCard(4, "NEUTRAL", 8, 8, "Razorgore, the Untamed (Golden)",
				   "['TRIGGER_VISUAL']", "DRAGON", "", 5, "MINION") {}
    virtual void end_turn_mechanic(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RazorgoreGolden>(*this); } // boilerplate that every drattle needs...
};

class RedWhelp : public BgBaseCard {
public:
    RedWhelp() : BgBaseCard(1, "NEUTRAL", 1, 2, "Red Whelp",
			       "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RedWhelp>(*this); } // boilerplate that every drattle needs...
};

class RedWhelpGolden : public BgBaseCard {
public:
    RedWhelpGolden() : BgBaseCard(1, "NEUTRAL", 1, 2, "Red Whelp",
			       "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RedWhelpGolden>(*this); } // boilerplate that every drattle needs...
private:
    RedWhelp rw;
};

class RefreshingAnomaly : public BattlecryCard {
public:
    RefreshingAnomaly() : BgBaseCard(1, "NEUTRAL", -1, 3, "Refreshing Anomaly",
				     "", "ELEMENTAL", "", 1, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RefreshingAnomaly>(*this); } // boilerplate that every drattle needs...
};

class RefreshingAnomalyGolden : public BattlecryCard {
public:
    RefreshingAnomalyGolden() : BgBaseCard(2, "NEUTRAL", -1, 6, "Refreshing Anomaly (Golden)",
					   "", "ELEMENTAL", "", 1, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RefreshingAnomalyGolden>(*this); } // boilerplate that every drattle needs...
};

class ReplicatingMenace : public DeathrattleCard {
public:
    ReplicatingMenace() : BgBaseCard(3, "NEUTRAL", 4, 1, "Replicating Menace",
					  "['DEATHRATTLE', 'MODULAR']", "MECHANICAL", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ReplicatingMenace>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class ReplicatingMenaceGolden : public DeathrattleCard {
public:
    ReplicatingMenaceGolden() : BgBaseCard(6, "NEUTRAL", 4, 2, "Replicating Menace (Golden)",
						"['DEATHRATTLE', 'MODULAR']", "MECHANICAL", "RARE", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ReplicatingMenaceGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class RipsnarlCaptain : public PirateCard {
public:
    RipsnarlCaptain() : BgBaseCard(3, "NEUTRAL", 4, 4, "Ripsnarl Captain",
				   "['TRIGGER_VISUAL']", "PIRATE", "", 4, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RipsnarlCaptain>(*this); } // boilerplate that every drattle needs...
};

class RipsnarlCaptainGolden : public PirateCard {
public:
    RipsnarlCaptainGolden() : BgBaseCard(6, "NEUTRAL", 4, 8, "Ripsnarl Captain (Golden)",
					 "['TRIGGER_VISUAL']", "PIRATE", "", 4, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RipsnarlCaptainGolden>(*this); } // boilerplate that every drattle needs...
};

class RockpoolHunter : public TargetedBattlecryCard {
public:
    RockpoolHunter() : BgBaseCard(2, "NEUTRAL", 2, 3, "Rockpool Hunter",
				  "['BATTLECRY']", "MURLOC", "COMMON", 1, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RockpoolHunter>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class RockpoolHunterGolden : public TargetedBattlecryCard {
public:
    RockpoolHunterGolden() : BgBaseCard(4, "NEUTRAL", 2, 6, "Rockpool Hunter (Golden)",
					"['BATTLECRY']", "MURLOC", "COMMON", 1, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<RockpoolHunterGolden>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class SaltyLooter : public BgBaseCard {
public:
    SaltyLooter() : BgBaseCard(4, "ROGUE", 4, 4, "Salty Looter",
			       "['TRIGGER_VISUAL']", "PIRATE", "", 3, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SaltyLooter>(*this); } // boilerplate that every drattle needs...
};

class SaltyLooterGolden : public BgBaseCard {
public:
    SaltyLooterGolden() : BgBaseCard(8, "ROGUE", 4, 8, "Salty Looter (Golden)",
				     "['TRIGGER_VISUAL']", "PIRATE", "", 3, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SaltyLooterGolden>(*this); } // boilerplate that every drattle needs...
};

class SavannahHighmane : public DeathrattleCard {
public:
    SavannahHighmane() : BgBaseCard(6, "HUNTER", 6, 5, "Savannah Highmane",
					 "['DEATHRATTLE']", "BEAST", "RARE", 4,"MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SavannahHighmane>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class SavannahHighmaneGolden : public DeathrattleCard {
public:
    SavannahHighmaneGolden() : BgBaseCard(12, "HUNTER", 6, 10, "Savannah Highmane (Golden)",
					       "['DEATHRATTLE']", "BEAST", "RARE", 4,"MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SavannahHighmaneGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class Scallywag : public DeathrattleCard, public PirateCard {
public:
    Scallywag() : BgBaseCard(2, "NEUTRAL", 1, 1, "Scallywag",
			     "['DEATHRATTLE']", "PIRATE", "", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Scallywag>(*this); } // boilerplate that every drattle needs...
};

class ScallywagGolden : public DeathrattleCard, public PirateCard {
public:
    ScallywagGolden() : BgBaseCard(4, "NEUTRAL", 2, 1, "Scallywag (Golden)",
					"['DEATHRATTLE']", "PIRATE", "", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ScallywagGolden>(*this); } // boilerplate that every drattle needs...
};

class ScavengingHyena : public BgBaseCard {
public:
    ScavengingHyena() : BgBaseCard(2, "HUNTER", 2, 2, "Scavenging Hyena",
				   "['TRIGGER_VISUAL']", "BEAST", "COMMON", 1, "MINION") {}
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;    
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ScavengingHyena>(*this); } // boilerplate that every drattle needs...
};

class ScavengingHyenaGolden : public BgBaseCard {
public:
    ScavengingHyenaGolden() : BgBaseCard(4, "HUNTER", 2, 4, "Scavenging Hyena (Golden)",
					 "['TRIGGER_VISUAL']", "BEAST", "COMMON", 1, "MINION") {}
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;    
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ScavengingHyenaGolden>(*this); } // boilerplate that every drattle needs...
private:
    ScavengingHyena sh;
};

class ScrewjankClunker : public TargetedBattlecryCard {
public:
    ScrewjankClunker() : BgBaseCard(2, "WARRIOR", 4, 5, "Screwjank Clunker",
				    "['BATTLECRY']", "MECHANICAL", "RARE", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ScrewjankClunker>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class ScrewjankClunkerGolden : public TargetedBattlecryCard {
public:
    ScrewjankClunkerGolden() : BgBaseCard(4, "WARRIOR", 4, 10, "Screwjank Clunker (Golden)",
					  "['BATTLECRY']", "MECHANICAL", "RARE", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ScrewjankClunkerGolden>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class SeabreakerGoliath : public BgBaseCard {
public:
    SeabreakerGoliath() : BgBaseCard(6, "NEUTRAL", 7, 7, "Seabreaker Goliath",
				     "['OVERKILL']", "PIRATE", "", 5, "MINION") {}
    virtual void do_postattack(std::shared_ptr<BgBaseCard> defender,
			       int def_pos,
			       Board* b1,
			       Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SeabreakerGoliath>(*this); } // boilerplate that every drattle needs...
};

class SeabreakerGoliathGolden : public BgBaseCard {
public:
    SeabreakerGoliathGolden() : BgBaseCard(12, "NEUTRAL", 7, 14, "Seabreaker Goliath (Golden)",
					   "['OVERKILL']", "PIRATE", "", 5, "MINION") {}
    virtual void do_postattack(std::shared_ptr<BgBaseCard> defender,
			       int def_pos,
			       Board* b1,
			       Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SeabreakerGoliathGolden>(*this); } // boilerplate that every drattle needs...
private:
    SeabreakerGoliath sbg;
};


class SecurityRover : public BgBaseCard {
public:
    SecurityRover() : BgBaseCard(2, "WARRIOR", 6, 6, "Security Rover",
				 "['TRIGGER_VISUAL']", "MECHANICAL", "RARE", 4, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SecurityRover>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
    void take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) override;
};

class SecurityRoverGolden : public BgBaseCard {
public:
    SecurityRoverGolden() : BgBaseCard(4, "WARRIOR", 6, 12, "Security Rover (Golden)",
				       "['TRIGGER_VISUAL']", "MECHANICAL", "RARE", 4, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SecurityRoverGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
    void take_damage(int damage, std::string who_from_race, Board* b1, Board* b2) override;
};

class SelflessHero : public DeathrattleCard {
public:
    SelflessHero() : BgBaseCard(2, "PALADIN", 1, 1, "Selfless Hero",
				"['DEATHRATTLE']", "", "RARE", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SelflessHero>(*this); } // boilerplate that every drattle needs...
};

class SelflessHeroGolden : public DeathrattleCard {
public:
    SelflessHeroGolden() : BgBaseCard(4, "PALADIN", 1, 2, "Selfless Hero (Golden)",
				      "['DEATHRATTLE']", "", "RARE", 1, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SelflessHeroGolden>(*this); } // boilerplate that every drattle needs...
private:
    SelflessHero hero;
};

class Sellemental : public BgBaseCard {
public:
    Sellemental() : BgBaseCard(2, "NEUTRAL", -1, 2, "Sellemental",
			       "", "ELEMENTAL", "", 1, "MINION") {}
    void on_sell(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Sellemental>(*this); } // boilerplate that every drattle needs...
};

class SellementalGolden : public BgBaseCard {
public:
    SellementalGolden() : BgBaseCard(4, "NEUTRAL", -1, 4, "Sellemental (Golden)",
				     "", "ELEMENTAL", "", 1, "MINION") {}
    void on_sell(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SellementalGolden>(*this); } // boilerplate that every drattle needs...
};

class Siegebreaker : public BgBaseCard {
public:
    Siegebreaker() : BgBaseCard(5, "WARLOCK", 7, 8,  "Siegebreaker",
				"['AURA', 'TAUNT']", "DEMON", "RARE", 4, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Siegebreaker>(*this); } // boilerplate that every drattle needs...
};

class SiegebreakerGolden : public BgBaseCard {
public:
    SiegebreakerGolden() : BgBaseCard(10, "WARLOCK", 7, 16,  "Siegebreaker (Golden)",
				      "['AURA', 'TAUNT']", "DEMON", "RARE", 4, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SiegebreakerGolden>(*this); } // boilerplate that every drattle needs...
private:
    Siegebreaker sb;
};

class SkyPirate : public PirateCard {
public:
    SkyPirate() : BgBaseCard(1, "ROGUE", 1, 1, "Sky Pirate",
			     "", "PIRATE", "", 1, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SkyPirate>(*this); } // boilerplate that every drattle needs...
};

class SkyPirateGolden : public PirateCard {
public:
    SkyPirateGolden() : BgBaseCard(2, "ROGUE", 1, 2, "Sky Pirate (Golden)",
				   "", "PIRATE", "", 1, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SkyPirateGolden>(*this); } // boilerplate that every drattle needs...
};


class SneedsOldShredder : public DeathrattleCard {
public:
    SneedsOldShredder() : BgBaseCard(5, "NEUTRAL", 8, 7,"Sneed's Old Shredder",
					  "['DEATHRATTLE']", "MECHANICAL", "LEGENDARY", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SneedsOldShredder>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;    
};

class SneedsOldShredderGolden : public DeathrattleCard {
public:
    SneedsOldShredderGolden() : BgBaseCard(10, "NEUTRAL", 8, 14, "Sneed's Old Shredder (Golden)",
						"['DEATHRATTLE']", "MECHANICAL", "LEGENDARY", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SneedsOldShredderGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    SneedsOldShredder shredder;
};

class SoulJuggler : public BgBaseCard {
public:
    SoulJuggler() : BgBaseCard(3, "WARLOCK", 3, 3, "Soul Juggler",
			       "['TRIGGER_VISUAL']", "", "", 3, "MINION") {}
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;    
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SoulJuggler>(*this); } // boilerplate that every drattle needs...
};

class SoulJugglerGolden : public BgBaseCard {
public:
    SoulJugglerGolden() : BgBaseCard(6, "WARLOCK", 3, 6, "Soul Juggler (Golden)",
				     "['TRIGGER_VISUAL']", "", "", 3, "MINION") {}
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;    
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SoulJugglerGolden>(*this); } // boilerplate that every drattle needs...
private:
    SoulJuggler soul_juggler;
};

class SouthseaCaptain : public PirateCard {
public:
    SouthseaCaptain() : BgBaseCard(3, "NEUTRAL", 3, 3, "Southsea Captain",
				   "['AURA']", "PIRATE", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SouthseaCaptain>(*this); } // boilerplate that every drattle needs...
};

class SouthseaCaptainGolden : public PirateCard {
public:
    SouthseaCaptainGolden() : BgBaseCard(6, "NEUTRAL", 3, 6, "Southsea Captain (Golden)",
					 "['AURA']", "PIRATE", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SouthseaCaptainGolden>(*this); } // boilerplate that every drattle needs...
private:
    SouthseaCaptain rw;
};

class SouthseaStrongarm : public TargetedBattlecryCard {
public:
    SouthseaStrongarm() : BgBaseCard(4, "NEUTRAL", 5, 3,  "Southsea Strongarm",
				     "", "PIRATE", "", 3,"MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SouthseaStrongarm>(*this); } // boilerplate that every drattle needs...
    void targeted_battlecry(std::shared_ptr<BgBaseCard> c, Player* p1) override;
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
private:
    int pirates_bought_this_turn;
};

class SouthseaStrongarmGolden : public TargetedBattlecryCard {
public:
    SouthseaStrongarmGolden() : BgBaseCard(8, "NEUTRAL", 5, 6, "Southsea Strongarm (Golden)",
					   "", "PIRATE", "", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SouthseaStrongarmGolden>(*this); } // boilerplate that every drattle needs...
    void targeted_battlecry(std::shared_ptr<BgBaseCard> c, Player* p1) override;
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
private:
    int pirates_bought_this_turn;
};

class SpawnOfNzoth : public DeathrattleCard {
public:
    SpawnOfNzoth() : BgBaseCard(2, "NEUTRAL", 3, 2, "Spawn of N'Zoth", "['DEATHRATTLE']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SpawnOfNzoth>(*this); } // boilerplate that every drattle needs...
};

class SpawnOfNzothGolden : public DeathrattleCard {
public:
    SpawnOfNzothGolden() : BgBaseCard(4, "NEUTRAL", 4, 2, "Spawn of N'Zoth (Golden)", "['DEATHRATTLE']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<SpawnOfNzothGolden>(*this); } // boilerplate that every drattle needs...
};

class StasisElemental : public BattlecryCard {
public:
    StasisElemental() : BgBaseCard(4, "NEUTRAL", -1, 4, "Stasis Elemental",
				   "", "ELEMENTAL", "", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<StasisElemental>(*this); } // boilerplate that every drattle needs...
};

class StasisElementalGolden : public BattlecryCard {
public:
    StasisElementalGolden() : BgBaseCard(8, "NEUTRAL", -1, 8, "Stasis Elemental (Golden)",
					 "", "ELEMENTAL", "", 3, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<StasisElementalGolden>(*this); } // boilerplate that every drattle needs...
private:
    StasisElemental se;
};

class StewardOfTime : public PirateCard {
public:
    StewardOfTime() : BgBaseCard(3, "NEUTRAL", 4, 4, "Steward of Time",
				 "", "DRAGON", "", 2, "MINION") {}
    void on_sell(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<StewardOfTime>(*this); } // boilerplate that every drattle needs...
};

class StewardOfTimeGolden : public PirateCard {
public:
    StewardOfTimeGolden() : BgBaseCard(6, "NEUTRAL", 4, 8, "Steward of Time (Golden)",
				       "", "DRAGON", "", 2, "MINION") {}
    void on_sell(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<StewardOfTimeGolden>(*this); } // boilerplate that every drattle needs...
};

class StrongshellScavenger : public BattlecryCard {
public:
    StrongshellScavenger() : BgBaseCard(2, "DRUID", 4, 3, "Strongshell Scavenger",
					"['BATTLECRY']", "", "RARE", 5, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<StrongshellScavenger>(*this); } // boilerplate that every drattle needs...
};

class StrongshellScavengerGolden : public BattlecryCard {
public:
    StrongshellScavengerGolden() : BgBaseCard(4, "DRUID", 4, 6, "Strongshell Scavenger (Golden)",
					      "['BATTLECRY']", "", "RARE", 5, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<StrongshellScavengerGolden>(*this); } // boilerplate that every drattle needs...
};

class TavernTempest : public BattlecryCard {
public:
    TavernTempest() : BgBaseCard(4, "NEUTRAL", 5, 4, "Tavern Tempest",
				 "['BATTLECRY']", "ELEMENTAL", "", 5, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<TavernTempest>(*this); } // boilerplate that every drattle needs...
};

class TavernTempestGolden : public BattlecryCard {
public:
    TavernTempestGolden() : BgBaseCard(8, "NEUTRAL", 5, 8, "Tavern Tempest (Golden)",
				       "['BATTLECRY']", "ELEMENTAL", "", 5, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<TavernTempestGolden>(*this); } // boilerplate that every drattle needs...
private:
    TavernTempest tt;
};

class TheTideRazor : public DeathrattleCard {
public:
    TheTideRazor() : BgBaseCard(6, "NEUTRAL", 7, 4, "The Tide Razor",
				     "['DEATHRATTLE']", "", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<TheTideRazor>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class TheTideRazorGolden : public DeathrattleCard {
public:
    TheTideRazorGolden() : BgBaseCard(12, "NEUTRAL", 7, 8, "The Tide Razor",
					   "['DEATHRATTLE']", "", "", 6, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<TheTideRazorGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
private:
    TheTideRazor ttr;
};

class TheBeast : public DeathrattleCard {
public:
    TheBeast() : BgBaseCard(9, "NEUTRAL", 6, 7, "The Beast",
				 "['DEATHRATTLE']", "BEAST", "LEGENDARY", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<TheBeast>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class TheBeastGolden : public DeathrattleCard {
public:
    TheBeastGolden() : BgBaseCard(18, "NEUTRAL", 6, 14, "The Beast (Golden)",
				       "['DEATHRATTLE']", "BEAST", "LEGENDARY", 3, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<TheBeastGolden>(*this); } // boilerplate that every drattle needs...
private:
    TheBeast the_beast;
};

class Toxfin : public TargetedBattlecryCard {
public:
    Toxfin() : BgBaseCard(1, "NEUTRAL", 1, 2, "Toxfin",
			  "['BATTLECRY']", "MURLOC", "", 4, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Toxfin>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class ToxfinGolden : public TargetedBattlecryCard {
public:
    ToxfinGolden() : BgBaseCard(2, "NEUTRAL", 1, 4, "Toxfin (Golden)",
				"['BATTLECRY']", "MURLOC", "", 4, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<ToxfinGolden>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
private:
    Toxfin tf;
};


class TwilightEmissary : public TargetedBattlecryCard {
public:
    TwilightEmissary() : BgBaseCard(4, "NEUTRAL", 6, 4, "Twilight Emissary",
				    "['BATTLECRY', 'TAUNT']", "DRAGON", "", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<TwilightEmissary>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class TwilightEmissaryGolden : public TargetedBattlecryCard {
public:
    TwilightEmissaryGolden() : BgBaseCard(8, "NEUTRAL", 6, 8, "Twilight Emissary (Golden)",
					  "['BATTLECRY', 'TAUNT']", "DRAGON", "", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<TwilightEmissaryGolden>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class UnstableGhoul : public DeathrattleCard {
public:
    UnstableGhoul() : BgBaseCard(1, "NEUTRAL", 2, 3, "Unstable Ghoul",	"['DEATHRATTLE', 'TAUNT']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<UnstableGhoul>(*this); } // boilerplate that every drattle needs...
};

class UnstableGhoulGolden : public DeathrattleCard {
public:
    UnstableGhoulGolden() : BgBaseCard(2, "NEUTRAL", 2, 6, "Unstable Ghoul (Golden)",	"['DEATHRATTLE', 'TAUNT']", "", "COMMON", 2, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<UnstableGhoulGolden>(*this); } // boilerplate that every drattle needs...
private:
    UnstableGhoul ghoul;
};

class VirmenSensei : public TargetedBattlecryCard {
public:
    VirmenSensei() : BgBaseCard(4, "DRUID", 5, 5, "Virmen Sensei",
				"['BATTLECRY']", "", "RARE", 4, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<VirmenSensei>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class VirmenSenseiGolden : public TargetedBattlecryCard {
public:
    VirmenSenseiGolden() : BgBaseCard(8, "DRUID", 5, 10, "Virmen Sensei (Golden)",
				      "['BATTLECRY']", "", "RARE", 4, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<VirmenSenseiGolden>(*this); } // boilerplate that every drattle needs...
    void do_targeted_battlecry(std::shared_ptr<BgBaseCard>) override;
};

class Voidlord : public DeathrattleCard {
public:
    Voidlord() : BgBaseCard(3, "WARLOCK", 9, 9, "Voidlord",
				 "['DEATHRATTLE', 'TAUNT']", "DEMON", "EPIC", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<Voidlord>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class VoidlordGolden : public DeathrattleCard {
public:
    VoidlordGolden() : BgBaseCard(6, "WARLOCK", 9, 18, "VoidlordGolden (Golden)",
				 "['DEATHRATTLE', 'TAUNT']", "DEMON", "EPIC", 5, "MINION") {}
    virtual void do_deathrattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<VoidlordGolden>(*this); } // boilerplate that every drattle needs...
    std::shared_ptr<BgBaseCard> summon() override;
};

class VulgarHomunculus : public BattlecryCard {
public:
    VulgarHomunculus() : BgBaseCard(2, "WARLOCK", 2, 4, "Vulgar Homunculus",
				    "['TAUNT']", "DEMON", "FREE", 1, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<VulgarHomunculus>(*this); } // boilerplate that every drattle needs...
};

class VulgarHomunculusGolden : public BattlecryCard {
public:
    VulgarHomunculusGolden() : BgBaseCard(4, "WARLOCK", 2, 8, "Vulgar Homunculus (Golden)",
					  "['TAUNT']", "DEMON", "FREE", 1, "MINION") {}
    virtual void do_battlecry(Player*) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<VulgarHomunculusGolden>(*this); } // boilerplate that every drattle needs...
};

class WaxriderTogwaggle : public BgBaseCard {
public:
    WaxriderTogwaggle() : BgBaseCard(1, "NEUTRAL", 3, 2, "Waxrider Togwaggle",
				     "['TRIGGER_VISUAL']", "", "", 2, "MINION") {}
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<WaxriderTogwaggle>(*this); } // boilerplate that every drattle needs...
};

class WaxriderTogwaggleGolden : public BgBaseCard {
public:
    WaxriderTogwaggleGolden() : BgBaseCard(2, "NEUTRAL", 3, 4, "Waxrider Togwaggle (Golden)",
					   "['TRIGGER_VISUAL']", "", "", 2, "MINION") {}
    virtual void do_postbattle(Board* b1,
			       Board* b2,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
			       std::vector<std::shared_ptr<BgBaseCard> > dead_b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<WaxriderTogwaggleGolden>(*this); } // boilerplate that every drattle needs...
private:
    WaxriderTogwaggle sh;
};

class WildfireElemental : public BgBaseCard {
public:
    WildfireElemental() : BgBaseCard(7, "NEUTRAL", 6, 3, "Wildfire Elemental",
				 "", "ELEMENTAL", "", 4, "MINION") {}
    virtual void do_postattack(std::shared_ptr<BgBaseCard> defender,
			       int def_pos,
			       Board* b1,
			       Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<WildfireElemental>(*this); } // boilerplate that every drattle needs...
};

class WildfireElementalGolden : public BgBaseCard {
public:
    WildfireElementalGolden() : BgBaseCard(14, "NEUTRAL", 6, 6, "Wildfire Elemental (Golden)",
					   "", "ELEMENTAL", "", 4, "MINION") {}
    virtual void do_postattack(std::shared_ptr<BgBaseCard> defender,
			       int def_pos,
			       Board* b1,
			       Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<WildfireElementalGolden>(*this); } // boilerplate that every drattle needs...
};

class WrathWeaver : public BgBaseCard {
public:
    WrathWeaver() : BgBaseCard(1, "NEUTRAL", 1, 3, "Wrath Weaver",
			       "['TRIGGER_VISUAL']", "", "COMMON", 1, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<WrathWeaver>(*this); } // boilerplate that every drattle needs...
};

class WrathWeaverGolden : public BgBaseCard {
public:
    WrathWeaverGolden() : BgBaseCard(2, "NEUTRAL", 1, 6, "Wrath Weaver (Golden)",
				     "['TRIGGER_VISUAL']", "", "COMMON", 1, "MINION") {}
    int mod_summoned(std::shared_ptr<BgBaseCard> card, Board*, bool from_hand) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<WrathWeaverGolden>(*this); } // boilerplate that every drattle needs...
};

class YoHoOgre : public PirateCard {
public:
    YoHoOgre() : BgBaseCard(2, "NEUTRAL", 6, 8, "Yo-Ho-Ogre",
			    "['TRIGGER_VISUAL', 'TAUNT']", "PIRATE", "", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<YoHoOgre>(*this); } // boilerplate that every drattle needs...
    void do_postdefense(std::shared_ptr<BgBaseCard> attacker, Board* b1, Board* b2) override;
};

class YoHoOgreGolden : public PirateCard {
public:
    YoHoOgreGolden() : BgBaseCard(4, "NEUTRAL", 6, 16, "Yo-Ho-Ogre (Golden)",
				  "['TRIGGER_VISUAL', 'TAUNT']", "PIRATE", "", 3, "MINION") {}
    virtual std::shared_ptr<BgBaseCard> get_copy() const override { return std::make_shared<YoHoOgreGolden>(*this); } // boilerplate that every drattle needs...
    void do_postdefense(std::shared_ptr<BgBaseCard> attacker, Board* b1, Board* b2) override;
};

