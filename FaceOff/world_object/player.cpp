#include "player.h"

#define TESTING_FIRST_POV_WEAPON_OFFSET_FLAG 0
#define TESTING_THIRD_POV_WEAPON_OFFSET_FLAG 1

// glm::vec3 Player::firstPOVWeaponOffset = glm::vec3(0.35, -0.64, 1.26);

// AK47
glm::vec3 Player::firstPOVWeaponOffset = glm::vec3(0.35, -0.64, 1.26);
glm::vec3 Player::thirdPOVWeaponOffset = glm::vec3(3.59, 2.27, 4.43);

// M4
// glm::vec3 Player::firstPOVWeaponOffset = glm::vec3(0.33, -0.35, 1.66);


Player::Player() : Player(0)
{

}

Player::Player(int id)
{
	setId(id);
	objectId.id = id;
//
	m_camera = new FirstPersonCamera();
//	((FirstPersonCamera*)m_camera)->setFreeMode(true);
//	m_camera = new ThirdPersonCamera();

	setPosition(0.0, 5.0, 0.0);
	setScale(5.0);


	m_camera->m_target = m_position;

	m_maxHP = 100;
	m_curHP = 100;

	m_maxArmor = 100;
	m_curArmor = 100;

	m_weapons.resize(NUM_WEAPON_SLOTS);

	m_grenadeGatherMode = false;

	m_healthBarGUI = NULL;

	m_dynamicType = DYNAMIC;


	jumpCoolDown = 30;
	curJumpCoolDown = 0;

	m_isDefaultPlayer = false;
	inMidAir = false;
	testedForNotInMidAir = false;

	jumped = false; 
	weaponCount = 0;
}

Player::~Player()
{

}


int Player::getId()
{
	return m_id;
}


void Player::setId(int id)
{
	m_id = id;
}


void Player::setPosition(glm::vec3 position)
{
	WorldObject::setPosition(position);
	m_camera->setEyePoint(position);
}

void Player::setPosition(float x, float y, float z)
{
	setPosition(glm::vec3(x, y, z));
}

/*
void Player::control()
{
	m_camera->control();
//	m_position += m_camera->getTargetPoint();
//	m_position += m_camera->m_velocity;
	m_rotation = m_camera->m_targetRotation;
}
*/

//	jumpCoolDown = 1000;
//	curJumpCoolDown = 0;


void Player::control()
{
	bool canJumpFlag = canJump();
	glm::vec3 vel(0.0);
	m_midAirHorVel = glm::vec3(0.0);
	m_camera->control(vel, canJumpFlag);

	Input input = m_camera->m_moveState.input;

	if (input.jump)
	{
		inMidAir = true;
		curJumpCoolDown = 0;
		jumped = true;
	}

	if (inMidAir)
	{
		vel.x = 0.5 * vel.x;
		vel.z = 0.5 * vel.z;

		m_midAirHorVel.x = vel.x;
		m_midAirHorVel.z = vel.z;
	}

	if (hasMoved())
	{
		utl::debug("before vel", m_velocity);
		utl::debug("control vel", vel);
	}

	m_velocity += vel;
	m_rotation = m_camera->m_targetRotation;
}


int counter = 0;


// only used for spawning
void Player::processUserCmd(UserCmd cmd)
{
	// glm::vec3 vel;
	bool canJumpFlag = canJump();
	glm::vec3 vel(0.0);
	m_midAirHorVel = glm::vec3(0.0);

	//	m_camera->processInput(move, vel, canJumpFlag);

	m_camera->processUserCmd(cmd, vel, canJumpFlag);

	if (cmd.buttons & JUMP)
	{
		inMidAir = true;
		curJumpCoolDown = 0;
		jumped = true;
	}

	if (inMidAir)
	{
		vel.x = 0.5 * vel.x;
		vel.z = 0.5 * vel.z;

		m_midAirHorVel.x = vel.x;
		m_midAirHorVel.z = vel.z;
	}


	m_velocity += vel;
	setRotation(cmd.angles[PITCH], cmd.angles[YAW]);
}



