#pragma once

#include <vector>
#include <string>
#include "Block.h"
#include "Point2D.h"

class Map
{
private:
	//	        Y   X !!!!!!!!!!!!!!!!!!!!!!
	Block _grid[20][70];
	int _width, _height;

	Point2D _playerStartPos;
	std::vector<std::string> _NPCnames;
	std::vector<Point2D> _NPCstartPos;

	std::vector<std::string> _mapNames;
	std::vector<Point2D> _mapLinkPos;

	std::vector<std::string> _enemiesNames;
	std::vector<Point2D> _enemiesPos;

	std::string _name;

public:
	Map(void);
	Map (std::string mapName);

	int load(std::string mapName);
	void draw();

	int getBlockTypeAtPos(int x, int y);

	//GETTERS
	int getWidth() { return _width; }
	int getHeight() { return _height; }
	std::string& getName() { return _name; }

	Point2D& getPlayerStartPos() { return _playerStartPos; }
	std::vector<std::string>& getNpcNames() { return _NPCnames; }
	std::vector<Point2D>& getNpcStartPos() { return _NPCstartPos; }
	std::vector<std::string>& getMapNames() { return _mapNames; }
	std::vector<Point2D>& getLinkPos() { return _mapLinkPos; }
	std::vector<std::string>& getEnemiesNames() { return _enemiesNames; }
	std::vector<Point2D>& getEnemiesPos() { return _enemiesPos; }
};

