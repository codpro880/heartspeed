#include <algorithm>
#include <iostream>
#include <memory>

#include "BgCards.hpp"
#include "BgCardFactory.hpp"
#include "../../bg_game/board.hpp"
#include "../../bg_game/battler.hpp"
#include "../../bg_game/rng_singleton.hpp"

// Some reusable funcs
void buff_attack_end_turn(Player* p1, int attack_buff, BgBaseCard* this_) {
    if (p1->get_board()->get_cards().size() < 1) return;
    auto cards = p1->get_board()->get_cards();
    auto card_to_buff =  cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    while (card_to_buff.get() == this_) {
        card_to_buff =  cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    }
    card_to_buff->set_attack(card_to_buff->get_attack() + attack_buff);
}

void menagerie_bcry(Player* p1,
                    uint8_t buff_amount,
                    size_t num_to_buff = 7) { // Default is to buff all
    auto board = p1->get_board();
    std::unordered_set<std::string> races;
    std::vector<std::shared_ptr<BgBaseCard>> cards_to_buff;
    auto cards = board->get_cards();
    while (!cards.empty()) {
        auto rand_pos = RngSingleton::getInstance().get_rand_int() % cards.size();
        auto card = cards[rand_pos];
        cards.erase(cards.begin() + rand_pos);
        auto race = card->get_race();
        if (races.find(race) == races.end() && race != "") {
            races.insert(race);
            cards_to_buff.push_back(card);
        }
        if (races.size() == num_to_buff) {
            break;
        }
    }

    for (auto c : cards_to_buff) {
        c->set_attack(c->get_attack() + buff_amount);
        c->set_health(c->get_health() + buff_amount);
    }
}

// Generic Classes
// TODO: Efficiency
void DeathrattleCard::deathrattle(Player* p1, Player* p2) {
    auto b1 = p1->get_board().get();
    auto b2 = p2->get_board().get();
    if (b1->contains("Baron Rivendare (Golden)")) {
        do_deathrattle(p1, p2);
        do_deathrattle(p1, p2);
        do_deathrattle(p1, p2);
        for (auto c : deathrattle_cards) {
            c->set_death_pos(this->death_pos);
            c->do_deathrattle(p1, p2);
            c->do_deathrattle(p1, p2);
            c->do_deathrattle(p1, p2);
        }
    }
    else if (b1->contains("Baron Rivendare")) {
        do_deathrattle(p1, p2);
        do_deathrattle(p1, p2);
        for (auto c : deathrattle_cards) {
            c->set_death_pos(this->death_pos);
            c->do_deathrattle(p1, p2);
            c->do_deathrattle(p1, p2);
        }
    }
    else {
        do_deathrattle(p1, p2);
        for (auto c : deathrattle_cards) {
            c->set_death_pos(this->death_pos);
            c->do_deathrattle(p1, p2);
        }
    }
}

// TODO: Efficiency
void BattlecryCard::battlecry(Player* p1) {
    if (p1->get_board()->contains("Brann Bronzebeard (Golden)")) {
        do_battlecry(p1);
        do_battlecry(p1);
        do_battlecry(p1);
    }
    else if (p1->get_board()->contains("Brann Bronzebeard")) {
        do_battlecry(p1);
        do_battlecry(p1);
    }
    else {
        do_battlecry(p1);
    }
}

// TODO: Efficiency...and design issues...
void TargetedBattlecryCard::targeted_battlecry(std::shared_ptr<BgBaseCard> c, Player* p1) {
    if (!is_valid_target(c)) {
        throw std::logic_error(c->get_name() + " is an invalid target for Houndmaster.");
    }
    
    if (p1->get_board()->contains("Brann Bronzebeard (Golden)")) {
        do_targeted_battlecry(c);
        do_targeted_battlecry(c);
        do_targeted_battlecry(c);
    }
    else if (p1->get_board()->contains("Brann Bronzebeard")) {
        do_targeted_battlecry(c);
        do_targeted_battlecry(c);
    }
    else {
        do_targeted_battlecry(c);
    }
}

std::vector<int> TargetedBattlecryCard::get_valid_target_indexes(Player* p1) {
    std::vector<int> res;
    auto board_cards = p1->get_board()->get_cards();
    for (int i = 0; (unsigned)i < board_cards.size(); i++) {
        if (is_valid_target(board_cards[i])) {
            res.push_back(i);
        }
    }
    return res;
}

void PermenantBattleStatsCard::deathrattle(Player* p1, Player* p2) {
    std::cerr << "Replace drattle..." << std::endl;
    p1->replace_card_on_original_board_by_id(this);
}

void PirateCard::do_preattack(std::shared_ptr<BgBaseCard> defender,
                              Player* p1,
                              Player* p2) {
    auto b1 = p1->get_board().get();
    auto b2 = p2->get_board().get();
    int num_eliza = 0;
    int num_eliza_gold = 0;
    for (auto c : b1->get_cards()) {
        if (c->get_name() == "Dread Admiral Eliza") {
            num_eliza += 1;
        }
        else if (c->get_name() == "Dread Admiral Eliza (Golden)") {
            num_eliza_gold += 1;
        }
    }
    for (auto c : b1->get_cards()) {
        if (c->get_name() == "Ripsnarl Captain" && c.get() != this) {
            set_attack(get_attack() + 2);
            set_health(get_health() + 2);
        }
        else if (c->get_name() == "Ripsnarl Captain (Golden)" && c.get() != this) {
            set_attack(get_attack() + 4);
            set_health(get_health() + 4);
        }
    }
    auto total_buff = 2 * num_eliza_gold + num_eliza;
    for (auto c : b1->get_cards()) {
        c->set_attack(c->get_attack() + total_buff);
        c->set_health(c->get_health() + total_buff);
    }
}

// Specific Cards

void Alleycat::do_battlecry(Player* p1) {
    basic_summon(p1, true);
    p1->check_for_triples();
}

std::shared_ptr<BgBaseCard> Alleycat::summon() {
    auto f = BgCardFactory();
    return f.get_card("Tabbycat");
}

void AlleycatGolden::do_battlecry(Player* p1) {
    basic_summon(p1, true);
}

std::shared_ptr<BgBaseCard> AlleycatGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Tabbycat (Golden)");
}

void Amalgadon::do_battlecry(Player* p1) {
    std::unordered_set<std::string> races;
    for (auto c : p1->get_board()->get_cards()) {
        if (c.get() == this) continue;
        races.insert(c->get_race());
    }
    races.erase("");
    for (size_t i = 0; i < races.size(); ++i) { 
        adapt();
    }
}

void AmalgadonGolden::do_battlecry(Player* p1) {
    am.do_battlecry(p1);
    am.do_battlecry(p1);
    adapt_count += am.get_adapt_count();
}

void AnnihilanBattlemaster::do_battlecry(Player* p1) {
    set_health(get_health() + p1->get_damage_taken());
}

void AnnihilanBattlemasterGolden::do_battlecry(Player* p1) {
    set_health(get_health() + p1->get_damage_taken() * 2);
}

void ArcaneAssistant::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "ELEMENTAL") {
            card->set_attack(card->get_attack() + 1);
            card->set_health(card->get_health() + 1);
        }
    }
}

void ArcaneAssistantGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "ELEMENTAL") {
            card->set_attack(card->get_attack() + 2);
            card->set_health(card->get_health() + 2);
        }
    }
}

int Bigfernal::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (card->get_race() == "DEMON") {
        set_attack(get_attack() + 1);
        set_health(get_health() + 1);
    }
    return 0;
}

int BigfernalGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (card->get_race() == "DEMON") {
        set_attack(get_attack() + 2);
        set_health(get_health() + 2);
    }
    return 0;
}

void BloodsailCannoneer::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "PIRATE") {
            card->set_attack(card->get_attack() + 3);
        }
    }
}

void BloodsailCannoneerGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "PIRATE") {
            card->set_attack(card->get_attack() + 6);
        }
    }
}


void CapnHoggarr::card_bought_trigger(Player* p1, std::shared_ptr<BgBaseCard> card_bought) {
    if (card_bought->get_race() != "PIRATE") return;
    p1->add_gold(1);
}

void CapnHoggarrGolden::card_bought_trigger(Player* p1, std::shared_ptr<BgBaseCard> card_bought) {
    if (card_bought->get_race() != "PIRATE") return;
    p1->add_gold(2);
}

void CobaltScalebane::end_turn_mechanic(Player* p1) {
    buff_attack_end_turn(p1, 3, this);
}