bool Player::hasMoved()
{
	return m_camera->hasMoved();
}

Move Player::getMoveState()
{
	Move move = (m_camera->getMoveState());
	move.playerId = m_id;
	return move;
}

void Player::setDefaultPlayerFlag(bool b)
{
	m_isDefaultPlayer = b;
}


bool Player::isDefaultPlayer()
{
	return m_isDefaultPlayer;
}

// this obviously doesn't work with a slanted falling incline
bool Player::canJump()
{
//	jumpCoolDown = 1000;
//	curJumpCoolDown = 0;

	return jumped == false && inMidAir == false && curJumpCoolDown >= jumpCoolDown;
}




void Player::updateMidAirVelocity()
{
	if (inMidAir)
	{
	//	m_velocity.x -= m_dv.x;
	//	m_velocity.z -= m_dv.z;
		
		m_velocity.x -= m_midAirHorVel.x;
		m_velocity.z -= m_midAirHorVel.z;

//		utl::debug("cdAngle", cdAngle);
	//	utl::debug("midAirHorVel", m_midAirHorVel);
	}
	/*
	else
	{
		utl::debug("cdAngle", cdAngle);
		utl::debug("not in MidAir");
	}
	*/
}

void Player::updateGameStats()
{
	static bool incrFlag = false;

	if (incrFlag)
	{
		m_curHP += 1;
		m_curArmor += 1;
	}
	else
	{
		m_curHP -= 1;
		m_curArmor -= 1;
	}

	if (m_curHP >= 100)
	{
		incrFlag = false;
	}

	if (m_curHP <= 0)
	{
		incrFlag = true;
	}

	if (m_healthBarGUI != NULL)
	{
		m_healthBarGUI->computeForegroundWidth(m_curHP);
	}
	
	if (m_armorBarGUI != NULL)
	{
		m_armorBarGUI->computeForegroundWidth(m_curArmor);
	}

	if (m_ammoBarGUI != NULL)
	{
		m_ammoBarGUI->computeForegroundWidth(m_curHP);
	}
}


void Player::updateCamera(Pipeline& p)
{
	m_camera->m_target = m_position;
	
	m_camera->updateViewMatrix(p);

	m_rotation = m_camera->m_targetRotation;

	m_xAxis = m_camera->m_targetXAxis;
	m_yAxis = m_camera->m_targetYAxis;
	m_zAxis = -m_camera->m_targetZAxis;

	updateCollisionDetectionGeometry();
	updateWeaponTransform();
}


void Player::updateGameInfo()
{
	WorldObject::updateGameInfo();
	testedForNotInMidAir = false;

	if (jumped == false)
	{
		curJumpCoolDown++;
	}

}


void Player::setRotation(float pitch, float yaw)
{
	glm::mat4 rot = glm::mat4(1.0);
	
	/*
	if (roll != 0)
	{
		rot = rot * glm::rotoate(roll, 0.0f, 0.0f, 1.0f);
	}
	*/

	if (yaw != 0)
	{
		rot = rot * glm::rotate(yaw, 0.0f, 1.0f, 0.0f);
	}

	if (pitch != 0)
	{
		rot = rot * glm::rotate(pitch, 1.0f, 0.0f, 0.0f);
	}

	
	m_xAxis = glm::vec3(rot[0][0], rot[0][1], rot[0][2]);
	m_yAxis = glm::vec3(rot[1][0], rot[1][1], rot[1][2]);
	m_zAxis = glm::vec3(rot[2][0], rot[2][1], rot[2][2]);
	

	/*
	float temp[16] = { rot[0][0], rot[1][0], rot[2][0], 0,
						rot[0][1], rot[1][1], rot[2][1], 0,
						rot[0][2], rot[1][2], rot[2][2], 0,
						0, 0, 0, 1 };
	*/				
	/*
	float temp[16] = { m_xAxis[0], m_yAxis[0], m_zAxis[0], 0,
						m_xAxis[1], m_yAxis[1], m_zAxis[1], 0,
						m_xAxis[2], m_yAxis[2], m_zAxis[2], 0,
						0, 0, 0, 1 };
	*/				
	m_rotation = rot;
}


