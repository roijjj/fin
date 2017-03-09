#pragma once

#include <random>
#include <string>
#include "Map.h"

enum AIType
{
	AI_STILL,
	AI_RAND,
	AI_CHASE
};

class Enemy
{
private:
	std::string _name;
	std::string _mapName;
	std::default_random_engine *_randomEngine;

	int _startX, _startY;
	int _viewRange, _maxSpawnDistance;
	int _x, _y, _ox, _oy;
	char _repr;

	int _aiType;
	int _deadTurns, _respawnTime;
	bool _dead;

	int _expReward, _moneyReward;


	int _health, _healthMax;
	int _attack, _defense;

	void load(std::string name);	
	void kill();
	void move(int playerX, int playerY, Map& gameMap);

	void randMove(int playerX, int playerY, Map& gameMap);
	void chaseMove(int playerX, int playerY, Map& gameMap);

public:
	Enemy(void);
	Enemy (std::string name);
	Enemy(std::string name, std::string mapName, int x, int y, std::default_random_engine* randomEngine);

	void draw();
	void update(int playerX, int playerY, Map& gameMap);

	bool isDead();
	void addHealth(int amount);

	//Getters
	int getX() { return _x; }
	int getY() { return _y; }
	char getRepr() { return _repr; }
	int getExpReward() { return _expReward; }
	int getMoneyReward() { return _moneyReward; }

	int getHealth() { return _health; }
	int getMaxHealth() { return _healthMax; }
	int getAttack() { return _attack; }
	int getDefense() { return _defense; }

	std::string& getName() { return _name; }
	std::string& getMapName() { return _mapName; }

	//Setters
	void setX(int x);
	void setY(int y);
};

