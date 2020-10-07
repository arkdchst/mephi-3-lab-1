.PHONY: all clean
.DEFAULT_GOAL := all
all: ui tests

CMD := g++ -std=c++17

ui: base.h sequence.h sort.h menu.h ui.cpp
	$(CMD) ui.cpp -pthread -o ui

tests: base.h sequence.h sort.h lib/gtest/ include/gtest/ tests.cpp
	$(CMD) tests.cpp lib/gtest/* -I include/ -pthread -o tests

lib/gtest/: googletest/
	cmake -B build/gtest/ googletest/
	$(MAKE) -C build/gtest/ gtest gtest_main
	mkdir -p lib/gtest/
	cp build/gtest/lib/libgtest* lib/gtest/

include/gtest/: googletest/
	mkdir -p include/
	cp -r googletest/googletest/include/ ./

clean:
	rm -rf ui tests lib/gtest/ include/gtest/ build/