void Player::updateContactNormalInfo(glm::vec3 normal)
{
	if (testedForNotInMidAir)
	{
		return;
	}


	glm::vec3 horizonVec3(normal.x, 0, normal.z);

	float angle = glm::angle(normal, horizonVec3);

	// utl::debug("angle", angle);
		

	if (angle > 60)
	{
		inMidAir = false;
		testedForNotInMidAir = true;
		cdAngle = angle;


		if (jumped == true)
		{
			jumped = false;
			curJumpCoolDown = 0;
		}
	}
	else
	{
		inMidAir = true;
	}
}



void Player::updateWeaponTransform()
{
	// adjustWeaponAndBulletPosition();
	glm::vec3 xOffset;
	glm::vec3 yOffset;
	glm::vec3 zOffset;

	if (m_curWeapon != NULL)
	{
		if (m_isDefaultPlayer)
		{
			glm::vec3 wOffset = m_curWeapon->m_firstPOVOffset;

			glm::vec3 pos;
			if (m_grenadeGatherMode)
			{
				xOffset = m_xAxis * wOffset.x;
				yOffset = m_yAxis * (wOffset.y + 0.05f);
				zOffset = m_zAxis * wOffset.z;
			}
			else
			{
				xOffset = m_xAxis * wOffset.x;
				yOffset = m_yAxis * wOffset.y;
				zOffset = m_zAxis * wOffset.z;
			}

			pos = m_camera->getEyePoint() + xOffset + yOffset + zOffset;
			m_curWeapon->setPosition(pos);
			// dont need to set scale, that's done at pickUp()
		}
		else
		{

			glm::vec3 wOffset = m_curWeapon->m_thirdPOVOffset;
		//	glm::vec3 wOffset = thirdPOVWeaponOffset;

		//	utl::debug("woffset", thirdPOVWeaponOffset);

			glm::vec3 pos;
			if (m_grenadeGatherMode)
			{
				xOffset = m_xAxis * wOffset.x;
				yOffset = m_yAxis * (wOffset.y + 0.05f);
				zOffset = m_zAxis * wOffset.z;
			}
			else
			{
				xOffset = m_xAxis * wOffset.x;
				yOffset = m_yAxis * wOffset.y;
				zOffset = m_zAxis * wOffset.z;
			}

			pos = m_position + xOffset + yOffset + zOffset;
			m_curWeapon->setPosition(pos);		
			// dont need to set scale, that's done at pickUp()
		}


		m_curWeapon->setRotation(m_rotation);
	
	//	glm::mat4 rot = glm::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	//	m_curWeapon->setRotation(rot);
	//	utl::clDebug("weaponRotation", m_curWeapon->m_rotation);
//		glm::mat4 rot = m_camera->m_targetRotation * 
	}

	/*
	glm::vec3 xAxis = m_camera->m_targetXAxis;
	glm::vec3 yAxis = m_camera->m_targetYAxis;
	glm::vec3 zAxis = -m_camera->m_targetZAxis;

	xAxis = xAxis * m_weaponPositionOffsets[m_curWeaponIndex].x;
	yAxis = yAxis * m_weaponPositionOffsets[m_curWeaponIndex].y;
	zAxis = zAxis * m_weaponPositionOffsets[m_curWeaponIndex].z;

	glm::vec3 pos = m_position + xAxis + yAxis + zAxis;
	m_weapons[m_curWeaponIndex]->setPosition(pos);


	if (m_camera->getCameraType() == FIRST_PERSON_CAMERA)
	{
		m_weapons[m_curWeaponIndex]->setScale(0.005);
	}
	else
	{
		m_weapons[m_curWeaponIndex]->setScale(0.05);
		m_weapons[m_curWeaponIndex]->setRotation(m_camera->m_targetRotation);
	}
	*/
}


WorldObjectType Player::getObjectType()
{
	return PLAYER; 
}


