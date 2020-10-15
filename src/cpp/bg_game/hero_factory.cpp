#include <memory>
#include <unordered_map>

#include "hero.hpp"
#include "hero_factory.hpp"

std::shared_ptr<Hero> HeroFactory::get_hero(std::string name) {
    auto hero_found_it = heros.find(name);
    if (hero_found_it == heros.end())  {
    	throw std::runtime_error("No hero has name '" + name + "'");
    }
    else {
	return (hero_found_it->second)->get_copy();
    }
}

void HeroFactory::init_heros() {
    heros.emplace("Al'Akir", std::make_shared<AlAkir>());
}
