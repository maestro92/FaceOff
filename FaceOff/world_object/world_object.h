#ifndef WORLD_OBJECT_H_
#define	WORLD_OBJECT_H_
#pragma warning(disable: 4996)
#include "utility.h"
#include "renderer.h"
#include "model.h"
#include <string>
#include "cube_wireframe_model.h"
#include "bounding_volume.h"

/*
struct KDTreeNode;
#include "collision_detection/kd_tree_node.h"
*/
struct KDTreeNode;
// #include "collision_detection/kd_tree_node.h"

#include <vector>

const glm::vec3 NEG_GRAVITY = glm::vec3(0, -9.8, 0);
const glm::vec3 NEG_HALF_GRAVITY = glm::vec3(0, -4.9, 0);

const glm::vec3 POS_GRAVITY = glm::vec3(0, 9.8, 0);
const glm::vec3 POS_HALF_GRAVITY = glm::vec3(0, 4.9, 0);

using namespace std;


enum DynamicType
{
	STATIC = 0,
	DYNAMIC
};


enum WorldObjectType
{
	SCENE_OBJECT = 0,
	PLAYER,
	WEAPON
};

class WorldObject
{
    public:
        WorldObject();
		string m_name;
		int m_instanceId;

		bool isTested;
		bool isCollided;
		bool isHit;
		
		bool alreadyTested;
		bool alreadyFireTested;

		int isHitCounter;

		DynamicType m_dynamicType;

		virtual WorldObjectType getObjectType();
		DynamicType getDynamicType();
		BVEnum getBoundingVolumeType();

		BVEnum m_BVType;

		AABB m_aabb;
		

		glm::vec3 m_position;
        glm::vec3 m_velocity;
        glm::vec3 m_scale;
        glm::mat4 m_rotation;
		glm::mat4 m_modelRotation;
		glm::mat4 m_modelMatrix;

		float m_longestExtentFromCenter;

		float m_mass;
		float m_invMass;

		Model* m_model;

        inline void setScale(float s);
        inline void setScale(glm::vec3 scale);
        inline void setScale(float x, float y, float z);

        inline void setPosition(glm::vec3 pos);
        inline void setPosition(float x, float y, float z);

        inline void setVelocity(glm::vec3 vel);
        inline void setVelocity(float x, float y, float z);

		inline void setModel(Model* model);

		inline void updateModelMatrix();

		inline glm::vec3 getPosition();
		inline glm::vec3 getVelocity();
		inline glm::vec3 getScale();
		inline glm::mat4 getRotation();

		inline float getMass();
		inline float getInvMass();

        virtual inline void setRotation(glm::mat4 rot);

		virtual void renderSingle(Pipeline& p, Renderer* r);
		virtual void renderSingle(Pipeline& p, Renderer* r, int pass);

		virtual void renderGroup(Pipeline& p, Renderer* r);
		virtual void renderGroup(Pipeline& p, Renderer* r, int pass);

		static Model* DEFAULT_MODEL;

		CubeWireFrameModel* m_wireFrameModel;
		CubeWireFrameModel* m_staticWireFrameModel;

		void renderStaticWireFrameGroup(Pipeline& p, Renderer* r);
		void renderWireFrameGroup(Pipeline& p, Renderer* r);

	
		void updateAABB();
		void updateAABB(glm::vec3& maxP, glm::vec3& minP, glm::vec3 pos, glm::mat4 rotation, glm::vec3 scale);
		void setAABBByPosition(float x, float y, float z);
		void setAABBByPosition(glm::vec3 pos);


		virtual void updateGameInfo();

		void update(); 

		/*
		void removeSelfFromNodes()
		{
			for (int i = 0; i < m_parentNodes.size(); i++)
			{
				KDTreeNode* kNode = m_parentNodes[i];
				(kNode->m_objects2).erase(m_instanceId);
			}
		}
		*/
		vector<KDTreeNode*> m_parentNodes;
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

inline glm::vec3 WorldObject::getVelocity()
{
	return m_velocity;
}

inline glm::vec3 WorldObject::getScale()
{
	return m_scale;
}

inline glm::mat4 WorldObject::getRotation()
{
	return m_rotation;
}

inline void WorldObject::setModel(Model* model)
{
	m_model = model;
	m_wireFrameModel = new CubeWireFrameModel(model->m_aabb);

//	updateAABB();

//	m_staticWireFrameModel = new CubeWireFrameModel(m_aabb);
}


inline void WorldObject::updateModelMatrix()
{
	m_modelMatrix = glm::translate(m_position) * m_rotation * glm::scale(m_scale);
}


inline float WorldObject::getMass()
{
	return m_mass;
}

inline float WorldObject::getInvMass()
{
	return m_invMass;
}


#endif
