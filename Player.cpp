#include "Player.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <conio.h>

#include "strings.h"
#include "utils.h"

using namespace std;

Player::Player(void)
{
	loadCurve();

	_dead = false;
	_name = "Player";
	_level = 0;
	_exp = 0;
	_health = _hpmaxCurve[_level];
	loadInitialResources();
}

Player::Player(string name)
{
	loadCurve();
	_dead = false;
	_name = name;
	_level = 0;
	_health = _hpmaxCurve[_level];
	loadInitialResources();
}

void Player::move (int x, int y, Map& map)
{
	int dest = map.getBlockTypeAtPos(x, y);

	switch(dest)
	{
	case BT_AIR:
		_ox = _x;
		_oy = _y;
		_x = x;
		_y = y;
		break;

	case BT_WALL:
		break;
	}
}

void Player::relMove (int dx, int dy, Map& map)
{
	int x, y;
	x = _x+dx;
	y = _y+dy;
	move(x, y, map);
}

void Player::dirMove (char direction, Map& map)
{
	switch (direction)
		{
		case 'w':
			relMove(0, -1, map);
			break;
		case 'a':
			relMove(-1, 0, map);
			break;
		case 's':
			relMove(0, 1, map);
			break;
		case 'd':
			relMove(1, 0, map);
			break;
		default:
			cout << "Invalid direction.\n";
			break;
		}
}

void Player::addExp(int exp)
{
	_exp += exp;
	updateLevel();
}

void Player::addHealth(int health)
{
	_health += health;
	if (_health <= 0)
		kill();
	else if (_health > _hpmaxCurve[_level])
		_health = _hpmaxCurve[_level];

}

void Player::showEquipment()
{
	cls();

	center("*** Equipment and Stats ***", 0);

	Item& weapon = _equipment.getWeapon();
	Item& helmet = _equipment.getHeadArmor();
	Item& chestplate = _equipment.getChestArmor();

	int y=1, addedDamage=0, addedDefense=0;

	for (unsigned int i=0; i<_equipment.getActivePotionsCount(); i++)
	{
		Item& activePotion = _equipment.getActivePotion(i);
		if (activePotion.getName() != "Missing")
		{
			addedDamage += activePotion.getAddAttack();
			addedDefense += activePotion.getAddDefense();
		}
	}

	//DISPLAY PLAYER STATS
	setColor(consoleColor(0, 12));

	gotoxy(2, y++);
	cout << "MONEY: " << _inventory.getMoney();

	gotoxy(2, y++);
	cout << "LEVEL: " << _level << "/" << LEVEL_MAX;

	if (_level < LEVEL_MAX)
	{
		gotoxy(2, y++);
		cout << "EXP: " << _exp << "/" << _expCurve[_level+1];
	}

	gotoxy(2, y++);
	cout << "HEALTH: " << _health << "/" << _hpmaxCurve[_level];

	gotoxy(2, y++);
	cout << "ATTACK: " << weapon.getWeaponDamage() + addedDamage;

	gotoxy(2, y++);
	cout << "DEFENSE: " << helmet.getArmorDefense() + chestplate.getArmorDefense() + addedDefense;


	//DISPLAY EQUIPMENT
	setColor(consoleColor(0, 14));
	if (weapon.getName() != "Missing")
	{
		gotoxy(1, y);
		cout << "Weapon: " << weapon.getName();
		gotoxy(30, y);
		cout <<" Attack: " << weapon.getWeaponDamage() << endl;
		y++;
	}
	if (helmet.getName() != "Missing")
	{
		gotoxy(1, y);
		cout << "Head: " << helmet.getName(); 
		gotoxy(30, y);
		cout << " Defense: " << helmet.getArmorDefense() << endl;
		y++;
	}
	if (chestplate.getName() != "Missing")
	{
		gotoxy(1, y);
		cout << "Chest: " << chestplate.getName();
		gotoxy(30, y);
		cout << " Defense: " << chestplate.getArmorDefense() << endl;
	}

	setColor(consoleColor(0, 7));

	system("pause > nul");
	cls();
}

