
#include "weapon.h"


Weapon::Weapon()
{
	hasOwner = false;
	isBeingUsed = false;
	m_angle = 0.0f;
	m_dynamicType = DYNAMIC;

	m_explodeDelayStartTime = 0;
	m_explodeDelayTime = 2000;
	m_ignorePhysicsAfterThrownTime = 200;

	m_explodeDelayMode = false;
	m_readyToExplode = false;
	m_grenadeThrowerInstanceId = -1;
}

Weapon::Weapon(WeaponData data)
{
	setData(data);
	hasOwner = false;
	isBeingUsed = false;
	m_angle = 0.0f;
	m_dynamicType = DYNAMIC;

	m_explodeDelayStartTime = 0;
	m_explodeDelayTime = 2000;
	m_ignorePhysicsAfterThrownTime = 200;

	m_explodeDelayMode = false;
	m_readyToExplode = false;
	m_grenadeThrowerInstanceId = -1;
}


Weapon::~Weapon()
{
	delete m_wireFrameModel;
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

	m_thirdPOVScale = data.thirdPOVScale;
	m_thirdPOVOffset = data.thirdPOVOffset;

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
		
	//	cout << "curDelay" << " " << curDelay << endl;
	//	cout << "m_explodeDelayTime" << " " << m_explodeDelayTime << endl;

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
	SmokeEffect* effect = new SmokeEffect();

	effect->setPosition(m_position);
	effect->m_position.y += 5.0;

//	effect->m_position.y = 5.0;

	utl::debug("position", m_position);

	effect->setScale(5.0);

	effect->init();
//	effect->setTexture("Assets/Images/smoke_sprite.png");

	return effect;
}

void Weapon::setGrenadeThrowerId(int id)
{
	m_grenadeThrowerInstanceId = id;
}

int Weapon::getGrenadeThrowerId()
{
	return m_grenadeThrowerInstanceId;
}


void Weapon::renderGroup(Pipeline& p, Renderer* r)
{
	if (hasOwner == false || (hasOwner && isBeingUsed))
	{
		WorldObject::renderGroup(p, r);
	}
}


void Weapon::renderWireFrameGroup(Pipeline& p, Renderer* r)
{
	if (hasOwner == false || (hasOwner && isBeingUsed))
	{
		WorldObject::renderWireFrameGroup(p, r);
	}
}


bool Weapon::ignorePhysics()
{
	return (hasOwner == true || m_slotEnum != PROJECTILE);
}


bool Weapon::ignorePhysicsWith(WorldObject* obj)
{
	int id = obj->getInstanceId();

	if (id != m_grenadeThrowerInstanceId)
	{
		return false;
	}

	if (m_explodeDelayMode)
	{
		long long curDelay = utl::getCurrentTimeMillis() - m_explodeDelayStartTime;

		if ((int)curDelay < (int)m_ignorePhysicsAfterThrownTime)
		{
			return true;
		}
	}
	return false;
}

