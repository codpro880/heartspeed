/**
These are the cards that weren't automatically roped in and should be generated by hand.
**/
#include <string>
#include <unordered_map>

#include "BgBaseCard.hpp"
#include "BgCardFactory.hpp"
#include "DeathrattleCards.hpp"

std::shared_ptr<BgBaseCard> BgCardFactory::get_card(std::string name) {
    auto card_found_it = cards.find(name);
    if (card_found_it == cards.end())  {
    	throw std::runtime_error("No card has name '" + name + "'");
    }
    else {
	return (card_found_it->second)->get_copy();
    }
}

std::shared_ptr<BgBaseCard> BgCardFactory::init_card(int attack,
				      std::string card_class,
				      int cost,
				      int health,
				      std::string name,
				      std::string mechanics,
				      std::string race,
				      std::string rarity,
				      int tech_level,
				      std::string type) {
    std::shared_ptr<BgBaseCard> card(new BgBaseCard(attack, card_class, cost, health, name,
						    mechanics, race, rarity, tech_level, type));
    return card;
}

void BgCardFactory::init_cards() {
    // A
    std::shared_ptr<BgBaseCard> alleycat_gold(new BgBaseCard(2, "HUNTER", 1, 2, "Alleycat (Golden)",
							     "['BATTLECRY']", "BEAST", "COMMON", 1, "MINION"));
    cards.emplace("Alleycat (Golden)", alleycat_gold);
    // cards.emplace("Amalgadon", BgBaseCard(6, "NEUTRAL", 8, 6, "Amalgadon",
    // 					  "['BATTLECRY']", "ALL", "", 6, "MINION"));
    // cards.emplace("Amalgadon (Golden)", BgBaseCard(12, "NEUTRAL", 8, 12, "Amalgadon (Golden)",
    // 						   "['BATTLECRY']", "ALL", "", 6, "MINION"));
    // cards.emplace("Annihilan Battlemaster", BgBaseCard(3, "NEUTRAL", 8, 1, "Annihilan Battlemaster",
    // 						       "['BATTLECRY']", "DEMON", "EPIC", 5, "MINION"));
    // cards.emplace("Annihilan Battlemaster (Golden)", BgBaseCard(6, "NEUTRAL", 8, 2, "Annihilan Battlemaster (Golden)",
    // 								"['BATTLECRY']", "DEMON", "EPIC", 5, "MINION"));
    // cards.emplace("Annoy-o-Module (Golden)", BgBaseCard(4, "PALADIN", 4, 8, "Annoy-o-Module (Golden)",
    // 							"['DIVINE_SHIELD', 'MODULAR', 'TAUNT']", "MECHANICAL", "RARE", 4, "MINION"));
    // cards.emplace("Arcane Cannon", BgBaseCard(2, "NEUTRAL", 3, 2, "Arcane Cannon",
    // 					      "['CANT_ATTACK', 'TRIGGER_VISUAL']", "", "", 2, "MINION"));
    // cards.emplace("Arcane Cannon (Golden)", BgBaseCard(4, "NEUTRAL", 3, 4, "Arcane Cannon (Golden)",
    // 						       "['CANT_ATTACK', 'TRIGGER_VISUAL']", "", "", 2, "MINION"));
    // cards.emplace("Avenge", BgBaseCard(-1, "PALADIN", 1, -1, "Avenge",
    // 				       "['SECRET']", "", "COMMON", -1, "SPELL"));
    // cards.emplace("Autodefense Matrix", BgBaseCard(-1, "PALADIN", 1, -1, "Autodefense Matrix",
    // 						   "['SECRET']", "", "COMMON", -1, "SPELL"));

    // B
    // cards.emplace("Baron Rivendare (Golden)", BgBaseCard(2, "NEUTRAL", 4, 14, "Baron Rivendare (Golden)",
    // 							 "['AURA']", "", "LEGENDARY", 5, "MINION"));
    std::shared_ptr<BgBaseCard> big_bad_wolf(new BgBaseCard(3, "HUNTER", 2, 2, "Big Bad Wolf",
							    "", "BEAST", "", 1, "MINION"));					    
    cards.emplace("Big Bad Wolf", big_bad_wolf);
    std::shared_ptr<BgBaseCard> big_bad_wolf_gold(new BgBaseCard(6, "HUNTER", 2, 4, "Big Bad Wolf (Golden)",
								 "", "BEAST", "", 1, "MINION"));
    cards.emplace("Big Bad Wolf (Golden)", big_bad_wolf_gold);
    // cards.emplace("Big Bad Wolf (Golden)", BgBaseCard(6, "HUNTER", 2, 4, "Big Bad Wolf (Golden)",
    // 						      "", "BEAST", "", 1, "MINION"));
    // cards.emplace("Bloodsail Cannoneer", BgBaseCard(4, "NEUTRAL", 4, 2, "Bloodsail Cannoneer",
    // 						    "", "PIRATE", "", 3, "MINION"));
    // cards.emplace("Bloodsail Cannoneer (Golden)", BgBaseCard(8, "NEUTRAL", 4, 4, "Bloodsail Cannoneer (Golden)",
    // 							     "", "PIRATE", "", 3,  "MINION"));
    // cards.emplace("Bolvar, Fireblood (Golden)", BgBaseCard(2, "PALADIN", 5, 14, "Bolvar, Fireblood (Golden)",
    // 							   "['DIVINE_SHIELD', 'TRIGGER_VISUAL']", "", "LEGENDARY", 4, "MINION"));
    // cards.emplace("Brann Bronzebeard (Golden)", BgBaseCard(4, "NEUTRAL", 3, 8, "Brann Bronzebeard (Golden)",
    // 							   "['AURA']", "", "LEGENDARY", 5, "MINION"));
    // cards.emplace("Bronze Warden", BgBaseCard(2, "NEUTRAL", 4, 1, "Bronze Warden",
    // 			     "['DIVINE_SHIELD', 'REBORN']", "DRAGON", "", 3, "MINION"));

    // C
    // cards.emplace("Cap'n Hoggarr", BgBaseCard(6, "NEUTRAL", 6, 6,  "Cap'n Hoggarr",
    // 					      "['TRIGGER_VISUAL']", "PIRATE", "", 5, "MINION"));
    // cards.emplace("Cap'n Hoggarr (Golden)", BgBaseCard(12, "NEUTRAL", 6, 12, "Cap'n Hoggarr (Golden)",
    // 						       "['TRIGGER_VISUAL']", "PIRATE", "", 5, "MINION"));
    // cards.emplace("Cobalt Scalebane (Golden)", BgBaseCard(10, "NEUTRAL", 5, 10, "Cobalt Scalebane (Golden)",
    // 							  "['TRIGGER_VISUAL']", "DRAGON", "COMMON", 4, "MINION"));
    // cards.emplace("Coldlight Seer (Golden)", BgBaseCard(4, "NEUTRAL", 3, 6, "Coldlight Seer (Golden)",
    // 							"['BATTLECRY']", "MURLOC", "RARE", 3, "MINION"));
    // cards.emplace("Crowd Favorite (Golden)", BgBaseCard(8, "NEUTRAL", 4, 8, "Crowd Favorite (Golden)",
    // 							"['TRIGGER_VISUAL']", "", "EPIC", 3, "MINION"));
    // cards.emplace("Crystalweaver (Golden)", BgBaseCard(10, "WARLOCK", 4, 8, "Crystalweaver (Golden)",
    // 						       "['BATTLECRY']", "", "COMMON", 3, "MINION"));

    // D
    std::shared_ptr<BgBaseCard> damaged_golem(new BgBaseCard(2, "NEUTRAL", 1, 1,
							     "Damaged Golem", "", "MECHANICAL", "", 1, "MINION"));
    cards.emplace("Damaged Golem", damaged_golem);
    std::shared_ptr<BgBaseCard> damaged_golem_gold(new BgBaseCard(4, "NEUTRAL", 1, 2,
								  "Damaged Golem (Golden)", "", "MECHANICAL", "", 1, "MINION"));
    cards.emplace("Damaged Golem (Golden)", damaged_golem_gold);
    // cards.emplace("Damaged Golem (Golden)", BgBaseCard(4, "NEUTRAL", 1, 2,
    // 						       "Damaged Golem (Golden)", "", "MECHANICAL", "", 1, "MINION"));
    // cards.emplace("Deck Swabbie", BgBaseCard(2, "NEUTRAL", 3, 2, "Deck Swabbie",
    // 					     "['BATTLECRY']", "PIRATE", "", 1, "MINION"));
    // cards.emplace("Deck Swabbie (Golden)", BgBaseCard(4, "NEUTRAL", 3, 4, "Deck Swabbie (Golden)",
    // 						      "['BATTLECRY']", "PIRATE", "", 1, "MINION"));
    // cards.emplace("Defender of Argus (Golden)", BgBaseCard(4, "NEUTRAL", 4, 6, "Defender of Argus (Golden)",
    // 							   "['BATTLECRY']", "", "RARE", 4, "MINION"));
    std::shared_ptr<BgBaseCard> deflecto(new BgBaseCard(3, "NEUTRAL", 4, 2, "Deflect-o-Bot (Golden)",
							"['TRIGGER_VISUAL']", "MECHANICAL", "", 3, "MINION"));
    deflecto->set_divine_shield();
    cards.emplace("Deflect-o-Bot", deflecto);
    // cards.emplace("Deflect-o-Bot (Golden)", BgBaseCard(6, "NEUTRAL", 4, 4, "Deflect-o-Bot (Golden)",
    // 						       "['TRIGGER_VISUAL']", "MECHANICAL", "", 3, "MINION"));
    // cards.emplace("Drakonid Enforcer", BgBaseCard(3, "NEUTRAL", 6, 6, "Drakonid Enforcer",
    // 						  "['TRIGGER_VISUAL']", "DRAGON", "", 4, "MINION"));
    // cards.emplace("Drakonid Enforcer (Golden)", BgBaseCard(6, "NEUTRAL", 6, 12, "Drakonid Enforcer (Golden)",
    // 							   "['TRIGGER_VISUAL']", "DRAGON", "", 4, "MINION"));
    // cards.emplace("Dragonspawn Lieutenant", BgBaseCard(2, "NEUTRAL", 2, 3, "Dragonspawn Lieutenant",
    // 						       "['TAUNT']", "DRAGON", "", 1, "MINION"));
    // cards.emplace("Dread Admiral Eliza", BgBaseCard(6, "NEUTRAL", 6, 7, "Dread Admiral Eliza",
    // 						    "['TRIGGER_VISUAL']", "PIRATE", "", 6, "MINION"));
    // cards.emplace("Dread Admiral Eliza (Golden)", BgBaseCard(12, "NEUTRAL", 6, 14, "Dread Admiral Eliza (Golden)",
    // 							     "['TRIGGER_VISUAL']", "PIRATE", "", 6, "MINION"));

    // E
    std::shared_ptr<BgBaseCard> effigy(new BgBaseCard(-1, "MAGE", 3, -1, "Effigy",
    						    "['SECRET']", "", "RARE", -1, "SPELL"));
    cards.emplace("Effigy", effigy);
    // cards.emplace("Effigy", BgBaseCard(-1, "MAGE", 3, -1, "Effigy",
    // 				       "['SECRET']", "", "RARE", -1, "SPELL"));

    // F
    // cards.emplace("Felfin Navigator (Golden)", BgBaseCard(8, "NEUTRAL", 4, 8, "Felfin Navigator (Golden)",
    // 							  "['BATTLECRY']", "MURLOC", "COMMON", 3, "MINION"));
    // std::shared_ptr<FiendishServant> fiendish_servant(new FiendishServant());
    // cards.emplace("Fiendish Servant", fiendish_servant);
    cards.emplace("Fiendish Servant", std::make_shared<FiendishServant>());
    cards.emplace("Fiendish Servant (Golden)",
		  std::make_shared<FiendishServantGolden>());
    // cards.emplace("Fiendish Servant", std::make_shared<FiendishServant>());
    // cards.emplace("Fiendish Servant (Golden)", BgBaseCard(4, "WARLOCK", 1, 2, "Fiendish Servant (Golden)",
    // 							  "['DEATHRATTLE']", "DEMON", "COMMON", 1, "MINION"));
    // cards.emplace("Floating Watcher (Golden)", BgBaseCard(8, "WARLOCK", 5, 8, "Floating Watcher (Golden)",
    // 							  "['TRIGGER_VISUAL']", "DEMON", "COMMON", 4, "MINION"));
    std::shared_ptr<BgBaseCard> foe_reaper(new BgBaseCard(6, "NEUTRAL", 8, 9, "Foe Reaper 4000",
							  "['CLEAVE']", "MECHANICAL", "LEGENDARY", 6, "Minion"));
    cards.emplace("Foe Reaper 4000", foe_reaper);
    // cards.emplace("Foe Reaper 4000", BgBaseCard(6, "NEUTRAL", 8, 9, "Foe Reaper 4000",
    // 						"['CLEAVE']", "MECHANICAL", "LEGENDARY", 6, "Minion"));
    std::shared_ptr<BgBaseCard> gambler(new BgBaseCard(3, "NEUTRAL", 3, 3, "Freedealing Gambler",
						       "", "PIRATE", "", 2, "MINION"));
    cards.emplace("Freedealing Gambler", gambler);
    // cards.emplace("Freedealing Gambler", BgBaseCard(3, "NEUTRAL", 3, 3, "Freedealing Gambler",
    // 						    "", "PIRATE", "", 2, "MINION"));
    std::shared_ptr<BgBaseCard> gambler_golden(new BgBaseCard(6, "NEUTRAL", 3, 6, "Freedealing Gambler (Golden)",
							      "", "PIRATE", "", 2, "MINION"));
    cards.emplace("Freedealing Gambler (Golden)", gambler_golden);
    // cards.emplace("Freedealing Gambler (Golden)", BgBaseCard(6, "NEUTRAL", 3, 6, "Freedealing Gambler (Golden)",
    // 							     "", "PIRATE", "", 2, "MINION"));
						
    // G
    // cards.emplace("Gentle Megasaur", BgBaseCard(5, "NEUTRAL", 4, 4, "Gentle Megasaur",
    // 						"['BATTLECRY']", "BEAST", "EPIC", 6, "MINION"));
    // cards.emplace("Gentle Megasaur (Golden)", BgBaseCard(10, "NEUTRAL", 4, 8, "Gentle Megasaur (Golden)",
    // 							 "['BATTLECRY']", "BEAST", "EPIC", 6, "MINION"));
    // cards.emplace("Ghastcoiler", BgBaseCard(7, "PRIEST", 6, 7, "Ghastcoiler",
    // 					    "['DEATHRATTLE']", "BEAST", "", 6, "MINION"));
    // cards.emplace("Ghastcoiler (Golden)", BgBaseCard(14, "PRIEST", 6, 14, "Ghastcoiler (Golden)",
    // 						     "['DEATHRATTLE']", "BEAST", "", 6, "MINION"));
    // cards.emplace("Glyph Guardian", BgBaseCard(2, "MAGE", 3, 4, "Glyph Guardian",
    // 					       "['TRIGGER_VISUAL']", "DRAGON", "", 2, "MINION"));
    // cards.emplace("Glyph Guardian (Golden)", BgBaseCard(4, "MAGE", 3, 8, "Glyph Guardian (Golden)",
    // 							"['TRIGGER_VISUAL']", "DRAGON", "", 2, "MINION"));
    // cards.emplace("Gold Coin", BgBaseCard(-1, "NEUTRAL", 0, -1, "Gold Coin",
    // 					  "", "", "RARE", -1, "SPELL"));
    // cards.emplace("Goldgrubber", BgBaseCard(2, "NEUTRAL", 5, 2, "Goldgrubber",
    // 					    "['TRIGGER_VISUAL']", "PIRATE", "", 4,  "MINION"));
    std::shared_ptr<BgBaseCard> goldgrubber_gold(new BgBaseCard(4, "NEUTRAL", 5, 4, "Goldgrubber (Golden)",
								"['TRIGGER_VISUAL']", "PIRATE", "", 4, "MINION"));
    cards.emplace("Goldgrubber (Golden)", goldgrubber_gold);
    // cards.emplace("Goldgrubber (Golden)", BgBaseCard(4, "NEUTRAL", 5, 4, "Goldgrubber (Golden)",
    // 						     "['TRIGGER_VISUAL']", "PIRATE", "", 4,
    // 						     "MINION"));
    std::shared_ptr<BgBaseCard> goldrinn(new BgBaseCard(4, "NEUTRAL", 8, 4, "Goldrinn, the Great Wolf",
    							 "['DEATHRATTLE']", "BEAST", "LEGENDARY", 6, "MINION"));
    cards.emplace("Goldrinn, the Great Wolf", goldrinn);
    // cards.emplace("Goldrinn, the Great Wolf", BgBaseCard(4, "NEUTRAL", 8, 4, "Goldrinn, the Great Wolf",
    // 							 "['DEATHRATTLE']", "BEAST", "LEGENDARY", 6, "MINION"));
    std::shared_ptr<BgBaseCard> goldrinn_gold(new BgBaseCard(8, "NEUTRAL", 8, 8, "Goldrinn, the Great Wolf (Golden)",
    							 "['DEATHRATTLE']", "BEAST", "LEGENDARY", 6, "MINION"));
    cards.emplace("Goldrinn, the Great Wolf (Golden)", goldrinn_gold);
    // cards.emplace("Goldrinn, the Great Wolf (Golden)", BgBaseCard(8, "NEUTRAL", 8, 8, "Goldrinn, the Great Wolf (Golden)",
    // 							 "['DEATHRATTLE']", "BEAST", "LEGENDARY", 6, "MINION"));
    // cards.emplace("Guard Bot (Golden)", BgBaseCard(4, "WARRIOR", 2, 6, "Guard Bot (Golden)",
    // 						   "['TAUNT']", "MECHANICAL", "", 1, "MINION"));

    // H
    // cards.emplace("Hand of Salvation", BgBaseCard(-1, "PALADIN", 1, -1, "Hand of Salvation",
    // 						  "['SECRET']", "", "FREE", -1, "SPELL"));
    // cards.emplace("Hangry Dragon", BgBaseCard(4, "NEUTRAL", 5, 4, "Hangry Dragon",
    // 					      "['TRIGGER_VISUAL']", "DRAGON", "", 3, "MINION"));
    // cards.emplace("Hangry Dragon (Golden)", BgBaseCard(8, "NEUTRAL", 5, 8, "Hangry Dragon (Golden)",
    // 						       "['TRIGGER_VISUAL']", "DRAGON", "", 3, "MINION"));
    cards.emplace("Harvest Golem", std::make_shared<HarvestGolem>());
    cards.emplace("Harvest Golem (Golden)", std::make_shared<HarvestGolemGolden>());
    // cards.emplace("Harvest Golem (Golden)", BgBaseCard(4, "NEUTRAL", 3, 6, "Harvest Golem (Golden)",
    // 						       "['DEATHRATTLE']", "MECHANICAL", "COMMON", 2, "MINION"));
    // cards.emplace("Herald of Flame", BgBaseCard(5, "WARRIOR", 5, 6, "Herald of Flame",
    // 						"['OVERKILL']", "DRAGON", "", 4, "MINION"));
    // cards.emplace("Herald of Flame (Golden)", BgBaseCard(10, "WARRIOR", 5, 12, "Herald of Flame (Golden)",
    // 							 "['OVERKILL']", "DRAGON", "", 4, "MINION"));
    // cards.emplace("Holy Mackerel", BgBaseCard(8, "PALADIN", 8, 4, "Holy Mackerel",
    // 					      "['TRIGGER_VISUAL']", "MURLOC", "", 6, "MINION"));
    std::shared_ptr<BgBaseCard> houndmaster(new BgBaseCard(4, "HUNTER", 4, 3, "Houndmaster",
							   "['BATTLECRY']", "", "FREE", 3, "MINION"));
    cards.emplace("Houndmaster", houndmaster);
    std::shared_ptr<BgBaseCard> houndmaster_gold(new BgBaseCard(8, "HUNTER", 4, 6, "Houndmaster (Golden)",
								"['BATTLECRY']", "", "FREE", 3, "MINION"));
    cards.emplace("Houndmaster (Golden)", houndmaster_gold);
    // cards.emplace("Houndmaster (Golden)", BgBaseCard(8, "HUNTER", 4, 6, "Houndmaster (Golden)",
    // 						     "['BATTLECRY']", "", "FREE", 3, "MINION"));
    // cards.emplace("Hyena (Golden)", BgBaseCard(4, "HUNTER", 2, 4, "Hyena (Golden)",
    // 					       "", "BEAST", "", 1,"MINION"));

    // I
    // cards.emplace("Ice Block", BgBaseCard(-1, "MAGE", 3, -1, "Ice Block",
    // 					  "['SECRET']", "", "EPIC", -1, "SPELL"));
    // cards.emplace("Infested Wolf (Golden)", BgBaseCard(6, "HUNTER", 4, 6, "Infested Wolf (Golden)",
    // 						       "['DEATHRATTLE']", "BEAST", "RARE", 3, "MINION"));
    std::shared_ptr<BgBaseCard> imp(new BgBaseCard(1, "WARLOCK", 1, 1, "Imp",
						   "", "DEMON", "", 1, "MINION"));
    cards.emplace("Imp", imp);
    std::shared_ptr<BgBaseCard> imp_gold(new BgBaseCard(2, "WARLOCK", 1, 2, "Imp (Golden)",
    					     "", "DEMON", "", 1, "MINION"));
    cards.emplace("Imp (Golden)", imp_gold);
    // cards.emplace("Imp Gang Boss (Golden)", BgBaseCard(4, "WARLOCK", 3, 8, "Imp Gang Boss (Golden)",
    // 						       "['TRIGGER_VISUAL']", "DEMON", "COMMON", 3, "MINION"));
    // cards.emplace("Imp Mama", BgBaseCard(6, "WARLOCK", 8, 10, "Imp Mama",
    // 					 "['TRIGGER_VISUAL']", "DEMON", "", 6, "MINION"));
    // cards.emplace("Imp Mama (Golden)", BgBaseCard(12, "WARLOCK", 8, 20, "Imp Mama (Golden)",
    // 						  "['TRIGGER_VISUAL']", "DEMON", "", 6, "MINION"));
    // cards.emplace("Imprisoner", BgBaseCard(3, "NEUTRAL", 3, 3, "Imprisoner",
    // 					   "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION"));
    cards.emplace("Imprisoner", std::make_shared<Imprisoner>());
    cards.emplace("Imprisoner (Golden)", std::make_shared<ImprisonerGolden>());
    // cards.emplace("Imprisoner (Golden)", BgBaseCard(6, "NEUTRAL", 3, 6, "Imprisoner (Golden)",
    // 						    "['DEATHRATTLE', 'TAUNT']", "DEMON", "RARE", 2, "MINION"));
    // cards.emplace("Iron Sensei (Golden)", BgBaseCard(4, "ROGUE", 3, 4, "Iron Sensei (Golden)",
    // 						     "['TRIGGER_VISUAL']", "MECHANICAL", "RARE", 4, "MINION"));
    // cards.emplace("Ironhide Direhorn (Golden)", BgBaseCard(14, "DRUID", 7, 14,  "Ironhide Direhorn (Golden)",
    // 							   "['OVERKILL']", "BEAST", "COMMON", 5, "MINION"));
    // cards.emplace("Ironhide Runt (Golden)", BgBaseCard(10, "DRUID", 5, 10, "Ironhide Runt (Golden)",
    // 						       "", "BEAST", "", 1, "MINION"));

    // J
    std::shared_ptr<BgBaseCard> joebot(new BgBaseCard(1, "NEUTRAL", 1, 1, "Jo-E Bot",
						      "", "MECHANICAL", "", 1, "MINION"));
    cards.emplace("Jo-E Bot", joebot);
    std::shared_ptr<BgBaseCard> joebot_gold(new BgBaseCard(2, "NEUTRAL", 1, 2, "Jo-E Bot (Golden)",
						      "", "MECHANICAL", "", 1, "MINION"));
    cards.emplace("Jo-E Bot (Golden)", joebot_gold);
    // cards.emplace("Junkbot (Golden)", BgBaseCard(2, "NEUTRAL", 5, 10, "Junkbot (Golden)",
    // 						 "['TRIGGER_VISUAL']", "MECHANICAL", "EPIC", 5, "MINION"));

    // K
    cards.emplace("Kaboom Bot", std::make_shared<KaboomBot>());
    cards.emplace("Kaboom Bot (Golden)", std::make_shared<KaboomBotGolden>());
    // cards.emplace("Kalecgos", BgBaseCard(4, "NEUTRAL", 8, 12, "Kalecgos, Arcane Aspect",
    // 					 "['TRIGGER_VISUAL']", "DRAGON", "", 6, "MINION"));
    // cards.emplace("Kalecgos (Golden)", BgBaseCard(8, "NEUTRAL", 8, 24, "Kalecgos, Arcane Aspect (Golden)",
    // 						  "['TRIGGER_VISUAL']", "DRAGON", "", 6, "MINION"));
    // cards.emplace("Kangor's Apprentice", BgBaseCard(3, "NEUTRAL", 9, 6, "Kangor's Apprentice",
    // 						    "['DEATHRATTLE']", "", "EPIC", 6, "MINION"));
    // cards.emplace("Kangor's Apprentice (Golden)", BgBaseCard(6, "NEUTRAL", 9, 12, "Kangor's Apprentice (Golden)",
    // 							     "['DEATHRATTLE']", "", "EPIC", 6,"MINION"));
    // cards.emplace("Khadgar (Golden)", BgBaseCard(4, "MAGE", 2, 4,  "Khadgar (Golden)",
    // 						 "['TRIGGER_VISUAL']", "", "LEGENDARY", 3, "MINION"));
    cards.emplace("Kindly Grandmother", std::make_shared<KindlyGrandmother>());
    cards.emplace("Kindly Grandmother (Golden)", std::make_shared<KindlyGrandmotherGolden>());
    // cards.emplace("Kindly Grandmother (Golden)", BgBaseCard(2, "HUNTER", 2, 2, "Kindly Grandmother (Golden)",
    // 							    "['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION"));
    // cards.emplace("King Bagurgle", BgBaseCard(6, "NEUTRAL", 6, 3, "King Bagurgle",
    // 					      "['BATTLECRY', 'DEATHRATTLE']", "MURLOC", "LEGENDARY", 5, "MINION"));
    // cards.emplace("King Bagurgle (Golden)", BgBaseCard(12, "NEUTRAL", 6, 6, "King Bagurgle (Golden)",
    // 						       "['BATTLECRY', 'DEATHRATTLE']", "MURLOC", "LEGENDARY", 5, "MINION"));

    // L
    // cards.emplace("Lightfang", BgBaseCard(2, "NEUTRAL", 6, 2, "Lightfang Enforcer",
    // 					  "['TRIGGER_VISUAL']", "", "EPIC", 5, "MINION"));
    // cards.emplace("Lightfang Enforcer (Golden)", BgBaseCard(4, "NEUTRAL", 6, 4, "Lightfang Enforcer (Golden)",
    // 							    "['TRIGGER_VISUAL']", "", "EPIC", 5, "MINION"));

    // M
    std::shared_ptr<BgBaseCard> malganis_gold(new BgBaseCard(18, "WARLOCK", 9, 14, "Mal'Ganis (Golden)",
							     "['AURA']", "DEMON", "LEGENDARY", 5, "MINION"));
    cards.emplace("Mal'Ganis (Golden)", malganis_gold);
    // cards.emplace("Mal'Ganis (Golden)", BgBaseCard(18, "WARLOCK", 9, 14, "Mal'Ganis (Golden)",
    // 						   "['AURA']", "DEMON", "LEGENDARY", 5, "MINION"));
    // cards.emplace("Mama Bear", BgBaseCard(5, "NEUTRAL", 8, 5, "Mama Bear",
    // 					  "['TRIGGER_VISUAL']", "BEAST", "EPIC", 6, "MINION"));
    // cards.emplace("Mama Bear (Golden)", BgBaseCard(10, "NEUTRAL", 8, 10, "Mama Bear (Golden)",
    // 						   "['TRIGGER_VISUAL']", "BEAST", "EPIC", 6, "MINION"));
    // // cards.emplace("Master Demonologist", BgBaseCard(4, "NEUTRAL", 7, 6, "Master Demonologist",
    // // 						    "['TRIGGER_VISUAL']", "DEMON", "", -1, "MINION"));
    // cards.emplace("Mechano-Egg (Golden)", BgBaseCard(0, "PALADIN", 5, 10, "Mechano-Egg (Golden)",
    // 						     "['DEATHRATTLE']", "MECHANICAL", "COMMON", 4, "MINION"));
    cards.emplace("Mecharoo", std::make_shared<Mecharoo>());
    cards.emplace("Mecharoo (Golden)", std::make_shared<MecharooGolden>());
    // cards.emplace("Mecharoo (Golden)", BgBaseCard(2, "NEUTRAL", 1, 2, "Mecharoo (Golden)",
    // 						  "['DEATHRATTLE']", "MECHANICAL", "COMMON", 1, "MINION"));
    // cards.emplace("Menagerie Magician (Golden)", BgBaseCard(8, "NEUTRAL", 5, 8, "Menagerie Magician (Golden)",
    // 							    "['BATTLECRY']", "", "COMMON", 4, "MINION"));
    // cards.emplace("Metaltooth Leaper (Golden)", BgBaseCard(6, "HUNTER", 3, 6, "Metaltooth Leaper (Golden)",
    // 							   "['BATTLECRY']", "MECHANICAL", "RARE", 2, "MINION"));
    // cards.emplace("Micro Machine", BgBaseCard(1, "NEUTRAL", 2, 2, "Micro Machine",
    // 					      "['TRIGGER_VISUAL']", "MECHANICAL", "COMMON", 1, "MINION"));
    std::shared_ptr<BgBaseCard> micro_machine_gold(new BgBaseCard(2, "NEUTRAL", 2, 4, "Micro Machine (Golden)",
								  "['TRIGGER_VISUAL']", "MECHANICAL", "COMMON", 1, "MINION"));
    cards.emplace("Micro Machine (Golden)", micro_machine_gold);
    // cards.emplace("Micro Machine (Golden)", BgBaseCard(2, "NEUTRAL", 2, 4, "Micro Machine (Golden)",
    // 						       "['TRIGGER_VISUAL']", "MECHANICAL", "COMMON", 1, "MINION"));
    // cards.emplace("Microbot (Golden)", BgBaseCard(2, "NEUTRAL", 1, 2, "Microbot (Golden)",
    // 						  "", "MECHANICAL", "", 1, "MINION"));
    std::shared_ptr<BgBaseCard> macaw(new BgBaseCard(3, "NEUTRAL", 3, 2, "Monstrous Macaw",
    						     "['TRIGGER_VISUAL']", "BEAST", "", 3, "MINION"));
    cards.emplace("Monstrous Macaw" , macaw);
    // cards.emplace("Monstrous Macaw", BgBaseCard(3, "NEUTRAL", 3, 2, "Monstrous Macaw",
    // 						"['TRIGGER_VISUAL']", "BEAST", "", 3, "MINION"));
    std::shared_ptr<BgBaseCard> macaw_gold(new BgBaseCard(6, "NEUTRAL", 3, 4, "Monstrous Macaw (Golden)",
    							  "['TRIGGER_VISUAL']", "BEAST", "", 3, "MINION"));
    cards.emplace("Monstrous Macaw (Golden)", macaw_gold);
    // cards.emplace("Monstrous Macaw (Golden)", BgBaseCard(6, "NEUTRAL", 3, 4, "Monstrous Macaw (Golden)",
    // 							 "['TRIGGER_VISUAL']", "BEAST", "", 3, "MINION"));    
    // cards.emplace("Mounted Raptor (Golden)", BgBaseCard(6, "DRUID", 3, 4, "Mounted Raptor (Golden)",
    // 							"['DEATHRATTLE']", "BEAST", "COMMON", 2, "MINION"));
    std::shared_ptr<BgBaseCard> murloc_scout(new BgBaseCard(1, "NEUTRAL", 1, 1, "Murloc Scout",
							    "", "MURLOC", "COMMON", 1, "MINION"));
    cards.emplace("Murloc Scout", murloc_scout);
    std::shared_ptr<BgBaseCard> murloc_scout_gold(new BgBaseCard(2, "NEUTRAL", 1, 2, "Murloc Scout (Golden)",
								 "", "MURLOC", "COMMON", 1, "MINION"));
    cards.emplace("Murloc Scout (Golden)", murloc_scout_gold);
    std::shared_ptr<BgBaseCard> tidecaller_gold(new BgBaseCard(2, "NEUTRAL", 1, 4, "Murloc Tidecaller (Golden)",
							       "['TRIGGER_VISUAL']", "MURLOC", "RARE", 1, "MINION"));
    cards.emplace("Murloc Tidecaller (Golden)", tidecaller_gold);
    // cards.emplace("Murloc Tidecaller (Golden)", BgBaseCard(2, "NEUTRAL", 1, 4, "Murloc Tidecaller (Golden)",
    // 							   "['TRIGGER_VISUAL']", "MURLOC", "RARE", 1, "MINION"));
    std::shared_ptr<BgBaseCard> tidehunter(new BgBaseCard(2, "NEUTRAL", 2, 1, "Murloc Tidehunter",
							  "['BATTLECRY']", "MURLOC", "FREE", 1, "MINION"));
    cards.emplace("Murloc Tidehunter", tidehunter);
    std::shared_ptr<BgBaseCard> tidehunter_gold(new BgBaseCard(4, "NEUTRAL", 2, 2, "Murloc Tidehunter (Golden)",
							       "['BATTLECRY']", "MURLOC", "FREE", 1, "MINION"));
    cards.emplace("Murloc Tidehunter (Golden)", tidehunter_gold);
    // cards.emplace("Murloc Tidehunter (Golden)", BgBaseCard(4, "NEUTRAL", 2, 2, "Murloc Tidehunter (Golden)",
    // 							  "['BATTLECRY']", "MURLOC", "FREE", 1, "MINION"));
    // cards.emplace("Murloc Warleader (Golden)", BgBaseCard(6, "NEUTRAL", 3, 6, "Murloc Warleader (Golden)",
    // 							  "['AURA']", "MURLOC", "EPIC", 2, "MINION"));
    // cards.emplace("Murozond", BgBaseCard(5, "NEUTRAL", 7, 5, "Murozond",
    // 					 "['BATTLECRY']", "DRAGON", "", 5, "MINION"));
    // cards.emplace("Murozond (Golden)", BgBaseCard(10, "NEUTRAL", 7, 10, "Murozond (Golden)",
    // 						  "['BATTLECRY']", "DRAGON", "", 5, "MINION"));
							   
    // N
    // cards.emplace("Nadina the Red", BgBaseCard(7, "NEUTRAL", 6, 4, "Nadina the Red",
    // 					       "['DEATHRATTLE']", "", "", 6, "MINION"));    
    // cards.emplace("Nat Pagle, Extreme Angler", BgBaseCard(8, "NEUTRAL", 7, 5, "Nat Pagle, Extreme Angler",
    // 							  "", "PIRATE", "", 5, "MINION"));
    // cards.emplace("Nat Pagle, Extreme Angler (Golden)", BgBaseCard(16, "NEUTRAL", 7, 10, "Nat Pagle, Extreme Angler (Golden)",
    // 								   "", "PIRATE", "", 5, "MINION"));
    // cards.emplace("Nathrezim Overseer", BgBaseCard(2, "NEUTRAL", 3, 3, "Nathrezim Overseer",
    // 						   "['BATTLECRY']", "DEMON", "RARE", 2, "MINION"));
    // cards.emplace("Nathrezim Overseer (Golden)", BgBaseCard(4, "NEUTRAL", 3, 6, "Nathrezim Overseer (Golden)",
    // 							    "['BATTLECRY']", "DEMON", "RARE", 2, "MINION"));

    // O
    // cards.emplace("Old Murk-Eye (Golden)", BgBaseCard(4, "NEUTRAL", 4, 8, "Old Murk-Eye (Golden)",
    // 						      "['CHARGE']", "MURLOC", "LEGENDARY", 2, "MINION"));

    // P
    // cards.emplace("Pack Leader (Golden)", BgBaseCard(6, "NEUTRAL", 3, 6, "Pack Leader (Golden)",
    // 						     "['TRIGGER_VISUAL']", "", "RARE", 3, "MINION"));
    // cards.emplace("Piloted Shredder", BgBaseCard(4, "NEUTRAL", 4, 3, "Piloted Shredder",
    // 	                                         "['DEATHRATTLE']", "MECHANICAL", "COMMON", 3, "MINION"));
    // cards.emplace("Piloted Shredder (Golden)", BgBaseCard(8, "NEUTRAL", 4, 6, "Piloted Shredder (Golden)",
    // 							  "['DEATHRATTLE']", "MECHANICAL", "COMMON", 3, "MINION"));
    // cards.emplace("Pogo-Hopper", BgBaseCard(1, "ROGUE", 1, 1, "Pogo-Hopper",
    // 					    "['BATTLECRY']", "MECHANICAL", "RARE", 2, "MINION"));
    // cards.emplace("Pogo-Hopper (Golden)", BgBaseCard(2, "ROGUE", 1, 2, "Pogo-Hopper (Golden)",
    // 						     "['BATTLECRY']", "MECHANICAL", "RARE", 2, "MINION"));
    // cards.emplace("Primalfin Lookout", BgBaseCard(3, "NEUTRAL", 3, 2, "Primalfin Lookout",
    // 						  "['BATTLECRY', 'DISCOVER']", "MURLOC", "COMMON", 5, "MINION"));
    // cards.emplace("Primalfin Lookout (Golden)", BgBaseCard(6, "NEUTRAL", 3, 4, "Primalfin Lookout (Golden)",
    // 							   "['BATTLECRY', 'DISCOVER']", "MURLOC", "COMMON", 5, "MINION"));

    // R
    // cards.emplace("Rabid Saurolisk", BgBaseCard(3, "HUNTER", 3, 2, "Rabid Saurolisk",
    // 						"['TRIGGER_VISUAL']", "BEAST", "", 2, "MINION"));
    // cards.emplace("Rabid Saurolisk (Golden)", BgBaseCard(6, "HUNTER", 3, 4, "Rabid Saurolisk (Golden)",
    // 							 "['TRIGGER_VISUAL']", "BEAST", "", 2, "MINION"));
    std::shared_ptr<BgBaseCard> rat(new BgBaseCard(1, "HUNTER", 1, 1, "Rat",
						   "", "BEAST", "", 1, "MINION"));
    cards.emplace("Rat", rat);
    std::shared_ptr<BgBaseCard> rat_gold(new BgBaseCard(2, "HUNTER", 1, 2, "Rat (Golden)",
							"", "BEAST", "", 1, "MINION"));
    cards.emplace("Rat (Golden)", rat_gold);
    cards.emplace("Rat Pack", std::make_shared<RatPack>());
    cards.emplace("Rat Pack (Golden)", std::make_shared<RatPackGolden>());
    // cards.emplace("Rat Pack (Golden)", BgBaseCard(4, "HUNTER", 3, 4, "Rat Pack (Golden)",
    // 						  "['DEATHRATTLE']", "BEAST", "EPIC", 2, "MINION"));
    std::shared_ptr<BgBaseCard> razorgore(new BgBaseCard(2, "NEUTRAL", 8, 4, "Razorgore, the Untamed",
							 "['TRIGGER_VISUAL']", "DRAGON", "", 5, "MINION"));
    cards.emplace("Razorgore", razorgore);
    // cards.emplace("Razorgore", BgBaseCard(2, "NEUTRAL", 8, 4, "Razorgore, the Untamed",
    // 						       "['TRIGGER_VISUAL']", "DRAGON", "", 5, "MINION"));
    // cards.emplace("Razorgore (Golden)", BgBaseCard(4, "NEUTRAL", 8, 8, "Razorgore, the Untamed (Golden)",
    // 						   "['TRIGGER_VISUAL']", "DRAGON", "", 5, "MINION"));
    // cards.emplace("Red Whelp", BgBaseCard(1, "NEUTRAL", 1, 2, "Red Whelp",
    // 					  "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION"));
    // cards.emplace("Red Whelp (Golden)", BgBaseCard(2, "NEUTRAL", 1, 4, "Red Whelp (Golden)",
    // 						   "['TRIGGER_VISUAL']", "DRAGON", "", 1, "MINION"));
    // cards.emplace("Redemption", BgBaseCard(-1, "PALADIN", 1, -1, "Redemption",
    // 					   "['SECRET']", "", "COMMON", -1, "SPELL"));
    // cards.emplace("Replicating Menace (Golden)", BgBaseCard(6, "NEUTRAL", 4, 2, "Replicating Menace (Golden)",
    // 							    "['DEATHRATTLE', 'MODULAR']", "MECHANICAL", "RARE", 3, "MINION"));
    // cards.emplace("Ripsnarl Captain", BgBaseCard(3, "NEUTRAL", 4, 4, "Ripsnarl Captain",
    // 						 "['TRIGGER_VISUAL']", "PIRATE", "", 4, "MINION"));
    // cards.emplace("Ripsnarl Captain (Golden)", BgBaseCard(6, "NEUTRAL", 4, 8, "Ripsnarl Captain (Golden)",
    // 							  "['TRIGGER_VISUAL']", "PIRATE", "", 4, "MINION"));
    // cards.emplace("Robosaur (Golden)", BgBaseCard(16, "PALADIN", 8, 16, "Robosaur (Golden)",
    // 						  "", "MECHANICAL", "", 1, "MINION"));
    // cards.emplace("Rockpool Hunter (Golden)", BgBaseCard(4, "NEUTRAL", 2, 6, "Rockpool Hunter (Golden)",
    // 							 "['BATTLECRY']", "MURLOC", "COMMON", 1, "MINION"));

    // S
    // cards.emplace("Salty Looter", BgBaseCard(3, "ROGUE", 4, 3, "Salty Looter",
    // 					     "['TRIGGER_VISUAL']", "PIRATE", "", 3, "MINION"));
    // cards.emplace("Salty Looter (Golden)", BgBaseCard(6, "ROGUE", 4, 6, "Salty Looter (Golden)",
    // 						      "['TRIGGER_VISUAL']", "PIRATE", "", 3, "MINION"));
    // cards.emplace("Savannah Highmane (Golden)", BgBaseCard(12, "HUNTER", 6, 10, "Savannah Highmane (Golden)",
    // 							   "['DEATHRATTLE']", "BEAST", "RARE", 4,"MINION"));
    cards.emplace("Scallywag", std::make_shared<Scallywag>());
    cards.emplace("Scallywag (Golden)", std::make_shared<ScallywagGolden>());
    // cards.emplace("Scallywag", BgBaseCard(2, "NEUTRAL", 1, 1, "Scallywag",
    // 					  "['DEATHRATTLE']", "PIRATE", "", 1, "MINION"));
    // cards.emplace("Scallywag (Golden)", BgBaseCard(4, "NEUTRAL", 1, 2, "Scallywag (Golden)",
    // 						   "['DEATHRATTLE']", "PIRATE", "", 1, "MINION"));
    // cards.emplace("Scavenging Hyena (Golden)", BgBaseCard(4, "HUNTER", 2, 4, "Scavenging Hyena (Golden)",
    // 							  "['TRIGGER_VISUAL']", "BEAST", "COMMON", 1, "MINION"));
    // cards.emplace("Screwjank Clunker (Golden)", BgBaseCard(4, "WARRIOR", 4, 10, "Screwjank Clunker (Golden)",
    // 							   "['BATTLECRY']", "MECHANICAL", "RARE", 3, "MINION"));
    // cards.emplace("Seabreaker Goliath", BgBaseCard(6, "NEUTRAL", 7, 7, "Seabreaker Goliath",
    // 						   "['OVERKILL']", "PIRATE", "", 5, "MINION"));
    // cards.emplace("Seabreaker Goliath (Golden)", BgBaseCard(12, "NEUTRAL", 7, 14, "Seabreaker Goliath (Golden)",
    // 							    "['OVERKILL']", "PIRATE", "", 5, "MINION"));
    // cards.emplace("Security Rover (Golden)", BgBaseCard(4, "WARRIOR", 6, 12, "Security Rover (Golden)",
    // 							"['TRIGGER_VISUAL']", "MECHANICAL", "RARE", 4, "MINION"));
    cards.emplace("Selfless Hero", std::make_shared<SelflessHero>());
    cards.emplace("Selfless Hero (Golden)", std::make_shared<SelflessHeroGolden>());
    // cards.emplace("Shifter Zerus", BgBaseCard(1, "NEUTRAL", 1, 1, "Shifter Zerus",
    // 					      "", "", "LEGENDARY", 3, "MINION"));
    // cards.emplace("Shifter Zerus (Golden)", BgBaseCard(2, "NEUTRAL", 1, 2, "Shifter Zerus (Golden)",
    // 						       "", "", "LEGENDARY", 3,"MINION"));
    // cards.emplace("Siegebreaker (Golden)", BgBaseCard(10, "WARLOCK", 7, 16,  "Siegebreaker (Golden)",
    // 						      "['AURA', 'TAUNT']", "DEMON", "RARE", 4, "MINION"));
    std::shared_ptr<BgBaseCard> sky_pirate(new BgBaseCard(1, "ROGUE", 1, 1, "Sky Pirate",
							  "", "PIRATE", "", 1, "MINION"));
    cards.emplace("Sky Pirate", sky_pirate);
    std::shared_ptr<BgBaseCard> sky_pirate_gold(new BgBaseCard(2, "ROGUE", 1, 2, "Sky Pirate (Golden)",
							       "", "PIRATE", "", 1, "MINION"));
    cards.emplace("Sky Pirate (Golden)", sky_pirate_gold);
    
    // cards.emplace("Snake Trap", BgBaseCard(-1, "HUNTER", 2, -1, "Snake Trap",
    // 					   "['SECRET']", "", "EPIC", -1, "SPELL"));
    // cards.emplace("Sneed's Old Shredder", BgBaseCard(5, "NEUTRAL", 8, 7,"Sneed's Old Shredder",
    // 						     "['DEATHRATTLE']", "MECHANICAL", "LEGENDARY", 5, "MINION"));
    // cards.emplace("Sneed's Old Shredder (Golden)", BgBaseCard(10, "NEUTRAL", 8, 14, "Sneed's Old Shredder (Golden)",
    // 							      "['DEATHRATTLE']", "MECHANICAL", "LEGENDARY", 5, "MINION"));
    // cards.emplace("Soul Juggler", BgBaseCard(3, "WARLOCK", 3, 3, "Soul Juggler",
    // 					     "['TRIGGER_VISUAL']", "", "", 3, "MINION"));
    // cards.emplace("Soul Juggler (Golden)", BgBaseCard(6, "WARLOCK", 3, 6, "Soul Juggler (Golden)",
    // 						      "['TRIGGER_VISUAL']", "", "", 3, "MINION"));
    // cards.emplace("Southsea Captain (Golden)", BgBaseCard(6, "NEUTRAL", 3, 6, "Southsea Captain (Golden)",
    // 							  "['AURA']", "PIRATE", "EPIC", 2, "MINION"));
    // cards.emplace("Southsea Strongarm", BgBaseCard(5, "NEUTRAL", 5, 4,  "Southsea Strongarm",
    // 						   "", "PIRATE", "", 4,"MINION"));
    // cards.emplace("Southsea Strongarm (Golden)", BgBaseCard(10, "NEUTRAL", 5, 8, "Southsea Strongarm (Golden)",
    // 							    "", "PIRATE", "", 4, "MINION"));
    cards.emplace("Spawn of Nzoth", std::make_shared<SpawnOfNzoth>());
    cards.emplace("Spawn of Nzoth (Golden)", std::make_shared<SpawnOfNzothGolden>());
    // cards.emplace("Spawn of N'Zoth (Golden)", BgBaseCard(4, "NEUTRAL", 3, 4, "Spawn of N'Zoth (Golden)",
    // 							 "['DEATHRATTLE']", "", "COMMON", 2, "MINION"));
    // cards.emplace("Spider (Golden)", BgBaseCard(2, "HUNTER", 1, 2, "Spider (Golden)",
    // 						"", "BEAST", "", 1, "MINION"));
    // cards.emplace("Splitting Image", BgBaseCard(-1, "MAGE", 3, -1, "Splitting Image",
    // 						"['SECRET']", "", "EPIC", -1, "SPELL"));
    // cards.emplace("Steward of Time", BgBaseCard(3, "NEUTRAL", 4, 4, "Steward of Time",
    // 						"", "DRAGON", "", 2, "MINION"));
    // cards.emplace("Steward of Time (Golden)", BgBaseCard(6, "NEUTRAL", 4, 8, "Steward of Time (Golden)",
    // 							 "", "DRAGON", "", 2, "MINION"));
    // cards.emplace("Strongshell Scavenger (Golden)", BgBaseCard(4, "DRUID", 4, 6, "Strongshell Scavenger (Golden)",
    // 							       "['BATTLECRY']", "", "RARE", 5, "MINION"));



    // T
    // cards.emplace("Tabbycat (Golden)", BgBaseCard(2, "HUNTER", 1, 2, "Tabbycat (Golden)",
    // 						  "", "BEAST", "", 1, "MINION"));
    // cards.emplace("The Beast (Golden)", BgBaseCard(18, "NEUTRAL", 6, 14, "The Beast (Golden)",
    // 						   "['DEATHRATTLE']", "BEAST", "LEGENDARY", 3, "MINION"));
    // cards.emplace("The Tide Razor", BgBaseCard(6, "NEUTRAL", 7, 4, "The Tide Razor",
    // 					       "['DEATHRATTLE']", "", "", 6, "MINION"));
    // cards.emplace("Treasure Chest", BgBaseCard(0, "NEUTRAL", 2, 2, "Treasure Chest",
    // 					       "", "", "", 1, "MINION"));
    // cards.emplace("Treasure Chest (Golden)", BgBaseCard(0, "NEUTRAL", 2, 2, "Treasure Chest (Golden)",
    // 							"", "", "", 1, "MINION"));
    // cards.emplace("Twilight Emissary", BgBaseCard(4, "NEUTRAL", 6, 4, "Twilight Emissary",
    // 						  "['BATTLECRY', 'TAUNT']", "DRAGON", "", 3, "MINION"));
    // cards.emplace("Twilight Emissary (Golden)", BgBaseCard(8, "NEUTRAL", 6, 8, "Twilight Emissary (Golden)",
    // 							   "['BATTLECRY', 'TAUNT']", "DRAGON", "", 3, "MINION"));

    // U
    cards.emplace("Unstable Ghoul", std::make_shared<UnstableGhoul>());
    // cards.emplace("Unstable Ghoul (Golden)", BgBaseCard(2, "NEUTRAL", 2, 6, "Unstable Ghoul (Golden)",
    // 							"['DEATHRATTLE', 'TAUNT']", "", "COMMON", 2, "MINION"));

    // V
    // cards.emplace("Venomstrike Trap", BgBaseCard(-1, "HUNTER", 2, -1, "Venomstrike Trap",
    // 						 "['SECRET']", "", "RARE", -1, "SPELL"));
    // cards.emplace("Virmen Sensei (Golden)", BgBaseCard(8, "DRUID", 5, 10, "Virmen Sensei (Golden)",
    // 						       "['BATTLECRY']", "", "RARE", 4, "MINION"));
    // cards.emplace("Voidlord (Golden)", BgBaseCard(6, "WARLOCK", 9, 18, "Voidlord (Golden)",
    // 						  "['DEATHRATTLE', 'TAUNT']", "DEMON", "EPIC", 5, "MINION"));
    // cards.emplace("Voidwalker (Golden)", BgBaseCard(2, "WARLOCK", 1, 6, "Voidwalker (Golden)",
    // 						    "['TAUNT']", "DEMON", "FREE", 1, "MINION"));
					     

    // W
    // cards.emplace("Waxrider Togwaggle", BgBaseCard(1, "NEUTRAL", 3, 2, "Waxrider Togwaggle",
    // 						   "['TRIGGER_VISUAL']", "", "", 2, "MINION"));
    // cards.emplace("Waxrider Togwaggle (Golden)", BgBaseCard(2, "NEUTRAL", 3, 4, "Waxrider Togwaggle (Golden)",
    // 							    "['TRIGGER_VISUAL']", "", "", 2, "MINION"));
    // cards.emplace("Wrath Weaver", BgBaseCard(1, "NEUTRAL", 1, 1, "Wrath Weaver",
    // 					     "['TRIGGER_VISUAL']", "", "COMMON", 1, "MINION"));
    // cards.emplace("Wrath Weaver (Golden)", BgBaseCard(2, "NEUTRAL", 1, 2, "Wrath Weaver (Golden)",
    // 						      "['TRIGGER_VISUAL']", "", "COMMON", 1, "MINION"));

    // Y
    // cards.emplace("Yo-Ho-Ogre", BgBaseCard(2, "NEUTRAL", 6, 8, "Yo-Ho-Ogre",
    // 					   "['TRIGGER_VISUAL']", "PIRATE", "", 3, "MINION"));

    // Z
//     cards.emplace("Zapp Slywick", BgBaseCard(7, "NEUTRAL", 8, 10, "Zapp Slywick",
// 					     "", "", "", 6, "MINION"));
//     cards.emplace("Zapp Slywick (Golden)", BgBaseCard(14, "NEUTRAL", 8, 20, "Zapp Slywick (Golden)",
// 						      "['WINDFURY']", "", "", 6, "MINION"));
//     cards.emplace("Zoobot (Golden)", BgBaseCard(6, "NEUTRAL", 3, 6, "Zoobot (Golden)",
// 						"['BATTLECRY']", "MECHANICAL", "COMMON", 2, "MINION"));
}
