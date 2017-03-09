#pragma once

#include <string>
#include <vector>
#include "Inventory.h"

class NPC
{
private:
	std::string _name;
	std::string _mapName;

	Inventory _inventory;

	int _x, _y;
	char _repr;

	int _minimumTradeLevel;
	bool _canTrade;

	void load(std::string name);

public:
	NPC(void);
	NPC(std::string name, std::string mapName, int startx, int starty);
	NPC(std::string name, std::string mapName, char repr, int startx, int starty);
	NPC(std::vector<std::string> properties);

	void draw();

	void addHealth(int amount);
	void trade(Inventory& playerInventory, std::string playerName);

	void NPC::getSaveString(std::string& str);

	//Getters
	int getX() { return _x; }
	int getY() { return _y; }
	char getRepr() { return _repr; }
	bool getCanTrade() { return _canTrade; }
	std::string& getName() { return _name; }
	std::string& getMapName() { return _mapName; }
	int getMinimumTradeLevel() { return _minimumTradeLevel; }

	//Setters
	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }
	void setRepr(char repr) { _repr = repr; }
};

