#ifndef KD_TREE_NODE_H_
#define KD_TREE_NODE_H_


#include "utility.h"
#include "world_object.h"
#include "cube_model.h"
#include "bounding_volume.h"
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



	bool isLeaf()
	{
		return m_left == NULL && m_right == NULL;
	}


	KDTreeNode* m_left, *m_right;

	int m_splitDirection;
	float m_splitValue;

	vector<WorldObject*> m_objects;

//	glm::vec3 m_maxP;
//	glm::vec3 m_minP;

	AABB m_aabb;

	CubeWireFrameModel* m_wireFrameModel;
	CubeModel* m_containedModel;
	CubeModel* m_hitModel;
};



#endif