.PHONY: all clean

CARD_CPP_FILES = $(shell find . -name "*.cpp" ! -name "*_test.cpp")
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG
OBJ_TARGETS = $(CARD_CPP_FILES:.cpp=.o)
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG

%.o: %.cpp %.hpp
	$(CXX) -std=c++17 -O3 $< -c $(OPENMP_FLAGS)

all: $(OBJ_TARGETS)

clean:
	rm -f *.o
