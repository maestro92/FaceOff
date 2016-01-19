#ifndef WORLD_OBJECT_H_
#define	WORLD_OBJECT_H_

#include "utility.h"
#include "renderer.h"
#include "Model.h"

const glm::vec3 NEG_GRAVITY = glm::vec3(0, -9.8, 0);
const glm::vec3 NEG_HALF_GRAVITY = glm::vec3(0, 9.8, 0);

const glm::vec3 POS_GRAVITY = glm::vec3(0, -4.9, 0);
const glm::vec3 POS_HALF_GRAVITY = glm::vec3(0, 4.9, 0);

using namespace std;

class WorldObject
{
    public:
        WorldObject();

        glm::vec3 m_position;
        glm::vec3 m_velocity;
        glm::vec3 m_scale;
        glm::mat4 m_rotation;
		glm::mat4 m_modelRotation;

        inline void setScale(float s);
        inline void setScale(glm::vec3 scale);
        inline void setScale(float x, float y, float z);

        inline void setPosition(glm::vec3 pos);
        inline void setPosition(float x, float y, float z);

        inline void setVelocity(glm::vec3 vel);
        inline void setVelocity(float x, float y, float z);

		inline glm::vec3 getPosition();
		inline glm::vec3 getScale();
		inline glm::mat4 getRotation();

        virtual inline void setRotation(glm::mat4 rot);

        virtual void renderSingle(Pipeline& m_pipeline, Renderer* renderer, Model* model);
        virtual void renderSingle(Pipeline& m_pipeline, Renderer* renderer, int pass, Model* model);

        virtual void renderGroup(Pipeline& m_pipeline, Renderer* renderer, Model* model);
        virtual void renderGroup(Pipeline& m_pipeline, Renderer* renderer, int pass, Model* model);

};

inline void WorldObject::setScale(float s)
{
    m_scale = glm::vec3(s, s, s);
}

inline void WorldObject::setScale(glm::vec3 scale)
{
    m_scale = scale;
}

inline void WorldObject::setScale(float x, float y, float z)
{
    m_scale = glm::vec3(x, y, z);
}

inline void WorldObject::setPosition(glm::vec3 pos)
{
    m_position = pos;
}

inline void WorldObject::setPosition(float x, float y, float z)
{
    m_position = glm::vec3(x, y, z);
}

inline void WorldObject::setVelocity(glm::vec3 vel)
{
    m_velocity = vel;
}

inline void WorldObject::setVelocity(float x, float y, float z)
{
    m_velocity = glm::vec3(x, y, z);
}

inline void WorldObject::setRotation(glm::mat4 rot)
{
    float temp[16] = {rot[0][0], rot[0][1], rot[0][2], 0.0,
                      rot[1][0], rot[1][1], rot[1][2], 0.0,
                      rot[2][0], rot[2][1], rot[2][2], 0.0,
                      0.0,       0.0,       0.0,       1.0};
    m_rotation = glm::make_mat4(temp);
}


inline glm::vec3 WorldObject::getPosition()
{
	return m_position;
}

inline glm::vec3 WorldObject::getScale()
{
	return m_scale;
}

inline glm::mat4 WorldObject::getRotation()
{
	return m_rotation;
}

#endif
