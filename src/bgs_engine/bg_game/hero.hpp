#pragma once

#include <string>
#include <memory>

#include "board.hpp"

class Hero {
public:
    Hero(std::string name) : _name(name) {};
    virtual void apply_hero_power(std::shared_ptr<Board>, int) {};
    std::string get_name() { return _name; }
    virtual std::shared_ptr<Hero> get_copy() { return std::make_shared<Hero>(*this); } // copy boilerplate
    virtual ~Hero() {};
private:
    std::string _name;
};

class AlAkir : public Hero {
public:
    AlAkir() : Hero("Al'Akir") {}
    void apply_hero_power(std::shared_ptr<Board> b, int pos) override;
    std::shared_ptr<Hero> get_copy() override { return std::make_shared<AlAkir>(*this); } // copy boilerplate
};

class LichKing : public Hero {
public:
    LichKing() : Hero("The Lich King") {}
    void apply_hero_power(std::shared_ptr<Board> b, int pos) override;
    std::shared_ptr<Hero> get_copy() override { return std::make_shared<LichKing>(*this); } // copy boilerplate
};