void CobaltScalebaneGolden::end_turn_mechanic(Player* p1) {
    buff_attack_end_turn(p1, 6, this);
}

void Crystalweaver::do_battlecry(Player* p1) {
    for (auto c : p1->get_board()->get_cards()) {
        if (c->get_race() == "DEMON") {
            c->set_attack(c->get_attack() + 1);
            c->set_health(c->get_health() + 1);
        }
    }

}

void CrystalweaverGolden::do_battlecry(Player* p1) {
    cw.do_battlecry(p1);
    cw.do_battlecry(p1);
}

void ColdlightSeer::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "MURLOC") {
            card->set_health(card->get_health() + 2);
        }
    }
}

void ColdlightSeerGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "MURLOC") {
            card->set_health(card->get_health() + 4);
        }
    }
}

void defender_of_argus_mod(std::shared_ptr<BgBaseCard> card, uint8_t buff) {
    card->set_attack(card->get_attack() + buff);
    card->set_health(card->get_health() + buff);
    card->set_taunt();
}

void DefenderOfArgus::do_battlecry(Player* p1) {
    int pos = 0;
    const uint8_t buff = 1;
    auto cards = p1->get_board()->get_cards();
    for (auto c : cards) {
        if (c.get() == this) {
            if (pos == 0) {
                defender_of_argus_mod(cards[1], buff);
            }
            else if ((unsigned)pos == cards.size() - 1) {
                defender_of_argus_mod(cards[cards.size() - 2], buff);
            }
            else {
                defender_of_argus_mod(cards[pos-1], buff);
                defender_of_argus_mod(cards[pos+1], buff);
            }
        }
        pos++;
    }
}

void DefenderOfArgusGolden::do_battlecry(Player* p1) {
    int pos = 0;
    const uint8_t buff = 2;
    auto cards = p1->get_board()->get_cards();
    for (auto c : cards) {
        if (c.get() == this) {
            if (pos == 0) {
                defender_of_argus_mod(cards[1], buff);
            }
            else if ((unsigned)pos == cards.size() - 1) {
                defender_of_argus_mod(cards[cards.size() - 2], buff);
            }
            else {
                defender_of_argus_mod(cards[pos-1], buff);
                defender_of_argus_mod(cards[pos+1], buff);
            }
        }
        pos++;
    }
}

void Djinni::do_deathrattle(Player* p1, Player*) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> Djinni::summon() {
    auto f = BgCardFactory();
    auto cards = f.get_cards_of_race("ELEMENTAL");
    // TOOD: Use a map or something more efficient
    int pos = 0;
    for (auto c : cards) {
        if (c->get_name() == "Djinni") {
            break;
        }
        pos++;
    }
    cards.erase(cards.begin() + pos);
    auto card = cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    return card;
}

void DjinniGolden::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> DjinniGolden::summon() {
    return dj.summon();
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void faceless_taverngoer_targeted_battlecry(BgBaseCard* _this,
                                            std::shared_ptr<BgBaseCard> c,
                                            bool golden) {
    auto name = c->get_name();
    if (golden && !hasEnding(name, "(Golden)")) {
        name += " (Golden)";
    }
    else if (!golden && hasEnding(name, "(Golden)")) {
        auto golden_size = std::string(" (Golden)").size();
        name.erase(name.end() - golden_size, name.end());
    }
    BgCardFactory f;
    auto copy = f.get_card(name);

    // Can't reassign this unfortunately...
    // TODO: make tooling to have this break/update if new attr is added to baseclass
    _this->set_base_attack(copy->get_base_attack());
    _this->set_attack(copy->get_base_attack());
    _this->set_card_class(copy->get_card_class());
    _this->set_divine_shield(copy->has_divine_shield());
    _this->set_base_health(copy->get_base_health());
    _this->set_health(copy->get_base_health());
    _this->set_mechanics(copy->get_mechanics());
    _this->set_name(copy->get_name());
    _this->set_poison(copy->has_poison());
    _this->set_race(copy->get_race());
    _this->set_rarity(copy->get_rarity());
    _this->set_tavern_tier(copy->get_tavern_tier());
    _this->set_taunt(copy->has_taunt());
    _this->set_type(copy->get_type());
    _this->set_reborn(copy->has_reborn());
    _this->set_windfury(copy->has_windfury());
    _this->set_windfury_active(copy->has_windfury_active());
}
                                            

void FacelessTaverngoer::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    return faceless_taverngoer_targeted_battlecry(this, c, false);
}

std::vector<int> FacelessTaverngoer::get_valid_target_indexes(Player* p1) {
    std::vector<int> res;
    for (int i = 0; (unsigned)i < p1->get_tavern_minions().size(); i++) {
        res.push_back(i);
    }
    return res;
}

void FacelessTaverngoerGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    return faceless_taverngoer_targeted_battlecry(this, c, true);
}

std::vector<int> FacelessTaverngoerGolden::get_valid_target_indexes(Player* p1) {
    return faceless.get_valid_target_indexes(p1);
}

void FelfinNavigator::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "MURLOC") {
            card->set_attack(card->get_attack() + 1);
            card->set_health(card->get_health() + 1);
        }
    }
}

void FelfinNavigatorGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "MURLOC") {
            card->set_attack(card->get_attack() + 2);
            card->set_health(card->get_health() + 2);
        }
    }
}

void FiendishServant::do_deathrattle(Player* p1, Player*) {
    auto b1 = p1->get_board().get();
    auto buffed_pos = RngSingleton::getInstance().get_rand_int() % b1->length();
    auto card = b1->get_cards()[buffed_pos];
    card->set_attack(card->get_attack() + attack);
}

void FiendishServantGolden::do_deathrattle(Player* p1, Player* p2) {
    serv.set_attack(this->get_attack());
    for (int i = 0; i < 2; i++) {
        serv.do_deathrattle(p1, p2);
    }
}

void FreedealingGambler::on_sell(Player* p1) {
    p1->add_gold(3);
}

void FreedealingGamblerGolden::on_sell(Player* p1) {
    p1->add_gold(6);
}

void Ghastcoiler::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> Ghastcoiler::summon() {
    auto f = BgCardFactory();
    auto cards = f.get_cards_with_deathrattle();
    auto card = cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    return card;
}

void GhastcoilerGolden::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(4, p1);
}

std::shared_ptr<BgBaseCard> GhastcoilerGolden::summon() {
    return coiler.summon();
}

void GlyphGuardian::do_preattack(std::shared_ptr<BgBaseCard> defender,
                                 Player* p1,
                                 Player* p2) {
    set_attack(get_attack() * 2);
}

void GlyphGuardianGolden::do_preattack(std::shared_ptr<BgBaseCard> defender,
                                       Player* p1,
                                       Player* p2) {
    set_attack(get_attack() * 3);
}

void Goldgrubber::end_turn_mechanic(Player* p1) {
    int num_gold_cards = 0;
    for (auto c : p1->get_board()->get_cards()) {
        if (c->is_golden()) {
            num_gold_cards++;
        }
    }
    set_attack(get_attack() + num_gold_cards*2);
    set_health(get_health() + num_gold_cards*2);
}

void GoldgrubberGolden::end_turn_mechanic(Player* p1) {
    int num_gold_cards = 0;
    for (auto c : p1->get_board()->get_cards()) {
        if (c->is_golden()) {
            num_gold_cards++;
        }
    }
    set_attack(get_attack() + num_gold_cards*4);
    set_health(get_health() + num_gold_cards*4);
}

void Goldrinn::do_deathrattle(Player* p1, Player* p2) {
    auto b1 = p1->get_board().get();
    auto cards = b1->get_cards();
    for (auto c : cards) {
        if (c->get_race() == "BEAST") {
            c->set_health(c->get_health() + 4);
            c->set_attack(c->get_attack() + 4);
        }
    }
}

void GoldrinnGolden::do_deathrattle(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        bag.do_deathrattle(p1, p2);
    }
}

void HangryDragon::start_turn_mechanic(Player* p1) {
    if (p1->won_last_turn()) {
        set_base_attack(get_base_attack() + 2);
        set_base_health(get_base_health() + 2);
    }
}

void HangryDragonGolden::start_turn_mechanic(Player* p1) {
    if (p1->won_last_turn()) {
        set_base_attack(get_base_attack() + 4);
        set_base_health(get_base_health() + 4);
    }
}


