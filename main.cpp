#include <filesystem>
#include <vector>

#include "src/cpp/bg_game/bobs_buddy.hpp"
#include "src/cpp/bg_game/player.hpp"
#include "src/cpp/bg_game/battler.hpp"

int main() {
    std::filesystem::path power_log = std::filesystem::current_path() / "test_data" / "Power.log";
    auto bb = BobsBuddy(power_log.string());
    auto battle_boards = bb.parse_full_log();

    // auto battle_boards = *battle_boards_p;
    auto our_eighth_board = battle_boards[7].first;
    auto their_eighth_board = battle_boards[7].second;

    // ours
    auto our_eighth_rockpool = our_eighth_board->get_cards()[0];
    auto our_eighth_salty_looter1 = our_eighth_board->get_cards()[1];
    auto our_eighth_salty_looter2 = our_eighth_board->get_cards()[2];
    auto our_eighth_bronzew1 = our_eighth_board->get_cards()[3];
    auto our_eighth_bronzew2 = our_eighth_board->get_cards()[4];
    auto our_eighth_stasis = our_eighth_board->get_cards()[5];
    auto our_eighth_yoho = our_eighth_board->get_cards()[6];
    
    // theirs
    auto their_eighth_spawn = their_eighth_board->get_cards()[0];
    auto their_eighth_tabby1 = their_eighth_board->get_cards()[1];
    auto their_eighth_tabby2 = their_eighth_board->get_cards()[2];
    auto their_eighth_selfless = their_eighth_board->get_cards()[3];
    auto their_eighth_scav = their_eighth_board->get_cards()[4];
    auto their_eighth_crackling = their_eighth_board->get_cards()[5];
    auto their_eighth_baron = their_eighth_board->get_cards()[6];

    // Give some tolerance, but bobs buddy gave:
    // Lethal: 13.6, Win 99.7, Tie .2, Loss .1, Lethal 0
    std::unique_ptr<Player> p1(new Player(our_eighth_board, "Ours"));
    std::unique_ptr<Player> p2(new Player(their_eighth_board, "Theirs"));
    auto battler = Battler(p1.get(), p2.get());
    auto res = battler.sim_battles_par(100000);
    std::cerr << "P1 win: " << res.p1_win << std::endl;
    std::cerr << "draw: " << res.draw << std::endl;
    std::cerr << "p2 win: " << res.p2_win << std::endl;
}
