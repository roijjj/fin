#include "NPC.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <conio.h>
#include "strings.h"
#include "utils.h"

using namespace std;

NPC::NPC(void)
{
	_x = 0;
	_y = 0;
	_repr = ' ';
	_canTrade=false;
	_minimumTradeLevel=0;
}

NPC::NPC(string name, string mapName, int startx=0, int starty=0)
{
	_name = name;
	_mapName = mapName;
	_x = startx;
	_y = starty;
	_canTrade=false;
	_minimumTradeLevel=0;

	load (name);
}

NPC::NPC(string name, string mapName, char repr=' ', int startx=0, int starty=0)
{
	_name = name;
	_mapName = mapName;
	_x = startx;
	_y = starty;
	_repr = repr;
	_canTrade=false;
	_minimumTradeLevel=0;
}

NPC::NPC(vector<string> properties)
{
	for (unsigned int i=0; i<properties.size(); i++)
	{
		string& line = properties[i];
		string name, value;

		if (!contains(line, '='))
			continue;

		name = split(line, '=', 0);
		value = split(line, '=', 1);

		// READ THE PROPERTIES
		if (name == "npc.repr")
			sscanf_s(value.c_str(), "%c", &_repr);

		else if (name == "npc.x")
			sscanf_s(value.c_str(), "%c", &_x);

		else if (name == "npc.y")
			sscanf_s(value.c_str(), "%c", &_y);

		else if (name == "npc.name")
			_name = value;

		else if (name == "npc.mapName")
			_mapName = value;

		else if (name == "npc.canTrade")
		{
			char cs_val[10];
			string val;
			sscanf_s(value.c_str(), "%9s", cs_val, _countof(cs_val));
			val = string(cs_val);
			if (val == "true" || val == "1")
				_canTrade = true;
			else if (val == "false" || val == "0")
				_canTrade = false;
		}
		else if (name == "npc.minimumTradeLevel")
		{
			sscanf_s(value.c_str(), "%d", &_minimumTradeLevel);
		}
		else if (name == "npc.money")
		{
			int money;
			sscanf_s(value.c_str(), "%d", &money);
			_inventory.setMoney(money);
		}
		else if (name == "npc.inventory")  //INITIALIZE THE INVENTORY
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
			cout << "Invalid property for NPC: " << name << ".\n";
	}
}

void NPC::draw()
{
	gotoxy(_x, _y);
	cout << _repr;
}

