#include "Game.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <ctime>
#include <conio.h>
#include "Point2D.h"
#include "strings.h"
#include "utils.h"

using namespace std;

Game::Game()
{
}

void Game::start(bool loadGame)
{
	_randomEngine = default_random_engine((unsigned)time(NULL));
	_isRunning = true;
	loadMaps();
	loadMap(_curMap);

	if (loadGame)
	{
		load("");
	}
	else
	{
		string playerName;

		cls();
		center("Enter your name: ", 11);
		getline(cin, playerName);
		cls();

		_player.setName(playerName);
	}

	draw();
}

void Game::update(char input)
{
	Map& map = _maps[_curMap];
	bool update = false;

	//Map links
	vector<string>& mapNames = map.getMapNames();
	vector<Point2D>& mapLinkPos = map.getLinkPos();

	switch(input)
	{
	case 'w':
	case 'a':
	case 's':
	case 'd':
		_player.dirMove(input, map);
		update = true;
		break;
	
	case K_ENTER:
		
		//Check for NPC
		for (unsigned int i=0; i<_npcs.size(); i++)
		{
			if (_npcs[i].getMapName() == map.getName() && _npcs[i].getX() == _player.getX() &&
				_npcs[i].getY() == _player.getY() && _npcs[i].getCanTrade())
			{
				if (_player.getLevel() >= _npcs[i].getMinimumTradeLevel())
					_npcs[i].trade(_player.getInventory(), _player.getName());
				else
				{
					stringstream sstr;
					sstr << "You must be at least level " << _npcs[i].getMinimumTradeLevel() << ".";

					cls();
					center("Your level is too low to speak with this character.", 12);
					center(sstr.str(), 13);
					system("pause > nul");
					cls();
				}
			}
		}

		for (unsigned int i=0; i<mapLinkPos.size(); i++)
		{
			if (mapLinkPos[i].getX() == _player.getX() && mapLinkPos[i].getY() == _player.getY())
			{
				loadMap(mapNames[i]);
			}
		}
		break;

	case 'm':
	case K_ESC:	
		menu();
		break;

	default:
		break;
	}

	if (_player.getHealth() <= 0)
		gameOver();

	if (update) //If player has moved
	{
		//Update enemies
		for (unsigned int i=0; i<_enemies.size(); i++)
			if (_enemies[i].getMapName() == map.getName())
				_enemies[i].update(_player.getX(), _player.getY(), map);

		//If an enemy is touching the player, BATTLE STARTS!
		for (unsigned int i=0; i<_enemies.size(); i++)
			if ( !_enemies[i].isDead() && _enemies[i].getMapName() == map.getName() && 
				(_enemies[i].getX() == _player.getX()) && (_enemies[i].getY() == _player.getY()))
				battle(_enemies[i]);
	}

	if (_isRunning)
		draw();
}

void Game::mainLoop()
{
	char input;

	do {
		input = lowCase(_getch());
		update(input);
	} while(_isRunning);
}

//Private

void Game::draw()
{
	Map& map = _maps[_curMap];

	//gotoxy(0, 0);//No More flicker!

	setColor(consoleColor(0, 9));
	_player.draw();

	//DRAW NPCS
	if (_npcs.size() > 0)
		setColor(consoleColor(0, 2));
	for (unsigned int i=0; i<_npcs.size(); i++)
		if (_npcs[i].getMapName() == map.getName())
			_npcs[i].draw();
	
	//DRAW ENEMIES
	if (_enemies.size() > 0)
		setColor(consoleColor(0, 12));
	for (unsigned int i=0; i<_enemies.size(); i++)
		if (_enemies[i].getMapName() == map.getName())
			_enemies[i].draw();


	setColor(consoleColor(0, 15));
	map.draw();

	gotoxy(0, map.getHeight());
	setColor(consoleColor(0, 14));
	cout << "Press W, A, S, D to move, M for menu.\n";

	setColor(consoleColor(0, 7));
}

