#ifndef WEAPON_H_
#define WEAPON_H_

#include "world_object.h"
#include "imported_model.h"
#include "weapon_enum.h"
#include "weapon_data.h"

using namespace std;


struct Magazine
{
	int cur;
	int max;
};


class Weapon : public WorldObject
{
	public:
		Weapon();
		Weapon(WeaponData data);
		~Weapon();

		void setData(WeaponData data);

		void setType(WeaponTypeEnum type);
		int getType();

		void setName(WeaponNameEnum type);
		int getName();

		int m_damage;
		Magazine m_magazine;
		int m_magazineCount;

	private:
		WeaponTypeEnum m_type;
		WeaponNameEnum m_name;
};



#endif