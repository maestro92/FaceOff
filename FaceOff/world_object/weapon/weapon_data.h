#ifndef WEAPON_DATA_H_
#define WEAPON_DATA_H_

#include <iostream>

using namespace std;

#include "fire_work_effect.h"
#include "weapon_enum.h"
#include "model.h"
#include "utility.h"

struct WeaponData
{
	string displayedName;
	WeaponNameEnum nameEnum;
	WeaponSlotEnum slotEnum;

	int damage;
	int magazineCapacity;
	int maxMagazineCount;

	float modelScale;
	float firstPOVScale;
	glm::vec3 firstPOVOffset;
	float thirdPOVScale;
	glm::vec3 thirdPOVOffset;

	Model* model;
};

#endif