void HarvestGolem::do_deathrattle(Player* p1, Player* p2) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> HarvestGolem::summon() {
    auto f = BgCardFactory();
    return f.get_card("Damaged Golem");
}

void HarvestGolemGolden::do_deathrattle(Player* p1, Player* p2) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> HarvestGolemGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Damaged Golem (Golden)");
}

void HeraldOfFlame::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                  int def_pos,
                                  Player* p1,
                                  Player* p2) {
    Board* b2 = p2->get_board().get();
    if (defender->get_health() < 0) {
        auto b2_cards = b2->get_cards();
        if (!b2_cards.empty()) {
            auto new_defender = b2_cards[0];
            auto f = BgCardFactory();
            auto hof = f.get_card("Herald of Flame");
            hof->set_attack(3);
            BoardBattler().take_dmg_simul(hof, new_defender, p1, p2);
        }
    }
}

void HeraldOfFlameGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                        int def_pos,
                                        Player* p1,
                                        Player* p2) {
    Board* b2 = p2->get_board().get();
    if (defender->get_health() < 0) {
        auto b2_cards = b2->get_cards();
        if (!b2_cards.empty()) {
            auto new_defender = b2_cards[0];
            auto f = BgCardFactory();
            auto hof = f.get_card("Herald of Flame (Golden)");
            hof->set_attack(6);
            BoardBattler().take_dmg_simul(hof, new_defender, p1, p2);
        }
    }
}

void Houndmaster::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    c->set_attack(c->get_attack() + 2);
    c->set_health(c->get_health() + 2);
    c->set_taunt();
}

bool Houndmaster::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "BEAST";
}

void HoundmasterGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    houndmaster.do_targeted_battlecry(c);
    houndmaster.do_targeted_battlecry(c);
}

bool HoundmasterGolden::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return houndmaster.is_valid_target(c);
}

void Imprisoner::do_deathrattle(Player* p1, Player* p2) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> Imprisoner::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp");
}

void ImpGangBoss::take_damage(int damage, std::string who_from_race, Player* p1, Player* p2) {
    BgBaseCard::take_damage(damage, who_from_race, p1, p2);
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> ImpGangBoss::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp");
}

void ImpGangBossGolden::take_damage(int damage, std::string who_from_race, Player* p1, Player* p2) {
    BgBaseCard::take_damage(damage, who_from_race, p1, p2);
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> ImpGangBossGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp (Golden)");
}

void ImpMama::take_damage(int damage, std::string who_from_race, Player* p1, Player* p2) {
    BgBaseCard::take_damage(damage, who_from_race, p1, p2);
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> ImpMama::summon() {
    auto f = BgCardFactory();
    auto demons = f.get_cards_of_race("DEMON");    
    auto demon = demons[RngSingleton::getInstance().get_rand_int() % demons.size()];
    demon->set_taunt();
    return demon;
}

void ImpMamaGolden::take_damage(int damage, std::string who_from_race, Player* p1, Player* p2) {
    BgBaseCard::take_damage(damage, who_from_race, p1, p2);
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> ImpMamaGolden::summon() {
    return imp_mama.summon();
}

void ImprisonerGolden::do_deathrattle(Player* p1, Player* p2) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> ImprisonerGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Imp (Golden)");
}

void InfestedWolf::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> InfestedWolf::summon() {
    auto f = BgCardFactory();
    return f.get_card("Spider");
}

void InfestedWolfGolden::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> InfestedWolfGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Spider (Golden)");
}

void IronhideDirehorn::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                     int def_pos,
                                     Player* p1,
                                     Player* p2) {
    if (defender->get_health() < 0) {
        basic_summon(p1);
    }
}

std::shared_ptr<BgBaseCard> IronhideDirehorn::summon() {
    auto f = BgCardFactory();
    return f.get_card("Ironhide Runt");
}


void IronhideDirehornGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                           int def_pos,
                                           Player* p1,
                                           Player* p2) {
    if (defender->get_health() < 0) {
        basic_summon(p1);
    }
}

std::shared_ptr<BgBaseCard> IronhideDirehornGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Ironhide Runt (Golden)");
}

void iron_sensei_end_turn(Player* p1, int buff, BgBaseCard* this_) {
    std::vector<std::shared_ptr<BgBaseCard>> mech_cards;
    for (auto c : p1->get_board()->get_cards()) {
        if (c.get() == this_) continue;
        if (c->get_race() == "MECHANICAL") {
            mech_cards.push_back(c);
        }
    }
    if (mech_cards.size() == 0) return;
    auto card_to_buff =  mech_cards[RngSingleton::getInstance().get_rand_int() % mech_cards.size()];
    card_to_buff->set_attack(card_to_buff->get_attack() + buff);
    card_to_buff->set_health(card_to_buff->get_health() + buff);
}

void IronSensei::end_turn_mechanic(Player* p1) {
    iron_sensei_end_turn(p1, 2, this);
}

void IronSenseiGolden::end_turn_mechanic(Player* p1) {
    iron_sensei_end_turn(p1, 4, this);
}


void Junkbot::do_postbattle(Player* p1,
                            Player* p2,
                            std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
                            std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    int dead_mech_count = 0;
    for (auto c : dead_b1) {
        if (c->get_race() == "MECHANICAL") {
            dead_mech_count++;
        }
    }
    for (int i = 0; i < dead_mech_count; i++) {
        set_attack(get_attack() + 2);
        set_health(get_health() + 2);
    }
}

void JunkbotGolden::do_postbattle(Player* p1,
                                  Player* p2,
                                  std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
                                  std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (int i = 0; i < 2; i++) {
        junk_bot.do_postbattle(p1, p2, dead_b1, dead_b2);
    }
}


void KaboomBot::do_deathrattle(Player* p1, Player* p2) {
    Board* b2 = p2->get_board().get();
    if (b2->length() == 0) {
        return;
    }
    auto bombed_pos = RngSingleton::getInstance().get_rand_int() % b2->length();
    auto bombed_card = b2->get_cards()[bombed_pos];
    BoardBattler b;
    b.take_dmg_simul(bombed_card, this->get_race(), 4, p2, p1);
}

void KaboomBotGolden::do_deathrattle(Player* p1, Player* p2) {
    kbot.do_deathrattle(p1, p2);
    kbot.do_deathrattle(p1, p2);
}

int Kalecgos::mod_summoned(std::shared_ptr<BgBaseCard> c, Player* p1, bool from_hand) {
    if (!from_hand) return 0;
    if (c->has_battlecry()) {
        if (c->get_race() == "DRAGON") {
            c->set_attack(c->get_attack() + 1);
            c->set_health(c->get_health() + 1);
        }
        for (auto card : p1->get_board()->get_cards()) {
            if (card->get_race() == "DRAGON") {
                card->set_attack(card->get_attack() + 1);
                card->set_health(card->get_health() + 1);
            }
        }
    }
    return 0;
}

int KalecgosGolden::mod_summoned(std::shared_ptr<BgBaseCard> c, Player* p1, bool from_hand) {
    k.mod_summoned(c, p1, from_hand);
    k.mod_summoned(c, p1, from_hand);
    return 0;
}

void Kangor::do_deathrattle(Player* p1, Player* p2) {
    reset_mech_queue(p1->get_board().get());
    auto available_mechs = mech_queue.size() < unsigned(2) ? mech_queue.size() : unsigned(2);
    multi_summon(available_mechs, p1);
}

std::shared_ptr<BgBaseCard> Kangor::summon() {
    auto card_name = mech_queue.front();
    mech_queue.pop();
    auto f = BgCardFactory();
    return f.get_card(card_name);
}

void Kangor::reset_mech_queue(Board* b) {
    std::queue<std::string>().swap(mech_queue); // clear current queue
    for (auto c : b->has_died()) {
        if (c->get_race() == "MECHANICAL") {
            mech_queue.push(c->get_name());
        }
    }
}

void KangorGolden::do_deathrattle(Player* p1, Player* p2) {
    kang.reset_mech_queue(p1->get_board().get());
    auto available_mechs = kang.mech_queue.size() < unsigned(4) ? kang.mech_queue.size() : unsigned(4);
    multi_summon(available_mechs, p1);
}

std::shared_ptr<BgBaseCard> KangorGolden::summon() {
    return kang.summon();
}


void KindlyGrandmother::do_deathrattle(Player* p1, Player* p2) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> KindlyGrandmother::summon() {
    auto f = BgCardFactory();
    return f.get_card("Big Bad Wolf");
}

