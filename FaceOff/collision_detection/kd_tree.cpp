
#include "kd_tree.h"



KDTree::KDTree()
{
	m_head = NULL;

}


void KDTree::build(vector<WorldObject*> objects, glm::vec3 maxP, glm::vec3 minP)
{
	if (objects.size() == 0)
		m_head = NULL;
	else
		m_head = recursiveBuild(objects, maxP, minP, 0);
}


// leaf nodes doens't have splitting planes
// so it will store items, but not have split items/split direction

KDTreeNode* KDTree::recursiveBuild(vector<WorldObject*> objects, glm::vec3 maxP, glm::vec3 minP, int depth)
{
	KDTreeNode* root = new KDTreeNode(maxP, minP);
 
	if (objects.size() < 2 || depth == 2)
	{
		root->createWireFrameModel();
		return root;
	}

	glm::vec3 diff = maxP - minP;	
	float maxDiff = max(diff.x, max(diff.y, diff.z));


	glm::vec3 lx, rx;	lx = rx = maxP;
	glm::vec3 ln, rn;	ln = rn = minP;

	

	if (maxDiff == diff.x)
	{
		root->m_splitDirection = SPLIT_AT_X_AXIS;
		float splitValue = minP.x + (maxP.x - minP.x) / 2.0;
		root->m_splitValue = splitValue;
		lx.x = splitValue;
		rn.x = splitValue;
	}
	else if (maxDiff == diff.y)
	{
		root->m_splitDirection = SPLIT_AT_Y_AXIS;
		float splitValue = minP.y + (maxP.y - minP.y) / 2.0;
		root->m_splitValue = splitValue;
		lx.y = splitValue;
		rn.y = splitValue;
	}
	else
	{
		root->m_splitDirection = SPLIT_AT_Z_AXIS;
		float splitValue = minP.z + (maxP.z - minP.z) / 2.0;
		root->m_splitValue = splitValue;
		lx.z = splitValue;
		rn.z = splitValue;
	}
	
	root->createWireFrameModel();


	vector<WorldObject*> leftObjects, rightObjects;

	for (int i = 0; i < objects.size(); i++)
	{
		if (testAABBAABB(lx, ln, objects[i]->m_maxP, objects[i]->m_minP))
			leftObjects.push_back(objects[i]);

		if (testAABBAABB(rx, rn, objects[i]->m_maxP, objects[i]->m_minP))
			rightObjects.push_back(objects[i]);
	}


	root->m_left = recursiveBuild(leftObjects, lx, ln, depth + 1);
//	root->m_right = recursiveBuild(rightObjects, rx, rn, depth + 1);


	return root;
}



void KDTree::insert(WorldObject* object)
{

}

bool KDTree::testAABBAABB(glm::vec3 aMax, glm::vec3 aMin, glm::vec3 bMax, glm::vec3 bMin)
{
	if (aMax.x < bMin.x || aMin.x > bMax.x)
		return false;

	if (aMax.y < bMin.y || aMin.y > bMax.y)
		return false;

	if (aMax.z < bMin.z || aMin.z > bMax.z)
		return false;

	return true;
}



/*
NODE_OBJ_RELATION KDTree::checkNodeOjbRelation(glm::vec3 nodeMax, glm::vec3 nodeMin,
												glm::vec3 objMax, glm::vec3 objMin)
{
	bool xFlag = (nodeMin.x <= objMin.x) && (objMax.x <= nodeMax.x);
	bool yFlag = (nodeMin.y <= objMin.y) && (objMax.y <= nodeMax.y);
	bool zFlag = (nodeMin.z <= objMin.z) && (objMax.z <= nodeMax.z);
	if (xFlag && yFlag && zFlag)
		return INSIDE;

	xFlag = (objMax.x < nodeMin.x) || (nodeMax.x < objMin.x);
	yFlag = (objMax.y < nodeMin.y) || (nodeMax.y < objMin.y);
	zFlag = (objMax.z < nodeMin.z) || (nodeMax.z < objMin.z);

	if (xFlag || yFlag || zFlag)
		return OUTSIDE;

	return PARTIAL;
}
*/



void KDTree::renderSingle(Pipeline& p, Renderer* r)
{
	r->enableShader();
		renderGroup(p, r);
	r->disableShader();
}


void KDTree::renderGroup(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->loadUniformLocations(p);		
		m_head->m_wireFrameModel->render();
	p.popMatrix();
}




void KDTree::render(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->loadUniformLocations(p);
		render(m_head, r);
	p.popMatrix();
}

void KDTree::render(KDTreeNode* root, Renderer* r)
{
	if (root == NULL)
		return;
	
	root->m_wireFrameModel->render();
	render(root->m_left, r);
	render(root->m_right, r);

}

