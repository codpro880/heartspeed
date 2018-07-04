.PHONY: all clean install test uninstall

all: install

clean:
	rm -rf build
	rm -rf dist
	rm -rf *.egg-info

install:
	python setup.py install

test: install
	python -B -m pytest -v -s test/unit

test_int:
	python -B -m pytest -v -s test/integration

test_all: install
	python -B -m pytest -v -s test

uninstall:
	pip uninstall HeartSpeed