void Player::updateBulletTransform()
{
	/*
	glm::mat4 camRot = glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	camRot *= glm::rotate(m_camera->m_pitch, 1.0f, 0.0f, 0.0f);
	glm::mat4 rot = glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);
	rot = camRot * rot;

	// m_weapons[m_curWeaponIndex]->setRotation(rot);

	// setting the bullet position and orientation
	glm::vec3 xAxis = glm::vec3(camRot[0][0], camRot[0][1], camRot[0][2]);
	glm::vec3 yAxis = glm::vec3(camRot[1][0], camRot[1][1], camRot[1][2]);
	glm::vec3 zAxis = -glm::vec3(camRot[2][0], camRot[2][1], camRot[2][2]);

	xAxis = xAxis * m_bulletStartPositionOffsetScale[m_curWeaponIndex].x;
	yAxis = yAxis * m_bulletStartPositionOffsetScale[m_curWeaponIndex].y;
	zAxis = zAxis * m_bulletStartPositionOffsetScale[m_curWeaponIndex].z;

	glm::vec3 pos = m_weapons[m_curWeaponIndex]->m_position + xAxis + yAxis + zAxis;

	m_bulletStartPositionOffsets[m_curWeaponIndex] = pos;
	*/
}


/*
void Player::update(Pipeline& p, Terrain* terrain)
{
	p.setMatrixMode(VIEW_MATRIX);
	p.loadIdentity();

	m_camera->control(p, terrain);
	m_position = m_camera->getTargetPoint();

	updateWeaponTransform();
	updateBulletTransform();
}
*/

void Player::updateModel()
{

}

/*
void Player::update(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
	zAxis = -zAxis;

	xAxis = utl::scaleGlmVec(xAxis, m_weaponPositionOffsets[m_curWeaponIndex].x);
	yAxis = utl::scaleGlmVec(yAxis, m_weaponPositionOffsets[m_curWeaponIndex].y);
	zAxis = utl::scaleGlmVec(zAxis, m_weaponPositionOffsets[m_curWeaponIndex].z);
	l
	glm::vec3 pos = m_position + xAxis + yAxis + zAxis;
	m_weapons[m_curWeaponIndex]->setPosition(pos);

	m_weapons[m_curWeaponIndex]->setScale(0.01);

	glm::mat4 rot = glm::mat4(1.0);
	rot = rot * glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);
	rot = rot * glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	rot = rot * glm::rotate(m_camera->m_pitch, 0.0f, 0.0f, -1.0f);

	m_weapons[m_curWeaponIndex]->setRotation(rot);
}
*/




bool Player::isUsingLongRangedWeapon()
{
	if (m_curWeapon == NULL)
		return false;

	int curWeaponEnum = m_curWeapon->getWeaponSlot();

	return curWeaponEnum == MAIN || curWeaponEnum == PISTOL;
}






void Player::switchWeapon(WeaponSlotEnum slot)
{
	utl::debug("weapon slot", slot);

	m_grenadeGatherMode = false;

	if (m_curWeapon != NULL)
	{
		m_curWeapon->isBeingUsed = false;
	}

	if (m_weapons[slot] != NULL)
	{	
		m_curWeapon = m_weapons[slot];
		m_curWeapon->isBeingUsed = true;
		utl::debug("m_curWeapon slot", m_curWeapon->getWeaponSlot());
	}
	else
	{
		m_curWeapon = NULL;
		utl::debug("swtiching NULL");
	}

}


bool Player::hasWeaponAtSlot(WeaponSlotEnum slot)
{
	return m_weapons[slot] != NULL;

}

/*
void Player::weaponOnDelete(Weapon* weapon)
{
	if (weapon->ownerId == this->m_id)
	{
		WeaponSlotEnum slot = weapon->getWeaponSlot();
		this->m_weapons[slot] = NULL;
	}
}
*/


