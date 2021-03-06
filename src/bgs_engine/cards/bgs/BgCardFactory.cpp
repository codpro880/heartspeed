#include <memory>
#include <string>
#include <unordered_map>

#include "BgBaseCard.hpp"
#include "BgCardFactory.hpp"
#include "BgCards.hpp"

std::shared_ptr<BgBaseCard> BgCardFactory::get_card(std::string name) const {
    auto card_found_it = cards.find(name);
    if (card_found_it == cards.end())  {
        throw std::runtime_error("No card has name '" + name + "'");
    }
    else {
        return (card_found_it->second)->get_copy();
    }
}

std::vector<std::shared_ptr<BgBaseCard> > BgCardFactory::get_cards_of_cost(int cost) const {
    std::vector<std::shared_ptr<BgBaseCard> > res;
    auto it = cards.begin();
    while (it != cards.end()) {
        auto card = it->second;
        if (card->get_cost() == cost) {
            // TODO: Not copying here may improve performance.
            // Maybe only return names?
            res.push_back(card->get_copy());
        }
        it++;
    }
    return res;
}

std::vector<std::shared_ptr<BgBaseCard> > BgCardFactory::get_cards_of_rarity(std::string rarity) const {
    std::vector<std::shared_ptr<BgBaseCard> > res;
    auto it = cards.begin();
    while (it != cards.end()) {
        auto card = it->second;
        if (card->get_rarity() == rarity) {
            // TODO: Not copying here may improve performance.
            // Maybe only return names?
            res.push_back(card->get_copy());
        }
        it++;
    }
    return res;
}

std::vector<std::shared_ptr<BgBaseCard> > BgCardFactory::get_cards_with_deathrattle(bool include_golden) const {
    std::vector<std::shared_ptr<BgBaseCard> > res;
    auto it = cards.begin();
    while (it != cards.end()) {
        auto card = it->second;
        if (card->has_deathrattle()) {
            // TODO: Not copying here may improve performance.
            // Maybe only return names?
            if (include_golden) {
                res.push_back(card->get_copy());
            }
            else {
                if (!card->is_golden()) {
                    res.push_back(card->get_copy());
                }
            }
        }
        it++;
    }
    return res;
}

std::vector<std::shared_ptr<BgBaseCard> > BgCardFactory::get_cards_of_race(std::string race, bool include_golden) const {
    std::vector<std::shared_ptr<BgBaseCard> > res;
    auto it = cards.begin();
    while (it != cards.end()) {
        auto card = it->second;
        if (card->get_race() == race) {
            // TODO: Not copying here may improve performance.
            // Maybe only return names?
            if (include_golden) {
                res.push_back(card->get_copy());
            }
            else {
                if (!card->is_golden()) {
                    res.push_back(card->get_copy());
                }
            }
        }
        it++;
    }
    return res;
}

std::unordered_map<int, std::vector<std::string>> BgCardFactory::get_card_names_by_tier() const {
    std::unordered_map<int, std::vector<std::string>> res;
    auto it = cards.begin();
    while (it != cards.end()) {
        auto card = it->second;
        auto tier = card->get_tavern_tier();
        if (!card->is_golden() && card->is_available_in_tavern()) {
            res[tier].push_back(card->get_name());
        }
        it++;
    }
    return res;
}

std::vector<std::string> BgCardFactory::get_card_names() const {
    std::vector<std::string> card_names;
    auto it = cards.begin();
    while (it != cards.end()) {
        auto card = it->first;
        card_names.push_back(card);
        it++;
    }
    return card_names;
}

