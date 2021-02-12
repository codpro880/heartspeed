.PHONY: bg_game cards game main premain test test_all all clean

CPP_FILES = $(shell find . -name "*.cpp" ! -name "*_test.cpp")
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG
OBJ_FILES = $(CPP_FILES:.cpp=.o)
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG

#LIBRARY_PATH=/usr/local/Cellar/tbb/2020_U3_1/lib:$LIBRARY_PATH
#DYLD_LIBRARY_PATH=/usr/local/Cellar/tbb/2020_U3_1/lib:$LIBRARY_PATH

# Matt's local rule for faster builds.
# I'll work incremental builds in docker when enough people complain...
export CXX = /usr/local/opt/llvm/bin/clang++
# export CXX = clang++
export OPENMP_FLAGS = -fopenmp

################################################################
###### You'll mainly want to build with the docker_* rules. ####
################################################################

all: ## Builds everything (default rule)
all: bg_game cards

# premain: all

# main: $(OBJ_TARGETS)
# 	clang -std=c++17 -O3 -o bb_par $^

bg_game: ## Build game
	cd bg_game; $(MAKE)

cards:  ## Build cards
	cd cards; $(MAKE)

clean:  ## Clean up object files and other build artifacts
	cd cards; $(MAKE) clean
	cd bg_game; $(MAKE) clean
	cd test; $(MAKE) clean

cli: all
	$(CXX) -std=c++17 -O3 main.cpp -c
	$(CXX) -std=c++17 -O3 -o main $(OBJ_FILES) -L/usr/local/lib $(OPENMP_FLAGS)


docker_rl_build:
	docker build -t bgs_rl_cli -f Dockerfile.rl_cli

docker_build: ## Build inside of docker container
	docker build -t bgs -f Dockerfile.test .

docker_test: docker_build ## Run automated tests inside of docker container
	docker run -it bgs

help:  ## Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

test: ## Run unit tests
test: cards game bg_game
	cd test; $(MAKE) $@

test_all: ## Run all tests
test_all: cards game bg_game
	cd test; $(MAKE) $@
