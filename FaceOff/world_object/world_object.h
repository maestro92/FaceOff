#ifndef WORLD_OBJECT_H_
#define	WORLD_OBJECT_H_
#pragma warning(disable: 4996)
#include "utility.h"
#include "weapon_enum.h"
#include "renderer.h"
#include "model_enum.h"
#include "model.h"
#include <string>
#include "cube_wireframe_model.h"
#include "collision_detection_geometry.h"
class ModelManager;

#include "shared.h"
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


enum EntityType
{
	SCENE_OBJECT = 0,
	PLAYER,
	WEAPON,
	PARTICLE_EFFECT
};

class WorldObject
{
    public:
        WorldObject();

		static WorldObject* getOne();

		virtual ~WorldObject();
		string m_name;
		// uint32_t m_instanceId;

		bool isTested;
		bool isCollided;
		bool isHit;
		
		bool alreadyTested;
		// bool alreadyFireTested;

		int isHitCounter;



		glm::vec3 m_position;
		glm::vec3 m_velocity;
		glm::vec3 m_scale;
		glm::mat4 m_rotation;
		glm::mat4 m_modelMatrix;





		ObjectId objectId;

		bool isPlayer();
		bool isWeapon();
		bool hasOwner();
		ObjectId ownerId;

		bool active;
		// CollisionDetectionGeometry* m_geometry;
		
		// collision geometry, we use component based
		CDEnum m_geometryType;
		AABB* m_aabb;
		Sphere* m_sphere;
		
		Model* m_model;



		EntityType getEntityType();
		DynamicType getDynamicType();
		CDEnum getGeometryType();


		inline void setName(string s);
		inline string getName();

		int getCollisionFlagIndex();
		void resetCollisionFlags();
		void registerCollsionFlag(int i);


        inline void setScale(float s);
        inline void setScale(glm::vec3 scale);
        inline void setScale(float x, float y, float z);

        inline void setPosition(glm::vec3 pos);
        inline void setPosition(float x, float y, float z);

        inline void setVelocity(glm::vec3 vel);
        inline void setVelocity(float x, float y, float z);

		inline void setModelEnum(int modelEnum);
		inline void setModel(Model* model);
		inline int getModelEnum();

		inline void updateModelMatrix();

		inline glm::vec3 getPosition();
		inline glm::vec3 getVelocity();
		inline glm::vec3 getScale();
		inline glm::mat4 getRotation();

		inline void setMass(float mass);
		inline float getMass();
		inline float getInvMass();

		glm::vec3 m_xAxis;
		glm::vec3 m_yAxis;
		glm::vec3 m_zAxis;
        inline void setRotation(glm::mat4 rot);
		inline void setRotation(float pitch, float yaw);
		inline void updateRotation();

		inline float getPitch();
		inline float getYaw();
		inline float getRoll();

		inline void setPitch(float pitch);
		inline void setYaw(float yaw);
		inline void setRoll(float roll);
		
		virtual bool shouldRender();

		virtual void renderSingle(Pipeline& p, Renderer* r);
		virtual void renderGroup(Pipeline& p, Renderer* r);

		CubeWireFrameModel* m_wireFrameModel;
		CubeWireFrameModel* m_staticWireFrameModel;

		virtual void renderStaticWireFrameGroup(Pipeline& p, Renderer* r);
		virtual void renderWireFrameGroup(Pipeline& p, Renderer* r);

		void setCollisionDetectionGeometry(CDEnum type);
		void updateCollisionDetectionGeometry();

		inline void setMaterialEnergyRestitution(float res);
		inline float getMaterialEnergyRestitution();
		inline float getHalfMaterialEnergyRestitution();
		
		inline void setMaterialSurfaceFriction(float friction);
		inline float getMaterialSurfaceFriction();
		inline float getHalfMaterialSurfaceFriction();
		inline float getMaterialSurfaceFrictionToBitStream();

		virtual void updateContactNormalInfo(glm::vec3 normal);

		virtual void updateGameInfo();

		void update(); 

		void addParentNode(KDTreeNode* node);

		// check if we should skip itself testing in the collision detection
		virtual bool ignorePhysics();

		// check if we ignore physics testing with the object with the instanceId
		virtual bool ignorePhysicsWith(WorldObject* obj);
		
		bool shouldSend(int clientId);

		bool alreadyTestedPhysicsWith(WorldObject* obj);

		virtual WeaponSlotEnum getWeaponSlot();
		virtual WeaponNameEnum getWeaponName();

		WorldObjectState prevState;
		inline WorldObjectState getState();

		void clearParentNodes();
		vector<KDTreeNode*> m_parentNodes;
		queue<int> m_emptyIndexPool;
		
		bool inMidAir;

		void takeDamage(int damage);

		

		virtual void serialize_New(RakNet::BitStream& bs);
		virtual void deserialize_New(RakNet::BitStream& bs, ModelManager* mm);

		void serialize_Delta(int flags, RakNet::BitStream& bs);
		void deserialize_Delta(int flags, RakNet::BitStream& bs);

		void setHP(int hp);
		void setArmor(int armor);

		void print_uint8_t(uint8_t n);

		void printParentTrees();

		uint8_t collisionFlags[ENTITY_COLLISION_FLAG_SIZE];
	protected:

//		uint8_t collisionFlags[ENTITY_COLLISION_FLAG_SIZE];
		
		int m_maxHP;
		int m_curHP;

		int m_maxArmor;
		int m_curArmor;


		/// expressed in degrees
		float m_pitch;
		float m_yaw;
		float m_roll;

		DynamicType m_dynamicType;
		EntityType m_entityType;

		int m_modelEnum;
		float m_mass;
		float m_invMass;

		float m_materialEnergyRestitution;
		float m_halfMaterialEnergyRestitution;

