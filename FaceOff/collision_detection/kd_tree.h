#ifndef KD_TREE_H_
#define KD_TREE_H_

#include "kd_tree_node.h"
#include "world_sphere.h"

class KDTree
{

	KDTreeNode* m_head;

	void visitOverlappedNodes(KDTreeNode* node, WorldSphere* sphere);
};


#endif