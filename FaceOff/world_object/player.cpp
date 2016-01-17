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
	



	m_weapons.resize(NUM_WEAPON_TYPE);

	// this value is obtained visually
	m_weaponPositionOffsets.resize(NUM_WEAPON_TYPE, glm::vec3(0.35, -0.64, 1.26));

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

	glm::mat4 viewMat = m_camera->getViewMatrix();

	glm::vec3 xAxis = m_camera->m_xAxis;
	glm::vec3 yAxis = m_camera->m_yAxis;
	glm::vec3 zAxis = m_camera->m_zAxis;
	zAxis = -zAxis;

//	glm::vec3 viewDir = m_camera->getViewDirection();


	xAxis = Utility::scaleGlmVec(xAxis, m_weaponPositionOffsets[m_curWeaponIndex].x);
	yAxis = Utility::scaleGlmVec(yAxis, m_weaponPositionOffsets[m_curWeaponIndex].y);
	zAxis = Utility::scaleGlmVec(zAxis, m_weaponPositionOffsets[m_curWeaponIndex].z);

	/*
	Utility::debug("m_pitch", m_camera->m_yaw);
	Utility::debug("m_weaponPositionOffsets[m_curWeaponIndex]", m_weaponPositionOffsets[m_curWeaponIndex]);
	*/

	glm::vec3 pos = m_position + xAxis + yAxis + zAxis;
	m_weapons[m_curWeaponIndex]->setPosition(pos);

	/*
	m_weapons[m_curWeaponIndex]->setPosition(m_position.x + zAxis.x + xAxis.x,
											 m_position.y + zAxis.y + xAxis.y,
											 m_position.z + zAxis.z + xAxis.z);
	*/
											 /*
	m_weapons[m_curWeaponIndex]->setPosition(m_position.x + zAxis.x + xAxis.x * m_weaponPositionOffsets[m_curWeaponIndex].x,
											 m_position.y + zAxis.y + xAxis.y * m_weaponPositionOffsets[m_curWeaponIndex].y,
											 m_position.z + zAxis.z + xAxis.z * m_weaponPositionOffsets[m_curWeaponIndex].z);
	*/

	glm::mat4 rot = glm::mat4(1.0);
	rot = rot * glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);
	rot = rot * glm::rotate(m_camera->m_yaw, 0.0f, 1.0f, 0.0f);
	rot = rot * glm::rotate(m_camera->m_pitch, 0.0f, 0.0f, -1.0f);

	m_weapons[m_curWeaponIndex]->setRotation(rot);


	Uint8* state = SDL_GetKeyState(NULL);
	float step = 0.005;

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


	if (state[SDLK_u])
	{

	}
	else if (state[SDLK_o])
	{

	}
}


void Player::render(Pipeline& p)
{


	m_renderer->setData("u_color", glm::vec3(0.8, 0.0, 0.8));
	renderSingle(p, m_renderer, RENDER_PASS1, m_model);


	m_weapons[m_curWeaponIndex]->render(p, &RendererManager::r_fullTexture);

}


void Player::renderWeapon(Pipeline& p)
{
	m_weapons[m_curWeaponIndex]->render(p, &RendererManager::r_fullTexture);
}



void Player::addWeapon(Weapon* weapon)
{
	weapon->setScale(0.005);
	int type = (int)weapon->getType();

	m_curWeaponIndex = type;

	m_weapons[(int)weapon->getType()] = weapon;


}
