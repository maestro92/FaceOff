#include "particle.h"


Particle::Particle()
{
	m_maxlifeTime = 1.0f;
	m_curLifeTime = 1.0f;
	m_growthRate = -0.05f;
	m_alive = true;
}


Particle::~Particle()
{

}

void Particle::update()
{
	m_curLifeTime += m_growthRate;
	if (m_curLifeTime < 0)
		m_alive = false;

}

bool Particle::isAlive()
{
	return m_alive;
}