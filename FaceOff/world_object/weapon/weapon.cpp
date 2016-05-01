
#include "weapon.h"


Weapon::Weapon()
{
//	m_type = MELEE;
	hasOwner = false;
	m_angle = 0.0f;
}

Weapon::Weapon(WeaponData data)
{
	setData(data);
	hasOwner = false;
	m_angle = 0.0f;
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

//	m_type = data.type;
	m_name = data.name;

	m_modelScale = data.modelScale;

	setScale(m_modelScale);

	m_firstPOVScale = data.firstPOVScale;
	m_firstPOVOffset = data.firstPOVOffset;


	m_slot = data.slot;
	m_model = data.model;

	m_wireFrameModel = new CubeWireFrameModel(m_model->m_aabb);
}


void Weapon::updateGameInfo()
{
	WorldObject::updateGameInfo();

	if (!hasOwner)
	{
		m_rotation *= glm::rotate(m_angle, 0.0f, 1.0f, 0.0f);
//		m_angle += ROTATION_SPEED;

		m_angle += 0.0001;

		if (m_angle > 360)
		{
			m_angle -= 360;
		}

	}
}


WorldObjectType Weapon::getObjectType()
{
	return WEAPON;
}
