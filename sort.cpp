#include "base.h"
#include "sequence.h"

#include <stack>
#include <utility>
#include <iostream>
#include <string>


template <typename T>
Sequence<T>* bubbleSort(Sequence<T> *seq, int (*cmp)(T, T)){
	Sequence<T> *newSeq = seq->clone();

	auto swap = [newSeq](int i, int j) {
		T tmp = newSeq->get(i);
		newSeq->set(newSeq->get(j), i);
		newSeq->set(tmp, j);
	};

	if(seq->getSize() == 0) return newSeq;
	if(seq->getSize() == 1) return newSeq;

	for(int i = 0; i < newSeq->getSize() - 1; i++)
	for(int j = 0; j < newSeq->getSize() - 1; j++){
		if(cmp(newSeq->get(j), newSeq->get(j + 1)) > 0){
			swap(j, j+1);
		}
	}

	return newSeq;
}


template <typename T>
Sequence<T>* quickSort(Sequence<T> *seq, int (*cmp)(T, T)){
	Sequence<T> *newSeq = seq->clone();

	auto swap = [newSeq](int i, int j) {
		T tmp = newSeq->get(i);
		newSeq->set(newSeq->get(j), i);
		newSeq->set(tmp, j);
	};

	if(seq->getSize() == 0) return newSeq;
	if(seq->getSize() == 1) return newSeq;

	std::stack<std::pair<int,int>> edges;
	edges.push({0, newSeq->getSize() - 1});

	while(!edges.empty()){
		int start = edges.top().first;
		int end = edges.top().second;
		edges.pop();

		int p = start;
		int i = p + 1;
		int j = end;

		while(i <= j && i <= end && j > p){
			if(cmp(newSeq->get(i), newSeq->get(p)) <= 0) i++;
			else if(cmp(newSeq->get(j), newSeq->get(p)) > 0) j--;
			else swap(i, j);
		}
		swap(p, j);
		if(end - start >= 2){
			if(j > p) edges.push({start, j});
			if(i <= end) edges.push({i, end});
		}
	}
	return newSeq;
}


template <typename T>
Sequence<T>* shakerSort(Sequence<T> *seq, int (*cmp)(T, T)){
	Sequence<T> *newSeq = seq->clone();

	if(seq->getSize() == 0) return newSeq;
	if(seq->getSize() == 1) return newSeq;

	auto swap = [newSeq](int i, int j) {
		T tmp = newSeq->get(i);
		newSeq->set(newSeq->get(j), i);
		newSeq->set(tmp, j);
	};

	int i = newSeq->getSize();
	int cur = -1;

	while(true){
		i--;
		if(i == 0) break;
		for(int j = 0; j < i; j++){
			cur++;

			if(cmp(newSeq->get(cur), newSeq->get(cur + 1)) > 0){
				swap(cur, cur+1);
			}
		}

		i--;
		if(i == 0) break;
		for(int j = 0; j < i; j++){
			cur--;

			if(cmp(newSeq->get(cur), newSeq->get(cur + 1)) > 0){
				swap(cur, cur+1);
			}
		}
	}

	return newSeq;
}



int main(int argc, const char *argv[]){
	Sequence<int> *seq = new ListSequence<int>();


	for(int i = 1; i < argc; i++){
		seq->append(std::stoi(argv[i]));
	}
	Sequence<int> *sorted = quickSort<int>(seq, [](int a, int b) {return a - b;});

	for(int i = 0; i < sorted->getSize(); i++){
		std::cout << sorted->get(i) << std::endl;
	}

	return 0;
}