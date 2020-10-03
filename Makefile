.PHONY: all clean
all: test ui

CMD := g++ -std=c++17 -g

test: base.h sequence.h sort.h sort.cpp
	$(CMD) sort.cpp -o test

ui: base.h sequence.h sort.h menu.h sort.cpp ui.cpp
	$(CMD) ui.cpp -o ui
