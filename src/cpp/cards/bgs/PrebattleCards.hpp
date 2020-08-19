#include "BgBaseCard.hpp"

#include <queue>

class PrebattleCard : public BgBaseCard {
public:
    using BgBaseCard::BgBaseCard;
    virtual void do_prebattle(Board* b1, Board* b2) override = 0;
    virtual std::shared_ptr<BgBaseCard> get_copy() override = 0; // boilerplate that every drattle needs...
};

class RedWhelp : public PrebattleCard {
public:
    RedWhelp() : PrebattleCard(1, "NEUTRAL", 1, 2, "Red Whelp",
			       "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION") {}
    virtual void do_prebattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RedWhelp>(*this); } // boilerplate that every drattle needs...
};

class RedWhelpGolden : public PrebattleCard {
public:
    RedWhelpGolden() : PrebattleCard(1, "NEUTRAL", 1, 2, "Red Whelp",
			       "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION") {}
    virtual void do_prebattle(Board* b1, Board* b2) override;
    virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<RedWhelpGolden>(*this); } // boilerplate that every drattle needs...
private:
    RedWhelp rw;
};
