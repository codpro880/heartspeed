.PHONY: cards game test test_all all clean

cards:
	cd cards; $(MAKE)

test: cards game
	cd test; $(MAKE) $@

test_all: cards game
	cd test; $(MAKE) $@

all: cards game

clean:
	cd cards; $(MAKE) clean
	cd game; $(MAKE) clean
	cd test; $(MAKE) clean