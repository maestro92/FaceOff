#ifndef WEAPON_H_
#define WEAPON_H_

#include "world_object.h"
#include "imported_model.h"
#include <unordered_map>
#include "json_spirit_reader_template.h"

#include "weapon_enum.h"

using namespace std;


struct Magazine
{
	int capacity;
	int cur;
};


class Weapon : public WorldObject
{
	public:
		Weapon();
		~Weapon();
		
		void setType(WeaponTypeEnum type);
		int getType();

		string m_weaponName;
		ImportedModel* m_model;

		int m_damage;
		Magazine m_magazine;
		int m_magazineCount;
		

		void render(Pipeline& p, Renderer* r);

		static unordered_map<string, ImportedModel> s_weaponModels;
		static void writeWeaponJsonFiles();
		static void initGameWeapons();



//	private:
		int m_type;
};



#endif