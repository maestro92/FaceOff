#ifndef KD_TREE_NODE_H_
#define KD_TREE_NODE_H_


class WorldObject;
#include "world_object.h"

#include "utility.h"
#include "world_object.h"
#include "cube_model.h"
#include "bounding_volume.h"

#include <unordered_map>
using namespace std;

// check 13.4.1 for optimization

enum SPLIT_DIRECTION
{
	SPLIT_AT_X_AXIS = 0,
	SPLIT_AT_Y_AXIS,
	SPLIT_AT_Z_AXIS
};

struct KDTreeNode
{

	KDTreeNode() : KDTreeNode(glm::vec3(1,1,1), glm::vec3(0,0,0))
	{ }

	
	KDTreeNode(glm::vec3 maxP, glm::vec3 minP)
	{
		// m_maxP = maxP;
		// m_minP = minP; 

		m_aabb.max = maxP;
		m_aabb.min = minP;

		m_left = NULL;
		m_right = NULL;
		createWireFrameModel();
		createCubeFrameModel();
	//	m_containedModel = new CubeModel(glm::vec3(0.0), glm::vec3(0.0));
	}
	

	void createWireFrameModel()
	{
//		m_wireFrameModel = new CubeWireFrameModel(m_maxP, m_minP);
		m_wireFrameModel = new CubeWireFrameModel(m_aabb);
	}

	void createWireFrameModel(glm::vec3 color)
	{
		m_wireFrameModel = new CubeWireFrameModel(m_aabb);
	}
	
	void createCubeFrameModel()
	{
//		m_containedModel = new CubeModel(m_maxP, m_minP);
		m_containedModel = new CubeModel(m_aabb.max, m_aabb.min);
	}

	void createCubeFrameModel(glm::vec3 color)
	{
//		m_containedModel = new CubeModel(m_maxP, m_minP, color);
		m_containedModel = new CubeModel(m_aabb.max, m_aabb.min, color);
	}

	void addObject(WorldObject* object)
	{
		m_objects2[object->m_instanceId] = object;
		object->m_parentNodes.push_back(this);
	}

	void setObjects(vector<WorldObject*> & objects)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			int id = objects[i]->m_instanceId;
			m_objects2[id] = objects[i];
			objects[i]->m_parentNodes.push_back(this);
		}
	}
			
	bool isLeaf()
	{
		return m_left == NULL && m_right == NULL;
	}


	KDTreeNode* m_left, *m_right;

	int m_splitDirection;
	float m_splitValue;

	// vector<WorldObject*> m_objects;
	unordered_map<int, WorldObject*> m_objects2;
	AABB m_aabb;

	CubeWireFrameModel* m_wireFrameModel;
	CubeModel* m_containedModel;
	CubeModel* m_hitModel;
};



#endif