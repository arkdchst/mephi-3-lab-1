#include "sort.hpp"
#include "menu.hpp"

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <fstream>
#include <functional>

enum struct Algorithm {bubbleSort, quickSort, shakerSort, compareAll};
enum struct Base {array, list};
enum struct DataType {random, file};

const std::string inFilename = "in.txt";

float deltaToSec(struct timespec tp1, struct timespec tp2){
	return tp2.tv_sec - tp1.tv_sec + (static_cast<float>(tp2.tv_nsec - tp1.tv_nsec)/1000000000);
}

void printSequence(Sequence<int> *seq){
	for(int i = 0; i < seq->getSize(); i++){
		std::cout << seq->get(i) << std::endl;
	}
	std::cout << std::endl;
}

int* getDataFromFile(int dataLen){
	std::ifstream infile;
	infile.open(inFilename);

	int *data = new int[dataLen];
	for(int i = 0; i < dataLen; i++){
		infile >> data[i];
	}
	infile.close();
	if(infile.fail()){
		throw std::runtime_error("file read error");
	}
	return data;
}

void startSorting(Algorithm algo, Base base, int dataLen, DataType dataType){
	Sequence<int> *sequence;

	if(dataType == DataType::random){
		static bool sranded = false;//we should srand only once
		if(!sranded){
			srand(time(0));
			sranded = true;
		}

		int *data = new int[dataLen];
		for(int i = 0; i < dataLen; i++){
			data[i] = rand() % (dataLen * 2);
		}
		if(base == Base::array)
			sequence = new ArraySequence<int>(data, dataLen);
		else if(base == Base::list)
			sequence = new ListSequence<int>(data, dataLen);

		delete[] data;
	}else
	if(dataType == DataType::file){
		int *data;
		try{
			data = getDataFromFile(dataLen);
		}catch(...){
			std::cout << "file read error occured" << std::endl;
			return;
		}
		if(base == Base::array)
			sequence = new ArraySequence<int>(data, dataLen);
		else if(base == Base::list)
			sequence = new ListSequence<int>(data, dataLen);
		delete[] data;
	}

	Sequence<int> *sorted;

	if(algo == Algorithm::compareAll){
		float bubbleTime, quickTime, shakerTime;
		auto threadFun = [&](float &time, SortFun<int> *fun, bool doDelete){ //function-stopwatch
			struct timespec tp1, tp2;
			Sequence<int> *_sorted;

			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp1);//start stopwatch

			_sorted = fun(sequence, defaultCompareFun);
			if(doDelete) delete _sorted;
			else sorted = _sorted;

			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp2);//stop stopwatch
			time = deltaToSec(tp1, tp2);
		};
		std::thread bubbleThread(threadFun, std::ref(bubbleTime), bubbleSort<int>, false);
		std::thread quickThread(threadFun, std::ref(quickTime), quickSort<int>, true);
		std::thread shakerThread(threadFun, std::ref(shakerTime), shakerSort<int>, true);

		bubbleThread.join();
		std::cout << "elapsed time for bubble sort: " + std::to_string(bubbleTime) + " sec" << std::endl;
		quickThread.join();
		std::cout << "elapsed time for quick sort: " + std::to_string(quickTime) + " sec" << std::endl;
		shakerThread.join();
		std::cout << "elapsed time for shaker sort: " + std::to_string(shakerTime) + " sec" << std::endl;
	}else{

		struct timespec tp1, tp2;
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp1);//start stopwatch
		switch(algo){
			case Algorithm::bubbleSort:
				sorted = bubbleSort(sequence, defaultCompareFun);
				break;
			case Algorithm::quickSort:
				sorted = quickSort(sequence, defaultCompareFun);
				break;
			case Algorithm::shakerSort:
				sorted = shakerSort(sequence, defaultCompareFun);
				break;
		}
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp2);//stop stopwatch


		std::cout << "sorted!" << std::endl;
		std::cout << "elapsed time: ";
		std::cout << std::to_string(deltaToSec(tp1, tp2)) + " sec" << std::endl;
	}

	Action input("show input data", [&sequence](){
		printSequence(sequence);
	});
	Action output("show output data", [&sorted](){
		printSequence(sorted);
	});

	Menu sortingMenu("sorting menu", {&input, &output}, false);
	sortingMenu.enter();


	delete sequence;
	delete sorted;
}

int main(){
	EnumChoice algorithmChoice = EnumChoice("algorithm", {"bubble sort", "quick sort", "shaker sort", "compare all"});
	EnumChoice baseChoice = EnumChoice("sequence base", {"array", "list"});

	EnumChoice dataTypeChoice = EnumChoice("data type", {"random", "from file (" + inFilename + ")"});
	IntChoice dataLenChoice = IntChoice("data length", 10, 0);

	Menu dataPropsMenu("data properties", {&dataTypeChoice, &dataLenChoice});

	Action sortAction = Action("sort!", [&]
	{
		startSorting(static_cast<Algorithm>(algorithmChoice.getChoosen()), static_cast<Base>(baseChoice.getChoosen()), dataLenChoice.getChoosen(), static_cast<DataType>(dataTypeChoice.getChoosen()));
	});

	Menu mainMenu("main menu", {&algorithmChoice, &dataPropsMenu, &baseChoice, &sortAction}, true);

	mainMenu.enter();

	return 0;
}