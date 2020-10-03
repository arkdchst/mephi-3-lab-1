#include "sort.h"
#include "menu.h"

#include <iostream>
#include <string>
#include <limits>

enum struct Algorithm {bubbleSort, quickSort, shakerSort, compareAll};
enum struct Base {array, list};


void sort(Algorithm algo, Base base, int dataLen, bool fromFile=false){
	
}

int main(){
	EnumChoice algorithmChoice = EnumChoice("algorithm", {"bubble sort", "quick sort", "shaker sort", "compare all"});
	EnumChoice baseChoice = EnumChoice("sequence base", {"array", "list"});

	EnumChoice dataTypeChoice = EnumChoice("data type", {"random", "from file (in.txt)"});
	IntChoice dataLenChoice = IntChoice("data length", 10, 0);

	Menu dataPropsMenu("data properties", {&dataTypeChoice, &dataLenChoice});

	Action sortAction = Action("sort!", [&]
	{
		sort(static_cast<Algorithm>(algorithmChoice.getChoosen()), static_cast<Base>(baseChoice.getChoosen()), dataLenChoice.getChoosen());
	});

	Menu mainMenu("main menu", {&algorithmChoice, &dataPropsMenu, &baseChoice, &sortAction}, true);

	mainMenu.enter();

	return 0;
}