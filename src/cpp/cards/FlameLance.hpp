#include "BaseCard.hpp"

class FlameLance : public BaseCard {
public:
    FlameLance();
    std::string get_name();
    int get_cost();
    std::string get_id();
    std::string get_text();
private:
    std::string name;
    int cost;
    std::string id;
    std::string text;
};
