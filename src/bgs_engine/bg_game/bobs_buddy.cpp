#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "bobs_buddy.hpp"
#include "hero_factory.hpp"
#include "hero.hpp"
#include "../utils/pyutils.hpp"

std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> BobsBuddy::parse_full_log() {
    std::vector<std::string> file_contents = get_file_contents();
    std::vector<std::vector<std::string>> chunks = get_chunks(file_contents);
    std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> res;
    auto count = 0;
    for (auto chunk : chunks) {
        std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>> pair = parse_chunk(chunk);    
        res.push_back(pair);
        count++;
        if (count == 10) break;
    }
    return res;
}

std::vector<std::string> BobsBuddy::get_file_contents() {
    std::string line;
    std::ifstream myfile (_power_log);
    std::vector<std::string> lines;
    if (myfile.is_open())
        {
            while ( getline (myfile,line) )
                {
                    lines.push_back(line);
                }
            myfile.close();
        }
    else {
        throw std::runtime_error("Unable to open power log at: '" + _power_log + "'");
    }
    return lines;
}

std::vector<std::vector<std::string>> BobsBuddy::get_chunks(std::vector<std::string> file_contents) {
    int line_count = 0;
    std::vector<int> starts;
    std::vector<int> real_starts;
    std::vector<int> ends;
    for (auto line : file_contents) {
        if (pyutils::in("BLOCK_START", line)) {
            starts.push_back(line_count);
        }
        else if (pyutils::in("BLOCK_END", line)) {
            if (starts.size() > 0) {
                real_starts.push_back(starts.back());
                ends.push_back(line_count);
            }
        }
        line_count++;
    }

    std::vector<std::vector<std::string>> possible_chunks;
    for (int i = 0; i < ends.size(); i++) {
        std::vector<std::string> chunk(file_contents.begin() + real_starts[i],
                                       file_contents.begin() + ends[i]);
        possible_chunks.push_back(chunk);
    }
    
    std::vector<std::vector<std::string>> chunks;
    for (auto possible_chunk : possible_chunks) {
        for (auto line : possible_chunk) {
            if (pyutils::in("FULL_ENTITY - Updating [entityName=", line)) {
                auto card_name = pyutils::get_str_between(line, "[entityName=", " id=");
                try {
                    card_factory.get_card(card_name);
                    chunks.push_back(possible_chunk);
                    break;
                }
                catch (...) {
                }
            }
        }
    }

    // Filter out chunks w/ SUB_SPELL_START in them
    std::vector<std::vector<std::string>> wo_subspell_chunks;
    auto pred = [](std::vector<std::string> chunk) {
                    for (auto line : chunk) {
                        if (pyutils::in("SUB_SPELL_START", line)) {
                            return false;
                        }
                        else if (pyutils::in("SUB_SPELL_END", line)) {
                            return false;
                        }
                    }
                    return true;
                };
    std::copy_if(chunks.begin(), chunks.end(), std::back_inserter(wo_subspell_chunks), pred);

    // Filter out blockType that aren't TRIGGER
    std::vector<std::vector<std::string>> trigger_blocks;
    auto pred_trig = [](std::vector<std::string> chunk) {
                         auto line = chunk[0];
                         return pyutils::in("BLOCK_START BlockType=TRIGGER", line);
                };
    std::copy_if(wo_subspell_chunks.begin(), wo_subspell_chunks.end(), std::back_inserter(trigger_blocks), pred_trig);    
    return trigger_blocks;
}

