#pragma once

#include <string>
#include <list>
#include "Item.h"

class Inventory
{
private:

	int _money;
	std::list<Item> _items;

	void insert_ordered(Item& item);

public:
	static const int ITEMS_PER_PAGE = 5;

	Inventory(void);

	void print(int page, std::string& ownerName, int firstRow=0);

	void addItem(std::string name, int amount);
	bool containsItem(std::string name);
	bool containsItem(std::string name, int amount);

	Item& getItem(std::string name);
	Item& getItem(int index);

	int size() { return _items.size(); }
	int nPages();

	bool addMoney(int amount);

	//GETTERS
	int getMoney() { return _money; }

	//SETTERS
	void setMoney(int amount) { _money = amount; }
};

