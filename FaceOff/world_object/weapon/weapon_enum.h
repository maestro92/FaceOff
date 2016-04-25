#ifndef WEAPON_ENUM
#define WEAPON_ENUM

#include <unordered_map>

using namespace std;


enum WeaponTypeEnum
{
	MELEE = 0,
	PISTOL,
	SHOTGUN,
	SUBMACHINE_GUN,
	ASSULT_RIFLE,
	SNIPER_RIFLE,
	MACHINE_GUN,
	NUM_WEAPON_TYPE
};


enum WeaponNameEnum
{
	KNIFE = 0,
	KATANA,
	
	// PISTOL
	DESERT_EAGLE,


	MAC_11,

	// SUBMACHINE_GUN
	MP5,	


	// assult rifle
	AK_47,

	M16,
	AWM,		// same as AWP



	MINIGUN,

	PISTOL_SHOTGUN,


	MG42,

	// gernades
	FLASH_BANG,
	SMOKE_GRENADE,
	HE_GRENADE
};





#endif