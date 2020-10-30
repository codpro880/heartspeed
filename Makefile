# Makefile for heartspeed
# Make is not the best build system, but it works well enough (for now)
# If anyone wants to update to cmake, go for it

# TODO: Define an install/executable rule
CARD_CPP_FILES = $(shell find . -name "*.cpp" ! -name "*_test.cpp")
OBJ_TARGETS = $(CARD_CPP_FILES:.cpp=.o)

#export CXX = /usr/local/opt/llvm/bin/clang
export CXX = clang

.PHONY: clean help test

all:
	cd src; make $@

main.o: main.cpp
	$(CXX) -std=c++17 -O3 $< -c

# main: $(OBJ_TARGETS) main.o
# 	$(CXX) -std=c++17 -O3 -o bb_par $^ -L/usr/local/lib -fopenmp

main: $(OBJ_TARGETS) main.o
	$(CXX) -std=c++17 -O3 -o bb_par $^

clean:  ## Clean up object files, executables, and other temps
	cd src; make $@

help:  ## Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

test:  ## Run the unit tests
	cd src; make $@
