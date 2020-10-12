.PHONY: all clean
.DEFAULT_GOAL := all

G++ := g++ -std=c++17

LIB_PATH := lib
INCLUDE_PATH := include
BIN_PATH := bin
BUILD_PATH := build
SOURCE_PATH := source

all: $(BIN_PATH)/ui $(BIN_PATH)/tests

.PHONY: $(SOURCE_PATH)
$(BIN_PATH)/ui $(BIN_PATH)/tests: $(SOURCE_PATH) $(LIB_PATH)/gtest $(INCLUDE_PATH)/gtest
	make -C source/


$(LIB_PATH)/gtest: googletest
	cmake -B $(BUILD_PATH)/gtest/ googletest/
	make -C $(BUILD_PATH)/gtest/ gtest gtest_main
	mkdir -p $(LIB_PATH)/gtest/
	cp $(BUILD_PATH)/gtest/lib/libgtest* $(LIB_PATH)/gtest/
	touch $@

$(INCLUDE_PATH)/gtest: googletest
	mkdir -p $(INCLUDE_PATH)/
	cp -r googletest/googletest/include/ ./
	touch $@

googletest: googletest/.git
	touch $@

googletest/.git: .gitmodules
	git submodule update --init googletest/
	touch $@


clean:
	make -C source/ clean
	rm -rf $(LIB_PATH)/gtest/ $(INCLUDE_PATH)/gtest/ $(BUILD_PATH)/