void NPC::trade(Inventory& playerInventory, string playerName)
{
	int page = 0;
	char input = ' ';
	int value;

	/*if (_inventory.size() == 0)
	{
		cls();
		center ("*** " + _name + "'s Inventory ***", 0);
		center("The inventory is empty.", 10);
		system("pause > nul");
		cls();
		return;
	}*/

	do{
		setColor(consoleColor(0, 15));
		_inventory.print(page, _name);

		setColor(consoleColor(0, 10));
		playerInventory.print(page, playerName, 3 + Inventory::ITEMS_PER_PAGE);

		setColor(consoleColor(0, 14));

		cout << "\n";
		if (page == 0)
			cout << "Next page: A\tB to buy\tS to sell\tESC to exit\t\n";
		else if (page == _inventory.nPages())
			cout << "Previous page: Q\tB to buy\tS to sell\tESC to exit\n";
		else
			cout << "Next page: A\tPrevious page: Q\tB to buy\tS to sell\tESC to exit\n";

		input = _getch();

		string itemName, strItemQuant;
		int itemQuant;

		switch (lowCase(input))
		{
		case 'q':
			if (page != 0)
				page--;
			break;

		case 'a':
			if (page != _inventory.nPages())
				page++;
			break;

		case 's':
			cout << "\n\nWhat would you like to sell? - ";
			getline(cin, itemName);
			cout << "How many? - ";
			getline(cin, strItemQuant);
			sscanf_s(strItemQuant.c_str(), "%d", &itemQuant);

			if (contains(itemName, "1234567890"))
			{
				int item;
				sscanf_s(itemName.c_str(), "%d", &item);
				itemName = playerInventory.getItem(item-1).getName();
			}

			if (itemQuant < 0)
			{
				cout << "Uh? What do you mean by 'negative number of items'?\n";
				system("pause>nul");
				break;
			}

			if (playerInventory.containsItem(itemName, itemQuant))
			{
				int value = playerInventory.getItem(itemName).getValue();

				if (_inventory.getMoney() >= value*itemQuant)
				{
					playerInventory.addItem(itemName, -itemQuant);
					playerInventory.addMoney(value*itemQuant);
					_inventory.addItem(itemName, itemQuant);
					_inventory.addMoney(-value*itemQuant);
				}
				else
				{
					cout << _name << " can't afford " << itemQuant << " x " << itemName <<  "s.\n";
					system("pause>nul");
				}
			}
			else
			{
				cout << "Player hasn't got enough " << itemName << "s.\n";
				system("pause>nul");
			}
			break;

		case 'b':
			cout << "\n\nWhat would you like to buy? - ";
			getline(cin, itemName);
			cout << "How many? - ";
			getline(cin, strItemQuant);
			sscanf_s(strItemQuant.c_str(), "%d", &itemQuant);

			if (contains(itemName, "1234567890"))
			{
				int item;
				sscanf_s(itemName.c_str(), "%d", &item);
				itemName = _inventory.getItem(item-1).getName();
			}

			if (itemQuant < 0)
			{
				cout << "Uh? What do you mean by 'negative number of items'?\n";
				system("pause>nul");
				break;
			}

			if (!_inventory.containsItem(itemName, itemQuant))
			{
				cout << _name << " hasn't got enough " << itemName << "s.\n";
				system("pause>nul");
				break;
			}
			value = _inventory.getItem(itemName).getValue();

			if (playerInventory.getMoney() >= value*itemQuant)
			{
				playerInventory.addItem(itemName, itemQuant);
				playerInventory.addMoney(-value*itemQuant);
				_inventory.addItem(itemName, -itemQuant);
				_inventory.addMoney(value*itemQuant);
			}
			else
			{
				cout << "You can't afford " << itemQuant << " x " << itemName <<  "s.\n";
				system("pause>nul");
			}
			
			break;

		default:
			break;
		}
	} while (input != K_ESC);

	cls();
}

void NPC::getSaveString(string& str)
{
	stringstream sstr;

	sstr << "npc.name=" << _name << "|";
	sstr << "npc.mapName=" << _mapName << "|";
	sstr << "npc.x=" << _x << "|";
	sstr << "npc.y=" << _y << "|";
	sstr << "npc.repr=" << _repr << "|";
	sstr << "npc.canTrade=" << (int) _canTrade << "|";
	sstr << "npc.minimumTradeLevel=" << _minimumTradeLevel << "|";
	sstr << "npc.inventory=";

	for (int i=0; i<_inventory.size(); i++)
	{
		Item& item = _inventory.getItem(i);
		sstr << item.getName() << "*" << item.getCount() << ":";
	}

	sstr << "|newNpc";

	str = sstr.str();
}

//Private
void NPC::load(string name)
{
	string path = "Data\\NPCs\\"+name+".npc";

	ifstream in;

	//LOAD NPC DATA FROM FILE --------------------------------
	in.open(path);

	if (in.fail())
	{
		cout << "Could not open file: " << path << ".\n";
	}
	
	while(!in.eof())
	{
		string line, name, value;
		getline(in, line);

		if (!contains(line, '='))
			continue;

		name = split(line, '=', 0);
		value = split(line, '=', 1);

		// READ THE PROPERTIES
		if (name == "repr")
			sscanf_s(value.c_str(), "%c", &_repr);

		else if (name == "canTrade")
		{
			char cs_val[10];
			string val;
			sscanf_s(value.c_str(), "%9s", cs_val, _countof(cs_val));
			val = string(cs_val);
			if (val == "true" || val == "1")
				_canTrade = true;
			else if (val == "false" || val == "0")
				_canTrade = false;
		}
		else if (name == "minimumTradeLevel")
		{
			sscanf_s(value.c_str(), "%d", &_minimumTradeLevel);
		}
		else if (name == "money")
		{
			int money;
			sscanf_s(value.c_str(), "%d", &money);
			_inventory.setMoney(money);
		}
		else if (name == "inventory")  //INITIALIZE THE INVENTORY
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
			cout << "Invalid property for NPC: " << name << ".\n";
	}

	in.close();
}
