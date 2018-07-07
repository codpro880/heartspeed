# About
Heartspeed is a Hearthstone simulator written in C++ (for speed). Ultimate goal is to use this simulator to train a bot using reenforcement learning: so simulations need to happen fast.

# Building
You can simply type "make" in order to build. However, I recommend building with the test rule, i.e. "make test". This will run the unit tests for the project.
If you have more time, consider running "make test_all" in order to run ALL the tests (integration/end-to-end tests will run as well).

## Valid Build Targets
"make" -- Install the Python heartspeed package and builds the heartspeed C++ libs as well
"make test" -- Does everything "make" does, plus runs the unit tests
"make test_all" -- Does everything "make" does, runs ALL the tests (unit/integration/end-to-end)

# Contributing
All contributions of any size/shape are welcome. Please open a pull request, and I'll review it when I have the time.
If the change is substantial (50+ lines of diff), please include an automated test to verify the behavior you're adding.
Even if the change is small, a test is ideal, but trivial changes do not require an automated test.