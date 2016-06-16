#include "particle_effect.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

ParticleEffect::ParticleEffect()
{
	m_currVB = 0;
	m_currTFB = 1;
	m_isFirst = true;
	m_textureId = NULL;
	m_time = 0;

	m_boundingVolume = new Sphere();
}

ParticleEffect::~ParticleEffect()
{
	if (m_transformFeedback[0] != 0)
	{
		glDeleteTransformFeedbacks(2, m_transformFeedback);
	}

	if (m_particleBuffer[0] != 0)
	{
		glDeleteBuffers(2, m_particleBuffer);
	}
}

/*
void ParticleEffect::init()
{

}
*/
void ParticleEffect::setTexture(string filename)
{
	m_textureId = utl::loadTexture(filename);
}


/*
void ParticleEffect::update(Pipeline& p, Renderer* r)
{

}

void ParticleEffect::render(Pipeline& p, Renderer* r)
{

}
*/

WorldObjectType ParticleEffect::getObjectType()
{
	return PARTICLE_EFFECT;
}