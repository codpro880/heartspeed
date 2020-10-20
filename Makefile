# Makefile for heartspeed
# Make is not the best build system, but it works well enough (for now)
# If anyone wants to update to cmake, go for it

# TODO: Define an install/executable rule

.PHONY: clean help test

all: test

clean:  ## Clean up object files, executables, and other temps
	cd src; make $@

help:  ## Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

test:  ## Run the unit tests
	cd src; make $@
