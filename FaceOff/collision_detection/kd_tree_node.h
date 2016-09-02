#ifndef KD_TREE_NODE_H_
#define KD_TREE_NODE_H_


class WorldObject;
#include "world_object.h"

#include <queue>
#include "utility.h"
#include "world_object.h"
#include "cube_model.h"
#include "collision_detection_geometry.h"

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
	// 0 = left,	1 = right
	KDTreeNode* m_child[2];
	int m_splitDirection;
	float m_splitValue;

	vector<WorldObject*> m_objects;
	queue<int> m_emptyIndexPool;

	AABB m_aabb;

	CubeWireFrameModel* m_wireFrameModel;
	CubeModel* m_containedModel;
	CubeModel* m_hitModel;

	int id;

	KDTreeNode() : KDTreeNode(glm::vec3(1,1,1), glm::vec3(0,0,0))
	{ }

	
	KDTreeNode(glm::vec3 maxP, glm::vec3 minP)
	{
		m_aabb.max = maxP;	
		m_aabb.min = minP;

		m_child[0] = NULL;
		m_child[1] = NULL;
		
		createWireFrameModel();
		createCubeFrameModel();
	}
	

	void createWireFrameModel()
	{
		m_wireFrameModel = new CubeWireFrameModel(m_aabb);
	}

	void createWireFrameModel(glm::vec3 color)
	{
		m_wireFrameModel = new CubeWireFrameModel(m_aabb);
	}
	
	void createCubeFrameModel()
	{
		m_containedModel = new CubeModel(m_aabb.max, m_aabb.min);
	}

	void createCubeFrameModel(glm::vec3 color)
	{
		m_containedModel = new CubeModel(m_aabb.max, m_aabb.min, color);
	}

	void addObject(WorldObject* object)
	{
		if (!m_emptyIndexPool.empty())
		{
			int index = m_emptyIndexPool.front();
			m_emptyIndexPool.pop();
			m_objects[index] = object;
		}
		else
		{
			m_objects.push_back(object);
		}

		object->addParentNode(this);
	}

	void setObjects(vector<WorldObject*> & objects)
	{
		m_objects.clear();
		while (!m_emptyIndexPool.empty())
		{
			m_emptyIndexPool.pop();
		}

		for (int i = 0; i < objects.size(); i++)
		{
			int id = objects[i]->m_instanceId;

			m_objects.push_back(objects[i]);
			objects[i]->addParentNode(this);
		}
	}
		

	// first remove object from m_objects by setting it to null
	// then push that index to the m_emptyIndexPool
	// so next time when we insert something, we'll insert it at that index
	void removeObject(WorldObject* object)
	{
		int index = 0;
		for (int i = 0; i < m_objects.size(); i++)
		{
			WorldObject* obj = m_objects[i];

			if (obj == NULL)
				continue;

			if (obj->m_instanceId == object->m_instanceId)
			{
				index = i;
				m_objects[i] = NULL;
				break;
			}
		}

		m_emptyIndexPool.push(index);
	}

	void setChildrenToNULL()
	{
		m_child[0] = NULL;
		m_child[1] = NULL;
	}

	bool isLeaf()
	{
		return m_child[0] == NULL && m_child[1] == NULL;
	}
};



#endif