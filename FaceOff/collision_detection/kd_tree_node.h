#ifndef KD_TREE_NODE_H_
#define KD_TREE_NODE_H_

// check 13.4.1 for optimization
struct KDTreeNode
{
	KDTreeNode* m_child[2];
	int m_splitType;
	float m_splitValue;
};



#endif