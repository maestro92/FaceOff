#ifndef PARTICLE_EFFECT_H_
#define PARTICLE_EFFECT_H_

#include "world_object.h"

using namespace std;

#include <vector>

/*
struct PEParticle
{
	float type;
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetimeMillis;
};
*/

enum ParticleEffectType
{
	FIRE_WORK = 0,
	SMOKE,
	INCENDIARY
};

class ParticleEffect : public WorldObject
{
	public:

		ParticleEffect();
		~ParticleEffect();

		virtual void init() = 0;
		void setTexture(string filename);
		virtual void update(Pipeline& p, Renderer* r) = 0;
		virtual void render(Pipeline& p, Renderer* r) = 0;

		GLuint m_textureId;

		bool m_isFirst;
		int m_time;

		GLuint m_currVB;
		GLuint m_currTFB;
		GLuint m_particleBuffer[2];
		GLuint m_transformFeedback[2];
		virtual ParticleEffectType getParticleEffectType() = 0;

};


#endif 