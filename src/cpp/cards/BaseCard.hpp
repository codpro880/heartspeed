#include <string>
#include <unordered_map>

class BaseCard {
public:
    BaseCard(){}
    //int get_damage() = 0;
    //int get_health() = 0;
    virtual std::string get_name() = 0;
    //HsClass get_class() = 0;
    virtual int get_cost() = 0;
    virtual std::string get_id() = 0;
    //HsSet get_set() = 0;
    virtual std::string get_text() = 0;
    //HsType get_type() = 0;  
    //PlayReqs get_play_requirements() = 0;
private:

};

// class PlayReqs {
// public:
//     PlayReqs(std::unordered_map);
// };
