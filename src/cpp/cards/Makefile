.PHONY: all clean

FlameLance.o: FlameLance.cpp FlameLance.hpp
	clang -std=c++17 FlameLance.cpp -c

all: FlameLance.o

clean:
	rm -f *.o