		float m_materialSurfaceFriction;
		float m_halfMaterialSurfaceFriction;

		bool testedForNotInMidAir;
};


inline void WorldObject::setName(string s)
{
	m_name = s;
}

inline string WorldObject::getName()
{
	return m_name;
}

inline void WorldObject::setPosition(glm::vec3 pos)
{
    m_position = pos;
}

inline void WorldObject::setPosition(float x, float y, float z)
{
    m_position = glm::vec3(x, y, z);
}

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

inline void WorldObject::setRotation(glm::mat4 rot)
{
	m_xAxis = glm::vec3(rot[0][0], rot[0][1], rot[0][2]);
	m_yAxis = glm::vec3(rot[1][0], rot[1][1], rot[1][2]);
	m_zAxis = glm::vec3(rot[2][0], rot[2][1], rot[2][2]);

	float temp[16] = {rot[0][0], rot[0][1], rot[0][2], 0.0,
                      rot[1][0], rot[1][1], rot[1][2], 0.0,
                      rot[2][0], rot[2][1], rot[2][2], 0.0,
                      0.0,       0.0,       0.0,       1.0};
    m_rotation = glm::make_mat4(temp);
}



void WorldObject::setRotation(float pitch, float yaw)
{
	m_pitch = pitch;
	m_yaw = yaw;

	glm::mat4 rot = glm::mat4(1.0);

	/*
	if (roll != 0)
	{
		rot = rot * glm::rotoate(roll, 0.0f, 0.0f, 1.0f);
	}
	*/

	if (m_yaw != 0)
	{
		rot = rot * glm::rotate(m_yaw, 0.0f, 1.0f, 0.0f);
	}

	if (m_pitch != 0)
	{
		rot = rot * glm::rotate(m_pitch, 1.0f, 0.0f, 0.0f);
	}


	m_xAxis = glm::vec3(rot[0][0], rot[0][1], rot[0][2]);
	m_yAxis = glm::vec3(rot[1][0], rot[1][1], rot[1][2]);
	m_zAxis = glm::vec3(rot[2][0], rot[2][1], rot[2][2]);

	/*
	float temp[16] = { rot[0][0], rot[1][0], rot[2][0], 0,
	rot[0][1], rot[1][1], rot[2][1], 0,
	rot[0][2], rot[1][2], rot[2][2], 0,
	0, 0, 0, 1 };
	*/
	/*
	float temp[16] = { m_xAxis[0], m_yAxis[0], m_zAxis[0], 0,
	m_xAxis[1], m_yAxis[1], m_zAxis[1], 0,
	m_xAxis[2], m_yAxis[2], m_zAxis[2], 0,
	0, 0, 0, 1 };
	*/
	m_rotation = rot;

}


inline float WorldObject::getPitch()
{
	return m_pitch;
}

inline float WorldObject::getYaw()
{
	return m_yaw;
}

inline float WorldObject::getRoll()
{
	return m_roll;
}

inline void WorldObject::setPitch(float pitch)
{
	m_pitch = pitch;
}

inline void WorldObject::setYaw(float yaw)
{
	m_yaw = yaw;
}

inline void WorldObject::setRoll(float roll)
{
	m_roll = roll;
}

inline void WorldObject::setVelocity(glm::vec3 vel)
{
	m_velocity = vel;
}

inline void WorldObject::setVelocity(float x, float y, float z)
{
	m_velocity = glm::vec3(x, y, z);
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


inline void WorldObject::setModelEnum(int modelEnum)
{
	m_modelEnum = modelEnum;
}

inline void WorldObject::setModel(Model* model)
{
	if (m_modelEnum == -1)
	{
		cout << m_name << " doesn't have a model" << endl;
		while (1)
		{}
	}
	m_model = model;
	m_wireFrameModel = new CubeWireFrameModel(model->m_aabb);

//	updateAABB();

//	m_staticWireFrameModel = new CubeWireFrameModel(m_aabb);
}



inline void WorldObject::updateModelMatrix()
{
	m_modelMatrix = glm::translate(m_position) * m_rotation * glm::scale(m_scale);
}


inline void WorldObject::setMass(float mass)
{
	m_mass = mass;
	m_invMass = 1 / mass;
}

inline float WorldObject::getMass()
{
	return m_mass;
}

inline float WorldObject::getInvMass()
{
	return m_invMass;
}

inline CDEnum WorldObject::getGeometryType()
{
	return m_geometryType;
}

inline void WorldObject::setMaterialEnergyRestitution(float res)
{
	m_materialEnergyRestitution = res;
	m_halfMaterialEnergyRestitution = res / 2;
}

inline float WorldObject::getMaterialEnergyRestitution()
{
	return m_materialEnergyRestitution;
}

inline float WorldObject::getHalfMaterialEnergyRestitution()
{
	return m_halfMaterialEnergyRestitution;
}

inline void WorldObject::setMaterialSurfaceFriction(float friction)
{
	m_materialSurfaceFriction = 1-friction;
	m_halfMaterialSurfaceFriction = m_materialSurfaceFriction / 2;
}

inline float WorldObject::getMaterialSurfaceFriction()
{
	return m_materialSurfaceFriction;
}

inline float WorldObject::getHalfMaterialSurfaceFriction()
{
	return m_halfMaterialSurfaceFriction;
}

inline float WorldObject::getMaterialSurfaceFrictionToBitStream()
{
	return 1 - m_materialSurfaceFriction;
}


inline WorldObjectState WorldObject::getState()
{
	WorldObjectState state;
	state.objectId = objectId;
	state.position = m_position;
	


	state.angles[PITCH] = m_pitch;
	state.angles[YAW] = m_yaw;
	state.angles[ROLL] = m_roll;
	
	state.isHit = isHit;
	return state;
}

#endif