void Player::showInventory()
{
	int page = 0;
	char input;

	if (_inventory.size() == 0)
	{
		cls();
		center ("*** " + _name + "'s Inventory ***", 0);
		center("The inventory is empty.", 10);
		system("pause > nul");
		return;
	}

	do{
		_inventory.print(page, _name);
		cout << "\n";
		if (page == 0)
			cout << "Next page: A\tE to equip\tU to use\tESC to exit\n";
		else if (page == _inventory.nPages())
			cout << "Previous page: Q\tE to equip\tU to use\tESC to exit\n";
		else
			cout << "Next page: A\tPrevious page: Q\tE to equip\tU to use\tESC to exit\n";

		input = _getch();


		string itemName;
		int itemType;

		switch(lowCase(input))
		{
		case 'q': //Previous page
			if (page != 0)
				page--;
			break;

		case 'a': //Next page
			if (page != _inventory.nPages())
				page++;
			break;

		//EQUIP ITEMS	-------------------------------------------------------------------------------
		case 'e':
			cout << "\n\nWhat would you like to equip? - ";
			getline(cin, itemName);
		
			if (contains(itemName, "123456789")) //Input of a number is ok, just get the right name
			{
				int item;
				sscanf_s(itemName.c_str(), "%d", &item);

				if (item <= 0 || item > _inventory.size())
					continue;
					
				itemName = _inventory.getItem(item-1).getName();
			}
			if (!_inventory.containsItem(itemName))
			{
				cout << "You haven't got a " << itemName << ".\n";
				system("pause>nul");
				break;
			}

			itemType = _inventory.getItem(itemName).getType();

			if (itemType == IT_WEAPON)
			{
				if (_equipment.getWeapon().getName() != "Missing")
					_inventory.addItem(_equipment.getWeapon().getName(), 1);
				_equipment.setWeapon(_inventory.getItem(itemName));
				_inventory.addItem(itemName, -1);
			}
			else if (itemType == IT_ARMOR)
			{
				int armorPlace = _inventory.getItem(itemName).getArmorPlace();

				if (armorPlace == AP_CHEST)
				{
					if (_equipment.getChestArmor().getName() != "Missing")
						_inventory.addItem(_equipment.getChestArmor().getName(), 1);
					_equipment.setChestArmor(_inventory.getItem(itemName));
					_inventory.addItem(itemName, -1);
				}
				else if (armorPlace == AP_HEAD)
				{
					if (_equipment.getHeadArmor().getName() != "Missing")
						_inventory.addItem(_equipment.getHeadArmor().getName(), 1);
					_equipment.setHeadArmor(_inventory.getItem(itemName));
					_inventory.addItem(itemName, -1);
				}
			}
			else
			{
				cout << "This item type cannot be equiped!\n";
				system("pause>nul");
			}
			break;

		//USE ITEM --------------------------------------------------------------------------------------------
		case 'u':
			cout << "\n\nWhat would you like to use? - ";
			getline(cin, itemName);

			if (contains(itemName, "123456789")) //Input of a number is ok, just get the right name
			{
				int item;
				sscanf_s(itemName.c_str(), "%d", &item);

				if (item <= 0 || item > _inventory.size())
					continue;

				itemName = _inventory.getItem(item-1).getName();
			}
			if (!_inventory.containsItem(itemName))
			{
				cout << "You haven't got a " << itemName << ".\n";
				system("pause>nul");
				break;
			}

			itemType = _inventory.getItem(itemName).getType();

			if (itemType == IT_POTION)
			{
				Item& potion = _inventory.getItem(itemName);
				
				addHealth(potion.getAddHealth());
				if (potion.getAddAttack() != 0 || potion.getAddDefense() != 0)
					_equipment.addActivePotion(potion);

				_inventory.addItem(itemName, -1);
			}
			else
			{
				cout << "This item cannot be used!\n";
				system("pause>nul");
			}

			break;

		}

	} while (input != K_ESC);
}

void Player::draw()
{
	gotoxy(_ox, _oy);
	cout << ' ';
	gotoxy(_x, _y);
	cout << '@';
}

void Player::clearActivePotions()
{
	_equipment.clearActivePotions();
}

int Player::getDefense()
{
	Item& chestplate = _equipment.getChestArmor();
	Item& helmet = _equipment.getHeadArmor();

	int defense=0;
	int addedDefense=0;

	for (unsigned int i=0; i<_equipment.getActivePotionsCount(); i++)
	{
		Item& activePotion = _equipment.getActivePotion(i);
		if (activePotion.getName() != "Missing")
		{
			addedDefense += activePotion.getAddDefense();
		}
	}

	if (helmet.getName() != "Missing")
		defense += helmet.getArmorDefense();

	if (chestplate.getName() != "Missing")
		defense += chestplate.getArmorDefense();

	return defense + addedDefense;
}

int Player::getDamage()
{
	Item& weapon = _equipment.getWeapon();

	int attack=0;
	int addedAttack=0;

	for (unsigned int i=0; i<_equipment.getActivePotionsCount(); i++)
	{
		Item& activePotion = _equipment.getActivePotion(i);
		if (activePotion.getName() != "Missing")
		{
			addedAttack += activePotion.getAddAttack();
		}
	}

	if (weapon.getName() != "Missing")
		attack += weapon.getWeaponDamage();

	return (attack + addedAttack);
}

void Player::getSaveString(string& saveString)
{
	stringstream sstr;

	sstr << "player.name=" << _name << "|";
	sstr << "player.x=" << _x << "|";
	sstr << "player.y=" << _y << "|";
	sstr << "player.money=" << _inventory.getMoney() << "|";
	sstr << "player.exp=" << _exp << "|";
	sstr << "player.health=" << _health << "|";
	sstr << "player.weapon=" << _equipment.getWeapon().getName() << "|";
	sstr << "player.chestArmor=" << _equipment.getChestArmor().getName() << "|";
	sstr << "player.headArmor=" << _equipment.getHeadArmor().getName() << "|";

	sstr << "player.inventory=";
	for (int i=0; i<_inventory.size(); i++)
	{
		Item& item = _inventory.getItem(i);
		sstr <<  item.getName() << "*" << item.getCount() << ":";
	}

	saveString = sstr.str();
}

