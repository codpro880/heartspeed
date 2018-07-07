.PHONY: cards test test_all all clean

cards:
	cd cards; $(MAKE)

test: cards
	cd test; $(MAKE) $@

test_all: cards
	cd test; $(MAKE) $@

all: cards

clean:
	cd cards; $(MAKE) clean
	cd test; $(MAKE) clean