void Player::pickUp(Weapon* weapon)
{
	WeaponSlotEnum slot = weapon->getWeaponSlot();

	weapon->ownerId = m_id;
	weapon->isBeingUsed = false;

	weapon->onDelete = [this](Weapon* weapon) 
	{
		if (weapon->ownerId == this->m_id)
		{
			if (m_curWeapon == weapon)
			{
				m_curWeapon = NULL;
			}

			WeaponSlotEnum slot = weapon->getWeaponSlot();
			this->m_weapons[slot] = NULL;
		}
	};

	m_weapons[slot] = weapon;

	weaponCount++;

	if (m_isDefaultPlayer)
	{
		weapon->setScale(weapon->m_firstPOVScale);

	}
	else
	{
		weapon->setScale(weapon->m_thirdPOVScale);
	}

	if (m_curWeapon != NULL)
	{
		if (slot < m_curWeapon->getWeaponSlot())
		{
			switchWeapon(slot);
		}
	}
	else
	{
		weapon->isBeingUsed = true;
		m_curWeapon = weapon;
	}

	updateWeaponTransform();
}

Weapon* Player::throwGrenade()
{
	m_grenadeGatherMode = false;

	Weapon* grenade = m_curWeapon;

	if (m_weapons[MAIN] != NULL)
		switchWeapon(MAIN);

	else if (m_weapons[PISTOL] != NULL)
		switchWeapon(PISTOL);

	else if (m_weapons[MELEE] != NULL)
		switchWeapon(MELEE);

	else if (m_weapons[PROJECTILE] != NULL)
		switchWeapon(PROJECTILE);

	else
		m_curWeapon = NULL;

	/*
	temp->setAABBByPosition(this->getPosition());

	glm::vec3 dir = -this->m_camera->m_zAxis;
	dir = 5.0f * dir;

	temp->setVelocity(dir);
	*/


	grenade->ownerId = NO_OWNER;
	grenade->isBeingUsed = false;
	// set it back to world model scale
	grenade->setScale(grenade->m_modelScale);
	grenade->setRotation(glm::mat4(1.0));


	// glm::vec3 pos = this->getPosition() - 50.0f * this->m_camera->m_zAxis;
	
//	utl::debug("pos is ", this->getPosition());
//	utl::debug("fire pos is", this->getFirePosition());


	// grenade->setAABBByPosition(this->getFirePosition());
	grenade->setPosition(this->getFirePosition());
	grenade->updateCollisionDetectionGeometry();


	glm::vec3 dir = -this->m_camera->m_zAxis;
	dir = 2.0f * dir;
	grenade->setVelocity(dir);

	grenade->startExplodeDelayTimer();
	grenade->setGrenadeThrowerId(objectId.id);

	utl::debug("ThrowGrenade pos is", this->getFirePosition());
	utl::debug("ThrowGrenade dir is", dir);

	// grenade->setVelocity(pos);


	return grenade;
}

Weapon* Player::drop()
{
	if (m_curWeapon == NULL)
		return NULL;

	Weapon* drop = m_curWeapon;
	weaponCount--;

	drop->ownerId = NO_OWNER;
	// set it back to world model scale
	drop->setScale(drop->m_modelScale);
	drop->setRotation(glm::mat4(1.0));

	glm::vec3 pos = this->m_position;
	glm::vec3 dir = this->m_camera->m_zAxis;
	dir.y = 0.0f;

	dir = glm::normalize(dir);

	pos += -10.0f * dir;



//	utl::debug("weapon pos is", pos);

//	drop->setPosition(pos);
//	drop->updateAABB();
	drop->setPosition(pos);
	drop->updateCollisionDetectionGeometry();

	int slot = m_curWeapon->getWeaponSlot();
	m_weapons[slot] = NULL;

	slot += 1;
	if (slot >= NUM_WEAPON_SLOTS)
	{
		m_curWeapon = NULL;
		return drop;
	}

	m_curWeapon = m_weapons[slot];

	return drop;

//	m_curWeapon = weapon;
}

void Player::reloadWeapon()
{


}


Weapon* Player::getWeapon(int index)
{
	return m_weapons[index];
}

