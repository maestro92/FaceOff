#include "player.h"


// glm::vec3 Player::firstPOVWeaponOffset = glm::vec3(0.35, -0.64, 1.26);

// AK47
glm::vec3 Player::firstPOVWeaponOffset = glm::vec3(0.35, -0.64, 1.26);
glm::vec3 Player::thirdPOVWeaponOffset = glm::vec3(2.11, -2.49, 5.2);

// M4
// glm::vec3 Player::firstPOVWeaponOffset = glm::vec3(0.33, -0.35, 1.66);


Player::Player() : Player(0)
{

}

Player::Player(int id)
{
	setId(id);
//
	m_camera = new FirstPersonCamera();
//	m_camera = new ThirdPersonCamera();


	vector<string> textures;  textures.push_back("Assets/Images/chess.png");
	m_model = new ImportedModel("./Assets/models/sphere/sphere.obj", textures);
	m_model->setMeshRandTextureIdx();


	// m_renderer = &RendererManager::r_fullColor;
	setModel(m_model);

	setPosition(0.0, 5.0, 0.0);
	setScale(5.0);


	m_camera->m_target = m_position;

	m_boundingSphere.center = m_position;
	m_boundingSphere.radius = m_scale.x;

	m_maxHP = 100;
	m_curHP = 100;

	m_maxArmor = 100;
	m_curArmor = 100;

	m_weapons.resize(NUM_WEAPON_TYPE);

	/*
	// this value is obtained visually
	if (m_camera->getCameraType() == FIRST_PERSON_CAMERA)
		m_weaponPositionOffsets.resize(NUM_WEAPON_TYPE, glm::vec3(0.35, -0.64, 1.26));
	else
		m_weaponPositionOffsets.resize(NUM_WEAPON_TYPE, glm::vec3(2.11, -2.49, 5.2));
	
	m_bulletStartPositionOffsets.resize(NUM_WEAPON_TYPE);
	m_bulletStartPositionOffsetScale.resize(NUM_WEAPON_TYPE, glm::vec3(-0.052, 0.474, 0.952));
	*/

//	addWeapon(new AssultRifle("Ak-47"));

	m_healthBarGUI = NULL;
}

Player::~Player()
{

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


void Player::update(Pipeline& p)
{
	p.setMatrixMode(VIEW_MATRIX);
	p.loadIdentity();

	m_camera->control(p);

	// first person camera: eye = target
	// third person camera: eye != target

	m_position = m_camera->getTargetPoint();

	updateAABB();

	updateWeaponTransform();

	// updateBulletTransform();
//	adjustWeaponAndBulletPosition();
}


void Player::control()
{
	m_camera->controlCD();
	m_position = m_camera->getTargetPoint();
}



void Player::updateGameStatus()
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
	p.setMatrixMode(VIEW_MATRIX);
	p.loadIdentity();

	m_camera->m_target = m_position;
	m_boundingSphere.center = m_position;
	// ((ThirdPersonCamera*)m_camera)->updateViewMatrix(p);
	
	// adjustWeaponAndBulletPosition();

	m_camera->updateViewMatrix(p);
	updateAABB();
	updateWeaponTransform();
}

void Player::updateWeaponTransform()
{

	if (m_curWeapon != NULL)
	{
		glm::vec3 xAxis = m_camera->m_targetXAxis;
		glm::vec3 yAxis = m_camera->m_targetYAxis;
		glm::vec3 zAxis = -m_camera->m_targetZAxis;

		glm::vec3 wOffset = m_curWeapon->m_firstPOVOffset;

		xAxis = xAxis * wOffset.x; // Player::firstPOVWeaponOffset.x;
		yAxis = yAxis * wOffset.y;
		zAxis = zAxis * wOffset.z;

		glm::vec3 pos = m_camera->getEyePoint() + xAxis + yAxis + zAxis;
//		glm::vec3 pos = m_camera->getTargetPoint() + xAxis + yAxis + zAxis;
		m_curWeapon->setPosition(pos);
//		m_curWeapon->setScale(0.005);
		m_curWeapon->setRotation(m_camera->m_targetRotation);
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


WorldObjectType Player::getWorldObject()
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



void Player::update(Pipeline& p, Terrain* terrain)
{
	p.setMatrixMode(VIEW_MATRIX);
	p.loadIdentity();

	m_camera->control(p, terrain);
	m_position = m_camera->getTargetPoint();

	updateWeaponTransform();
	updateBulletTransform();
}


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



void Player::update(glm::vec3 wPos, float pitch, float yaw)
{
	/*
	m_weapons[m_curWeaponIndex]->setPosition(wPos);
	m_weapons[m_curWeaponIndex]->setScale(0.01);

	glm::mat4 rot = glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	rot *= glm::rotate(m_camera->m_pitch, 1.0f, 0.0f, 0.0f);
	rot *= glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);

	m_weapons[m_curWeaponIndex]->setRotation(rot);
*/
}






/*

void Player::render(Pipeline& p, RendererManager& rm, ModelManager& mm)
{
	if (m_camera->getCameraType() == FIRST_PERSON_CAMERA)
	{

	}
	else
	{


	}
}
*/

/*
void Player::renderModel(Pipeline& p, Renderer* r)
{
//	renderGroup(p, r, m_model);
}
*/


/*
void Player::renderWeapon(Pipeline& p)
{
//	m_weapons[m_curWeaponIndex]->render(p, &RendererManager::r_fullTexture);
}
*/


void Player::pickUpWeapon(Weapon* weapon)
{
	/*
	if (m_curWeapon != NULL && m_curWeapon->getType() == weapon->getType())
	{
		releaseWeapon(m_curWeapon);
	}
	*/
	weapon->setScale(weapon->m_modelScale / 10.0f);
	m_curWeapon = weapon;
}



void Player::releaseWeapon()
{
	m_curWeapon->setScale(0.005);



//	m_curWeapon = weapon;
}





void Player::fireWeapon(list<Particle>& bullets)
{
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


Weapon* Player::getCurWeapon()
{
//	return m_weapons[m_curWeaponIndex];
	return NULL;
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
	if (m_camera->getCameraType() == THIRD_PERSON_CAMERA)
	{
		WorldObject::renderGroup(p, r);
	}

	// render teh weapon
	if (m_curWeapon != NULL)
	{
		m_curWeapon->renderGroup(p, r);
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
		m_bulletStartPositionOffsetScale[m_curWeaponIndex].y -= step;
	}
	*/
	
	if (state[SDLK_t])
	{
		firstPOVWeaponOffset.z += step;
	}

	else if (state[SDLK_g])
	{
		firstPOVWeaponOffset.z -= step;
	}


	if (state[SDLK_f])
		firstPOVWeaponOffset.x -= step;
	
	else if (state[SDLK_h])
		firstPOVWeaponOffset.x += step;
	


	if (state[SDLK_r])
		firstPOVWeaponOffset.y += step;
	else if (state[SDLK_y])
		firstPOVWeaponOffset.y -= step;
	
	utl::debug("x y z", firstPOVWeaponOffset);

	
}


glm::vec3 Player::getFirePosition()
{
	return m_camera->getFirePosition();
}




/*

void Player::addWeapon(Weapon* weapon)
{


}

*/