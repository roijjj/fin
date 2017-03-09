#include "Inventory.h"

#include <iostream>
#include "utils.h"
#include "strings.h"

using namespace std;

Inventory::Inventory(void)
{
}

void Inventory::addItem(string name, int amount)
{
	bool found = false;
	list<Item>::iterator lit;

	for(lit = _items.begin(); lit != _items.end(); lit++)
	{
		if ((*lit).getName() == name)	
		{
			(*lit).add(amount);
			if ((*lit).getCount() <= 0)
				_items.erase(lit);

			found = true;
			break;
		}
	}

	if (!found && amount > 0)
	{
		Item item(name);
		item.add(amount-1);
		insert_ordered(item);
	}
}

bool Inventory::containsItem(string name)
{
	list<Item>::iterator lit;

	for(lit = _items.begin(); lit != _items.end(); lit++)
		if ((*lit).getName() == name)	
			return true;

	return false;
}

bool Inventory::containsItem(string name, int amount)
{
	list<Item>::iterator lit;

	int count=0;

	for(lit = _items.begin(); lit != _items.end(); lit++)
		if ((*lit).getName() == name)	
			count = (*lit).getCount();

	return (count >= amount);
}

void Inventory::print(int page, string& ownerName, int firstRow)
{
	if (!firstRow)
		cls();
	list<Item>::iterator lit;

	center("*** " + ownerName + "'s Inventory ***", firstRow);
	cout << "\n[PAGE " << page+1 << "]\tMoney: " << _money << "\n";

	int i = 0;

	for( lit = _items.begin(); lit != _items.end(); lit++, i++)
	{
		if ((i >= page*ITEMS_PER_PAGE) && (i < page*ITEMS_PER_PAGE + ITEMS_PER_PAGE))
		{
			int y = i-page*ITEMS_PER_PAGE+2 + firstRow;

			gotoxy(0, y); //PRINT NAME
			cout << i+1 << ": " << (*lit).getName();

			gotoxy(21, y); //PRINT COUNT
			cout << "x" << (*lit).getCount();

			gotoxy(28, y); //PRINT VALUE
			cout << "V=" << (*lit).getValue();

			if ((*lit).getType() == IT_WEAPON)
			{
				gotoxy(36, y); //PRINT WEAPON DAMAGE
				cout << "A=" << (*lit).getWeaponDamage();
			}
			else if ((*lit).getType() == IT_ARMOR)
			{
				gotoxy(36, y); //PRINT ARMOR DEFENSE
				cout << "D=" << (*lit).getArmorDefense();
				gotoxy(41, y); //PRINT ARMOR PLACE
				if ((*lit).getArmorPlace() == AP_HEAD)
					cout << "HEAD";
				else if ((*lit).getArmorPlace() == AP_CHEST)
					cout << "CHEST";
			}
			else if ((*lit).getType() == IT_POTION)
			{
				int x=36;
				int addHealth = (*lit).getAddHealth();
				int addAttack = (*lit).getAddAttack();
				int addDefense = (*lit).getAddDefense();

				if (addHealth > 0)
				{
					gotoxy(x, y); //PRINT WEAPON DAMAGE
					cout << "+H=" << addHealth;
					x+=6;
				}
				else if (addHealth < 0)
				{
					gotoxy(x, y); //PRINT WEAPON DAMAGE
					cout << "-H=" << -addHealth;
					x+=6;
				}
				
				if (addAttack > 0)
				{
					gotoxy(x, y); //PRINT WEAPON DAMAGE
					cout << "+A=" << addAttack;
					x+=6;
				}
				else if (addAttack < 0)
				{
					gotoxy(x, y); //PRINT WEAPON DAMAGE
					cout << "-A=" << -addAttack;
					x+=6;
				}

				if (addDefense > 0)
				{
					gotoxy(x, y); //PRINT WEAPON DAMAGE
					cout << "+D=" << addDefense;
					x+=6;
				}
				else if (addDefense < 0)
				{
					gotoxy(x, y); //PRINT WEAPON DAMAGE
					cout << "-D=" << -addDefense;
					x+=6;
				}
			}
		}
	}

	cout << "\n\n";
}

Item& Inventory::getItem(string name)
{
	list<Item>::iterator lit;

	for (lit=_items.begin(); lit != _items.end(); lit++)
	{
		if (name == (*lit).getName())
			return (*lit);
	}

	lit=_items.begin();
	return (*lit);
}

Item& Inventory::getItem(int index)
{
	list<Item>::iterator lit = _items.begin();
	for (int i=0; i<index; i++)
		lit++;

	return (*lit);
}

//PRIVATE
void Inventory::insert_ordered(Item& item)
{
	list<Item>::iterator lit;

	for(lit = _items.begin(); lit != _items.end(); lit++)
	{
		if (lowerCase((*lit).getName()) > lowerCase(item.getName()))
		{
			_items.insert(lit, item);
			return;
		}
	}

	_items.push_back(item);
}

int Inventory::nPages()
{
	if (_items.size() > 0)
		return (_items.size()-1)/Inventory::ITEMS_PER_PAGE; 
	else 
		return 0;
}

bool Inventory::addMoney(int amount)
{
	if (amount >= 0)
	{
		_money += amount;
		return true;
	}
	else if (amount < 0 && -amount <= _money)
	{
		_money += amount;
		return true;
	}
	else
		return false;
}