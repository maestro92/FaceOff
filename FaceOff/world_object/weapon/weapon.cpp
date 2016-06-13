
#include "weapon.h"


Weapon::Weapon()
{
//	m_type = MELEE;
	hasOwner = false;
	m_angle = 0.0f;
	m_dynamicType = DYNAMIC;

	m_explodeDelayStartTime = 0;
	m_explodeDelayTime = 2000;
	m_readyToExplode = false;
}

Weapon::Weapon(WeaponData data)
{
	setData(data);
	hasOwner = false;
	m_angle = 0.0f;
	m_dynamicType = DYNAMIC;

	m_explodeDelayStartTime = 0;
	m_explodeDelayTime = 2000;
	m_readyToExplode = false;
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


	m_nameEnum = data.nameEnum;

	m_modelScale = data.modelScale;

	setScale(m_modelScale);

	m_firstPOVScale = data.firstPOVScale;
	m_firstPOVOffset = data.firstPOVOffset;


	m_slotEnum = data.slotEnum;
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


	if (m_slotEnum == PROJECTILE && m_explodeDelayMode)                                                                                                                                                                                                                        
	{
		long long curDelay = utl::getCurrentTimeMillis() - m_explodeDelayStartTime;
		
		cout << "curDelay" << " " << curDelay << endl;
		cout << "m_explodeDelayTime" << " " << m_explodeDelayTime << endl;

		/*
		utl::debug("curDelay", curDelay);
		utl::debug("m_explodeDelayTime", m_explodeDelayTime);
		*/

		if ((int)curDelay > (int)m_explodeDelayTime)
		{
			m_readyToExplode = true;
		}

		
	}
}


WorldObjectType Weapon::getObjectType()
{
	return WEAPON;
}


void Weapon::startExplodeDelayTimer()
{
	m_explodeDelayMode = true;
	m_explodeDelayStartTime = utl::getCurrentTimeMillis();
}



WeaponSlotEnum Weapon::getWeaponSlot()
{
	return m_slotEnum;
}

WeaponNameEnum Weapon::getWeaponName()
{
	return m_nameEnum;
}


bool Weapon::shouldExplode()
{
	return m_readyToExplode;
}


ParticleEffect* Weapon::explode()
{
	ParticleEffect* effect = new ParticleEffect();

	effect->setPosition(m_position);
	effect->setScale(50.0);

	effect->init();
	effect->setTexture("Assets/fireworks_red.jpg");

	return effect;
}