void Game::menu()
{
	vector <string> menuItems;

	menuItems.push_back("Continue");
	menuItems.push_back("Equipment");
	menuItems.push_back("Inventory");
	menuItems.push_back("Save");
	menuItems.push_back("Load");
	menuItems.push_back("Exit");

	int sel = choiceMenu("GAME MENU", menuItems, -1); //-1 means center vertically
	int exitSel;

	vector <string> menu;

	switch(sel)
	{
	case 0:
		//Just return to game
		break;

	case 1:
		_player.showEquipment();
		break;

	case 2:
		_player.showInventory();
		break;

	case 3:
		save();
		break;

	case 4:
		load("");
		break;

	case 5:
		menu.push_back("NO");
		menu.push_back("YES");

		exitSel = choiceMenu("Do you want to exit the game?", menu, -1);

		if (exitSel)
			_isRunning = false;

		break;

	default:
		break;
	}

	cls();
}

void Game::save()
{
	stringstream sstr;
	string properties, tmp, name;

	//Build the properties string
	sstr << "game.currentMap=" << _curMap << "|";

	_player.getSaveString(tmp);
	sstr << tmp << "|";

	for (unsigned int i=0; i<_npcs.size(); i++)
	{
		_npcs[i].getSaveString(tmp);
		sstr << tmp << "|";
	}

	properties = sstr.str();

	//Enter a filename
	cls();
	cout << "Save game: ";
	getline(cin, name);

	//SAVE
	ofstream out("Data\\GameSaves\\" + name + ".sav");

	if (out.fail()) //If cannot save, tell player and return
	{
		cout << "Could not save the file at 'Data\\GameSaves\\" + name + ".sav'.\n";
		system("pause > nul");
		cls();
		return;
	}

	for (unsigned int i=0; i<properties.size(); i++) //Save characters in the file
		out.put(properties[i]);

	out.close();
}

void Game::load(string saveName)
{
	string str, name;

	if (saveName == "")
	{
		cls();
		cout << "Load game: ";
		getline(cin, name);
	}

	ifstream in("Data\\GameSaves\\" + name + ".sav");
	
	if (in.fail())
	{
		cout << "Could not load the file 'Data\\GameSaves\\" + name + ".sav'.\n";
		system("pause > nul");
		cls();
		return;
	}

	while(!in.eof())
	{
		char c;
		in.get(c);
		if (!in.eof())
			str += c;
	}

	vector<string> properties, playerProperties, gameProperties, npcProperties;

	splitInto(str, '|', properties);

	for (unsigned int i=0; i<properties.size(); i++)
	{
		string owner = split(properties[i], '.', 0);

		if (owner == "player")
			playerProperties.push_back(properties[i]);
		else if (owner == "game")
			gameProperties.push_back(properties[i]);
		else if (owner == "npc")
		{
			if (properties[i] == "npc.newNpc")
			{
				_npcs.push_back(NPC(npcProperties));
				npcProperties.clear();
			}
			else
				npcProperties.push_back(properties[i]);
		}

	}

	_player.loadSave(playerProperties);
	loadProperties(gameProperties);
}

void Game::loadProperties(std::vector<std::string> properties)
{
	for (unsigned int i=0; i<properties.size(); i++)
	{
		string& line = properties[i];
		string name, value;

		name = split(line, '=', 0);
		value = split(line, '=', 1);

		if (name == "game.currentMap")
			sscanf_s(value.c_str(), "%d", &_curMap);
	}
}

