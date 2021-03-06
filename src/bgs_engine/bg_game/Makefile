.PHONY: all clean

CARD_CPP_FILES = $(shell find . -name "*.cpp" ! -name "*_test.cpp")
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG
OBJ_TARGETS = $(CARD_CPP_FILES:.cpp=.o)
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG

# CXX = /usr/local/opt/llvm/bin/clang
# main.c -L/usr/local/lib -fopenmp

%.o: %.cpp %.hpp
	$(CXX) -std=c++17 $(OPTIM_FLAGS) $< -c $(OPENMP_FLAGS)

all: $(OBJ_TARGETS)

# bob_main.o: bob_main.cpp
# 	$(CXX) -std=c++17 -O3 $< -c -ltbb	

# bob: bob_main.o bobs_buddy.o hero_factory.o ../cards/BgCardFactory.o
# 	$(CXX) -std=c++17 -O3 -o bb_par $^

clean:
	rm -f *.o
