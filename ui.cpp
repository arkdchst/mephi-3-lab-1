#include "sort.h"
#include "menu.h"

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <fstream>

enum struct Algorithm {bubbleSort, quickSort, shakerSort, compareAll};
enum struct Base {array, list};


int* getDataFromFile(int dataLen){
	std::ifstream infile;
	infile.open("in.txt");

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

void sort(Algorithm algo, Base base, int dataLen, bool fromFile){
	Sequence<int> *sequence;

	if(!fromFile){
		static bool sranded = false;
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
	}else{
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
	int (*compareFun)(int, int) = [](int a,int b){return a - b;};

	if(algo == Algorithm::compareAll){
		float bubbleTime, quickTime, shakerTime;
		std::thread shakerThread([&](){
			struct timespec tp1, tp2;
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp1);
			sorted = shakerSort(sequence, compareFun);
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp2);
			shakerTime = tp2.tv_sec - tp1.tv_sec + (static_cast<float>(tp2.tv_nsec - tp1.tv_nsec)/1000000000);
		});
		std::thread bubbleThread([&](){
			struct timespec tp1, tp2;
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp1);
			delete bubbleSort(sequence, compareFun);
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp2);
			bubbleTime = tp2.tv_sec - tp1.tv_sec + (static_cast<float>(tp2.tv_nsec - tp1.tv_nsec)/1000000000);
		});
		std::thread quickThread([&](){
			struct timespec tp1, tp2;
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp1);
			delete quickSort(sequence, compareFun);
			clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp2);
			quickTime = tp2.tv_sec - tp1.tv_sec + (static_cast<float>(tp2.tv_nsec - tp1.tv_nsec)/1000000000);
		});

		bubbleThread.join();
		print("bubble time: " + std::to_string(bubbleTime) + " sec");
		quickThread.join();
		print("quick time: " + std::to_string(quickTime) + " sec");
		shakerThread.join();
		print("shaker time: " + std::to_string(shakerTime) + " sec");
	}else{

		struct timespec tp1;
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp1);
		switch(algo){
			case Algorithm::bubbleSort:
				sorted = bubbleSort(sequence, compareFun);
				break;
			case Algorithm::quickSort:
				sorted = quickSort(sequence, compareFun);
				break;
			case Algorithm::shakerSort:
				sorted = shakerSort(sequence, compareFun);
				break;
		}
		struct timespec tp2;
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp2);


		print("sorted!");
		print("elapsed CPU time:");
		print(std::to_string(tp2.tv_sec - tp1.tv_sec + (static_cast<float>(tp2.tv_nsec - tp1.tv_nsec)/1000000000)) + " sec");
	}

	Action input("show input data", [&sequence](){
		for(int i = 0; i < sequence->getSize(); i++){
			std::cout << sequence->get(i) << std::endl;
		}
		std::cout << std::endl;
	});
	Action output("show output data", [&sorted](){
		for(int i = 0; i < sorted->getSize(); i++){
			std::cout << sorted->get(i) << std::endl;
		}
		std::cout << std::endl;
	});

	Menu sortingMenu("sorting menu", {&input, &output}, false);
	sortingMenu.enter();


	delete sequence;
	delete sorted;
}

int main(){
	EnumChoice algorithmChoice = EnumChoice("algorithm", {"bubble sort", "quick sort", "shaker sort", "compare all"});
	EnumChoice baseChoice = EnumChoice("sequence base", {"array", "list"});

	EnumChoice dataTypeChoice = EnumChoice("data type", {"random", "from file (in.txt)"});
	IntChoice dataLenChoice = IntChoice("data length", 10, 0);

	Menu dataPropsMenu("data properties", {&dataTypeChoice, &dataLenChoice});

	Action sortAction = Action("sort!", [&]
	{
		sort(static_cast<Algorithm>(algorithmChoice.getChoosen()), static_cast<Base>(baseChoice.getChoosen()), dataLenChoice.getChoosen(), dataTypeChoice.getChoosen() == 1);
	});

	Menu mainMenu("main menu", {&algorithmChoice, &dataPropsMenu, &baseChoice, &sortAction}, true);

	mainMenu.enter();

	return 0;
}