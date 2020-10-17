#pragma once

#include "hero.hpp"

class HeroFactory {
public:
    HeroFactory() { init_heros(); };
    std::shared_ptr<Hero> get_hero(std::string name);
private:
    void init_heros();
    std::unordered_map<std::string, std::shared_ptr<Hero> > heros;
};
