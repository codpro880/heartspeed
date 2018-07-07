.PHONY: all clean

%.o: %.cpp %.hpp
	clang -std=c++17 FlameLance.cpp -c

all: FlameLance.o

clean:
	rm -f *.o