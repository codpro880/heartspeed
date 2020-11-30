# About
Heartspeed is a Hearthstone simulator written in C++ (for speed). Ultimate goal is to use this simulator to train a bot using reinforcement learning: so simulations need to happen fast.
It's currently under construction: not officially released.

# Building

## Prereqs

You'll need a working installation of Docker.
GNU Make will also make your life easier, but it's not strictly required (you can copy the commands)

## Valid Build Targets (GNU Make)

Please use `make help` to get a valid list of targets. Note that unless you want to build/run outside of docker, you'll
only be using the `make docker_*` rules in order to build/run.

You can execute `make docker_build` in order to build the container within the `src/cpp` directory.
`make docker_test` is similar, but also runs the automated tests, which is the main driver when developing.

Should see something like:

```
Matthews-MacBook-Pro:cpp matt$ make docker_test
docker build -t bgs -f Dockerfile.test .
[+] Building 541.1s (28/28) FINISHED                                                                                                                          
 => [internal] load build definition from Dockerfile.test                                                                                                0.0s
 => => transferring dockerfile: 839B                                                                                                                     0.0s
 => [internal] load .dockerignore                                                                                                                        0.0s
 => => transferring context: 2B                                                                                                                          0.0s
 => [internal] load metadata for docker.io/library/ubuntu:20.04                                                                                          0.8s
 => [internal] load build context                                                                                                                        0.9s
 => => transferring context: 18.81MB
...
...
...
...
[----------] Global test environment tear-down
[==========] 131 tests from 7 test cases ran. (8751 ms total)
[  PASSED  ] 131 tests.
```

List of all valid targets (`make help`):

```
Matthews-MacBook-Pro:cpp matt$ make help
##############################################################
#### You'll mainly want to build with the docker_* rules. ####
##############################################################
all:  Builds everything (default rule)
bg_game:  Build game
cards:   Build cards
clean:   Clean up object files and other build artifacts
docker_build:  Build inside of docker container
docker_test: docker_build  Run automated tests inside of docker container
help:   Show this help.
test:  Run unit tests
test_all:  Run all tests
```

# Contributing

## General Contributions

All contributions of any size/shape are welcome. Please open a pull request, and I'll review it when I have the time.
If the change is substantial (50+ lines of diff), please include an automated test to verify the behavior you're adding.
Even if the change is small, a test is ideal, but trivial changes do not require an automated test.

There's an Asana board with issues on it. If you're interested in contributing, please contact me. My email is messersmith.matt at gmail.

## Testing

We use googletest. Build system expects test files to end with `_test.cpp`. Currently tests are sitting next to the production code.

## Visualization Contributions

In order to debug and watch rollouts, a visualization feature is going to be required. Preferably this would be in browser, and
so Javascript seems like a natural choice.

## ML Contributions

Will need some MTCS, RL, and a way to farm this compute out. Initial thoughts would entail wrapping the simulator in Python (pybind11),
and use Spark. Another idea on this space is to use GPU where possible. I have shyed away from GPU programming, though, because I don't know
a lot about it. Might pivot to it after a working implementation in C++.
