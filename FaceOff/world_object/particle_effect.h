#ifndef PARTICLE_EFFECT_H_
#define PARTICLE_EFFECT_H_

#include "world_object.h"

using namespace std;

#include <vector>


struct PEParticle
{
	float type;
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetimeMillis;
};


class ParticleEffect : public WorldObject
{
	public:

		ParticleEffect();
		~ParticleEffect();

		void init();
		void setTexture(string filename);
		void update(Pipeline& p, Renderer* r);
		void render(Pipeline& p, Renderer* r);
		void renderParticles();

		bool initRandomTexture(int size);

		GLuint m_textureId;
		GLuint m_randomTextureId;

		int m_time;
		bool m_isFirst;
		GLuint m_currVB;
		GLuint m_currTFB;
		GLuint m_particleBuffer[2];
		GLuint m_transformFeedback[2];
		virtual WorldObjectType getObjectType();
};


#endif 