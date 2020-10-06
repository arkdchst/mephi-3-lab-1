.PHONY: all
.DEFAULT_GOAL := all
all: ui

CMD := g++ -std=c++17 -g

ui: base.h sequence.h sort.h menu.h ui.cpp
	$(CMD) ui.cpp -pthread -o ui