void KindlyGrandmotherGolden::do_deathrattle(Player* p1, Player* p2) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> KindlyGrandmotherGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Big Bad Wolf (Golden)");
}

void KingBagurgle::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "MURLOC") {
            card->set_attack(card->get_attack() + 2);
            card->set_health(card->get_health() + 2);
        }
    }
}

void KingBagurgle::do_deathrattle(Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    auto cards = b1->get_cards();
    for (auto c : cards) {
        if (c->get_race() == "MURLOC") {
            c->set_health(c->get_health() + 2);
            c->set_attack(c->get_attack() + 2);
        }
    }
}

void KingBagurgleGolden::do_battlecry(Player* p1) {
        for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->get_race() == "MURLOC") {
            card->set_attack(card->get_attack() + 4);
            card->set_health(card->get_health() + 4);
        }
    }
}

void KingBagurgleGolden::do_deathrattle(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        bag.do_deathrattle(p1, p2);
    }
}

void lt_gar_mod_sum(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand, int buff_factor, BgBaseCard* to_buff) {
    if (card->get_race() == "ELEMENTAL" && from_hand) {
        int num_elementals = 1; // Already have one that we're playing
        for (auto c : p1->get_board()->get_cards()) {
            if (c->get_race() == "ELEMENTAL") num_elementals++;
        }
        to_buff->set_health(to_buff->get_health() + num_elementals * buff_factor);
    }
}

int LieutenantGarr::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    lt_gar_mod_sum(card, p1, from_hand, 1, this);
    return 0;
}

int LieutenantGarrGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    lt_gar_mod_sum(card, p1, from_hand, 2, this);
    return 0;
}

void LightfangEnforcer::end_turn_mechanic(Player* p1) {
    menagerie_bcry(p1, 2);
}

void LightfangEnforcerGolden::end_turn_mechanic(Player* p1) {
    menagerie_bcry(p1, 4);
}

int LilRag::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    if (card->get_race() == "ELEMENTAL" && from_hand) {
        auto all_elem_cards = p1->get_board()->get_cards();
        all_elem_cards.clear();
        all_elem_cards.push_back(card);
        for (auto c : p1->get_board()->get_cards()) {
            if (c->get_race() == "ELEMENTAL") all_elem_cards.push_back(c);
        }
        auto card_to_buff =  all_elem_cards[RngSingleton::getInstance().get_rand_int() % all_elem_cards.size()];
        card_to_buff->set_attack(card_to_buff->get_attack() + card->get_tavern_tier());
        card_to_buff->set_health(card_to_buff->get_health() + card->get_tavern_tier());
    }
    return 0;
}

int LilRagGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    lr.mod_summoned(card, p1, from_hand);
    lr.mod_summoned(card, p1, from_hand);
    return 0;
}

void LivingSporeDrattle::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> LivingSporeDrattle::summon() {
    auto f = BgCardFactory();
    return f.get_card("Living Spore");
}

void MajordomoExecutus::end_turn_mechanic(Player* p1) {
    auto left_most_minion = p1->get_board()->get_cards()[0];
    auto buff = p1->get_elementals_played_this_turn() + 1;
    left_most_minion->set_base_attack(left_most_minion->get_base_attack() + buff);
    left_most_minion->set_base_health(left_most_minion->get_base_health() + buff);
}

void MajordomoExecutusGolden::end_turn_mechanic(Player* p1) {
    majordomo.end_turn_mechanic(p1);
    majordomo.end_turn_mechanic(p1);
}

void MalGanis::do_precombat(Player* p1, Player*) {
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "DEMON") {
            card->set_attack(card->get_attack() + 2);
            card->set_health(card->get_health() + 2);
        }
    }
    set_attack(get_attack() - 2); // MalGanis doesn't apply to itself
    set_health(get_health() - 2); // MalGanis doesn't apply to itself
}

void MalGanis::do_deathrattle(Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "DEMON") {
            card->set_attack(card->get_attack() - 2);
            card->set_health(card->get_health() - 2);
        }
    }
    set_attack(get_attack() + 2); // Malganis doesn't apply to itself
    set_health(get_health() + 2); // Malganis doesn't apply to itself
}

void MalGanisGolden::do_precombat(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        rw.do_precombat(p1, p2);
    }
}

void MalGanisGolden::do_deathrattle(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        rw.do_deathrattle(p1, p2);
    }
}

int MamaBear::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool) {
    if (card->get_race() == "BEAST") {
        card->set_attack(card->get_attack() + 4);
        card->set_health(card->get_health() + 4);
    }
    return 0;
}

int MamaBearGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool) {
    for (int i = 0; i < 2; i++) {
        pl.mod_summoned(card, p1, false); // from hand doesn't matter
    }
    return 0;
}


void MechanoEgg::do_deathrattle(Player* p1, Player* p2) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> MechanoEgg::summon() {
    auto f = BgCardFactory();
    return f.get_card("Robosaur");
}

void MechanoEggGolden::do_deathrattle(Player* p1, Player* p2) {
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> MechanoEggGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Robosaur (Golden)");
}

void MenagerieJug::do_battlecry(Player* p1) {
    menagerie_bcry(p1, 2, 3);
}

void MenagerieJugGolden::do_battlecry(Player* p1) {
    menagerie_bcry(p1, 4, 3);
}

void MenagerieMug::do_battlecry(Player* p1) {
    menagerie_bcry(p1, 1, 3);
}

void MenagerieMugGolden::do_battlecry(Player* p1) {
    menagerie_bcry(p1, 2, 3);
}

void metaltooth_bcry(Player* p1, int buff, BgBaseCard* _this) {
    auto board = p1->get_board();
    for (auto card : board->get_cards()) {
        if (card.get() == _this) {
            continue;
        }
        if (card->get_race() == "MECHANICAL") {
            card->set_attack(card->get_attack() + buff);
        }
    }
}

void MetaltoothLeaper::do_battlecry(Player* p1) {
    metaltooth_bcry(p1, 2, this);
}

void MetaltoothLeaperGolden::do_battlecry(Player* p1) {
    metaltooth_bcry(p1, 4, this);
}

void micro_machine_start_turn(Player*, int attack_buff, BgBaseCard* this_) {
    this_->set_attack(this_->get_attack() + attack_buff);
}

void MicroMachine::start_turn_mechanic(Player* p1) {
    micro_machine_start_turn(p1, 1, this);
}

void MicroMachineGolden::start_turn_mechanic(Player* p1) {
    micro_machine_start_turn(p1, 2, this);
}

void MicroMummy::end_turn_mechanic(Player* p1) {
    buff_attack_end_turn(p1, 1, this);
}

void MicroMummyGolden::end_turn_mechanic(Player* p1) {
    buff_attack_end_turn(p1, 2, this);
}

int MoltenRock::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (from_hand && card->get_race() == "ELEMENTAL") {
        set_health(get_health() + 1);
    }
    return 0;
}

int MoltenRockGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (from_hand && card->get_race() == "ELEMENTAL") {
        set_health(get_health() + 2);
    }
    return 0;
}

void MonstrousMacaw::do_preattack(std::shared_ptr<BgBaseCard> defender,
                                  Player* p1,
                                  Player* p2) {
    std::vector<std::shared_ptr<BgBaseCard>> drattles;
    Board* b1 = p1->get_board().get();
    for (auto c : b1->get_cards()) {
        if (c->has_deathrattle()) {
            drattles.push_back(c);
        }
    }
    if (!drattles.empty()) {
        auto random_drattle = drattles[RngSingleton::getInstance().get_rand_int() % drattles.size()];
        random_drattle->do_deathrattle(p1, p2);
    }
}

void MonstrousMacawGolden::do_preattack(std::shared_ptr<BgBaseCard> defender,
                                        Player* p1,
                                        Player* p2) {
    macaw.do_preattack(defender, p1, p2);
    macaw.do_preattack(defender, p1, p2);
}

int MurlocTidecaller::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (card->get_race() == "MURLOC" && from_hand) {
        set_attack(get_attack() + 1);
    }
    return 0;
}

int MurlocTidecallerGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (card->get_race() == "MURLOC" && from_hand) {
        set_attack(get_attack() + 2);
    }
    return 0;
}

void MurlocTidehunter::do_battlecry(Player* p1) {
    basic_summon(p1, true);
    p1->check_for_triples();
}

std::shared_ptr<BgBaseCard> MurlocTidehunter::summon() {
    auto f = BgCardFactory();
    return f.get_card("Murloc Scout");
}

