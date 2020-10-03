#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <exception>

void print(std::string str){
	if(str.empty()) return;

	if(str.back() == ':')
		std::cout << str << "\n";
	else
		std::cout << str << "\n\n";

	std::cout.flush();
}


int readInt(int from=std::numeric_limits<int>::min(), int to=std::numeric_limits<int>::max()){
	while(true){
		try{
			std::string line;
			std::getline(std::cin, line);

			int read = std::stoi(line);
			if(read < from || read > to) throw nullptr;

			std::cout << "\n";
			return read;
		}catch(...){
			print("please, enter a number from " + std::to_string(from) + " to " + std::to_string(to) + ":");
		}

	}
}


class MenuItem{
public:
	virtual void enter() = 0;
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
		::print(out);
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
		::print("enter integer from " + std::to_string(from) + " to " + std::to_string(to) + ":");
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
		::print(out);
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