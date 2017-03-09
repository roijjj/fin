#pragma once

#include <string>
#include "Map.h"
#include "Block.h"
#include "Inventory.h"
#include "Equipment.h"

const int LEVEL_MAX = 10;

class Player
{
private:	
	std::string _name;

	int _x, _y, _ox, _oy;
	int _exp, _level;
	int _health;

	Equipment _equipment;

	bool _dead;

	Inventory _inventory;

	int _expCurve[11], _hpmaxCurve[11];

	void updateLevel();

	//Data
	void loadCurve();
	void loadInitialResources();

	void kill();

public:
	Player();
	Player(std::string name);

	void draw();
	
	//SETTERS
	void setX(int x) { _ox = _x; _x = x; }
	void setY(int y) { _oy = _y; _y = y; }
	void setName(std::string name) { _name = name; }

	//GETTERS
	int getX() { return _x; }
	int getY() { return _y; } 
	bool isDead() { return _dead; }
	Inventory& getInventory() { return _inventory; }

	int getHealth() { return _health; }
	int getDefense();
	int getDamage();
	int getLevel() { return _level; }

	std::string& getName() { return _name; }
	
	
	//Movement
	void move (int x, int y, Map& map);
	void relMove (int dx, int dy, Map& map);
	void dirMove (char direction, Map& map);

	//player health, exp
	void addExp(int exp);
	void addHealth(int health);

	void clearActivePotions();

	void showInventory();
	void showEquipment();

	void getSaveString(std::string& saveString);
	void loadSave(std::vector<std::string> properties);
};