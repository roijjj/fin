#include "Map.h"

#include <map>
#include <fstream>
#include <iostream>
#include "strings.h"
#include "utils.h"

using namespace std;

Map::Map(void)
{
}

Map::Map(string mapName)
{
	load(mapName);
}

int Map::load(string mapName)
{
	string path;

	_name = mapName;
	path = "Data\\Maps\\" + mapName;

	map<char, string> maps;
	map<char, string> npcs;
	map<char, string> enemies;

	ifstream in;

	//PROPERTIES ------------------------------------------------------
	in.open(path+".properties");

	if (in.fail())
		return 1; //ERROR!


	while(!in.eof())
	{
		string line, name, value;
		getline(in, line);

		if (!contains(line, '='))
			continue;

		name = split(line, '=', 0);
		value = split(line, '=', 1);

		char firstChar = name[0];

		if (name == "height")
		{
			if (sscanf_s(value.c_str(), "%d", &_height) == 0)
				cout << "Error while reading height property for map " << mapName;
		}
		else if (name == "width")
		{
			if (sscanf_s(value.c_str(), "%d", &_width) == 0)
				cout << "Error while reading width property for map " << mapName;
		}
		else if (contains("123456789", firstChar)) //Link
		{	
			maps[firstChar] = value;
		}
		else if (contains("ABCDEFGHIJKLMNOPQRSTUVWXYZ", firstChar))	//LOAD npc
		{
			npcs[firstChar] = value;
		}
		else if (contains("abcdefghijklmnopqrstuvwxyz", firstChar))
		{
			enemies[firstChar] = value;
		}
		else
		{
			cout << "Error found while loading properties for map " << _name << ", invalid property '" << name << "'.\n";
			system("pause");
		}

	}

	in.close();	

	//DATA ------------------------------------------------------------
	in.open(path+".dat");

	if (in.fail())
		return 1; //ERROR!

	int x=0, y=0;

	while (!in.eof())
	{
		char c;
		in.get(c);

		if (c == ' ')
			_grid[y][x] = Block(' ', BT_AIR);
		else if (c == '#')
			_grid[y][x] = Block('#', BT_WALL);
		else if (c == '0')
		{
			_playerStartPos.set(x, y);
			_grid[y][x] = Block(' ', BT_AIR);
		}
		else if (contains("123456789", c))
		{
			_grid[y][x] = Block(' ', BT_AIR);
			if (maps.count(c)) //Stackoverflow.com <3
			{
				_mapNames.push_back(maps[c]);
				_mapLinkPos.push_back(Point2D(x, y));
			}
		}
		else if (contains("ABCDEFGHIJKLMNOPQRSTUVWXYZ", c))
		{
			_grid[y][x] = Block(' ', BT_AIR);
			if (npcs.count(c))
			{
				_NPCnames.push_back(npcs[c]);
				_NPCstartPos.push_back(Point2D(x, y));
			}
		}
		else if (contains("abcdefghijklmnopqrstuvwxyz", c))
		{
			_grid[y][x] = Block(' ', BT_AIR);
			if (enemies.count(c))
			{
				_enemiesNames.push_back(enemies[c]);
				_enemiesPos.push_back(Point2D(x, y));
			}
		}
		else if (contains("\n\r\t", c))
			x--;
	
		x++;
		if (x >= _width)
		{
			x = 0;
			y++;
			if (y >= _height)
				break;
		}
	}

	in.close();

	//'IMAGE' --------------------------------------------------------
	in.open(path+".map");

	if (in.fail())
		return 1; //ERROR!

	x=0;
	y=0;

	while (!in.eof())
	{
		char c;
		in.get(c);

		if (contains("\n\r\t", c))
			continue;

		_grid[y][x].setRepr(c);
		
	
		x++;
		if (x >= _width)
		{
			x = 0;
			y++;
			if (y >= _height)
				break;
		}
	}

	in.close();
	return 0;
}

void Map::draw()
{
	for (int y=0; y<_height; y++)
	{
		for (int x=0; x<_width; x++)
			if (_grid[y][x].getRepr() != ' ')
			{
				gotoxy(x, y);
				printf ("%c", _grid[y][x].getRepr());
			}		
		printf("\n");
	}
}

int Map::getBlockTypeAtPos(int x, int y)
{
	if (x >= 0 && y >= 0 && x <_width && y <_height)
	{
		return _grid[y][x].getType();
	}

	return BT_WALL;
}
