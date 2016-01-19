#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "world_object.h"
using namespace std;

class Particle : public WorldObject
{
	public:
		Particle();
		~Particle();

		inline void setMaxLifeTime(float lifetime);
		inline float getMaxLifeTime();

		inline void setCurLifeTime(float lifetime);
		inline float getCurLifeTime();

		void update();
		bool isAlive();

	private:
		float m_maxlifeTime;
		float m_curLifeTime;
		float m_growthRate;
		float m_alive;
};


inline void Particle::setMaxLifeTime(float lifetime)
{
	m_maxlifeTime = lifetime;
}

inline float Particle::getMaxLifeTime()
{
	return m_maxlifeTime;
}

inline void Particle::setCurLifeTime(float lifetime)
{
	m_curLifeTime = lifetime;
}

inline float Particle::getCurLifeTime()
{
	return m_curLifeTime;
}

#endif