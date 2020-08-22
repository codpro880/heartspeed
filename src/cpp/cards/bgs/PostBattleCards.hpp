// #include "BgBaseCard.hpp"

// #include <queue>

// class PostBattleCard : public BgBaseCard {
// public:
//     using BgBaseCard::BgBaseCard;
//     virtual void do_postbattle(Board* b1, std::vector<std::shared_ptr<BgBaseCard> > new_dead) override = 0;
//     virtual std::shared_ptr<BgBaseCard> get_copy() override = 0; // boilerplate that every drattle needs...
// };

// class ScavagingHyena : public PostBattleCard {
// public:
//     ScavagingHyena() : PostBattleCard(2, "HUNTER", 2, 2, "Scavenging Hyena",
// 				      "['TRIGGER_VISUAL']", "BEAST", "COMMON", 1, "MINION") {}
//     virtual void do_postbattle(Board* b1, std::vector<std::shared_ptr<BgBaseCard> > new_dead) override;
//     virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ScavagingHyena>(*this); } // boilerplate that every drattle needs...
// };

// class ScavagingHyenaGolden : public PostBattleCard {
// public:
//     ScavagingHyenaGolden() : PostBattleCard(4, "HUNTER", 2, 4, "Scavenging Hyena (Golden)",
// 					    "['TRIGGER_VISUAL']", "BEAST", "COMMON", 1, "MINION") {}
//     virtual void do_postbattle(Board* b1, std::vector<std::shared_ptr<BgBaseCard> > new_dead) override;
//     virtual std::shared_ptr<BgBaseCard> get_copy() override { return std::make_shared<ScavagingHyenaGolden>(*this); } // boilerplate that every drattle needs...
// private:
//     ScavagingHyena sh;
// };

