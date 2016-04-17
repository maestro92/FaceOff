
#include "weapon.h"


Weapon::Weapon()
{
	m_type = MELEE;
}

Weapon::Weapon(WeaponData data)
{
	setData(data);
}


Weapon::~Weapon()
{

}


void Weapon::setData(WeaponData data)
{
	m_damage = data.damage;

	m_magazine.cur = data.magazineCapacity;
	m_magazine.max = data.magazineCapacity;
	m_magazineCount = data.maxMagazineCount;

	m_type = data.type;
	m_name = data.name;

	m_model = data.model;
}