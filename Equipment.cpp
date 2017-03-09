#include "Equipment.h"

#include <iostream>

using namespace std;

Equipment::Equipment(void)
{
}

unsigned int Equipment::getActivePotionsCount()
{
	return _activePotions.size();
}

Item& Equipment::getActivePotion(unsigned int index)
{
	if (index >= 0 && index < _activePotions.size())
		return _activePotions[index];
	else 
	{
		cout << "Index out of range!\n";
		system("pause");
		return _weapon;
	}
}

void Equipment::addActivePotion(Item& potion)
{
	_activePotions.push_back(potion);
}

void Equipment::clearActivePotions()
{
	_activePotions.clear();
}