void MurlocTidehunterGolden::do_battlecry(Player* p1) {
    basic_summon(p1, true);
}

std::shared_ptr<BgBaseCard> MurlocTidehunterGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Murloc Scout (Golden)");
}

void MurlocWarleader::do_precombat(Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "MURLOC") {
            card->set_attack(card->get_attack() + 2);
        }
    }
    set_attack(get_attack() - 2); // Warleader doesn't apply to itself
}

void MurlocWarleader::do_deathrattle(Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "MURLOC") {
            card->set_attack(card->get_attack() - 2);
        }
    }
    set_attack(get_attack() + 2); // Warleader doesn't apply to itself
}

void MurlocWarleaderGolden::do_precombat(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        rw.do_precombat(p1, p2);
    }
}

void MurlocWarleaderGolden::do_deathrattle(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        rw.do_deathrattle(p1, p2);
    }
}

void mythrax_end_of_turn(Player* p1, int attack_buff_mult, int health_buff_mult, BgBaseCard* this_) {
    auto board = p1->get_board();
    std::unordered_set<std::string> races;
    auto cards = board->get_cards();
    for (auto c : cards) {
        races.insert(c->get_race());
    }
    // Empty string race doesn't count, so subtract one
    auto num_different_races = races.size() - 1;
    this_->set_attack(this_->get_attack() + attack_buff_mult * num_different_races);
    this_->set_health(this_->get_health() + health_buff_mult * num_different_races);
}

void murozond_battlecry(Player* p1, bool golden) {
    auto opp_board = p1->get_opponents_last_board();
    auto opp_cards = opp_board->get_cards();
    auto card_to_copy =  opp_cards[RngSingleton::getInstance().get_rand_int() % opp_cards.size()];
    std::string name = card_to_copy->get_name();
    if (golden && !hasEnding(name, "(Golden)")) {
        name += " (Golden)";
    }
    else if (!golden && hasEnding(name, "(Golden)")) {
        auto golden_size = std::string(" (Golden)").size();
        name.erase(name.end() - golden_size, name.end());
    }
    BgCardFactory f;
    auto copy = f.get_card(name);
    p1->add_card_to_hand(copy);
}

void Murozond::do_battlecry(Player* p1) {
    murozond_battlecry(p1, false);
}

void MurozondGolden::do_battlecry(Player* p1) {
    murozond_battlecry(p1, true);
}

void MythraxTheUnraveler::end_turn_mechanic(Player* p1) {
    mythrax_end_of_turn(p1, 1, 2, this);
}

void MythraxTheUnravelerGolden::end_turn_mechanic(Player* p1) {
    mythrax_end_of_turn(p1, 2, 4, this);
}

void Nadina::do_deathrattle(Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    auto cards = b1->get_cards();
    for (auto c : cards) {
        if (c->get_race() == "DRAGON") {
            c->set_divine_shield();
        }
    }
}

void NadinaGolden::do_deathrattle(Player* p1, Player* p2) {
    bag.do_deathrattle(p1, p2);
}

void NatPagle::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                     int def_pos,
                                     Player* p1,
                                     Player* p2) {
    if (defender->get_health() <= 0) {
        BgCardFactory f;
        auto cur_tav_tier = p1->get_tavern_tier();
        auto tier_to_chose_from = (RngSingleton::getInstance().get_rand_int() % cur_tav_tier) + 1;
        auto card_names = f.get_card_names_by_tier()[tier_to_chose_from];
        auto card_name = card_names[RngSingleton::getInstance().get_rand_int() % card_names.size()];
        p1->add_card_to_hand(f.get_card(card_name));
    }
}

void NatPagleGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                           int def_pos,
                                           Player* p1,
                                           Player* p2) {
    pagle.do_postattack(defender, def_pos, p1, p2);
    pagle.do_postattack(defender, def_pos, p1, p2);
}

void NathrezimOverseer::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    c->set_attack(c->get_attack() + 2);
    c->set_health(c->get_health() + 2);
}

void NathrezimOverseerGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    nathrezim_overseer.do_targeted_battlecry(c);
    nathrezim_overseer.do_targeted_battlecry(c);
}

bool NathrezimOverseer::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "DEMON";
}

bool NathrezimOverseerGolden::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return nathrezim_overseer.is_valid_target(c);
}

int Nomi::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    if (card->get_race() != "ELEMENTAL") return 0;
    p1->add_to_nomi_tav_count();
    return 0;
}

int NomiGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    nomi.mod_summoned(card, p1, from_hand);
    nomi.mod_summoned(card, p1, from_hand);
    return 0;
}

void OldMurkeye::do_precombat(Player* p1, Player*) {
    Board* b1 = p1->get_board().get();
    int murloc_count = 0;
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "MURLOC") {
            murloc_count++;
        }
    }
    set_attack(get_attack() + murloc_count - 1); // Old Murkeye doesn't apply to itself
}

void OldMurkeyeGolden::do_precombat(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        rw.do_precombat(p1, p2);
    }
}

void OldMurkeye::do_postbattle(Player* p1,
                               Player*,
                               std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
                               std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    Board* b1 = p1->get_board().get();
    int dead_murloc_count = 0;
    for (auto c : dead_b1) {
        if (c->get_race() == "MURLOC") {
            dead_murloc_count++;
        }
    }
    set_attack(get_attack() - dead_murloc_count);
}

void OldMurkeyeGolden::do_postbattle(Player* p1,
                                     Player* p2,
                                     std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
                                     std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (int i = 0; i < 2; i++) {
        rw.do_postbattle(p1, p2, dead_b1, dead_b2);
    }
}

int PackLeader::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool) {
    if (card->get_race() == "BEAST") {
        card->set_attack(card->get_attack() + 3);
    }
    return 0;
}

int PackLeaderGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool) {
    for (int i = 0; i < 2; i++) {
        pl.mod_summoned(card, p1, false); // from_hand doesn't matter
    }
    return 0;
}

int party_elemental_mod_sum(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand, int buff, BgBaseCard* this_) {
    if (!from_hand || card->get_race() != "ELEMENTAL") return 0;

    std::vector<std::shared_ptr<BgBaseCard>> elementals;
    for (auto c : p1->get_board()->get_cards()) {
        if (c->get_race() == "ELEMENTAL") {         
            elementals.push_back(c);
        }
    }
    
    auto card_to_buff =  elementals[RngSingleton::getInstance().get_rand_int() % elementals.size()];
    card_to_buff->set_attack(card_to_buff->get_attack() + buff);
    card_to_buff->set_health(card_to_buff->get_health() + buff);

    return 0;
}

int PartyElemental::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    return party_elemental_mod_sum(card, p1, from_hand, 1, this);
}

int PartyElementalGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    return party_elemental_mod_sum(card, p1, from_hand, 2, this);
}

void PilotedShredder::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(1, p1);
}

std::shared_ptr<BgBaseCard> PilotedShredder::summon() {
    auto f = BgCardFactory();
    auto two_cost_cards = f.get_cards_of_cost(2);
    auto card = two_cost_cards[RngSingleton::getInstance().get_rand_int() % two_cost_cards.size()];
    return card;
}

void PilotedShredderGolden::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> PilotedShredderGolden::summon() {
    return shredder.summon();
}

int rabid_sauro_mod_sum(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand, int buff, BgBaseCard* this_) {
    if (!from_hand || !card->has_deathrattle()) return 0;
    this_->set_attack(this_->get_attack() + buff);
    this_->set_health(this_->get_health() + buff);
    return 0;
}

int RabidSaurolisk::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    return rabid_sauro_mod_sum(card, p1, from_hand, 1, this);
}

int RabidSauroliskGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player* p1, bool from_hand) {
    return rabid_sauro_mod_sum(card, p1, from_hand, 2, this);
}

void RatPack::do_deathrattle(Player* p1, Player*) {
    auto attack = get_attack();
    multi_summon(attack, p1);
}

std::shared_ptr<BgBaseCard> RatPack::summon() {
    auto f = BgCardFactory();
    return f.get_card("Rat");
}

void RatPackGolden::do_deathrattle(Player* p1, Player*) {
    auto attack = get_attack();
    multi_summon(attack, p1);
}

std::shared_ptr<BgBaseCard> RatPackGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Rat (Golden)");
}

void razorgore_end_of_turn(Player* p1, int buff_mult, BgBaseCard* this_) {
    auto board = p1->get_board();
    int num_dragons = 0;
    auto cards = board->get_cards();
    for (auto c : cards) {
        if (c->get_race() == "DRAGON") num_dragons++;
    }
    this_->set_attack(this_->get_attack() + num_dragons * buff_mult);
    this_->set_health(this_->get_health() + num_dragons * buff_mult);
}

