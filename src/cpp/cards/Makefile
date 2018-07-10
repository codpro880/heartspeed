.PHONY: all clean

#CARD_CPP_FILES = $(shell find . -name "*.cpp" ! -name "*_test.cpp")
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG
#OBJ_TARGETS = $(CARD_CPP_FILES:.cpp=.o)
# $(info $$CARD_CPP_FILES is [${CARD_CPP_FILES}]) # DEBUG

%.o: %.cpp %.hpp
	clang -std=c++17 $< -c

all: CardFactory.o

clean:
	rm -f *.o