std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>> BobsBuddy::parse_chunk(std::vector<std::string> chunk) {
    std::map<int, std::shared_ptr<BgBaseCard>> our_id_to_card;
    std::map<int, std::shared_ptr<BgBaseCard>> their_id_to_card;

    // TAG_CHANGE Entity=[entityName=Rabid Saurolisk id=3868 zone=PLAY zonePos=4 cardId=BGS_075 player=16] tag=TAUNT value=1

    auto our_zone_pos = 0;
    auto their_zone_pos = 0;
    if (debug) {
        std::cerr << std::endl;
        std::cerr << std::endl;
        std::cerr << "CHUNK :" << std::endl;
    }
    for (auto line : chunk) {
        if (debug) std::cerr << line << std::endl;
        if (pyutils::in("FULL_ENTITY - Updating", line)) {
            auto is_golden = pyutils::in("BaconUps", pyutils::get_str_between(line, "cardId=", " player="));
            auto card_name = pyutils::get_str_between(line, "entityName=", " id=");
            if (is_golden) {
                card_name += " (Golden)";
            }
            std::shared_ptr<BgBaseCard> card;
            try {
                card = card_factory.get_card(card_name);
            }
            catch (...) {
                continue;
            }
            auto player = pyutils::get_str_between(line, "player=", "]");
            if (player == "8") {
                our_id_to_card[++our_zone_pos] = card;
            }
            else {
                their_id_to_card[++their_zone_pos] = card;
            }
        }
        else if (pyutils::in("TAG_CHANGE", line)) {
            auto tag = pyutils::get_str_between(line, "tag=", " value");
            if (tag == "HEALTH") {
                // auto zone_pos = atoi(pystr.get_str_between(line, "zonePos=", " cardId").c_str());
                auto health = atoi(pyutils::get_str_between(line, "value=", " ").c_str());
                auto player = pyutils::get_str_between(line, "player=", "]");
                if (player == "8") {
                    our_id_to_card[our_zone_pos]->set_health(health);
                }
                else {
                    their_id_to_card[their_zone_pos]->set_health(health);
                }
            }
            else if (tag == "ATK") {
                auto player = pyutils::get_str_between(line, "player=", "]");
                auto attack = atoi(pyutils::get_str_between(line, "value=", " ").c_str());
                if (player == "8") {
                    our_id_to_card[our_zone_pos]->set_attack(attack);
                }
                else {
                    their_id_to_card[their_zone_pos]->set_attack(attack);
                }
            }
            else if (tag == "TAUNT") {
                auto player = pyutils::get_str_between(line, "player=", "]");
                if (player == "8") {
                    our_id_to_card[our_zone_pos]->set_taunt();
                }
                else {
                    their_id_to_card[their_zone_pos]->set_taunt();
                }
                
            }
        }
    }
    
    std::vector<std::shared_ptr<BgBaseCard>> our_cards;
    for (auto kv : our_id_to_card) {
        our_cards.push_back(kv.second);
    }

    std::vector<std::shared_ptr<BgBaseCard>> their_cards;
    for (auto kv : their_id_to_card) {
        their_cards.push_back(kv.second);
    }

    auto our_board = std::make_shared<Board>(our_cards);
    auto their_board = std::make_shared<Board>(their_cards);

    // Hero battle considerations, like al-akir
    // TODO: get our hero in here...
    std::shared_ptr<Hero> their_hero = get_their_hero(chunk);
    int pos = get_their_hero_power_pos(their_hero, chunk);
    their_hero->apply_hero_power(their_board, pos);

    auto pair = std::make_pair(our_board, their_board);
    return pair;
}

std::shared_ptr<Hero> BobsBuddy::get_their_hero(std::vector<std::string> chunk) {
    auto hero_fac = HeroFactory();
    std::shared_ptr<Hero> hero(new Hero("Default"));
    for (auto line : chunk) {
        if (pyutils::in("FULL_ENTITY", line)) {
            auto hero_name = pyutils::get_str_between(line, "entityName=", " id=");
            try {
                hero = hero_fac.get_hero(hero_name);
                return hero;
            }
            catch (...) {
            }
        }
    }
    return hero;
}

int BobsBuddy::get_their_hero_power_pos(std::shared_ptr<Hero> hero, std::vector<std::string> chunk) {
    if (hero->get_name() == "The Lich King") {
        for (auto line : chunk) {
            if (pyutils::in("FULL_ENTITY - Updating [entityName=Reborn Rites", line)) {
                int zone_pos = atoi(pyutils::get_str_between(line, "zonePos=", " cardId=").c_str());
                return zone_pos;
            }
        }
    }
    return -1;
}
