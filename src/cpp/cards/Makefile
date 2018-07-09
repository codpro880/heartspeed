.PHONY: all clean

CARD_CPP_FILES = $(shell find . -name "*.cpp" ! -name "*_test.cpp")
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG
OBJ_TARGETS = $(CARD_CPP_FILES:.cpp=.o)
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG

%.o: %.cpp %.hpp
	clang -std=c++17 FlameLance.cpp -c

all: $(OBJ_TARGETS)

clean:
	rm -f *.o