void Player::loadSave(vector<string> properties)
{
	_inventory = Inventory();

	for (unsigned int i=0; i<properties.size(); i++)
	{
		string& line = properties[i];

		string name = split(line, '=', 0);
		string value = split(line, '=', 1);
	
		if (name == "player.money")
		{
			int money;
			sscanf_s(value.c_str(), "%d", &money);
			_inventory.setMoney(money);
		}	
		else if (name == "player.name")
		{
			_name=value;
		}
		else if (name == "player.x")
		{
			sscanf_s(value.c_str(), "%d", &_x);
		}
		else if (name == "player.y")
		{
			sscanf_s(value.c_str(), "%d", &_y);
		}
		else if (name == "player.health")
		{
			sscanf_s(value.c_str(), "%d", &_health);
		}
		else if (name == "player.exp")
		{
			sscanf_s(value.c_str(), "%d", &_exp);
			updateLevel();
		}
		else if (name == "player.weapon")
		{
			if (value != "Missing")
				_equipment.setWeapon(Item(value));
		}
		else if (name == "player.chestArmor")
		{
			if (value != "Missing")
				_equipment.setChestArmor(Item(value));
		}
		else if (name == "player.headArmor")
		{
			if (value != "Missing")
				_equipment.setHeadArmor(Item(value));
		}
		else if (name == "player.inventory")
		{
			vector<string> items;
			splitInto(value, ':', items);


			for(unsigned int j=0; j<items.size(); j++)
			{
				string itemName, strItemCount;
				int itemCount;

				itemName = split(items[j], '*', 0);
				strItemCount = split(items[j], '*', 1);
				if (sscanf_s(strItemCount.c_str(), "%d", &itemCount) == 1)
				{
					_inventory.addItem(itemName, itemCount);
				}
			}
		}
		else
		{
			cout << "Invalid property for player initial resources: " << name << ".\n";
			system("pause");
		}
	}
}

//PRIVATE
void Player::updateLevel()
{
	if (_level == LEVEL_MAX)
		return;

	while (_exp >= _expCurve[_level+1])
	{
		if (_level == LEVEL_MAX)
			return;
		_health = _hpmaxCurve[++_level];
	}
}

void Player::kill()
{
	_dead = true;
}

void Player::loadCurve()
{
	string path = "Data\\curve.txt";

	ifstream in;
	in.open(path);

	if (in.fail())
		cout << "Could not load file '" << path << "'.\n";

	while (!in.eof())
	{
		int level;
		string line, strlevel, strexp, strmaxhp;

		getline(in, line);

		if (!contains(line, ':'))
			continue;

		strlevel = split(line, ':', 0);
		sscanf_s (strlevel.c_str(), "%d", &level);
		
		if (level >= 0 && level <= LEVEL_MAX)
		{
			strexp = split(line, ':', 1);
			sscanf_s (strexp.c_str(), "%d", &_expCurve[level]);
			
			strmaxhp = split(line, ':', 2);
			sscanf_s (strmaxhp.c_str(), "%d", &_hpmaxCurve[level]);
			//cout << "Level: " << level << ", Exp: " << _expCurve[level] << endl;
		}
		else
			cout << "Invalid level: " << level << endl;
	}

	in.close();
}

void Player::loadInitialResources()
{
	ifstream in;
	in.open("Data\\Player.properties");

	if (in.fail())
	{
		cout << "Could not load initial resources file 'Data\\Player.txt'!\n";
		system("pause");
	}

	while(!in.eof())
	{
		string line, name, value;
		getline(in, line);

		if (!contains(line, '='))
			continue;

		name = split(line, '=', 0);
		value = split(line, '=', 1);
		

		if (name == "money")
		{
			int money;
			sscanf_s(value.c_str(), "%d", &money);
			_inventory.setMoney(money);
		}
		else if (name == "exp")
		{
			sscanf_s(value.c_str(), "%d", &_exp);
			updateLevel();
		}
		else if (name == "weapon")
			_equipment.setWeapon(Item(value));

		else if (name == "chestArmor")
			_equipment.setChestArmor(Item(value));

		else if (name == "headArmor")
			_equipment.setHeadArmor(Item(value));
		
		else if (name == "inventory")
		{
			vector<string> items;

			splitInto(value, ':', items);

			for(unsigned int i=0; i<items.size(); i++)
			{
				string itemName, strItemCount;
				int itemCount;
				
				itemName = split(items[i], '*', 0);
				strItemCount = split(items[i], '*', 1);
				if (sscanf_s(strItemCount.c_str(), "%d", &itemCount) == 1)
				{
					_inventory.addItem(itemName, itemCount);
				}
			}
		}
		else
		{
			cout << "Invalid property for player initial resources: " << name << ".\n";
			system("pause");
		}
	}

	in.close();
}
