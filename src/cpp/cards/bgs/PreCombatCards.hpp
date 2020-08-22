#include "BgBaseCard.hpp"

#include <queue>

class PreCombatCard : public BgBaseCard {
public:
    using BgBaseCard::BgBaseCard;
    virtual void do_precombat(Board* b1, Board* b2) override = 0;
    virtual std::shared_ptr<BgBaseCard> get_copy() override = 0; // boilerplate that every drattle needs...
};

class RedWhelp : public PreCombatCard {
public:
    RedWhelp() : PreCombatCard(1, "NEUTRAL", 1, 2, "Red Whelp",
			       "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RedWhelp>(*this); } // boilerplate that every drattle needs...
};

class RedWhelpGolden : public PreCombatCard {
public:
    RedWhelpGolden() : PreCombatCard(1, "NEUTRAL", 1, 2, "Red Whelp",
			       "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RedWhelpGolden>(*this); } // boilerplate that every drattle needs...
private:
    RedWhelp rw;
};

class MurlocWarleader : public PreCombatCard {
public:
    MurlocWarleader() : PreCombatCard(3, "NEUTRAL", 3, 3, "Murloc Warleader",
				      "['AURA']", "MURLOC", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<MurlocWarleader>(*this); } // boilerplate that every drattle needs...
};

class MurlocWarleaderGolden : public PreCombatCard {
public:
    MurlocWarleaderGolden() : PreCombatCard(6, "NEUTRAL", 3, 6, "Murloc Warleader (Golden)",
					    "['AURA']", "MURLOC", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<MurlocWarleaderGolden>(*this); } // boilerplate that every drattle needs...
private:
    MurlocWarleader rw;
};

class OldMurkeye : public PreCombatCard {
public:
    OldMurkeye() : PreCombatCard(2, "NEUTRAL", 4, 4, "Old Murk-Eye",
				 "['CHARGE']", "MURLOC", "LEGENDARY", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_postbattle(Board* b1,
			       std::vector<std::shared_ptr<BgBaseCard> > new_dead) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<OldMurkeye>(*this); } // boilerplate that every drattle needs...
};

class OldMurkeyeGolden : public PreCombatCard {
public:
    OldMurkeyeGolden() : PreCombatCard(4, "NEUTRAL", 4, 8, "Old Murk-Eye (Golden)",
				       "['CHARGE']", "MURLOC", "LEGENDARY", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_postbattle(Board* b1,
			       std::vector<std::shared_ptr<BgBaseCard> > new_dead) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<OldMurkeyeGolden>(*this); } // boilerplate that every drattle needs...
private:
    OldMurkeye rw;
};

class SouthseaCaptain : public PreCombatCard {
public:
    SouthseaCaptain() : PreCombatCard(3, "NEUTRAL", 3, 3, "Southsea Captain",
				      "['AURA']", "PIRATE", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SouthseaCaptain>(*this); } // boilerplate that every drattle needs...
};

class SouthseaCaptainGolden : public PreCombatCard {
public:
    SouthseaCaptainGolden() : PreCombatCard(6, "NEUTRAL", 3, 6, "Southsea Captain (Golden)",
					    "['AURA']", "PIRATE", "EPIC", 2, "MINION") {}
    virtual void do_precombat(Board* b1, Board* b2) override;
    virtual void do_deathrattle(Board* b1, Board* b2) override; // Not really a drattle
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<SouthseaCaptainGolden>(*this); } // boilerplate that every drattle needs...
private:
    SouthseaCaptain rw;
};