void BgCardFactory::init_cards() {
    // Vanilla cards are defined inline
    
    // A
    std::shared_ptr<BgBaseCard> aco_of_cthun(new BgBaseCard(2, "NEUTRAL", 1, 2, "Acolyte of C'Thun",
                                                            "['REBORN', 'TAUNT']", "", "COMMON", 1, "MINION"));
    aco_of_cthun->set_reborn();
    cards.emplace("Acolyte of C'Thun", aco_of_cthun);
    std::shared_ptr<BgBaseCard> aco_of_cthun_golden(new BgBaseCard(4, "NEUTRAL", 1, 4, "Acolyte of C'Thun (Golden)",
                                                                   "['REBORN', 'TAUNT']", "", "COMMON", 1, "MINION"));
    aco_of_cthun_golden->set_reborn();
    cards.emplace("Acolyte of C'Thun (Golden)", aco_of_cthun_golden);
    cards.emplace("Alleycat", std::make_shared<Alleycat>());
    cards.emplace("Alleycat (Golden)", std::make_shared<AlleycatGolden>());
    cards.emplace("Amalgadon", std::make_shared<Amalgadon>());
    cards.emplace("Amalgadon (Golden)", std::make_shared<AmalgadonGolden>());                                  
    cards.emplace("Annihilan Battlemaster", std::make_shared<AnnihilanBattlemaster>());
    cards.emplace("Annihilan Battlemaster (Golden)", std::make_shared<AnnihilanBattlemasterGolden>());
    std::shared_ptr<BgBaseCard> annoyo(new BgBaseCard(2, "PALADIN", 4, 4, "Annoy-o-Module",
                                                      "['DIVINE_SHIELD', 'MODULAR', 'TAUNT']", "MECHANICAL", "RARE", 4, "MINION"));
    annoyo->set_divine_shield();
    cards.emplace("Annoy-o-Module", annoyo);
    std::shared_ptr<BgBaseCard> annoyo_gold(new BgBaseCard(4, "PALADIN", 4, 8, "Annoy-o-Module (Golden)",
                                                           "['DIVINE_SHIELD', 'MODULAR', 'TAUNT']", "MECHANICAL", "RARE", 4, "MINION"));
    annoyo_gold->set_divine_shield();
    cards.emplace("Annoy-o-Module (Golden)", annoyo_gold);
    cards.emplace("Arcane Assistant", std::make_shared<ArcaneAssistant>());
    cards.emplace("Arcane Assistant (Golden)", std::make_shared<ArcaneAssistantGolden>());
    // Mechanic handled in BgBaseCard
    cards.emplace("Arm of the Empire", std::make_shared<BgBaseCard>(4, "NEUTRAL", 5, 5, "Arm of the Empire",
                                                                    "['TRIGGER_VISUAL']", "", "COMMON", 4, "MINION"));
    cards.emplace("Arm of the Empire (Golden)", std::make_shared<BgBaseCard>(8, "NEUTRAL", 5, 10, "Arm of the Empire (Golden)",
                                                                             "['TRIGGER_VISUAL']", "", "COMMON", 4, "MINION"));
    // TODO: Secrets
    // cards.emplace("Arcane Cannon", BgBaseCard(2, "NEUTRAL", 3, 2, "Arcane Cannon",
    //                                        "['CANT_ATTACK', 'TRIGGER_VISUAL']", "", "", 2, "MINION"));
    // cards.emplace("Arcane Cannon (Golden)", BgBaseCard(4, "NEUTRAL", 3, 4, "Arcane Cannon (Golden)",
    //                                                 "['CANT_ATTACK', 'TRIGGER_VISUAL']", "", "", 2, "MINION"));
    // cards.emplace("Avenge", BgBaseCard(-1, "PALADIN", 1, -1, "Avenge",
    //                                 "['SECRET']", "", "COMMON", -1, "SPELL"));
    // cards.emplace("Autodefense Matrix", BgBaseCard(-1, "PALADIN", 1, -1, "Autodefense Matrix",
    //                                             "['SECRET']", "", "COMMON", -1, "SPELL"));

    // B
    std::shared_ptr<BgBaseCard> baron(new BgBaseCard(1, "NEUTRAL", 4, 7, "Baron Rivendare",
                                                     "['AURA']", "", "LEGENDARY", 5, "MINION"));
    cards.emplace("Baron Rivendare", baron);
    std::shared_ptr<BgBaseCard> baron_gold(new BgBaseCard(2, "NEUTRAL", 4, 14, "Baron Rivendare (Golden)",
                                                          "['AURA']", "", "LEGENDARY", 5, "MINION"));
    cards.emplace("Baron Rivendare (Golden)", baron_gold);
    std::shared_ptr<BgBaseCard> big_bad_wolf(new BgBaseCard(3, "HUNTER", 2, 2, "Big Bad Wolf",
                                                            "", "BEAST", "", 1, "MINION"));
    big_bad_wolf->set_available_in_tavern(false);
    cards.emplace("Big Bad Wolf", big_bad_wolf);
    std::shared_ptr<BgBaseCard> big_bad_wolf_gold(new BgBaseCard(6, "HUNTER", 2, 4, "Big Bad Wolf (Golden)",
                                                                 "", "BEAST", "", 1, "MINION"));
    cards.emplace("Big Bad Wolf (Golden)", big_bad_wolf_gold);
    cards.emplace("Bigfernal", std::make_shared<Bigfernal>());
    cards.emplace("Bigfernal (Golden)", std::make_shared<BigfernalGolden>());
    cards.emplace("Bloodsail Cannoneer", std::make_shared<BloodsailCannoneer>());
    cards.emplace("Bloodsail Cannoneer (Golden)", std::make_shared<BloodsailCannoneerGolden>());
    std::shared_ptr<BgBaseCard> bolvar(new BgBaseCard(1, "PALADIN", 5, 7, "Bolvar",
                                                      "['DIVINE_SHIELD', 'TRIGGER_VISUAL']", "", "LEGENDARY", 4, "MINION"));
    cards.emplace("Bolvar", bolvar);
    std::shared_ptr<BgBaseCard> bolvar_golden(new BgBaseCard(2, "PALADIN", 5, 14, "Bolvar (Golden)",
                                                             "['DIVINE_SHIELD', 'TRIGGER_VISUAL']", "", "LEGENDARY", 4, "MINION"));
    cards.emplace("Bolvar (Golden)", bolvar_golden);
    std::shared_ptr<BgBaseCard> bran(new BgBaseCard(2, "NEUTRAL", 3, 4, "Brann Bronzebeard",
                                                    "['AURA']", "", "LEGENDARY", 5, "MINION"));
    cards.emplace("Brann Bronzebeard", bran);
    std::shared_ptr<BgBaseCard> bran_gold(new BgBaseCard(4, "NEUTRAL", 3, 8, "Brann Bronzebeard (Golden)",
                                                         "['AURA']", "", "LEGENDARY", 5, "MINION"));
    cards.emplace("Brann Bronzebeard (Golden)", bran_gold);
    std::shared_ptr<BgBaseCard> bronze_warden(new BgBaseCard(2, "NEUTRAL", 4, 1, "Bronze Warden",
                                                             "['DIVINE_SHIELD', 'REBORN']", "DRAGON", "", 3, "MINION"));
    bronze_warden->set_divine_shield();
    bronze_warden->set_reborn();
    cards.emplace("Bronze Warden", bronze_warden);
    std::shared_ptr<BgBaseCard> bronze_warden_golden(new BgBaseCard(4, "NEUTRAL", 4, 2, "Bronze Warden (Golden)",
                                                                    "['DIVINE_SHIELD', 'REBORN']", "DRAGON", "", 3, "MINION"));
    bronze_warden_golden->set_divine_shield();
    bronze_warden_golden->set_reborn();
    cards.emplace("Bronze Warden (Golden)", bronze_warden_golden);

    // C
    std::shared_ptr<BgBaseCard> cave_hydra(new BgBaseCard(2, "BEAST", 3, 4, "Cave Hydra",
                                                          "['CLEAVE']", "BEAST", "", 4, "MINION"));
    cards.emplace("Cave Hydra", cave_hydra);
    std::shared_ptr<BgBaseCard> cave_hydra_golden(new BgBaseCard(4, "BEAST", 3, 8, "Cave Hydra (Golden)",
                                                                    "['CLEAVE']", "BEAST", "", 4, "MINION"));
    cards.emplace("Cave Hydra (Golden)", cave_hydra_golden);
    cards.emplace("Cap'n Hoggarr", std::make_shared<CapnHoggarr>());
    cards.emplace("Cap'n Hoggarr (Golden)", std::make_shared<CapnHoggarrGolden>());
    // Handled in BgBaseCard...TODO: Get better "trigger all" sort of effect going...
    cards.emplace("Champion of Y'Shaarj", std::make_shared<ChampionOfYshaarj>());
    cards.emplace("Champion of Y'Shaarj (Golden)", std::make_shared<ChampionOfYshaarjGolden>());
    cards.emplace("Cobalt Scalebane", std::make_shared<CobaltScalebane>());
    cards.emplace("Cobalt Scalebane (Golden)", std::make_shared<CobaltScalebaneGolden>());
    cards.emplace("Coldlight Seer", std::make_shared<ColdlightSeer>());
    cards.emplace("Coldlight Seer (Golden)", std::make_shared<ColdlightSeerGolden>());
    std::shared_ptr<BgBaseCard> crackling_cyclone(new BgBaseCard(4, "NEUTRAL", -1, 1, "Crackling Cyclone",
                                                                 "", "ELEMENTAL", "", 3, "MINION"));
    crackling_cyclone->set_divine_shield();
    crackling_cyclone->set_windfury();    
    cards.emplace("Crackling Cyclone", crackling_cyclone);
    std::shared_ptr<BgBaseCard> crackling_cyclone_golden(new BgBaseCard(6, "NEUTRAL", -1, 4, "Crackling Cyclone (Golden)",
                                                                        "", "ELEMENTAL", "", 3, "MINION"));
    crackling_cyclone_golden->set_divine_shield();
    crackling_cyclone_golden->set_windfury();
    cards.emplace("Crackling Cyclone", crackling_cyclone_golden);
    cards.emplace("Crystalweaver", std::make_shared<Crystalweaver>());
    cards.emplace("Crystalweaver (Golden)", std::make_shared<CrystalweaverGolden>());

    // D
    std::shared_ptr<BgBaseCard> damaged_golem(new BgBaseCard(2, "NEUTRAL", 1, 1,
                                                             "Damaged Golem", "", "MECHANICAL", "", 1, "MINION"));
    damaged_golem->set_available_in_tavern(false);
    cards.emplace("Damaged Golem", damaged_golem);    
    std::shared_ptr<BgBaseCard> damaged_golem_gold(new BgBaseCard(4, "NEUTRAL", 1, 2,
                                                                  "Damaged Golem (Golden)", "", "MECHANICAL", "", 1, "MINION"));
    cards.emplace("Damaged Golem (Golden)", damaged_golem_gold);
    std::shared_ptr<BgBaseCard> spore(new BgBaseCard(1, "NEUTRAL", 1, 1, "Deadly Spore",
                                                     "", "", "", 4, "MINION"));
    spore->set_poison();
    cards.emplace("Deadly Spore", spore);
    std::shared_ptr<BgBaseCard> spore_gold(new BgBaseCard(2, "NEUTRAL", 1, 2, "Deadly Spore (Golden)",
                                                          "", "", "", 4, "MINION"));
    spore_gold->set_poison();
    cards.emplace("Deadly Spore (Golden)", spore_gold);
    std::shared_ptr<BgBaseCard> deck_swab(new BgBaseCard(2, "NEUTRAL", 3, 2, "Deck Swabbie",
                                                         "['BATTLECRY']", "PIRATE", "", 1, "MINION"));
    cards.emplace("Deck Swabbie", deck_swab);
    std::shared_ptr<BgBaseCard> deck_swab_gold(new BgBaseCard(4, "NEUTRAL", 3, 4, "Deck Swabbie (Golden)",
                                                              "['BATTLECRY']", "PIRATE", "", 1, "MINION"));
    cards.emplace("Deck Swabbie (Golden)", deck_swab_gold);
    cards.emplace("Defender of Argus", std::make_shared<DefenderOfArgus>());
    cards.emplace("Defender of Argus (Golden)", std::make_shared<DefenderOfArgusGolden>());
    std::shared_ptr<BgBaseCard> deflecto(new BgBaseCard(3, "NEUTRAL", 4, 2, "Deflect-o-Bot (Golden)",
                                                        "['TRIGGER_VISUAL']", "MECHANICAL", "", 3, "MINION"));
    deflecto->set_divine_shield();
    cards.emplace("Deflect-o-Bot", deflecto);
    std::shared_ptr<BgBaseCard> deflecto_golden(new BgBaseCard(6, "NEUTRAL", 4, 4, "Deflect-o-Bot (Golden)",
                                                               "['TRIGGER_VISUAL']", "MECHANICAL", "", 3, "MINION"));
    deflecto->set_divine_shield();
    cards.emplace("Deflect-o-Bot (Golden)", deflecto_golden);
    std::shared_ptr<BgBaseCard> drakonid_encorcer(new BgBaseCard(3, "NEUTRAL", 6, 6, "Drakonid Enforcer",
                                                                 "['TRIGGER_VISUAL']", "DRAGON", "", 4, "MINION"));
    cards.emplace("Djinni", std::make_shared<Djinni>());
    cards.emplace("Djinni (Golden)", std::make_shared<DjinniGolden>());
    cards.emplace("Drakonid Enforcer", drakonid_encorcer);
    std::shared_ptr<BgBaseCard> drakonid_enforcer_golden(new BgBaseCard(6, "NEUTRAL", 6, 12, "Drakonid Enforcer (Golden)",
                                                                        "['TRIGGER_VISUAL']", "DRAGON", "", 4, "MINION"));
    cards.emplace("Drakonid Enforcer (Golden)", drakonid_enforcer_golden);
    std::shared_ptr<BgBaseCard> dragonspawn_lt(new BgBaseCard(2, "NEUTRAL", 2, 3, "Dragonspawn Lieutenant",
                                                              "['TAUNT']", "DRAGON", "", 1, "MINION"));
    cards.emplace("Dragonspawn Lieutenant", dragonspawn_lt);
    std::shared_ptr<BgBaseCard> dragonspawn_lt_gold(new BgBaseCard(4, "NEUTRAL", 2, 6, "Dragonspawn Lieutenant (Golden)",
                                                                   "['TAUNT']", "DRAGON", "", 1, "MINION"));
    cards.emplace("Dragonspawn Lieutenant (Golden)", dragonspawn_lt_gold);
    cards.emplace("Dread Admiral Eliza", std::make_shared<DreadAdmiralEliza>());
    cards.emplace("Dread Admiral Eliza (Golden)", std::make_shared<DreadAdmiralElizaGolden>());

    // E
    // std::shared_ptr<BgBaseCard> effigy(new BgBaseCard(-1, "MAGE", 3, -1, "Effigy",
    //                                                 "['SECRET']", "", "RARE", -1, "SPELL"));
    // cards.emplace("Effigy", effigy);
    // TODO: Secrets
    // cards.emplace("Effigy", BgBaseCard(-1, "MAGE", 3, -1, "Effigy",
    //                                 "['SECRET']", "", "RARE", -1, "SPELL"));
    auto elistra = std::make_shared<BgBaseCard>(4, "NEUTRAL", 8, 4, "Elistra the Immortal",
                                                "", "", "", 6, "MINION");
    elistra->set_reborn();
    elistra->set_divine_shield();
    cards.emplace("Elistra the Immortal", elistra);

    auto elistra_gold = std::make_shared<BgBaseCard>(8, "NEUTRAL", 8, 8, "Elistra the Immortal",
                                                     "", "", "", 6, "MINION");
    elistra_gold->set_reborn();
    elistra_gold->set_divine_shield();
    cards.emplace("Elistra the Immortal (Golden)", elistra_gold);

    // F
    cards.emplace("Faceless Taverngoer", std::make_shared<FacelessTaverngoer>());
    cards.emplace("Faceless Taverngoer (Golden)", std::make_shared<FacelessTaverngoerGolden>());
    cards.emplace("Felfin Navigator", std::make_shared<FelfinNavigator>());
    cards.emplace("Felfin Navigator (Golden)", std::make_shared<FelfinNavigatorGolden>());
    cards.emplace("Fiendish Servant", std::make_shared<FiendishServant>());
    cards.emplace("Fiendish Servant (Golden)", std::make_shared<FiendishServantGolden>());
    std::shared_ptr<BgBaseCard> floating_watcher(new BgBaseCard(4, "WARLOCK", 5, 4, "Floating Watcher",
                                                                "['TRIGGER_VISUAL']", "DEMON", "COMMON", 4, "MINION"));
    cards.emplace("Floating Watcher", floating_watcher);
    std::shared_ptr<BgBaseCard> floating_watcher_gold(new BgBaseCard(8, "WARLOCK", 5, 8, "Floating Watcher (Golden)",
                                                                     "['TRIGGER_VISUAL']", "DEMON", "COMMON", 4, "MINION"));
    cards.emplace("Floating Watcher (Golden)", floating_watcher_gold);
    std::shared_ptr<BgBaseCard> foe_reaper(new BgBaseCard(6, "NEUTRAL", 8, 9, "Foe Reaper 4000",
                                                          "['CLEAVE']", "MECHANICAL", "LEGENDARY", 6, "MINION"));
    cards.emplace("Foe Reaper 4000", foe_reaper);
    std::shared_ptr<BgBaseCard> foe_reaper_gold(new BgBaseCard(12, "NEUTRAL", 8, 18, "Foe Reaper 4000",
                                                               "['CLEAVE']", "MECHANICAL", "LEGENDARY", 6, "MINION"));
    cards.emplace("Foe Reaper 4000 (Golden)", foe_reaper_gold);

    // G
    cards.emplace("Lieutenant Garr", std::make_shared<LieutenantGarr>());
    cards.emplace("Lieutenant Garr (Golden)", std::make_shared<LieutenantGarrGolden>());
    cards.emplace("Freedealing Gambler", std::make_shared<FreedealingGambler>());
    cards.emplace("Freedealing Gambler (Golden)", std::make_shared<FreedealingGamblerGolden>());
    cards.emplace("Ghastcoiler", std::make_shared<Ghastcoiler>());
    cards.emplace("Ghastcoiler (Golden)", std::make_shared<GhastcoilerGolden>());
    cards.emplace("Glyph Guardian", std::make_shared<GlyphGuardian>());
    cards.emplace("Glyph Guardian (Golden)", std::make_shared<GlyphGuardianGolden>());
    cards.emplace("Gold Coin", std::make_shared<BgBaseCard>(-1, "NEUTRAL", 0, -1, "Gold Coin",
                                                            "", "", "RARE", -1, "SPELL"));
    cards.emplace("Goldgrubber", std::make_shared<Goldgrubber>());
    cards.emplace("Goldgrubber (Golden)", std::make_shared<GoldgrubberGolden>());
    cards.emplace("Goldrinn", std::make_shared<Goldrinn>());
    cards.emplace("Goldrinn (Golden)", std::make_shared<GoldrinnGolden>());
    std::shared_ptr<BgBaseCard> guard_bot(new BgBaseCard(2, "WARRIOR", 2, 3, "Guard Bot",
                                                         "['TAUNT']", "MECHANICAL", "", 1, "MINION"));
    guard_bot->set_available_in_tavern(false);
    cards.emplace("Guard Bot", guard_bot);
    std::shared_ptr<BgBaseCard> guard_bot_golden(new BgBaseCard(4, "WARRIOR", 2, 6, "Guard Bot (Golden)",
                                                                "['TAUNT']", "MECHANICAL", "", 1, "MINION"));
    cards.emplace("Guard Bot (Golden)", guard_bot_golden);

    // H
    // TODO: Secrets
    // cards.emplace("Hand of Salvation", BgBaseCard(-1, "PALADIN", 1, -1, "Hand of Salvation",
    //                                            "['SECRET']", "", "FREE", -1, "SPELL"));
    cards.emplace("Hangry Dragon", std::make_shared<HangryDragon>());
    cards.emplace("Hangry Dragon (Golden)", std::make_shared<HangryDragonGolden>());
    cards.emplace("Harvest Golem", std::make_shared<HarvestGolem>());
    cards.emplace("Harvest Golem (Golden)", std::make_shared<HarvestGolemGolden>());
    cards.emplace("Herald of Flame", std::make_shared<HeraldOfFlame>());
    cards.emplace("Herald of Flame (Golden)", std::make_shared<HeraldOfFlameGolden>());
    cards.emplace("Houndmaster", std::make_shared<Houndmaster>());
    cards.emplace("Houndmaster (Golden)", std::make_shared<HoundmasterGolden>());
    std::shared_ptr<BgBaseCard> hyena(new BgBaseCard(2, "HUNTER", 2, 2, "Hyena",
                                                     "", "BEAST", "", 1,"MINION"));
    hyena->set_available_in_tavern(false);
    cards.emplace("Hyena", hyena);
    std::shared_ptr<BgBaseCard> hyena_gold(new BgBaseCard(4, "HUNTER", 2, 4, "Hyena (Golden)",
                                                          "", "BEAST", "", 1,"MINION"));
    cards.emplace("Hyena (Golden)", hyena_gold);

    // I
    // TODO: Secrets
    // cards.emplace("Ice Block", BgBaseCard(-1, "MAGE", 3, -1, "Ice Block",
    //                                    "['SECRET']", "", "EPIC", -1, "SPELL"));
    cards.emplace("Infested Wolf", std::make_shared<InfestedWolf>());
    cards.emplace("Infested Wolf (Golden)", std::make_shared<InfestedWolfGolden>());
    std::shared_ptr<BgBaseCard> imp(new BgBaseCard(1, "WARLOCK", 1, 1, "Imp",
                                                   "", "DEMON", "", 1, "MINION"));
    imp->set_available_in_tavern(false);
    cards.emplace("Imp", imp);
    std::shared_ptr<BgBaseCard> imp_gold(new BgBaseCard(2, "WARLOCK", 1, 2, "Imp (Golden)",
                                             "", "DEMON", "", 1, "MINION"));
    cards.emplace("Imp (Golden)", imp_gold);
    cards.emplace("Imp Gang Boss", std::make_shared<ImpGangBoss>());
    cards.emplace("Imp Gang Boss (Golden)", std::make_shared<ImpGangBossGolden>());
    cards.emplace("Imp Mama", std::make_shared<ImpMama>());
    cards.emplace("Imp Mama (Golden)", std::make_shared<ImpMamaGolden>());
    cards.emplace("Imprisoner", std::make_shared<Imprisoner>());
    cards.emplace("Imprisoner (Golden)", std::make_shared<ImprisonerGolden>());
    cards.emplace("Iron Sensei", std::make_shared<IronSensei>());
    cards.emplace("Iron Sensei (Golden)", std::make_shared<IronSenseiGolden>());
    cards.emplace("Ironhide Direhorn", std::make_shared<IronhideDirehorn>());
    cards.emplace("Ironhide Direhorn (Golden)", std::make_shared<IronhideDirehornGolden>());
    std::shared_ptr<BgBaseCard> ironhide_runt(new BgBaseCard(5, "DRUID", 5, 5, "Ironhide Runt",
                                                             "", "BEAST", "", 1, "MINION"));
    cards.emplace("Ironhide Runt", ironhide_runt);
    std::shared_ptr<BgBaseCard> ironhide_runt_gold(new BgBaseCard(10, "DRUID", 5, 10, "Ironhide Runt (Golden)",
                                                                  "", "BEAST", "", 1, "MINION"));
    cards.emplace("Ironhide Runt (Golden)", ironhide_runt_gold);

    // J
    cards.emplace("Junkbot", std::make_shared<Junkbot>());
    cards.emplace("Junkbot (Golden)", std::make_shared<JunkbotGolden>());

    // K
    cards.emplace("Kaboom Bot", std::make_shared<KaboomBot>());
    cards.emplace("Kaboom Bot (Golden)", std::make_shared<KaboomBotGolden>());
    cards.emplace("Kangor", std::make_shared<Kangor>());
    cards.emplace("Kangor (Golden)", std::make_shared<KangorGolden>());
    cards.emplace("Kalecgos, Arcane Aspect", std::make_shared<Kalecgos>());
    cards.emplace("Kalecgos, Arcane Aspect (Golden)", std::make_shared<KalecgosGolden>());
    // Khadgar's effect is hardcoded in the summoning logic, not handled by the card itself
    std::shared_ptr<BgBaseCard> khadgar(new BgBaseCard(2, "MAGE", 2, 2,  "Khadgar",
                                                       "['TRIGGER_VISUAL']", "", "LEGENDARY", 3, "MINION"));
    cards.emplace("Khadgar", khadgar);
    std::shared_ptr<BgBaseCard> khadgar_golden(new BgBaseCard(4, "MAGE", 2, 4,  "Khadgar (Golden)",
                                                              "['TRIGGER_VISUAL']", "", "LEGENDARY", 3, "MINION"));
    cards.emplace("Khadgar (Golden)", khadgar_golden);
    cards.emplace("Kindly Grandmother", std::make_shared<KindlyGrandmother>());
    cards.emplace("Kindly Grandmother (Golden)", std::make_shared<KindlyGrandmotherGolden>());
    cards.emplace("King Bagurgle", std::make_shared<KingBagurgle>());
    cards.emplace("King Bagurgle (Golden)", std::make_shared<KingBagurgleGolden>());

    // L
    cards.emplace("Lightfang Enforcer", std::make_shared<LightfangEnforcer>());
    cards.emplace("Lightfang Enforcer (Golden)", std::make_shared<LightfangEnforcerGolden>());
    cards.emplace("Lil' Rag", std::make_shared<LilRag>());
    cards.emplace("Lil' Rag (Golden)", std::make_shared<LilRagGolden>());
    std::shared_ptr<BgBaseCard> living_spore(new BgBaseCard(1, "NEUTRAL", 1, 1,  "Living Spore",
                                                            "", "", "COMMON", 1, "MINION"));
    living_spore->set_available_in_tavern(false);
    cards.emplace("Living Spore", living_spore);
    // NOTE: Not a real card, useful for adapt/magnetic mechanic
    cards.emplace("Living Spore Drattle", std::make_shared<LivingSporeDrattle>());
    


    // M
    std::shared_ptr<BgBaseCard> maexxna(new BgBaseCard(2, "NEUTRAL", 6, 8, "Maexxna",
                                                      "", "BEAST", "", 6, "MINION"));
    maexxna->set_poison();
    cards.emplace("Maexxna", maexxna);
    std::shared_ptr<BgBaseCard> maexxna_gold(new BgBaseCard(4, "NEUTRAL", 6, 16, "Maexxna (Golden)",
                                                           "", "BEAST", "", 6, "MINION"));
    maexxna_gold->set_poison();
    cards.emplace("Maexxna (Golden)", maexxna_gold);
    cards.emplace("Majordomo Executus", std::make_shared<MajordomoExecutus>());
    cards.emplace("Majordomo Executus (Golden)", std::make_shared<MajordomoExecutusGolden>());
    cards.emplace("Mal'Ganis", std::make_shared<MalGanis>());
    cards.emplace("Mal'Ganis (Golden)", std::make_shared<MalGanisGolden>());
    cards.emplace("Mama Bear", std::make_shared<MamaBear>());
    cards.emplace("Mama Bear (Golden)", std::make_shared<MamaBearGolden>());
    cards.emplace("Mechano-Egg", std::make_shared<MechanoEgg>());
    cards.emplace("Mechano-Egg (Golden)", std::make_shared<MechanoEggGolden>());
    cards.emplace("Menagerie Jug", std::make_shared<MenagerieJug>());
    cards.emplace("Menagerie Jug (Golden)", std::make_shared<MenagerieJugGolden>());
    cards.emplace("Menagerie Mug", std::make_shared<MenagerieMug>());
    cards.emplace("Menagerie Mug (Golden)", std::make_shared<MenagerieMugGolden>());
    cards.emplace("Metaltooth Leaper", std::make_shared<MetaltoothLeaper>());
    cards.emplace("Metaltooth Leaper (Golden)", std::make_shared<MetaltoothLeaperGolden>());
    cards.emplace("Micro Machine", std::make_shared<MicroMachine>());
    cards.emplace("Micro Machine (Golden)", std::make_shared<MicroMachineGolden>());
    auto micro_mummy = std::make_shared<MicroMummy>();
    micro_mummy->set_reborn();
    cards.emplace("Micro Mummy", micro_mummy);
    auto micro_mummy_golden = std::make_shared<MicroMummyGolden>();
    micro_mummy_golden->set_reborn();
    cards.emplace("Micro Mummy (Golden)", micro_mummy_golden);
    std::shared_ptr<BgBaseCard> microbot(new BgBaseCard(1, "NEUTRAL", 1, 1, "Microbot",
                                                        "", "MECHANICAL", "", 1, "MINION"));
    microbot->set_available_in_tavern(false);
    cards.emplace("Microbot", microbot);
    std::shared_ptr<BgBaseCard> microbot_gold(new BgBaseCard(2, "NEUTRAL", 1, 2, "Microbot (Golden)",
                                                             "", "MECHANICAL", "", 1, "MINION"));
    cards.emplace("Microbot (Golden)", microbot_gold);
    cards.emplace("Monstrous Macaw" , std::make_shared<MonstrousMacaw>());
    cards.emplace("Monstrous Macaw (Golden)" , std::make_shared<MonstrousMacawGolden>());
    cards.emplace("Molten Rock", std::make_shared<MoltenRock>());
    cards.emplace("Molten Rock (Golden)", std::make_shared<MoltenRockGolden>());
    std::shared_ptr<BgBaseCard> murloc_scout(new BgBaseCard(1, "NEUTRAL", 1, 1, "Murloc Scout",
                                                            "", "MURLOC", "COMMON", 1, "MINION"));
    murloc_scout->set_available_in_tavern(false);
    cards.emplace("Murloc Scout", murloc_scout);
    std::shared_ptr<BgBaseCard> murloc_scout_gold(new BgBaseCard(2, "NEUTRAL", 1, 2, "Murloc Scout (Golden)",
                                                                 "", "MURLOC", "COMMON", 1, "MINION"));
    cards.emplace("Murloc Scout (Golden)", murloc_scout_gold);
    cards.emplace("Murloc Tidecaller", std::make_shared<MurlocTidecaller>());
    cards.emplace("Murloc Tidecaller (Golden)", std::make_shared<MurlocTidecallerGolden>());
    cards.emplace("Murloc Tidehunter", std::make_shared<MurlocTidehunter>());
    cards.emplace("Murloc Tidehunter (Golden)", std::make_shared<MurlocTidehunterGolden>());
    cards.emplace("Murloc Warleader", std::make_shared<MurlocWarleader>());
    cards.emplace("Murloc Warleader (Golden)", std::make_shared<MurlocWarleaderGolden>());
    cards.emplace("Murozond", std::make_shared<Murozond>());
    cards.emplace("Murozond (Golden)", std::make_shared<MurozondGolden>());
    cards.emplace("Mythrax the Unraveler", std::make_shared<MythraxTheUnraveler>());
    cards.emplace("Mythrax the Unraveler (Golden)", std::make_shared<MythraxTheUnravelerGolden>());
                                                           
    // N
    cards.emplace("Nadina", std::make_shared<Nadina>());
    cards.emplace("Nadina (Golden)", std::make_shared<NadinaGolden>());
    cards.emplace("Nat Pagle, Extreme Angler", std::make_shared<NatPagle>());
    cards.emplace("Nat Pagle, Extreme Angler (Golden)", std::make_shared<NatPagleGolden>());
    cards.emplace("Nathrezim Overseer", std::make_shared<NathrezimOverseer>());
    cards.emplace("Nathrezim Overseer (Golden)", std::make_shared<NathrezimOverseerGolden>());
    cards.emplace("Nomi, Kitchen Nightmare", std::make_shared<Nomi>());
    cards.emplace("Nomi, Kitchen Nightmare (Golden)", std::make_shared<NomiGolden>());
    
    // O
    cards.emplace("Old Murk-Eye", std::make_shared<OldMurkeye>());
    cards.emplace("Old Murk-Eye (Golden)", std::make_shared<OldMurkeyeGolden>());

    // P
    cards.emplace("Pack Leader", std::make_shared<PackLeader>());
    cards.emplace("Pack Leader (Golden)", std::make_shared<PackLeaderGolden>());
    cards.emplace("Party Elemental", std::make_shared<PartyElemental>());
    cards.emplace("Party Elemental (Golden)", std::make_shared<PartyElementalGolden>());
    cards.emplace("Piloted Shredder", std::make_shared<PilotedShredder>());
    cards.emplace("Piloted Shredder (Golden)", std::make_shared<PilotedShredderGolden>());
    std::shared_ptr<BgBaseCard> primalfin(new BgBaseCard(3, "NEUTRAL", 3, 2, "Primalfin Lookout",
                                                         "['BATTLECRY', 'DISCOVER']", "MURLOC", "COMMON", 5, "MINION"));
    cards.emplace("Primalfin Lookout", primalfin);
    std::shared_ptr<BgBaseCard> primalfin_gold(new BgBaseCard(6, "NEUTRAL", 3, 4, "Primalfin Lookout (Golden)",
                                                              "['BATTLECRY', 'DISCOVER']", "MURLOC", "COMMON", 5, "MINION"));
    cards.emplace("Primalfin Lookout (Golden)", primalfin_gold);

    // Q
    cards.emplace("Qiraji Harbinger", std::make_shared<BgBaseCard>(5, "NEUTRAL", 6, 5, "Qiraji Harbinger",
                                                                   "", "NEUTRAL", "COMMON", 4, "MINION"));
    cards.emplace("Qiraji Harbinger (Golden)", std::make_shared<BgBaseCard>(10, "NEUTRAL", 6, 10, "Qiraji Harbinger (Golden)",
                                                                            "", "NEUTRAL", "COMMON", 4, "MINION"));

    // R
    cards.emplace("Rabid Saurolisk", std::make_shared<RabidSaurolisk>());
    cards.emplace("Rabid Saurolisk (Golden)", std::make_shared<RabidSauroliskGolden>());
    std::shared_ptr<BgBaseCard> rat(new BgBaseCard(1, "HUNTER", 1, 1, "Rat",
                                                   "", "BEAST", "", 1, "MINION"));
    rat->set_available_in_tavern(false);
    cards.emplace("Rat", rat);
    std::shared_ptr<BgBaseCard> rat_gold(new BgBaseCard(2, "HUNTER", 1, 2, "Rat (Golden)",
                                                        "", "BEAST", "", 1, "MINION"));
    cards.emplace("Rat (Golden)", rat_gold);
    cards.emplace("Rat Pack", std::make_shared<RatPack>());
    cards.emplace("Rat Pack (Golden)", std::make_shared<RatPackGolden>());
    cards.emplace("Razorgore, the Untamed", std::make_shared<Razorgore>());
    cards.emplace("Razorgore, the Untamed (Golden)", std::make_shared<RazorgoreGolden>());
    cards.emplace("Red Whelp", std::make_shared<RedWhelp>());
    cards.emplace("Red Whelp (Golden)", std::make_shared<RedWhelpGolden>());
    // cards.emplace("Redemption", BgBaseCard(-1, "PALADIN", 1, -1, "Redemption",
    //                                     "['SECRET']", "", "COMMON", -1, "SPELL"));
    cards.emplace("Refreshing Anomaly", std::make_shared<RefreshingAnomaly>());
    cards.emplace("Refreshing Anomaly (Golden)", std::make_shared<RefreshingAnomalyGolden>());
    cards.emplace("Replicating Menace", std::make_shared<ReplicatingMenace>());
    cards.emplace("Replicating Menace (Golden)", std::make_shared<ReplicatingMenaceGolden>());
    cards.emplace("Ripsnarl Captain", std::make_shared<RipsnarlCaptain>());
    cards.emplace("Ripsnarl Captain (Golden)", std::make_shared<RipsnarlCaptainGolden>());
    std::shared_ptr<BgBaseCard> robosaur(new BgBaseCard(8, "PALADIN", 8, 8, "Robosaur",
                                                        "", "MECHANICAL", "", 1, "MINION"));
    robosaur->set_available_in_tavern(false);
    cards.emplace("Robosaur", robosaur);
    std::shared_ptr<BgBaseCard> robosaur_gold(new BgBaseCard(16, "PALADIN", 8, 16, "Robosaur (Golden)",
                                                             "", "MECHANICAL", "", 1, "MINION"));
    cards.emplace("Robosaur (Golden)", robosaur_gold);
    cards.emplace("Rockpool Hunter", std::make_shared<RockpoolHunter>());
    cards.emplace("Rockpool Hunter (Golden)", std::make_shared<RockpoolHunterGolden>());

    // S
    cards.emplace("Salty Looter", std::make_shared<SaltyLooter>());
    cards.emplace("Salty Looter (Golden)", std::make_shared<SaltyLooterGolden>());
    cards.emplace("Savannah Highmane", std::make_shared<SavannahHighmane>());
    cards.emplace("Savannah Highmane (Golden)", std::make_shared<SavannahHighmaneGolden>());
    cards.emplace("Scallywag", std::make_shared<Scallywag>());
    cards.emplace("Scallywag (Golden)", std::make_shared<ScallywagGolden>());
    cards.emplace("Scavenging Hyena", std::make_shared<ScavengingHyena>());
    cards.emplace("Scavenging Hyena (Golden)", std::make_shared<ScavengingHyenaGolden>());
    cards.emplace("Screwjank Clunker", std::make_shared<ScrewjankClunker>());
    cards.emplace("Screwjank Clunker (Golden)", std::make_shared<ScrewjankClunkerGolden>());
    auto seabreaker = std::make_shared<SeabreakerGoliath>();
    seabreaker->set_windfury();
    cards.emplace("Seabreaker Goliath", seabreaker);
    auto seabreaker_gold = std::make_shared<SeabreakerGoliathGolden>();
    seabreaker_gold->set_windfury();
    cards.emplace("Seabreaker Goliath (Golden)", seabreaker_gold);
    cards.emplace("Security Rover", std::make_shared<SecurityRover>());
    cards.emplace("Security Rover (Golden)", std::make_shared<SecurityRoverGolden>());
    cards.emplace("Selfless Hero", std::make_shared<SelflessHero>());
    cards.emplace("Selfless Hero (Golden)", std::make_shared<SelflessHeroGolden>());
    cards.emplace("Sellemental", std::make_shared<Sellemental>());
    cards.emplace("Sellemental (Golden)", std::make_shared<SellementalGolden>());
    cards.emplace("Siegebreaker", std::make_shared<Siegebreaker>());
    cards.emplace("Siegebreaker (Golden)", std::make_shared<SiegebreakerGolden>());
    cards.emplace("Sky Pirate", std::make_shared<SkyPirate>());
    cards.emplace("Sky Pirate (Golden)", std::make_shared<SkyPirateGolden>());
    // cards.emplace("Snake Trap", BgBaseCard(-1, "HUNTER", 2, -1, "Snake Trap",
    //                                     "['SECRET']", "", "EPIC", -1, "SPELL"));
    cards.emplace("Sneed's Old Shredder", std::make_shared<SneedsOldShredder>());
    cards.emplace("Sneed's Old Shredder (Golden)", std::make_shared<SneedsOldShredderGolden>());
    cards.emplace("Soul Juggler", std::make_shared<SoulJuggler>());
    cards.emplace("Soul Juggler (Golden)", std::make_shared<SoulJugglerGolden>());
    cards.emplace("Southsea Captain", std::make_shared<SouthseaCaptain>());
    cards.emplace("Southsea Captain (Golden)", std::make_shared<SouthseaCaptainGolden>());
    cards.emplace("Southsea Strongarm", std::make_shared<SouthseaStrongarm>());
    cards.emplace("Southsea Strongarm (Golden)", std::make_shared<SouthseaStrongarmGolden>());
    cards.emplace("Spawn of N'Zoth", std::make_shared<SpawnOfNzoth>());
    cards.emplace("Spawn of N'Zoth (Golden)", std::make_shared<SpawnOfNzothGolden>());
    std::shared_ptr<BgBaseCard> spider(new BgBaseCard(1, "HUNTER", 1, 1, "Spider",
                                                      "", "BEAST", "", 1, "MINION"));
    spider->set_available_in_tavern(false);
    cards.emplace("Spider", spider);
    std::shared_ptr<BgBaseCard> spider_gold(new BgBaseCard(2, "HUNTER", 1, 2, "Spider (Golden)",
                                                           "", "BEAST", "", 1, "MINION"));
    cards.emplace("Spider (Golden)", spider_gold);
    // TODO: Secrets
    // cards.emplace("Splitting Image", BgBaseCard(-1, "MAGE", 3, -1, "Splitting Image",
    //                                          "['SECRET']", "", "EPIC", -1, "SPELL"));
    cards.emplace("Stasis Elemental", std::make_shared<StasisElemental>());
    cards.emplace("Stasis Elemental (Golden)", std::make_shared<StasisElementalGolden>());
    cards.emplace("Steward of Time", std::make_shared<StewardOfTime>());
    cards.emplace("Steward of Time (Golden)", std::make_shared<StewardOfTimeGolden>());
    cards.emplace("Strongshell Scavenger", std::make_shared<StrongshellScavenger>());
    cards.emplace("Strongshell Scavenger (Golden)", std::make_shared<StrongshellScavengerGolden>());

    // T
    std::shared_ptr<BgBaseCard> cat(new BgBaseCard(1, "HUNTER", 1, 1, "Tabbycat",
                                                   "", "BEAST", "COMMON", 1, "MINION"));
    cat->set_available_in_tavern(false);
    cards.emplace("Tabbycat", cat);
    std::shared_ptr<BgBaseCard> cat_gold(new BgBaseCard(2, "HUNTER", 1, 2, "Tabbycat (Golden)",
                                                             "", "BEAST", "COMMON", 1, "MINION"));
    cards.emplace("Tabbycat (Golden)", cat_gold);
    cards.emplace("Tavern Tempest", std::make_shared<TavernTempest>());
    cards.emplace("Tavern Tempest (Golden)", std::make_shared<TavernTempestGolden>());
    cards.emplace("The Tide Razor", std::make_shared<TheTideRazor>());
    cards.emplace("The Tide Razor (Golden)", std::make_shared<TheTideRazorGolden>());
    cards.emplace("Tormented Ritualist", std::make_shared<TormentedRitualist>());
    cards.emplace("Tormented Ritualist (Golden)", std::make_shared<TormentedRitualistGolden>());
    cards.emplace("Toxfin", std::make_shared<Toxfin>());
    cards.emplace("Toxfin (Golden)", std::make_shared<ToxfinGolden>());
    cards.emplace("Triple Discover", std::make_shared<TripleDiscover>());
    cards.emplace("Twilight Emissary", std::make_shared<TwilightEmissary>());
    cards.emplace("Twilight Emissary (Golden)", std::make_shared<TwilightEmissaryGolden>());

    // U
    cards.emplace("Unstable Ghoul", std::make_shared<UnstableGhoul>());
    cards.emplace("Unstable Ghoul (Golden)", std::make_shared<UnstableGhoulGolden>());

    // V
    // TODO: Secrets
    // cards.emplace("Venomstrike Trap", BgBaseCard(-1, "HUNTER", 2, -1, "Venomstrike Trap",
    //                                           "['SECRET']", "", "RARE", -1, "SPELL"));
    cards.emplace("Virmen Sensei", std::make_shared<VirmenSensei>());
    cards.emplace("Virmen Sensei (Golden)", std::make_shared<VirmenSenseiGolden>());
    cards.emplace("Voidlord", std::make_shared<Voidlord>());
    cards.emplace("Voidlord (Golden)", std::make_shared<Voidlord>());
    std::shared_ptr<BgBaseCard> voidwalker(new BgBaseCard(1, "WARLOCK", 1, 3, "Voidwalker",
                                                          "['TAUNT']", "DEMON", "FREE", 1, "MINION"));
    cards.emplace("Voidwalker", voidwalker);
    std::shared_ptr<BgBaseCard> voidwalker_gold(new BgBaseCard(2, "WARLOCK", 1, 6, "Voidwalker (Golden)",
                                                               "['TAUNT']", "DEMON", "FREE", 1, "MINION"));
    cards.emplace("Voidwalker (Golden)", voidwalker_gold);
    cards.emplace("Vulgar Homunculus", std::make_shared<VulgarHomunculus>());
    cards.emplace("Vulgar Homunculus (Golden)", std::make_shared<VulgarHomunculusGolden>());
                                           
    // W
    cards.emplace("Warden of Old", std::make_shared<WardenOfOld>());
    cards.emplace("Warden of Old (Golden)", std::make_shared<WardenOfOldGolden>());
    std::shared_ptr<BgBaseCard> water_droplet(new BgBaseCard(2, "NEUTRAL", 1, 2, "Water Droplet",
                                                             "", "ELEMENTAL", "", 1, "MINION"));
    water_droplet->set_available_in_tavern(false);
    cards.emplace("Water Droplet", water_droplet);
    cards.emplace("Water Droplet (Golden)", std::make_shared<BgBaseCard>(4, "NEUTRAL", 1, 4, "Water Droplet (Golden)",
                                                                         "", "ELEMENTAL", "", 1, "MINION"));
    cards.emplace("Waxrider Togwaggle", std::make_shared<WaxriderTogwaggle>());
    cards.emplace("Waxrider Togwaggle (Golden)", std::make_shared<WaxriderTogwaggleGolden>());
    cards.emplace("Whirlwind Tempest", std::make_shared<BgBaseCard>(6, "NEUTRAL", 8, 6, "Whirlwind Tempest",
                                                              "", "ELEMENTAL", "", 4, "MINION"));
    cards.emplace("Whirlwind Tempest (Golden)", std::make_shared<BgBaseCard>(12, "NEUTRAL", 8, 12, "Whirlwind Tempest (Golden)",
                                                                       "", "ELEMENTAL", "", 4, "MINION"));
    cards.emplace("Wildfire Elemental", std::make_shared<WildfireElemental>());
    cards.emplace("Wildfire Elemental (Golden)", std::make_shared<WildfireElementalGolden>());
    cards.emplace("Wrath Weaver", std::make_shared<WrathWeaver>());
    cards.emplace("Wrath Weaver (Golden)", std::make_shared<WrathWeaverGolden>());

    // Y
    cards.emplace("Yo-Ho-Ogre", std::make_shared<YoHoOgre>());
    cards.emplace("Yo-Ho-Ogre (Golden)", std::make_shared<YoHoOgreGolden>());

    // Z
    std::shared_ptr<BgBaseCard> zapp(new BgBaseCard(7, "NEUTRAL", 8, 10, "Zapp",
                                                    "", "", "", 6, "MINION"));
    zapp->set_windfury();
    cards.emplace("Zapp", zapp);
    std::shared_ptr<BgBaseCard> zapp_gold(new BgBaseCard(14, "NEUTRAL", 8, 20, "Zapp (Golden)",
                                                         "['WINDFURY']", "", "", 6, "MINION"));
    zapp_gold->set_windfury();
    cards.emplace("Zapp (Golden)", zapp_gold);
}
