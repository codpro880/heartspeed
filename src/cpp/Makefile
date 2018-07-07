.PHONY: cards test all clean

cards:
	cd cards; $(MAKE)

test: cards
	cd test; $(MAKE) test

test_all: cards
	cd test; $(MAKE) test

all: cards

clean:
	cd cards; $(MAKE) clean
	cd test; $(MAKE) clean