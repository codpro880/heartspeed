.PHONY: all bg_game cards clean clean_cli cli main premain test test_all

CPP_FILES = $(shell find . -name "*.cpp" ! -name "*_test.cpp")
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG
OBJ_FILES = $(CPP_FILES:.cpp=.o)
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG

#LIBRARY_PATH=/usr/local/Cellar/tbb/2020_U3_1/lib:$LIBRARY_PATH
#DYLD_LIBRARY_PATH=/usr/local/Cellar/tbb/2020_U3_1/lib:$LIBRARY_PATH

# Matt's local rule for faster builds.
# I'll work incremental builds in docker when enough people complain...
export CXX = clang++
# export CXX = /usr/local/opt/llvm/bin/clang++
export OPENMP_FLAGS = -fopenmp
export OPTIM_FLAGS = -O3
# export OPTIM_FLAGS = -g

################################################################
###### You'll mainly want to build with the docker_* rules. ####
################################################################

all: ## Builds everything (default rule)
all: bg_game cards

bg_game: ## Build bg_game
	cd bg_game; $(MAKE)

cards:  ## Build cards
	cd cards; $(MAKE)

clean:  ## Clean up object files and other build artifacts
	cd cards; $(MAKE) clean
	cd bg_game; $(MAKE) clean
	cd test; $(MAKE) clean

clean_cli:  ## Cleans up the main object file and executable of bgscli
	rm -f bgscli
	rm -f p1.json
	rm -f p2.json
	rm -f main.o

cli: ## Build the cli executable (bgscli)
cli: all clean_cli
	$(CXX) -std=c++17 $(OPTIM_FLAGS) main.cpp -c
	$(CXX) -std=c++17 $(OPTIM_FLAGS) -o bgscli $(OBJ_FILES) -L/usr/local/lib $(OPENMP_FLAGS)

docker_cli: ## Builds Docker image which runs bgscli executable
	docker build -t bgs_cli -f Dockerfile.cli .

docker_build: ## Build inside of docker container
	docker build -t bgs -f Dockerfile.test .

docker_test: ## Run automated tests inside of docker container
docker_test: docker_build 
	docker run -it bgs

help:  ## Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

test: ## Run unit tests
test: cards bg_game
	cd test; $(MAKE) $@

test_all: ## Run all tests
test_all: cards bg_game
	cd test; $(MAKE) $@

