// #include <algorithm>
// #include <iostream>
// #include <memory>

// #include "PostBattleCards.hpp"
// #include "BgCardFactory.hpp"
// #include "../../bg_game/board.hpp"
// #include "../../bg_game/battler.hpp"

// void ScavagingHyena::do_postbattle(Board* b1, std::vector<std::shared_ptr<BgBaseCard> > new_dead) {
//     for (auto c : new_dead) {
// 	if (c->get_race() == "BEAST") {
// 	    set_health(get_health() + 1);
// 	    set_attack(get_attack() + 2);
// 	}
//     }
// }

// void ScavagingHyenaGolden::do_postbattle(Board* b1, std::vector<std::shared_ptr<BgBaseCard> > new_dead) {
//     for (auto c : new_dead) {
// 	if (c->get_race() == "BEAST") {
// 	    set_health(get_health() + 2);
// 	    set_attack(get_attack() + 4);
// 	}
//     }
// }
