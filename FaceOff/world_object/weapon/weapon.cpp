
#include "weapon.h"



Weapon::Weapon()
{
/*
	ownerId = NO_OWNER;
	isBeingUsed = false;
	m_angle = 0.0f;
	m_dynamicType = DYNAMIC;

	m_explodeDelayStartTime = 0;
	m_explodeDelayTime = 2000;
	m_ignorePhysicsAfterThrownTime = 200;

	m_explodeDelayMode = false;
	m_readyToExplode = false;
	m_grenadeThrowerInstanceId = -1;
*/
}

Weapon::Weapon(WeaponData data)
{
	init(data);
}


Weapon::~Weapon()
{
	if (onDelete != NULL)
	{
		onDelete(this);
	}

	if (m_wireFrameModel != NULL)
	{
		delete m_wireFrameModel;
	}
}

void Weapon::init(WeaponData data)
{
	setData(data);
	ownerId = ObjectId::NO_OWNER;

	setBeingUsed(false);
	m_angle = 0.0f;

	m_entityType = WEAPON;
	m_dynamicType = DYNAMIC;

	m_explodeDelayStartTime = 0;
	m_explodeDelayTime = 2000;
	m_ignorePhysicsAfterThrownTime = 200;

	m_explodeDelayMode = false;
	m_readyToExplode = false;

//	m_grenadeThrowerId = -1;
}

void Weapon::setData(WeaponData data)
{
	m_damage = data.damage;

	m_magazine.cur = data.magazineCapacity;
	m_magazine.max = data.magazineCapacity;
	m_curNumMagazine = data.maxMagazineCount;
	m_maxNumMagazine = data.maxMagazineCount;
	m_rpm = data.rpm;

	m_roundsCooldown = m_rpm;
	m_roundsCooldown /= 60;
	m_roundsCooldown = 1000 / m_roundsCooldown;

//	cout << "cooldown is " << m_roundsCooldown << endl;
//	cout << "data.magazineCapacity " << data.magazineCapacity << endl;

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


int Weapon::getCurAmmo()
{
	return m_magazine.cur;
}

int Weapon::getTotalAmmoCount()
{
	return m_magazine.max * m_maxNumMagazine;
}

void Weapon::reload()
{
	if (m_curNumMagazine > 0)
	{
		m_curNumMagazine--;
		m_magazine.cur = m_magazine.max;
	}
}

void Weapon::fire()
{
	if (m_magazine.cur > 0)
	{
		m_magazine.cur--;
	}

	// playing the firing animation
}

void Weapon::updateGameInfo()
{
	WorldObject::updateGameInfo();

	if (!hasOwner())
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


void Weapon::startExplodeDelayTimer()
{
	m_explodeDelayMode = true;
	m_explodeDelayStartTime = utl::getCurrentTimeMillis();
}



bool Weapon::shouldRender()
{
	if (hasOwner())
	{
		if (isBeingUsed())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
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


int Weapon::getDamage()
{
	return m_damage;
}

long long Weapon::getRoundsCooldown()
{
	return m_roundsCooldown;
}

/*
void Weapon::setGrenadeThrowerId(ObjectId id)
{
	m_grenadeThrowerId = id;
}

int Weapon::getGrenadeThrowerId()
{
	return m_grenadeThrowerId;
}
*/

void Weapon::renderGroup(Pipeline& p, Renderer* r)
{
	if (hasOwner() == false || (hasOwner() && isBeingUsed()))
	{
		WorldObject::renderGroup(p, r);
	}
}


void Weapon::renderWireFrameGroup(Pipeline& p, Renderer* r)
{
	if (hasOwner() == false || (hasOwner() && isBeingUsed()))
	{
		WorldObject::renderWireFrameGroup(p, r);
	}
}


bool Weapon::ignorePhysics()
{
	return (hasOwner() || m_slotEnum != PROJECTILE);
}



void Weapon::setBeingUsed(bool flag)
{
	isBeingUsedFlag = flag;
}

bool Weapon::isBeingUsed()
{
	return isBeingUsedFlag;
}

bool Weapon::ignorePhysicsWith(WorldObject* obj)
{
	ObjectId id = obj->objectId;

	if (id != ownerId)
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


void Weapon::serialize_New(RakNet::BitStream& bs)
{
//	bs.Write(objectId.getId());

	bs.Write(objectId.getTag());
	bs.Write(objectId.getIndex());


	utl::write(bs, m_name);
	bs.WriteVector(m_position.x, m_position.y, m_position.z);
	bs.Write(m_nameEnum);
	bs.Write(m_slotEnum);

//	bs.Write(ownerId.getId());		// need to put these two in bit flags	
	ownerId.serialize(bs);

	bool b = isBeingUsed();
	bs.Write(b);

#if DEBUG 
	{
		utl::debug(">>>>>>	weapon Serialize ");
		utl::debug("Serialize weapon tag ", objectId.getTag());
		utl::debug("Serialize weapon Index ", objectId.getIndex());
		int a = 1;
	}
#endif // DEBUG 
}


void Weapon::deserialize_New(RakNet::BitStream& bs)
{
	uint16_t tag = 0;
	uint16_t index = 0;
	
	bs.Read(tag);
	bs.Read(index);

	objectId.setTag(tag);
	objectId.setIndex(index);

	utl::read(bs, m_name);
	bs.ReadVector(m_position.x, m_position.y, m_position.z);

	bs.Read(m_nameEnum);
	// important to run this before the operations below
	init(global.modelMgr->getWeaponData((WeaponNameEnum)m_nameEnum));	

	bs.Read(m_slotEnum);

//	bs.Read(id);
//	ownerId.setId(id);

	ownerId.deserialize(bs);

	bool b = isBeingUsed();
	bs.Read(b);
	setBeingUsed(b);
	
#if DEBUG 
	{
		utl::clDebug("<<<<<<	weapon deserialize ");
		utl::clDebug("deserialize weapon tag ", objectId.getTag());
		utl::clDebug("deserialize weapon Index ", objectId.getIndex());
		int a = 1;
	}
#endif // DEBUG 



	/*
	if (objectId.s.index == 27)
	{
		utl::debug("ownerId", ownerId);
		int a = 1;
	}
	*/

//	prevState = GetState();
}

/*
void Weapon::serialize_Delta(RakNet::BitStream& bs)
{

}

void Weapon::deserialize_Delta(RakNet::BitStream& bs)
{

}
*/