void Razorgore::end_turn_mechanic(Player* p1) {
    razorgore_end_of_turn(p1, 1, this);
}

void RazorgoreGolden::end_turn_mechanic(Player* p1) {
    razorgore_end_of_turn(p1, 2, this);
}


void RedWhelp::do_precombat(Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    int drag_count = 0;
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "DRAGON") {
            drag_count++;
        }
    }
    auto defender_pos = RngSingleton::getInstance().get_rand_int() % b2->length();
    auto defender = (*b2)[defender_pos];
    BoardBattler b;
    b.take_dmg_simul(defender, this->get_race(), drag_count, p2, p1);
}

void RedWhelpGolden::do_precombat(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        rw.do_precombat(p1, p2);
    }
}

void RefreshingAnomaly::do_battlecry(Player* p1) {
    p1->set_free_refreshes(1);
}

void RefreshingAnomalyGolden::do_battlecry(Player* p1) {
    p1->set_free_refreshes(2);
}

void ReplicatingMenace::do_deathrattle(Player* p1, Player*) {
    multi_summon(3, p1);
}

std::shared_ptr<BgBaseCard> ReplicatingMenace::summon() {
    auto f = BgCardFactory();
    return f.get_card("Microbot");
}

void ReplicatingMenaceGolden::do_deathrattle(Player* p1, Player*) {
    multi_summon(3, p1);
}

std::shared_ptr<BgBaseCard> ReplicatingMenaceGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Microbot (Golden)");
}

void RingMatron::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> RingMatron::summon() {
    auto f = BgCardFactory();
    return f.get_card("Fiery Imp");
}

void RingMatronGolden::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> RingMatronGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Fiery Imp (Golden)");
}

void RockpoolHunter::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    c->set_attack(c->get_attack() + 1);
    c->set_health(c->get_health() + 1);
}

void RockpoolHunterGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    rockpool_hunter.do_targeted_battlecry(c);
    rockpool_hunter.do_targeted_battlecry(c);
}

bool RockpoolHunter::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "MURLOC";
}

bool RockpoolHunterGolden::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return rockpool_hunter.is_valid_target(c);
}

int SaltyLooter::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (card->get_race() == "PIRATE" && from_hand) {
        set_attack(get_attack() + 1);
        set_health(get_health() + 1);
    }
    return 0;
}

int SaltyLooterGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (card->get_race() == "PIRATE" && from_hand) {
        set_attack(get_attack() + 2);
        set_health(get_health() + 2);
    }
    return 0;
}

void SavannahHighmane::do_deathrattle(Player* p1, Player*) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> SavannahHighmane::summon() {
    auto f = BgCardFactory();
    return f.get_card("Hyena");
}

void SavannahHighmaneGolden::do_deathrattle(Player* p1, Player*) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> SavannahHighmaneGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Hyena (Golden)");
}

void Scallywag::do_deathrattle(Player* p1, Player* p2) {
    auto f = BgCardFactory();
    auto sky_pirate = f.get_card("Sky Pirate");
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    b1->insert_card(death_pos, sky_pirate, p1);
    if (!b2->empty()) {
        BoardBattler().battle_boards(death_pos, p1, p2); // Modifies b1/b2
    }
}

void ScallywagGolden::do_deathrattle(Player* p1, Player* p2) {
    auto f = BgCardFactory();
    auto sky_pirate = f.get_card("Sky Pirate (Golden)");
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    b1->insert_card(death_pos, sky_pirate, p1);
    if (!b2->empty()) {
        BoardBattler().battle_boards(death_pos, p1, p2); // Modifies b1/b2
    }
}

void ScavengingHyena::do_postbattle(Player*,
                                    Player*,
                                    std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
                                    std::vector<std::shared_ptr<BgBaseCard> >) {
    for (auto c : dead_b1) {
        if (c->get_race() == "BEAST") {
            set_health(get_health() + 1);
            set_attack(get_attack() + 2);
        }
    }
}

void ScavengingHyenaGolden::do_postbattle(Player*,
                                          Player*,
                                          std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
                                          std::vector<std::shared_ptr<BgBaseCard> >) {
    for (auto c : dead_b1) {
        if (c->get_race() == "BEAST") {
            set_health(get_health() + 2);
            set_attack(get_attack() + 4);
        }
    }
}

void ScrewjankClunker::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    c->set_attack(c->get_attack() + 2);
    c->set_health(c->get_health() + 2);
}

void ScrewjankClunkerGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    screwjank_clunker.do_targeted_battlecry(c);
    screwjank_clunker.do_targeted_battlecry(c);
}

bool ScrewjankClunker::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "MECHANICAL";
}

bool ScrewjankClunkerGolden::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return screwjank_clunker.is_valid_target(c);
}


void SeabreakerGoliath::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                      int,
                                      Player* p1,
                                      Player* ) {
    if (defender->get_health() < 0) {
        Board* b1 = p1->get_board().get();
        for (auto c : b1->get_cards()) {
            if (c->get_race() == "PIRATE" && c.get() != this) {
                c->set_attack(c->get_attack() + 2);
                c->set_health(c->get_health() + 2);
            }
        }
    }
}

void SeabreakerGoliathGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                            int def_pos,
                                            Player* p1,
                                            Player* p2) {
    for (int i = 0; i < 2; i++) {
        sbg.do_postattack(defender, def_pos, p1, p2);
    }
}


void SecurityRover::take_damage(int damage, std::string who_from_race, Player* p1, Player* p2) {
    BgBaseCard::take_damage(damage, who_from_race, p1, p2);
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> SecurityRover::summon() {
    auto f = BgCardFactory();
    return f.get_card("Guard Bot");
}

void SecurityRoverGolden::take_damage(int damage, std::string who_from_race, Player* p1, Player* p2) {
    BgBaseCard::take_damage(damage, who_from_race, p1, p2);
    basic_summon(p1);
}

std::shared_ptr<BgBaseCard> SecurityRoverGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Guard Bot (Golden)");
}

void SelflessHero::do_deathrattle(Player* p1, Player*) {
    // Cards w/o divine shield
    std::vector<std::shared_ptr<BgBaseCard> > cards;
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (!card->has_divine_shield()) {
            cards.push_back(card);
        }
    }
    if (cards.size() > 0) {
        auto buffed_pos = RngSingleton::getInstance().get_rand_int() % cards.size();
        auto card = cards[buffed_pos];
        card->set_divine_shield();
    }
}

void SelflessHeroGolden::do_deathrattle(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        hero.do_deathrattle(p1, p2);
    }
}

void Sellemental::on_sell(Player* p1) {
    BgCardFactory f;
    auto wd = f.get_card("Water Droplet");
    p1->add_card_to_hand(wd);
    p1->add_gold(1);
    p1->check_for_triples();
}

void SellementalGolden::on_sell(Player* p1) {
    BgCardFactory f;
    auto wd1 = f.get_card("Water Droplet");
    auto wd2 = f.get_card("Water Droplet");
    p1->add_card_to_hand(wd1);
    p1->add_card_to_hand(wd2);
    p1->add_gold(1);
    p1->check_for_triples();
}

void Siegebreaker::do_precombat(Player* p1, Player*) {
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "DEMON") {
            card->set_attack(card->get_attack() + 1);
        }
    }
    set_attack(get_attack() - 1); // Siegbreaker doesn't apply to itself
}

void Siegebreaker::do_deathrattle(Player* p1, Player*) {
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "DEMON") {
            card->set_attack(card->get_attack() - 1);
        }
    }
    set_attack(get_attack() + 1); // Siegebreaker doesn't apply to itself
}

void SiegebreakerGolden::do_precombat(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        sb.do_precombat(p1, p2);
    }
}

void SiegebreakerGolden::do_deathrattle(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        sb.do_deathrattle(p1, p2);
    }
}

void SneedsOldShredder::do_deathrattle(Player* p1, Player*) {
    multi_summon(1, p1);
}

std::shared_ptr<BgBaseCard> SneedsOldShredder::summon() {
    auto f = BgCardFactory();
    auto legendary_cards = f.get_cards_of_rarity("LEGENDARY");
    auto card = legendary_cards[RngSingleton::getInstance().get_rand_int() % legendary_cards.size()];
    return card;
}

