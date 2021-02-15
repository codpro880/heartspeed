#include <algorithm>
#include <string>
#include <vector>

#include "../cards/bgs/BgBaseCard.hpp"

class Hand {
public:
    Hand(std::vector<std::shared_ptr<BgBaseCard>> cards) : cards(cards) {}
    Hand() {}
    void add_card(std::shared_ptr<BgBaseCard> card) { cards.push_back(card); }
    void remove(int pos) { cards.erase(cards.begin() + pos); }
    std::vector<std::shared_ptr<BgBaseCard>> get_cards() { return cards; }

    // TOOD: Condense copy/pasta w/ board
    auto get_pos(std::shared_ptr<BgBaseCard> c) const {
        auto it = std::find(cards.begin(), cards.end(), c);
        return std::distance(cards.begin(), it);
    }
    
    bool contains(std::shared_ptr<BgBaseCard> c) const {
        auto pos = get_pos(c);
        return pos != -1 && (unsigned)pos != cards.size();
    }
    
    void remove(std::shared_ptr<BgBaseCard> c) {
        auto it = std::find(cards.begin(), cards.end(), c);
        cards.erase(it);
    }

    nlohmann::json to_json() {
        nlohmann::json j;
        for (int i = 0; (unsigned)i < cards.size(); i++) {
            j["cards"][i] = cards[i]->to_json();
        }
        // I don't think any of the other attrs are ever useful to serialize
        // They're used in battle and cleared at end of battle, or can be calculated
        return j;
    }

    static Hand from_json(std::string infile) {
        std::ifstream i(infile);
        nlohmann::json j;
        i >> j;
        
        std::vector<std::shared_ptr<BgBaseCard> > deserialized_cards;
        for (auto card_json : j["cards"]) {
            auto card = std::make_shared<BgBaseCard>(BgBaseCard::from_json(card_json));
            deserialized_cards.push_back(card);
        }
        auto res_hand = Hand(deserialized_cards);
        return res_hand;
    }     
    
    auto size() const { return cards.size(); }

private:
    std::vector<std::shared_ptr<BgBaseCard>> cards;
};
