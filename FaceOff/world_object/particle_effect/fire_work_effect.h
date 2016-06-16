#ifndef FIRE_WORK_EFFECT_H_
#define FIRE_WORK_EFFECT_H_

#include "particle_effect.h"

using namespace std;

#include <vector>


struct FireWorkParticle
{
	float type;
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetimeMillis;
};


class FireWorkEffect : public ParticleEffect
{
	public:
		
		FireWorkEffect();
		~FireWorkEffect();
		
		virtual void init();
		virtual void update(Pipeline& p, Renderer* r);
		virtual void render(Pipeline& p, Renderer* r);
		void renderParticles();

		bool initRandomTexture(int size);
		
		GLuint m_randomTextureId;

		virtual ParticleEffectType getParticleEffectType();		
};


#endif 