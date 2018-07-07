#include "FlameLance.hpp"

FlameLance::FlameLance() : name("Flame Lance"),
			   cost(5),
			   id("AT_001"),
			   text("Deal $8 damage to a minion.") {}

std::string FlameLance::get_name() { return name; }
int FlameLance::get_cost() { return cost; }
std::string FlameLance::get_id() { return id; }
std::string FlameLance::get_text() { return text; }
