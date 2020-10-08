#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <exception>


int readInt(int from=std::numeric_limits<int>::min(), int to=std::numeric_limits<int>::max()){
	while(true){
		int read;
		std::cin >> read;
		if(read < from || read > to || std::cin.fail()){
			std::cout << "please, enter a number from " + std::to_string(from) + " to " + std::to_string(to) + ": ";
			std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else return read;
	}
}


class MenuItem{
public:
	virtual void enter() = 0;//enter into item's context
	virtual std::string toString() = 0;
};

class EnumChoice : public MenuItem{
private:
	std::vector<std::string> values;
	int chosenNum;
	std::string name;

	void print(){
		std::string out;
		for(int i = 0; i < values.size(); i++){
			out += std::to_string(i + 1) + ". " + values[i] + "\n";
		}
		std::cout << out << std::endl;
	}
public:

	EnumChoice(std::string name, std::vector<std::string> values, int defaultNum = 0) :
		values(values), name(name), chosenNum(defaultNum)
	{
		if(values.empty())
			throw std::runtime_error("values is empty");
		if(defaultNum < 0 || defaultNum >= values.size())
			throw std::runtime_error("default value out of bounds");
	}

	int getChoosen() const {
		return this->chosenNum;
	}

	virtual std::string toString() override {
		if(values.empty() || chosenNum == -1)
			return this->name;
		else
			return this->name + " (" + values[chosenNum] + ")";
	}

	virtual void enter() override {
		print();
		int choose = readInt(1, values.size());
		chosenNum = choose - 1;
	}
};

class IntChoice : public MenuItem{
private:
	int value;
	std::string name;
	int from, to;

	void print(){
		std::cout << "enter integer from " + std::to_string(from) + " to " + std::to_string(to) + ": ";
	}
public:

	IntChoice(std::string name, int defaultValue, int from=std::numeric_limits<int>::min(), int to=std::numeric_limits<int>::max()) :
		name(name), value(defaultValue), from(from), to(to)
	{
		if(from > to)
			throw std::runtime_error("from is greater than to");
		if(defaultValue < from || defaultValue > to)
			throw std::runtime_error("default value out of bounds");

	}

	int getChoosen() const {
		return this->value;
	}

	virtual std::string toString() override {
		return this->name + " (" + std::to_string(value) + ")";
	}

	virtual void enter() override {
		print();
		value = readInt(from, to);
	}	
};

class Menu : public MenuItem {
private:
	std::vector<MenuItem*> items;
	std::string name;
	bool isRoot;

	void print(){
		std::string out;
		for(int i = 0; i < items.size(); i++){
			out += std::to_string(i + 1) + ". " + items[i]->toString() + "\n";
		}
		out += std::to_string(items.size() + 1) + ". " + (isRoot ? "exit" : "back") + "\n";
		std::cout << out << std::endl;
	}
public:
	Menu(std::string name, std::vector<MenuItem*> items, bool isRoot=false) :
		items(items), name(name), isRoot(isRoot)
	{
		if(items.empty())
			throw std::runtime_error("items is empty");
	
	}

	virtual std::string toString() override {
		return this->name;
	}

	virtual void enter() override {
		while(true){
			this->print();
			int choose = readInt(1, items.size() + 1);
			if(choose == items.size() + 1)
				return;
			else
				items[choose - 1]->enter();
		}
	}
};

template <typename Function>
class Action : public MenuItem {
private:
	Function actionFun;
	std::string name;
public:
	Action(std::string name, Function actionFun) :
		name(name), actionFun(actionFun){}

	virtual std::string toString() override {
		return name;
	}

	virtual void enter() override{
		actionFun();
	}
};