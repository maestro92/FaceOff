#ifndef WEAPON_DATA_H_
#define WEAPON_DATA_H_

#include <iostream>

using namespace std;

#include "weapon_enum.h"
#include "model.h"

struct WeaponData
{
	string displayedName;
	WeaponNameEnum name;
	int damage;
	int magazineCapacity;
	int maxMagazineCount;
	WeaponTypeEnum type;
	Model* model;
};

#endif