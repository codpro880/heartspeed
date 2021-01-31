class Hand {
public:
    Hand(std::vector<std::shared_ptr<BgBaseCard>> cards) : cards(cards) {}
    Hand() {}
    void add_card(std::shared_ptr<BgBaseCard> card) { cards.push_back(card); }
    void remove(int pos) { cards.erase(cards.begin() + pos); }
    std::vector<std::shared_ptr<BgBaseCard>> get_cards() { return cards; }
    // void play_card(std::shared_ptr<BgBaseCard> card,

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
    
    auto size() const { return cards.size(); }

private:
    std::vector<std::shared_ptr<BgBaseCard>> cards;
};