void SneedsOldShredderGolden::do_deathrattle(Player* p1, Player*) {
    multi_summon(2, p1);
}

std::shared_ptr<BgBaseCard> SneedsOldShredderGolden::summon() {
    return shredder.summon();
}

void SoulJuggler::do_postbattle(Player* p1,
                                Player* p2,
                                std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
                                std::vector<std::shared_ptr<BgBaseCard> > ) {
    int dead_demon_count = 0;    
    for (auto c : dead_b1) {
        if (c->get_race() == "DEMON") {
            dead_demon_count++;
        }
    }
    Board* b2 = p2->get_board().get();
    auto cards = b2->get_cards();
    if (cards.empty()) {
        return;
    }
    auto battler = BoardBattler();
    for (int i = 0; i < dead_demon_count; i++) {
        auto card = cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
        battler.take_dmg_simul(card, "NEUTRAL", 3, p1, p2);
    }
    
}

void SoulJugglerGolden::do_postbattle(Player* p1,
                                      Player* p2,
                                      std::vector<std::shared_ptr<BgBaseCard> > dead_b1,
                                      std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (int i = 0; i < 2; i++) {
        soul_juggler.do_postbattle(p1, p2, dead_b1, dead_b2);
    }
}

void SouthseaCaptain::do_precombat(Player* p1, Player*) {
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "PIRATE") {
            card->set_attack(card->get_attack() + 1);
            card->set_health(card->get_health() + 1);
        }
    }
    set_attack(get_attack() - 1); // Southsea doesn't apply to itself
    set_health(get_health() - 1); // Southsea doesn't apply to itself
}

void SouthseaCaptain::do_deathrattle(Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    for (auto card : b1->get_cards()) {
        if (card->get_race() == "PIRATE") {
            card->set_attack(card->get_attack() - 1);
            card->set_health(card->get_health() - 1);
        }
    }
    set_attack(get_attack() + 1);  // Southsea doesn't apply to itself
    set_health(get_health() + 1);  // Southsea doesn't apply to itself
}

void SouthseaCaptainGolden::do_precombat(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        rw.do_precombat(p1, p2);
    }
}

void SouthseaCaptainGolden::do_deathrattle(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        rw.do_deathrattle(p1, p2);
    }
}

void SouthseaStrongarm::targeted_battlecry(std::shared_ptr<BgBaseCard> c, Player* p1) {
    pirates_bought_this_turn = p1->get_pirates_bought_this_turn();
    TargetedBattlecryCard::targeted_battlecry(c, p1);
}

void SouthseaStrongarm::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "PIRATE") {
        c->set_attack(c->get_attack() + pirates_bought_this_turn);
        c->set_health(c->get_health() + pirates_bought_this_turn);
    }
}

bool SouthseaStrongarm::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "PIRATE";
}

void SouthseaStrongarmGolden::targeted_battlecry(std::shared_ptr<BgBaseCard> c, Player* p1) {
    pirates_bought_this_turn = p1->get_pirates_bought_this_turn();
    TargetedBattlecryCard::targeted_battlecry(c, p1);
}

void SouthseaStrongarmGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    if (c->get_race() == "PIRATE") {
        c->set_attack(c->get_attack() + pirates_bought_this_turn * 2);
        c->set_health(c->get_health() + pirates_bought_this_turn * 2);
    }
}

bool SouthseaStrongarmGolden::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "PIRATE";
}

void SpawnOfNzoth::do_deathrattle(Player* p1, Player*) {
    Board* b1 = p1->get_board().get();
    for (auto c : b1->get_cards()) {
        c->set_attack(c->get_attack() + 1);
        c->set_health(c->get_health() + 1);
    }
}

void SpawnOfNzothGolden::do_deathrattle(Player* p1, Player*) {
    Board* b1 = p1->get_board().get();
    for (auto c : b1->get_cards()) {
        c->set_attack(c->get_attack() + 2);
        c->set_health(c->get_health() + 2);
    }
}

void StasisElemental::do_battlecry(Player* p1) {
    BgCardFactory f;
    auto elementals = f.get_cards_of_race("ELEMENTAL");
    std::vector<std::shared_ptr<BgBaseCard>> elementals_at_or_below_tier;
    for (auto card : elementals) {
        if (card->get_tavern_tier() <= p1->get_tavern_tier()) {
            elementals_at_or_below_tier.push_back(card);
        }
    }
    auto to_freeze = elementals_at_or_below_tier[RngSingleton::getInstance().get_rand_int() % elementals_at_or_below_tier.size()];
    p1->add_to_frozen_minions(to_freeze->get_name());
}

void StasisElementalGolden::do_battlecry(Player* p1) {
    se.do_battlecry(p1);
    se.do_battlecry(p1);
}

void StewardOfTime::on_sell(Player* p1) {
    p1->buff_tav_till_refresh(1, 1);
}

void StewardOfTimeGolden::on_sell(Player* p1) {
    p1->buff_tav_till_refresh(2, 2);
}

void StrongshellScavenger::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->has_taunt()) {
            card->set_attack(card->get_attack() + 2);
            card->set_health(card->get_health() + 2);
        }
    }
}

void StrongshellScavengerGolden::do_battlecry(Player* p1) {
    for (auto card : p1->get_board()->get_cards()) {
        if (card.get() == this) continue;
        if (card->has_taunt()) {
            card->set_attack(card->get_attack() + 4);
            card->set_health(card->get_health() + 4);
        }
    }
}

void TavernTempest::do_battlecry(Player* p1) {
    auto f = BgCardFactory();
    auto elementals = f.get_cards_of_race("ELEMENTAL");
    auto card = elementals[RngSingleton::getInstance().get_rand_int() % elementals.size()];
    p1->add_card_to_hand(card);
}

void TavernTempestGolden::do_battlecry(Player* p1) {
    tt.do_battlecry(p1);
    tt.do_battlecry(p1);
}

void TheTideRazor::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(3, p1);
}

std::shared_ptr<BgBaseCard> TheTideRazor::summon() {
    auto f = BgCardFactory();
    auto cards = f.get_cards_of_race("PIRATE");
    auto card = cards[RngSingleton::getInstance().get_rand_int() % cards.size()];
    return card;
}

void TheTideRazorGolden::do_deathrattle(Player* p1, Player* p2) {
    multi_summon(6, p1);
}

std::shared_ptr<BgBaseCard> TheTideRazorGolden::summon() {
    return ttr.summon();
}

void tormented_ritualist_predefense(Player* p1, BgBaseCard* this_, int buff) {
    Board* b1 = p1->get_board().get();
    if (b1->size() <= (unsigned)1) return;
    auto pos = b1->get_pos(this_);
    // Give minions on either side +1/+1
    if (pos == 0) {
        auto to_buff = b1->get_cards()[pos+1];
        to_buff->set_attack(to_buff->get_attack() + buff);
        to_buff->set_health(to_buff->get_health() + buff);
    }
    else if (pos == b1->get_cards().size() - 1) {
        auto to_buff = b1->get_cards()[pos-1];
        to_buff->set_attack(to_buff->get_attack() + buff);
        to_buff->set_health(to_buff->get_health() + buff);
    }
    else { // More than 2 on board and not on ends
        auto to_buff_left = b1->get_cards()[pos-1];
        auto to_buff_right = b1->get_cards()[pos-1];
        to_buff_left->set_attack(to_buff_left->get_attack() + buff);
        to_buff_left->set_health(to_buff_left->get_health() + buff);
        to_buff_right->set_attack(to_buff_right->get_attack() + buff);
        to_buff_right->set_health(to_buff_right->get_health() + buff);
    }
}
                                                                

void TormentedRitualist::do_predefense(std::shared_ptr<BgBaseCard> attacker, Player* p1, Player* p2) {
    BgBaseCard::do_predefense(attacker, p1, p2);
    tormented_ritualist_predefense(p1, this, 1);
}

void TormentedRitualistGolden::do_predefense(std::shared_ptr<BgBaseCard> attacker, Player* p1, Player* p2) {
    BgBaseCard::do_predefense(attacker, p1, p2);
    tormented_ritualist_predefense(p1, this, 2);
}

void Toxfin::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    c->set_poison();
}

bool Toxfin::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "MURLOC";
}

void ToxfinGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    tf.do_targeted_battlecry(c);
}

bool ToxfinGolden::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return tf.is_valid_target(c);
}

