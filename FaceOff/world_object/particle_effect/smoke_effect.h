#ifndef SMOKE_EFFECT_H_
#define SMOKE_EFFECT_H_

#include "particle_effect.h"

struct SmokeParticle
{
	float type;
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetimeMillis;
};


class SmokeEffect : public ParticleEffect
{
	SmokeEffect();
	~SmokeEffect();

	virtual void init();
	virtual void update(Pipeline& p, Renderer* r);
	virtual void render(Pipeline& p, Renderer* r);
	void renderParticles();

	bool initRandomTexture(int size);

	GLuint m_randomTextureId;

	virtual ParticleEffectType getParticleEffectType();

};



#endif