void Game::loadMaps()
{
	ifstream in;
	in.open("Data\\maplist.txt");

	if (in.fail())
	{
		cout << "Could not load file 'Data\\maplist.txt'.\n";
		system("pause");
	}

	while (!in.eof())
	{
		string line;
		getline(in, line);

		if (contains(line, '='))
		{
			if (split(line, '=', 0) == "start")
				sscanf_s(split(line, '=', 1).c_str(), "%d", &_curMap);
		}
		else
		{
			_maps.push_back(Map(line));

			//NPCs
			vector<string>& NPCnames = _maps.back().getNpcNames();
			vector<Point2D>& NPCpos = _maps.back().getNpcStartPos();
			
			for (unsigned int i=0; i<NPCnames.size(); i++)
				_npcs.push_back(NPC(NPCnames[i], _maps.back().getName(), NPCpos[i].getX(), NPCpos[i].getY()));

			//Enemies
			vector<string>& enemyNames = _maps.back().getEnemiesNames();
			vector<Point2D>& enemyPos = _maps.back().getEnemiesPos();

			for (unsigned int i=0; i<enemyNames.size(); i++)
				_enemies.push_back(Enemy(enemyNames[i], _maps.back().getName(), enemyPos[i].getX(), enemyPos[i].getY(), &_randomEngine));
		}
	}

	in.close();
}

void Game::loadMap(int index)
{
	cls();

	Map& m = _maps[index];
	_curMap = index;

	//Player starting position
	Point2D& playerPos = m.getPlayerStartPos();
	_player.move(playerPos.getX(), playerPos.getY(), m);
}

void Game::loadMap(string name)
{
	for (unsigned int i=0; i<_maps.size(); i++)
	{
		if (_maps[i].getName() == name)
		{
			loadMap(i);
			return;
		}
	}

	cout << "Could not load map '" << name << "'!\n";
	system("pause");
}

void Game::battle(Enemy& enemy)
{
		
	//FIGHT BATTLE
	bool playerTurn = true;

	int plDamageAttenuation = _player.getDefense();
	int enDamageAttenuation = enemy.getDefense();

	while(!_player.isDead() && !enemy.isDead())
	{
		cls();
		setColor(consoleColor(0, 15));
		drawScreen("battle");

		//DRAW BATTLE INFO

		//Player
		setColor(consoleColor(0, 10));

		gotoxy(17, 4);
		cout << _player.getName();

		gotoxy(6, 6);
		cout << "Health: " << _player.getHealth();
		gotoxy(6, 7);
		cout << "Attack: " << _player.getDamage();
		gotoxy(6, 8);
		cout << "Defense: " << _player.getDefense();

		//Enemy
		setColor(consoleColor(0, 14));

		gotoxy(57, 4);
		cout << enemy.getName();

		gotoxy(53, 6);
		cout << "Health: " << enemy.getHealth();
		gotoxy(53, 7);
		cout << "Attack: " << enemy.getAttack();
		gotoxy(53, 8);
		cout << "Defense: " << enemy.getDefense();


		//BATTLE
		setColor(consoleColor(0, 12));

		if (playerTurn)
		{
			int damage, t;

			damage = _player.getDamage() - enemy.getDefense()/4;

			if (damage < 0)	damage = 0;
			enemy.addHealth(-damage);

			for (t=16; t>=14; t--) //Animation! 
			{
				gotoxy(54, t);
				cout << "-" << damage;
				sleep(250);
				gotoxy(54, t);
				cout << "          ";
			}
		}
		else
		{
			int damage, t;
				
			damage = enemy.getAttack() - _player.getDefense()/4;
			if (damage < 0)	damage = 0;
			_player.addHealth(-damage);

			for (t=16; t>=14; t--) //Animation
			{
				gotoxy(14, t);
				cout << "-" << damage;
				sleep(250);
				gotoxy(14, t);
				cout << "          ";
			}
		}

		sleep(250);

		playerTurn = !playerTurn;
	}

	if (enemy.isDead())
	{
		_player.addExp(enemy.getExpReward());
		_player.getInventory().addMoney(enemy.getMoneyReward());

		cls();
		setColor(consoleColor(0, 10));
		center("YOU WON THE BATTLE!\n", 12);
		system("pause > nul");
	}
	else //Player has died
	{
		setColor(consoleColor(0, 12));
		gameOver();
	}

	_player.clearActivePotions();

	setColor(consoleColor(0, 7));
	cls();
}

void Game::gameOver()
{
	cls();
	drawScreen("GameOver");
	_isRunning = false;
	system("pause > nul");
}