#ifndef WEAPON_H_
#define WEAPON_H_

#include "world_object.h"
#include "imported_model.h"
#include <unordered_map>

using namespace std;

enum WeaponType
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


class Weapon : public WorldObject
{
	public:


		Weapon();
		~Weapon();
		
		virtual int getType() = 0;

		string m_gunType;
		ImportedModel* m_model;

		void render(Pipeline& p, Renderer* r);

		static unordered_map<string, ImportedModel> s_weaponModels;
		static void initWeaponModels();
		
};



#endif