void TripleDiscover::cast(Player* p1, uint8_t choice) {
    set_tavern_tier(p1->get_tavern_tier());
    auto choice_str = get_discover_choices()[choice];
    BgCardFactory f;
    auto choice_card = f.get_card(choice_str);
    p1->add_card_to_hand(choice_card);
}

std::vector<std::string> TripleDiscover::get_discover_choices() {
    if (_choices.size() == 0) {
        BgCardFactory f;
        auto cards_by_tier = f.get_card_names_by_tier();
        auto our_tier = tavern_tier < 6 ? tavern_tier + 1 : tavern_tier;
        auto choice_cards = cards_by_tier[our_tier];

        std::unordered_set<int> inds;
        while (inds.size() < 3) {
            auto ind = RngSingleton::getInstance().get_rand_int() % choice_cards.size();
            inds.insert(ind);
        }

        for (auto i : inds) {
            _choices.push_back(choice_cards[i]);
        }
    }
    return _choices;
}

void TwilightEmissary::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    c->set_attack(c->get_attack() + 2);
    c->set_health(c->get_health() + 2);
}

bool TwilightEmissary::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "DRAGON";
}

void TwilightEmissaryGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    twilight_emissary.do_targeted_battlecry(c);
    twilight_emissary.do_targeted_battlecry(c);
}

bool TwilightEmissaryGolden::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return twilight_emissary.is_valid_target(c);
}

void Voidlord::do_deathrattle(Player* p1, Player*) {
    multi_summon(3, p1);
}

std::shared_ptr<BgBaseCard> Voidlord::summon() {
    auto f = BgCardFactory();
    return f.get_card("Voidwalker");
}

void VoidlordGolden::do_deathrattle(Player* p1, Player*) {
    multi_summon(3, p1);
}

std::shared_ptr<BgBaseCard> VoidlordGolden::summon() {
    auto f = BgCardFactory();
    return f.get_card("Voidwalker (Golden)");
}

void UnstableGhoul::do_deathrattle(Player* p1, Player* p2) {
    Board* b1 = p1->get_board().get();
    Board* b2 = p2->get_board().get();
    auto b1_cards = b1->get_cards();
    auto b2_cards = b2->get_cards();
    b1_cards.insert(b1_cards.end(), b2_cards.begin(), b2_cards.end());
    std::vector<std::string> who_from_race(b1_cards.size(), this->get_race());
    BoardBattler b;
    b.take_dmg_simul(b1_cards, who_from_race, 1, p1, p2);
}

void UnstableGhoulGolden::do_deathrattle(Player* p1, Player* p2) {
    for (int i = 0; i < 2; i++) {
        ghoul.do_deathrattle(p1, p2);
    }
}

void VirmenSensei::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    c->set_attack(c->get_attack() + 2);
    c->set_health(c->get_health() + 2);
}

bool VirmenSensei::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return c->get_race() == "BEAST";
}

void VirmenSenseiGolden::do_targeted_battlecry(std::shared_ptr<BgBaseCard> c) {
    virmen_sensei.do_targeted_battlecry(c);
    virmen_sensei.do_targeted_battlecry(c);    
}

bool VirmenSenseiGolden::is_valid_target(std::shared_ptr<BgBaseCard> c) {
    return virmen_sensei.is_valid_target(c);
}

void VulgarHomunculus::do_battlecry(Player* p1) {
    p1->take_damage(2, true);
}

void VulgarHomunculusGolden::do_battlecry(Player* p1) {
    p1->take_damage(4, true);
}

void WaxriderTogwaggle::do_postbattle(Player*,
                                      Player*,
                                      std::vector<std::shared_ptr<BgBaseCard> >,
                                      std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (auto c : dead_b2) {
        if (c->who_killed_race() == "DRAGON") {
            set_health(get_health() + 2);
            set_attack(get_attack() + 2);
        }
    }
}

void WardenOfOld::do_deathrattle(Player* p1, Player* p2) {
    auto f = BgCardFactory();
    p1->add_card_to_hand(f.get_card("Gold Coin"));
}

void WardenOfOldGolden::do_deathrattle(Player* p1, Player* p2) {
    woo.do_deathrattle(p1, p2);
    woo.do_deathrattle(p1, p2);
}

void WaxriderTogwaggleGolden::do_postbattle(Player*,
                                            Player*,
                                            std::vector<std::shared_ptr<BgBaseCard> > ,
                                            std::vector<std::shared_ptr<BgBaseCard> > dead_b2) {
    for (auto c : dead_b2) {
        if (c->who_killed_race() == "DRAGON") {
            set_health(get_health() + 4);
            set_attack(get_attack() + 4);
        }
    }
}

void WildfireElemental::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                      int def_pos,
                                      Player* p1,
                                      Player* p2) {
    Board* b2 = p2->get_board().get();
    auto b2_cards = b2->get_cards();
    if (b2_cards.size() == 0) return;
    if (defender->get_health() < 0) {
        auto damage = -1 * defender->get_health();
        int new_defender_pos = 0;
        if (b2_cards.size() > 1) {
            auto lor = RngSingleton::getInstance().get_rand_int() % 2;
            if (lor) { // left
                new_defender_pos = std::max(0, def_pos - 1);
            }
            else { // right
                // def_pos since defender is dead
                // (everyone shifted left to fill)
                new_defender_pos = std::min(b2_cards.size() - 1, (size_t)def_pos);
            }
        }
        auto new_defender = b2_cards[new_defender_pos];
        BoardBattler().take_dmg_simul(new_defender,
                                      "ELEMENTAL",
                                      damage,
                                      p1,
                                      p2);
    }
}

void WildfireElementalGolden::do_postattack(std::shared_ptr<BgBaseCard> defender,
                                            int def_pos,
                                            Player* p1,
                                            Player* p2) {
    Board* b2 = p2->get_board().get();
    auto b2_cards = b2->get_cards();
    if (b2_cards.size() == 0) return;
    if (defender->get_health() < 0) {   
        auto damage = -1 * defender->get_health();
        int new_defender_pos = 0;
        if (b2_cards.size() == 1) {
            auto new_defender = b2_cards[0];
            BoardBattler().take_dmg_simul(new_defender,
                                          "ELEMENTAL",
                                          damage,
                                          p1,
                                          p2);
        }
        else if ((unsigned)def_pos == b2_cards.size()) { // one already dead
            // b2_cards.size()-1 b/c end minion already dead
            auto new_defender = b2_cards[b2_cards.size()-1];
            BoardBattler().take_dmg_simul(new_defender,
                                          "ELEMENTAL",
                                          damage,
                                          p1,
                                          p2);

        }
        else if ((unsigned)def_pos == 0) {
            // index 1 gauranteed to exist, or first if would be hit
            auto new_defender = b2_cards[1];
            BoardBattler().take_dmg_simul(new_defender,
                                          "ELEMENTAL",
                                          damage,
                                          p1,
                                          p2);

        }
        else {
            auto new_defender_left = b2_cards[def_pos - 1];
            auto new_defender_right = b2_cards[def_pos];
            std::vector<std::shared_ptr<BgBaseCard>> cards{new_defender_left, new_defender_right};
            std::vector<int> dmg{damage, damage};
            std::vector<std::string> race{"ELEMENTAL", "ELEMENTAL"};
            BoardBattler().take_dmg_simul(cards,
                                          race,
                                          dmg,
                                          p1,
                                          p2);
        }
    }
}

int WrathWeaver::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (card->get_race() == "DEMON" && from_hand) {
        set_attack(get_attack() + 2);
        set_health(get_health() + 2);
        return 1;
    }
    return 0;
}

int WrathWeaverGolden::mod_summoned(std::shared_ptr<BgBaseCard> card, Player*, bool from_hand) {
    if (card->get_race() == "DEMON" && from_hand) {
        set_attack(get_attack() + 4);
        set_health(get_health() + 4);
        return 1;
    }    
    return 0;
}

void YoHoOgre::do_postdefense(std::shared_ptr<BgBaseCard> attacker, Player* p1, Player* p2) {
    if (this->is_dead()) {
        return;
    }
    Board* b1 = p1->get_board().get();
    BoardBattler().battle_boards(b1->get_pos(this), p1, p2); // Modifies b1/b2    
}

void YoHoOgreGolden::do_postdefense(std::shared_ptr<BgBaseCard> attacker, Player* p1, Player* p2) {
    if (this->is_dead()) {
        return;
    }
    Board* b1 = p1->get_board().get();
    BoardBattler().battle_boards(b1->get_pos(this), p1, p2); // Modifies b1/b2    
}
