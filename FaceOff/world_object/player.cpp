#include "player.h"


Player::Player() : Player(0)
{

}

Player::Player(int id)
{
	setId(id);

	m_camera = new FirstPersonCamera();
	m_model = new ImportedModel("./Assets/models/sphere10.obj");
	m_renderer = &RendererManager::r_fullColor;
	
	setPosition(0.0, 5.0, 0.0);

	m_maxHP = 100;
	m_curHP = 100;

	m_weapons.resize(NUM_WEAPON_TYPE);

	// this value is obtained visually
	m_weaponPositionOffsets.resize(NUM_WEAPON_TYPE, glm::vec3(0.35, -0.64, 1.26));
	m_bulletStartPositionOffsets.resize(NUM_WEAPON_TYPE);
	m_bulletStartPositionOffsetScale.resize(NUM_WEAPON_TYPE, glm::vec3(-0.052, 0.474, 0.952));

	addWeapon(new AssultRifle("Ak-47"));
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
	m_position = m_camera->getEyePoint();

	updateWeaponTransform();
	updateBulletTransform();

	// adjustWeaponAndBulletPosition()
}


void Player::updateWeaponTransform()
{
	glm::mat4 viewMat = m_camera->getViewMatrix();

	glm::vec3 xAxis = m_camera->m_xAxis;
	glm::vec3 yAxis = m_camera->m_yAxis;
	glm::vec3 zAxis = m_camera->m_zAxis;
	zAxis = -zAxis;

	xAxis = xAxis * m_weaponPositionOffsets[m_curWeaponIndex].x;
	yAxis = yAxis * m_weaponPositionOffsets[m_curWeaponIndex].y;
	zAxis = zAxis * m_weaponPositionOffsets[m_curWeaponIndex].z;

	glm::vec3 pos = m_position + xAxis + yAxis + zAxis;
	m_weapons[m_curWeaponIndex]->setPosition(pos);
	m_weapons[m_curWeaponIndex]->setScale(0.005);
}

void Player::updateBulletTransform()
{
	glm::mat4 camRot = glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	camRot *= glm::rotate(m_camera->m_pitch, 1.0f, 0.0f, 0.0f);
	glm::mat4 rot = glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);
	rot = camRot * rot;

	m_weapons[m_curWeaponIndex]->setRotation(rot);

	// setting the bullet position and orientation
	glm::vec3 xAxis = glm::vec3(camRot[0][0], camRot[0][1], camRot[0][2]);
	glm::vec3 yAxis = glm::vec3(camRot[1][0], camRot[1][1], camRot[1][2]);
	glm::vec3 zAxis = -glm::vec3(camRot[2][0], camRot[2][1], camRot[2][2]);

	xAxis = xAxis * m_bulletStartPositionOffsetScale[m_curWeaponIndex].x;
	yAxis = yAxis * m_bulletStartPositionOffsetScale[m_curWeaponIndex].y;
	zAxis = zAxis * m_bulletStartPositionOffsetScale[m_curWeaponIndex].z;

	glm::vec3 pos = m_weapons[m_curWeaponIndex]->m_position + xAxis + yAxis + zAxis;

	m_bulletStartPositionOffsets[m_curWeaponIndex] = pos;
}



void Player::update(Pipeline& p, Terrain* terrain)
{
	p.setMatrixMode(VIEW_MATRIX);
	p.loadIdentity();

	m_camera->control(p, terrain);
	m_position = m_camera->getEyePoint();

	updateWeaponTransform();
	updateBulletTransform();
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



void Player::update(glm::vec3 wPos, float pitch, float yaw)
{
	m_weapons[m_curWeaponIndex]->setPosition(wPos);
	m_weapons[m_curWeaponIndex]->setScale(0.01);

	glm::mat4 rot = glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	rot *= glm::rotate(m_camera->m_pitch, 1.0f, 0.0f, 0.0f);
	rot *= glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);

	m_weapons[m_curWeaponIndex]->setRotation(rot);
}

void Player::render(Pipeline& p)
{
	m_renderer->setData("u_color", glm::vec3(0.8, 0.8, 0.8));
	renderSingle(p, m_renderer, RENDER_PASS1, m_model);

	m_weapons[m_curWeaponIndex]->render(p, &RendererManager::r_fullTexture);
}


void Player::renderWeapon(Pipeline& p)
{
	m_weapons[m_curWeaponIndex]->render(p, &RendererManager::r_fullTexture);
}



void Player::addWeapon(Weapon* weapon)
{
	int type = (int)weapon->getType();

	m_curWeaponIndex = type;

	m_weapons[(int)weapon->getType()] = weapon;
}



void Player::fireWeapon(list<Particle>& bullets)
{
	Particle bullet;

	glm::mat4 rot = glm::mat4(1.0);

	
	rot *= glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	rot *= glm::rotate(m_camera->m_pitch, 1.0f, 0.0f, 0.0f);
	rot *= glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	
	bullet.setRotation(rot);

	bullet.setPosition(m_bulletStartPositionOffsets[m_curWeaponIndex]);
	bullet.setScale(0.05, 2.0, 0.05);

	bullets.push_back(bullet);
}

Weapon* Player::getCurWeapon()
{
	return m_weapons[m_curWeaponIndex];
} 


float Player::getCameraPitch()
{
	return m_camera->m_pitch;
}

float Player::getCameraYaw()
{
	return m_camera->m_yaw;
}


void Player::adjustWeaponAndBulletPosition()
{
	Uint8* state = SDL_GetKeyState(NULL);
	float step = 0.001;

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

	/*
	if (state[SDLK_i])
	{
	m_weaponPositionOffsets[m_curWeaponIndex].z += step;
	}

	else if (state[SDLK_k])
	{
	m_weaponPositionOffsets[m_curWeaponIndex].z -= step;
	}


	if (state[SDLK_j])
	{
	m_weaponPositionOffsets[m_curWeaponIndex].x -= step;
	}
	else if (state[SDLK_l])
	{
	m_weaponPositionOffsets[m_curWeaponIndex].x += step;
	}


	if (state[SDLK_n])
	{
	m_weaponPositionOffsets[m_curWeaponIndex].y += step;
	}
	else if (state[SDLK_m])
	{
	m_weaponPositionOffsets[m_curWeaponIndex].y -= step;
	}
	*/
}
