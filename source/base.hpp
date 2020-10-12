#pragma once

#include <stdexcept>
#include <string>

const std::string INDEX_OUT_OF_RANGE_MESSAGE = "index out of range";
const std::string NEGATIVE_SIZE_MESSAGE = "size is negative";
const std::string ZERO_SIZE_MESSAGE = "size is 0";

template <typename T> class DynamicArray{
private:
	T *data = nullptr;
	int size = 0;
public:
	DynamicArray();
	DynamicArray(int size);
	DynamicArray(const T *data, int size);
	DynamicArray(const DynamicArray<T> &array, int size);
	DynamicArray(const DynamicArray<T> &array);
	virtual ~DynamicArray();
	T get(int index) const;
	int getSize() const;
	void set(const T &value, int index);
	void resize(int size);
	bool operator==(const DynamicArray<T> &arr) const;
	DynamicArray<T>& operator=(const DynamicArray &array);
};

template <typename T> class LinkedList{
private:
	struct Record{
		T item;
		Record *next = nullptr;
	};
	Record *head = nullptr;
	int size = 0;
public:
	LinkedList();
	LinkedList(const T *items, int size);
	LinkedList(int size);
	LinkedList(const LinkedList<T> &list);
	virtual ~LinkedList();
	T getFirst() const;
	T getLast() const;
	T get(int index) const;
	void set(const T &item, int index);
	LinkedList<T>* getSublist(int start, int end) const;
	int getSize() const;
	void append(const T &item);
	void prepend(const T &item);
	void removeAt(int index);
	void insertAt(const T &item, int index);
	LinkedList<T>* concat(const LinkedList<T> &list) const;
	bool operator==(const LinkedList<T> &list) const;
};

#include "base.tpp"