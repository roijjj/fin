#pragma once

#include <random>
#include <vector>
#include "Map.h"
#include "Player.h"
#include "NPC.h"
#include "Enemy.h"

class Game
{
private:
	int _curMap;
	bool _isRunning;

	Player _player;
	std::vector<Map> _maps;
	std::vector<NPC> _npcs;
	std::vector<Enemy> _enemies;

	std::default_random_engine _randomEngine;

	void loadMaps();
	void loadMap(int index);
	void loadMap(std::string name);

	void draw();

	void menu();
	void save();
	void load(std::string saveName);

	void loadProperties(std::vector<std::string> properties);

	void gameOver();
	void battle(Enemy& enemy);


public:
	Game();

	void start(bool loadGame);
	void update(char input);
	void mainLoop();

	//Getters
	bool isRunning() {return _isRunning; }
};

