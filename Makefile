.PHONY: all clean
all: test

CMD := g++ -std=c++17 -g

test: base.h sequence.h sort.cpp
	$(CMD) sort.cpp -o test
