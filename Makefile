.PHONY: all clean
.DEFAULT_GOAL := all
all: ui tests

G++ := g++ -std=c++17

ui: base.h base.tpp sequence.h sequence.tpp sort.h menu.h ui.cpp
	$(G++) ui.cpp -pthread -o ui

tests: base.h base.tpp sequence.h sequence.tpp sort.h lib/gtest/ include/gtest/ tests.cpp
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

googletest/: .gitmodules
	git submodule update --init googletest/
	touch $@


clean:
	rm -rf ui tests lib/gtest/ include/gtest/ build/