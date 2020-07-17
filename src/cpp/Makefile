.PHONY: bg_game cards game test test_all all clean

all: bg_game cards game

bg_game:
	cd bg_game; $(MAKE)

cards:
	cd cards; $(MAKE)

game:
	cd game; $(MAKE)

test: cards game bg_game
	cd test; $(MAKE) $@

test_all: cards game bg_game
	cd test; $(MAKE) $@

clean:
	cd cards; $(MAKE) clean
	cd game; $(MAKE) clean
	cd bg_game; $(MAKE) clean
	cd test; $(MAKE) clean
