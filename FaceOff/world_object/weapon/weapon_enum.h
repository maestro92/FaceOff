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
	BUTTERFLY_KNIFE = 0,

	// SUBMACHINE_GUN
	MP_5,	


	// assult rifle
	AK_47,




	// gernades
	FLASH_BANG,
	SMOKE_GRENADE,
	HE_GRENADE
};





#endif