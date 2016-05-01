#ifndef WEAPON_DATA_H_
#define WEAPON_DATA_H_

#include <iostream>

using namespace std;

#include "weapon_enum.h"
#include "model.h"
#include "utility.h"

struct WeaponData
{
	string displayedName;
	WeaponNameEnum name;
	int damage;
	int magazineCapacity;
	int maxMagazineCount;
	float modelScale;

	float firstPOVScale;
	glm::vec3 firstPOVOffset;

	WeaponSlotEnum slot;
//	WeaponTypeEnum type;
	Model* model;
};

#endif