/*
  Class to replicate what HSTracker is doing, reads in Power.log.
  Mostly used to validate simulator, but might be used to extend bobs
  buddy to show min/max/expected damage, along w/ playouts of each of those
  cases.
*/
#include <string>
#include <utility>

#include "board.hpp"
#include "../cards/bgs/BgCardFactory.hpp"
#include "hero.hpp"

class PyString {
public:
    std::string get_str_between(std::string item, std::string start_token, std::string end_token) {
        // std::string start_token = "[entityName=";
        auto start = item.find(start_token);
        auto real_start = start + start_token.size();
        // auto end = line.find(" id=");
        auto end = item.find(end_token);
        std::string card_name = item.substr(real_start, end - real_start);
        return card_name;
    }
};


// NOTES:
// m_chosenEntities == which triple reward was chosen
// Reborn:
// D 08:18:51.3434270 GameState.DebugPrintPower() -     TAG_CHANGE Entity=[entityName=Reborn Rites id=2442 zone=PLAY zonePos=0 cardId=TB_BaconShop_HP_024 player=16] tag=TAG_SCRIPT_DATA_NUM_1 value=1 
// D 08:18:51.3452210 GameState.DebugPrintPower() -         TAG_CHANGE Entity=[entityName=Glyph Guardian id=2432 zone=PLAY zonePos=1 cardId=BGS_045 player=16] tag=REBORN value=1
// Stats:
// Each 'battle' starts with:
// D 08:18:55.5798440 PowerTaskList.DebugDump() - ID=1662 ParentID=1630 PreviousID=0 TaskCount=234
// D 08:18:55.5802390 PowerTaskList.DebugPrintPower() - BLOCK_START BlockType=TRIGGER Entity=[entityName=BaconShop8PlayerEnchant id=71 zone=PLAY zonePos=0 cardId=TB_BaconShop_8P_PlayerE player=8] EffectCardId=System.Collections.Generic.List`1[System.String] EffectIndex=9 Target=0 SubOption=-1 TriggerKeyword=0
// D 08:18:55.5803010 PowerTaskList.DebugPrintPower() -     TAG_CHANGE Entity=GameEntity tag=1453 value=1
// Each card within battle:
// GET NAME: D 08:18:55.5863080 PowerTaskList.DebugPrintPower() -     FULL_ENTITY - Updating [entityName=Hangry Dragon id=2437 zone=PLAY zonePos=5 cardId=BGS_033 player=16] CardID=BGS_033
// REMOVE: D 08:18:55.5867780 PowerTaskList.DebugPrintPower() -     FULL_ENTITY - Updating [entityName=UNKNOWN ENTITY [cardType=INVALID] id=2438 zone=SETASIDE zonePos=0 cardId= player=16] CardID=
// D 08:18:55.5872800 PowerTaskList.DebugPrintPower() -     TAG_CHANGE Entity=[entityName=Hangry Dragon id=2437 zone=PLAY zonePos=5 cardId=BGS_033 player=16] tag=HEALTH value=6 
// D 08:18:55.5873340 PowerTaskList.DebugPrintPower() -     TAG_CHANGE Entity=[entityName=Hangry Dragon id=2437 zone=PLAY zonePos=5 cardId=BGS_033 player=16] tag=ATK value=6 
// ... BLOCK_END
class BobsBuddy {
public:
    BobsBuddy(std::string power_log, bool debug=false) : _power_log(power_log), debug(debug) {}
    std::vector<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>> parse_full_log();
private:
    std::string _power_log;
    std::vector<std::string> get_file_contents();
    std::vector<std::vector<std::string>> get_chunks(std::vector<std::string>);
    std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>> parse_chunk(std::vector<std::string>);
    std::shared_ptr<Hero> get_their_hero(std::vector<std::string> chunk);
    int get_their_hero_power_pos(std::shared_ptr<Hero> h, std::vector<std::string> chunk);
    PyString pystr;
    BgCardFactory card_factory;
    bool debug;
};
