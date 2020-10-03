#include "sort.h"
#include "menu.h"

#include <iostream>
#include <string>
#include <limits>

enum struct Algorithm {bubbleSort=1, quickSort, shakerSort, compareAll};
enum struct Base {array=1, list};



int main(){
	EnumChoice algorithmChoice = EnumChoice("algorithm", {"bubble sort", "quick sort", "shaker sort", "compare all"});
	IntChoice dataLenChoice = IntChoice("data length", 10, 0);
	EnumChoice baseChoice = EnumChoice("sequence base", {"array", "list"});


	Menu mainMenu("main menu", {&algorithmChoice, &dataLenChoice, &baseChoice}, true);

	mainMenu.enter();

	return 0;
}