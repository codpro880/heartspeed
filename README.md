# About
Heartspeed is a Hearthstone simulator written in C++ (for speed). Ultimate goal is to use this simulator to train a bot using reinforcement learning: so simulations need to happen fast.
It's currently under construction: not officially released.

# Building

## Prereqs

You'll need a working installation fo the clang compiler and GNU Make. These ship with MacOS.

## Valid Build Targets (GNU Make)

NOTE: I recently ruined building for others for the sake of OpenMP. I'll eventually get this building w/ Cmake and in a Docker container...
For now, you can replace the CXX = ... line in the main Makefile to point to your compiler.

You can simply type "make" in order to build. This will also run the unit tests (googletest) for the project.
Should see something like:

```
(base) Matthews-MacBook-Pro:heart_speed matt$ make
cd src; make test
cd cpp; make test
cd cards; /Library/Developer/CommandLineTools/usr/bin/make
cd standard; make all
clang -std=c++17 CardFactory.cpp -c
cd bgs; make all
clang -std=c++17 BgCardFactory.cpp -c
clang -std=c++17 BgBaseCard.cpp -c
clang -std=c++17 BgCards.cpp -c
cd game; /Library/Developer/CommandLineTools/usr/bin/make
clang -std=c++17 player.cpp -c
clang -std=c++17 deck.cpp -c
clang -std=c++17 game.cpp -c
cd bg_game; /Library/Developer/CommandLineTools/usr/bin/make
clang -std=c++17 player.cpp -c
clang -std=c++17 battler.cpp -c
clang -std=c++17 bobs_buddy.cpp -c
clang -std=c++17 board.cpp -c
clang -std=c++17 hero_factory.cpp -c
clang -std=c++17 hero.cpp -c
cd test; /Library/Developer/CommandLineTools/usr/bin/make test
c++ -isystem googletest/include -std=c++17 -g -Wall -Wextra -pthread -c ../bg_game/battler_test.cpp
c++ -isystem googletest/include -std=c++17 -g -Wall -Wextra -pthread -c ../bg_game/board_test.cpp
c++ -isystem googletest/include -std=c++17 -g -Wall -Wextra -pthread -c ../bg_game/bobs_buddy_test.cpp
c++ -isystem googletest/include -std=c++17 -g -Wall -Wextra -pthread -c ../cards/bgs/BgCardFactory_test.cpp
c++ -isystem googletest/include -std=c++17 -g -Wall -Wextra -pthread -c ../cards/standard/CardFactory_test.cpp
c++ -isystem googletest/include -std=c++17 -g -Wall -Wextra -pthread -c ../game/deck_test.cpp
c++ -isystem googletest/include -std=c++17 -g -Wall -Wextra -pthread -c ../game/game_test.cpp
...
...
...
[----------] 1 test from GameObj
[ RUN      ] GameObj.CanStart
[       OK ] GameObj.CanStart (1 ms)
[----------] 1 test from GameObj (1 ms total)

[----------] Global test environment tear-down
[==========] 96 tests from 8 test cases ran. (13117 ms total)
[  PASSED  ] 96 tests.
```

To get all the valid build targets, do `make help`:

```
(base) Matthews-MacBook-Pro:heart_speed matt$ make help
clean:   Clean up object files, executables, and other temps
help:   Show this help.
test:   Run the unit tests
```

# Contributing

## General Contributions

All contributions of any size/shape are welcome. Please open a pull request, and I'll review it when I have the time.
If the change is substantial (50+ lines of diff), please include an automated test to verify the behavior you're adding.
Even if the change is small, a test is ideal, but trivial changes do not require an automated test.

## Testing

We use googletest. Build system expects test files to end with `_test.cpp`. Currently tests are sitting next to the production code.

## Visualization Contributions

In order to debug and watch rollouts, a visualization feature is going to be required. Preferably this would be in browser, and
so Javascript seems like a natural choice.

## ML Contributions

Will need some MTCS, RL, and a way to farm this compute out. Initial thoughts would entail wrapping the simulator in Python (pybind11),
and use Spark. Another idea on this space is to use GPU where possible. I have shyed away from GPU programming, though, because I don't know
a lot about it. Might pivot to it after a working implementation in C++.
