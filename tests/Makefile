GRB_DIR=../include

all: tests

run: all
	./tests

test: all
	./tests

tests: tests.cpp
	g++ tests.cpp -o tests -std=gnu++17 -O3 -I$(GRB_DIR)

clean:
	rm -fv tests
