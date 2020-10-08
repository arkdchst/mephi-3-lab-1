.PHONY: all clean
.DEFAULT_GOAL := all
all: ui tests

G++ := g++ -std=c++17

ui: base.hpp base.tpp sequence.hpp sequence.tpp sort.hpp menu.hpp ui.cpp
	$(G++) ui.cpp -pthread -o ui

tests: base.hpp base.tpp sequence.hpp sequence.tpp sort.hpp lib/gtest/ include/gtest/ tests.cpp
	$(G++) tests.cpp lib/gtest/* -I include/ -pthread -o tests

lib/gtest/: googletest/
	cmake -B build/gtest/ googletest/
	make -C build/gtest/ gtest gtest_main
	mkdir -p lib/gtest/
	cp build/gtest/lib/libgtest* lib/gtest/
	touch $@

include/gtest/: googletest/
	mkdir -p include/
	cp -r googletest/googletest/include/ ./
	touch $@

googletest/: googletest/.git
	touch $@

googletest/.git: .gitmodules
	git submodule update --init googletest/
	touch $@


clean:
	rm -rf ui tests lib/gtest/ include/gtest/ build/