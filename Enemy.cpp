#include "Enemy.h"

#include <iostream>
#include <fstream>
#include "strings.h"
#include "utils.h"

using namespace std;

Enemy::Enemy(void)
{

}

Enemy::Enemy(string name)
{
	_name = name;
	load (name);
	_health = _healthMax;
	_dead = false;
}

Enemy::Enemy(string name, string mapName, int x, int y, default_random_engine* randomEngine)
{
	_randomEngine = randomEngine;

	_ox = _startX = _x = x;
	_oy = _startY = _y = y;

	_name = name;
	_mapName = mapName;
	load (name);

	_health = _healthMax;
	_dead = false;
}

void Enemy::setX(int x)
{
	_ox = _x;
	_oy = _y;
	_x = x;
}

void Enemy::setY(int y)
{
	_oy = _y;
	_ox = _x;
	_y = y;
}

void Enemy::addHealth(int amount)
{
	_health += amount;

	if (_health > _healthMax)
		_health = _healthMax;
	else if (_health <= 0)
		kill();
}

bool Enemy::isDead()
{
	return _dead;
}

void Enemy::draw()
{
	if (_dead) return;

	gotoxy(_ox, _oy);
	cout << ' ';
	gotoxy(_x, _y);
	cout << _repr;
}

void Enemy::update(int playerX, int playerY, Map& gameMap)
{
	if (_dead)
	{
		if (_deadTurns ==  _respawnTime)
		{
			_ox = _x;
			_oy = _y;
			_x = _startX;
			_y = _startY;
			_dead = false;
			_health = _healthMax;
		}
		else
			_deadTurns++;
	}
	else
	{
		move(playerX, playerY, gameMap);
	}
}

//Private
void Enemy::move(int playerX, int playerY, Map& gameMap)
{
	switch(_aiType)
	{
	case AI_STILL:
		return;

	case AI_RAND:
		randMove(playerX, playerY, gameMap);
		break;

	case AI_CHASE:
		chaseMove(playerX, playerY, gameMap);
		break;
	}
}

void Enemy::randMove(int playerX, int playerY, Map& gameMap)
{
	uniform_int_distribution<int> range(1, 8); //1-4 move, 5-8 don't move
	int direction = range(*_randomEngine);

	switch(direction)
	{
	case 1: //up
		if (gameMap.getBlockTypeAtPos(_x, _y-1) == BT_AIR && iDistance(_x, _y-1, _startX, _startY) < _maxSpawnDistance)
			setY(_y-1);
		break;

	case 2: //down
		if (gameMap.getBlockTypeAtPos(_x, _y+1) == BT_AIR && iDistance(_x, _y+1, _startX, _startY) < _maxSpawnDistance)
			setY(_y+1);
		break;

	case 3: //left
		if (gameMap.getBlockTypeAtPos(_x-1, _y) == BT_AIR && iDistance(_x-1, _y, _startX, _startY) < _maxSpawnDistance)
			setX(_x-1);
		break;

	case 4: //right
		if (gameMap.getBlockTypeAtPos(_x+1, _y) == BT_AIR && iDistance(_x+1, _y, _startX, _startY) < _maxSpawnDistance)
			setX(_x+1);
		break;

	default:
		return;
	}
}

void Enemy::chaseMove(int playerX, int playerY, Map& gameMap)
{
	//Distance between the player and enemy's position
	int d = iDistance(_x, _y, playerX, playerY);

	int dx=0, dy=0;

	if (d > _viewRange)
	{
		randMove(playerX, playerY, gameMap);
	}
	else
	{
		if (playerX > _x)
			dx = 1;
		else if (playerX < _x)
			dx = -1;
		
		if (playerY > _y)
			dy = 1;
		else if (playerY < _y)
			dy = -1;

		if (gameMap.getBlockTypeAtPos(_x+dx, _y+dy) == BT_AIR)
		{
			_ox = _x;
			_oy = _y;
			_x += dx;
			_y += dy;
		}
	}
}

void Enemy::kill()
{
	_deadTurns = 0;
	_dead = true;
}

void Enemy::load(string name)
{
	string path = "Data\\Enemies\\"+name+".enemy";

	ifstream in;
	in.open(path);

	if (in.fail())
	{
		cout << "Could not open file '" << path << "'.\n";
		system("pause");
	}

	while (!in.eof())
	{
		string line, name, value;
		getline(in, line);

		name = split(line, '=', 0);
		value = split(line, '=', 1);

		if (name == "repr")
		{
			sscanf_s(value.c_str(), "%c", &_repr);
		}
		else if (name == "healthMax")
		{
			sscanf_s(value.c_str(), "%d", &_healthMax);
		}
		else if (name == "attack")
		{
			sscanf_s(value.c_str(), "%d", &_attack);
		}
		else if (name == "defense")
		{
			sscanf_s(value.c_str(), "%d", &_defense);
		}
		else if (name == "exp")
		{
			sscanf_s(value.c_str(), "%d", &_expReward);
		}
		else if (name == "money")
		{
			sscanf_s(value.c_str(), "%d", &_moneyReward);
		}
		else if (name == "respawnTime")
		{
			sscanf_s(value.c_str(), "%d", &_respawnTime);
		}
		else if (name == "aiType")
		{
			if (value == "still")
				_aiType = AI_STILL;
			else if (value == "rand")
				_aiType = AI_RAND;
			else if (value == "chase")
				_aiType = AI_CHASE;
		}
		else if (name == "viewRange")
		{
			sscanf_s(value.c_str(), "%d", &_viewRange);
		}
		else if (name == "maxSpawnDistance")
		{
			sscanf_s(value.c_str(), "%d", &_maxSpawnDistance);
		}
	}

	in.close();
}