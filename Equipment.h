#pragma once
#include "Item.h"

#include <vector>

class Equipment
{
private:
	Item _weapon;
	Item _armorChest, _armorHead;
	std::vector<Item> _activePotions;

public:
	Equipment(void);

	void addActivePotion(Item& potion);
	void clearActivePotions();

	//SETTERS
	void setWeapon(Item& weapon) { _weapon = weapon; }
	void setHeadArmor(Item& hArmor) { _armorHead = hArmor; }
	void setChestArmor(Item& cArmor) { _armorChest = cArmor; }

	//GETTERS
	Item& getWeapon() { return _weapon; }
	Item& getHeadArmor() { return _armorHead; }
	Item& getChestArmor() { return _armorChest; }
	unsigned int getActivePotionsCount();
	Item& getActivePotion(unsigned int index);

};

