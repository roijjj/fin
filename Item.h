#pragma once

#include <string>

enum ItemType
{
	IT_WEAPON,
	IT_POTION,
	IT_ARMOR
};

enum ArmorPlace
{
	AP_HEAD,
	AP_CHEST,
};

class Item
{
private:
	std::string _name;

	int _type;

	int _count;
	int _value;

	//Type-specific
	int _weaponDamage;
	int _armorDefense, _armorPlace;
	int _addHealth;

	int _addAttack, _addDefense;

	void load(std::string& name);

public:
	Item(void);
	Item(std::string name);

	void add(int amount);

	//GETTERS
	std::string& getName() { return _name; }
	int getCount() { return _count; }
	int getValue() { return _value; }
	int getType() { return _type; }

	int getWeaponDamage() { return _weaponDamage; }
	int getArmorDefense() { return _armorDefense; }
	int getArmorPlace() { return _armorPlace; }
	int getAddHealth() { return _addHealth; }
	int getAddAttack() { return _addAttack; }
	int getAddDefense() { return _addDefense; }

};