void Player::fireWeapon()
{
	if (m_curWeapon->getWeaponSlot() == PROJECTILE)
	{
		m_grenadeGatherMode = true;
	}

	/*
	Particle bullet;

	glm::mat4 rot = glm::mat4(1.0);

	
	rot *= glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	rot *= glm::rotate(m_camera->m_pitch, 1.0f, 0.0f, 0.0f);
	rot *= glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	
	bullet.setRotation(rot);

	bullet.setPosition(m_bulletStartPositionOffsets[m_curWeaponIndex]);
	bullet.setScale(0.05, 2.0, 0.05);

	bullets.push_back(bullet);
	*/
}




bool Player::inGrenadeGatherMode()
{
	return m_grenadeGatherMode;
}

Weapon* Player::getCurWeapon()
{
	return m_curWeapon;
} 


float Player::getCameraPitch()
{
	return m_camera->m_pitch;
}

float Player::getCameraYaw()
{
	return m_camera->m_yaw;
}





void Player::renderGroup(Pipeline& p, Renderer* r)
{
	if (!m_isDefaultPlayer)
	{
		WorldObject::renderGroup(p, r);
	}
}

void Player::renderWireFrameGroup(Pipeline& p, Renderer* r)
{
	if (!m_isDefaultPlayer)
	{
		WorldObject::renderWireFrameGroup(p, r);
	}
}


void Player::adjustWeaponAndBulletPosition()
{
	Uint8* state = SDL_GetKeyState(NULL);
	float step = 0.01;

	/*
	if (state[SDLK_i])
	{
		m_bulletStartPositionOffsetScale[m_curWeaponIndex].z += step;
	}

	else if (state[SDLK_k])
	{
		m_bulletStartPositionOffsetScale[m_curWeaponIndex].z -= step;
	}


	if (state[SDLK_j])
	{
		m_bulletStartPositionOffsetScale[m_curWeaponIndex].x -= step;
	}
	else if (state[SDLK_l])
	{
		m_bulletStartPositionOffsetScale[m_curWeaponIndex].x += step;
	}


	if (state[SDLK_n])
	{
		m_bulletStartPositionOffsetScale[m_curWeaponIndex].y += step;
	}
	else if (state[SDLK_m])
	{
		m_bulletStartPositionOf
		fsetScale[m_curWeaponIndex].y -= step;
	}
	*/
	


	glm::vec3 offset;
#if TESTING_FIRST_POV_WEAPON_OFFSET_FLAG == 1
	offset = firstPOVWeaponOffset;
#else if TESTING_THIRD_POV_WEAPON_OFFSET_FLAG == 1
	offset = thirdPOVWeaponOffset;
#endif 

//	if (m_isDefaultPlayer)
	{
		if (state[SDLK_i])
		{
			offset.z += step;
		}
		else if (state[SDLK_k])
		{
			offset.z -= step;
		}


		if (state[SDLK_j])
		{
			offset.x -= step;
		}
		else if (state[SDLK_l])
		{
			offset.x += step;
		}


		if (state[SDLK_n])
		{
			offset.y += step;
		}
		else if (state[SDLK_m])
		{
			offset.y -= step;
		}
//		utl::debug("x y z", offset);

#if TESTING_FIRST_POV_WEAPON_OFFSET_FLAG == 1
		firstPOVWeaponOffset = offset;
#else if TESTING_THIRD_POV_WEAPON_OFFSET_FLAG == 1
		thirdPOVWeaponOffset = offset;
#endif 
	//	if (m_id == 1)
	//		utl::debug("thirdPOVWeaponOffset in adjust", thirdPOVWeaponOffset);
	}
	
}


glm::vec3 Player::getFirePosition()
{
	return m_camera->getFirePosition();
}

bool Player::ignorePhysicsWith(WorldObject* obj)
{
	if (obj->getWeaponSlot() == PROJECTILE)
	{
		return (obj->ignorePhysicsWith(this));
	}
	return false;
}




// a slight hack, will comeback
int Player::getInstanceId()
{
	return objectId.id + NUM_MAX_OBJECTS;

}



