#include "Item.h"

#include <iostream>
#include <fstream>
#include "strings.h"

using namespace std;

Item::Item(void)
{
	_name = "Missing";
	_count = 0;

	_weaponDamage = 0;
	_armorDefense = 0;
	_addAttack = 0;
	_addDefense = 0;
}


Item::Item(string name)
{
	_name = name;
	_count = 1;

	_weaponDamage = 0;
	_armorDefense = 0;
	_addAttack = 0;
	_addDefense = 0;

	load(name);
}

void Item::add(int amount)
{
	_count += amount;
}

//Private
void Item::load(string& name)
{
	string path = "Data\\Items\\" + name + ".item";
	ifstream in;

	in.open(path);

	if (in.fail())
	{
		cout << "Could not open file '" << path << "'.\n";
		system("Pause>nul");
		return;
	}

	while(!in.eof())
	{
		string line, value, name;
		getline(in, line);

		if (!contains(line, '='))
			continue;

		name = split(line, '=', 0);
		value = split(line, '=', 1);

		if (name == "type")
		{
			if (value == "weapon")
				_type = IT_WEAPON;
			else if (value == "armor")
				_type = IT_ARMOR;
			else if (value == "potion")
				_type = IT_POTION;
			else
			{
				cout << "Invalid type.\n";
				system("pause");
			}
		}
		else if(name == "value")
		{
			sscanf_s(value.c_str(), "%d", &_value);
		}
		else if (name == "weaponDamage")
		{
			sscanf_s(value.c_str(), "%d", &_weaponDamage);
		}
		else if (name == "addHealth")
		{
			sscanf_s(value.c_str(), "%d", &_addHealth);
		}
		else if (name == "addAttack")
		{
			sscanf_s(value.c_str(), "%d", &_addAttack);
		}
		else if (name == "addDefense")
		{
			sscanf_s(value.c_str(), "%d", &_addDefense);
		}
		else if (name == "armorDefense")
		{
			sscanf_s(value.c_str(), "%d", &_armorDefense);
		}
		else if (name == "armorPlace")
		{
			if (value == "head")
				_armorPlace = AP_HEAD;
			else if (value == "chest")
				_armorPlace = AP_CHEST;
			else
			{
				cout << "Invalid place.\n";
				system("pause");
			}

			//sscanf_s(value.c_str(), "%d", &_armorPlace);
		}
	}

	in.close();
}