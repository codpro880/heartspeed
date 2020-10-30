.PHONY: bg_game cards game main premain test test_all all clean

CARD_CPP_FILES = $(shell find . -name "*.cpp" ! -name "*_test.cpp")
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG
OBJ_TARGETS = $(CARD_CPP_FILES:.cpp=.o)
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG

#LIBRARY_PATH=/usr/local/Cellar/tbb/2020_U3_1/lib:$LIBRARY_PATH
#DYLD_LIBRARY_PATH=/usr/local/Cellar/tbb/2020_U3_1/lib:$LIBRARY_PATH


all: bg_game cards game

# premain: all

main: $(OBJ_TARGETS)
	clang -std=c++17 -O3 -o bb_par $^

bg_game:
	cd bg_game; $(MAKE)

cards:
	cd cards; $(MAKE)

test: cards game bg_game
	cd test; $(MAKE) $@

test_all: cards game bg_game
	cd test; $(MAKE) $@

clean:
	cd cards; $(MAKE) clean
	cd bg_game; $(MAKE) clean
	cd test; $(MAKE) clean