void Player::serialize(RakNet::BitStream& bs)
{

	bs.Write(m_id);
	bs.WriteVector(m_position.x, m_position.y, m_position.z);
	bs.Write(getCameraPitch());
	bs.Write(getCameraYaw());

	// cout << "			m_modelEnum " << m_modelEnum << endl;


	bs.Write(m_modelEnum);
	bs.Write(getGeometryType());
	bs.Write(getMass());
	bs.Write(getMaterialEnergyRestitution());
	bs.Write(getMaterialSurfaceFrictionToBitStream());

	bs.Write(weaponCount);
	for (int i = 0; i < m_weapons.size(); i++)
	{
		Weapon* weapon = m_weapons[i];
		if (weapon != NULL)
		{
			weapon->serialize(bs);
		}
	}


	// if (m_position.x != 0 || m_position.z != 0)
#if DEBUG
	{
		utl::debug(">>>>>>	Player Serialize ");
		utl::debug("m_id id ", m_id);
		utl::debug("position is", m_position);
	}
#endif
}




// Note we still have to add this Player to the world if we haven't done that yet
void Player::deserialize(RakNet::BitStream& bs, ModelManager* mm)
{
//	utl::clDebug("<<<<<<	Player deserialize ");
	// the message id is already ignored
	bs.Read(m_id);
	objectId.id = m_id;

	bs.ReadVector(m_position.x, m_position.y, m_position.z);
	
	// cout << "deserialize playerId " << m_id << endl;


	float pitch, yaw;
	bs.Read(pitch);					bs.Read(yaw);
	m_camera->m_pitch = pitch;		m_camera->m_yaw = yaw;
	setRotation(pitch, yaw);

	bs.Read(m_modelEnum);		
	setModel(mm->get(m_modelEnum));


	bs.Read(m_geometryType);	 
	setCollisionDetectionGeometry(m_geometryType);

	float mass = 0;
	bs.Read(mass);	setMass(mass);

	float restitution = 0;
	bs.Read(restitution);		setMaterialEnergyRestitution(restitution);

	float friction = 0;
	bs.Read(friction);			setMaterialSurfaceFriction(friction);


	int weaponCount = 0;
	bs.Read(weaponCount);


	for (int i = 0; i < weaponCount; i++)
	{
		Weapon* weapon = new Weapon();
		weapon->deserialize(bs, mm);

		pickUp(weapon);
	}

	prevState = GetState();

#if DEBUG
	{
		utl::clDebug("<<<<<<	Player deserialize ");
		utl::clDebug("deserialize playerId ", m_id);
		utl::clDebug("position is", m_position);
	}
#endif
}



vector<Weapon*>& Player::getWeapons()
{
	return m_weapons;
}


void Player::svDebug()
{
	if (m_position.x != 0 || m_position.z != 0)
	{
		utl::debug("playerId", objectId.id);
		utl::debug("	player position", m_position);
	}
}


void Player::clDebug()
{
	if (m_position.x != 0 || m_position.z != 0)
	{
		utl::clDebug("playerId", objectId.id);
		utl::clDebug("	player position", m_position);
	}
}




/*
void Player::update(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis)
{
	zAxis = -zAxis;

	xAxis = utl::scaleGlmVec(xAxis, m_weaponPositionOffsets[m_curWeaponIndex].x);
	yAxis = utl::scaleGlmVec(yAxis, m_weaponPositionOffsets[m_curWeaponIndex].y);
	zAxis = utl::scaleGlmVec(zAxis, m_weaponPositionOffsets[m_curWeaponIndex].z);
	
	glm::vec3 pos = m_position + xAxis + yAxis + zAxis;
	m_weapons[m_curWeaponIndex]->setPosition(pos);

	m_weapons[m_curWeaponIndex]->setScale(0.01);

	glm::mat4 rot = glm::mat4(1.0);
	rot = rot * glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);
	rot = rot * glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	rot = rot * glm::rotate(m_camera->m_pitch, 0.0f, 0.0f, -1.0f);

	m_weapons[m_curWeaponIndex]->setRotation(rot);
}
*/
