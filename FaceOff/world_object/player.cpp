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
	
}

Player::~Player()
{

}

void Player::setId(int id)
{
	m_id = id;
}

void Player::update(Pipeline& p)
{
	p.setMatrixMode(VIEW_MATRIX);
	p.loadIdentity();

	m_camera->control(p);
	m_position = m_camera->getEyePoint();
}


void Player::render(Pipeline& p)
{
	m_renderer->setData("u_color", glm::vec3(0.8, 0.0, 0.8));
	renderSingle(p, m_renderer, RENDER_PASS1, m_model);
}
