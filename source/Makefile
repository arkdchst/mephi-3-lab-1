.PHONY: all clean
.DEFAULT_GOAL := all

G++ := g++ -std=c++17

LIB_PATH := ../lib
INCLUDE_PATH := ../include
BIN_PATH := ../bin

all: $(BIN_PATH)/ui $(BIN_PATH)/tests

$(BIN_PATH)/ui: base.hpp base.tpp sequence.hpp sequence.tpp sort.hpp menu.hpp ui.cpp
	mkdir -p $(BIN_PATH)
	$(G++) ui.cpp -pthread -o $(BIN_PATH)/ui

$(BIN_PATH)/tests: base.hpp base.tpp sequence.hpp sequence.tpp sort.hpp $(LIB_PATH)/gtest $(INCLUDE_PATH)/gtest tests.cpp
	mkdir -p $(BIN_PATH)
	$(G++) tests.cpp $(LIB_PATH)/gtest/* -I $(INCLUDE_PATH)/ -pthread -o $(BIN_PATH)/tests

$(LIB_PATH)/gtest $(INCLUDE_PATH):

clean:
	rm -rf $(BIN_PATH)/ui $(BIN_PATH)/tests