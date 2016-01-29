
#include "kd_tree.h"

struct CompX
{
	bool operator() (WorldObject* a, WorldObject* b)
	{
		return a->m_position.x < b->m_position.x;
	}
} compX;

struct CompY
{
	bool operator() (WorldObject* a, WorldObject* b)
	{
		return a->m_position.y < b->m_position.y;
	}
} compY;

struct CompZ
{
	bool operator() (WorldObject* a, WorldObject* b)
	{
		return a->m_position.z < b->m_position.z;
	}
} compZ;

KDTree::KDTree()
{
	m_head = NULL;

}


void KDTree::build(vector<WorldObject*> objects, glm::vec3 maxP, glm::vec3 minP)
{
	if (objects.size() == 0)
		m_head = NULL;
	else
	{
		int count = 0;
		m_head = recursiveBuild(objects, maxP, minP, 0, count);
	}
}


// leaf nodes doens't have splitting planes
// so it will store items, but not have split items/split direction

KDTreeNode* KDTree::recursiveBuild(vector<WorldObject*> objects, glm::vec3 maxP, glm::vec3 minP, int depth, int& count)
{
	// utl::debug("depth", depth);

	KDTreeNode* root = new KDTreeNode(maxP, minP);
 
//	if (objects.size() < 3 || depth == 6)
	if (objects.size() < 3 || depth == 4)
	{
		vector <glm::vec3> colors = { RED, GREEN, BLUE };

		int rem = count % 3;
		root->createWireFrameModel(colors[rem]);

		root->m_objects = objects;
		utl::debug("depth", depth);
		utl::debug("count", count);
		utl::debug("max", root->m_maxP);
		utl::debug("min", root->m_minP);
		count++;
		for (int i = 0; i < root->m_objects.size(); i++)
		{
			utl::debug("obj name", root->m_objects[i]->m_name);
		}
		utl::debugLn(2);
		return root;
	}

	glm::vec3 lx, rx;	lx = rx = maxP;
	glm::vec3 ln, rn;	ln = rn = minP;

	
	float varX, varY, varZ; 
	float medX, medY, medZ;
	computeSplitInfo(objects, 0, varX, medX);
	computeSplitInfo(objects, 1, varY, medY);
	computeSplitInfo(objects, 2, varZ, medZ);

	float maxVar = max(varX, max(varY, varZ));
	
	if (maxVar == varX)
	{
		root->m_splitDirection = SPLIT_AT_X_AXIS;
		root->m_splitValue = medX;
		lx.x = rn.x = medX;
	}
	else if (maxVar == varY)
	{
		root->m_splitDirection = SPLIT_AT_Y_AXIS;
		root->m_splitValue = medY;
		lx.y = rn.y = medY;
	}
	else
	{
		root->m_splitDirection = SPLIT_AT_Z_AXIS;
		root->m_splitValue = medZ;
		lx.z = rn.z = medZ;
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
	
	if (leftObjects.size() == objects.size() || 
		rightObjects.size() == objects.size() || 
		leftObjects.size() == rightObjects.size() == objects.size())
		return root;

	/*
	utl::debug("root->m_splitDirection", root->m_splitDirection);
	utl::debug("root->m_splitValue", root->m_splitValue);

	for (int i = 0; i < leftObjects.size(); i++)
		utl::debug("obj name", leftObjects[i]->m_name);
*/

	root->m_left = recursiveBuild(leftObjects, lx, ln, depth + 1, count);
	root->m_right = recursiveBuild(rightObjects, rx, rn, depth + 1, count);


	return root;
}

/*
void KDTree::computeSplitInfo(vector<WorldObject*> objects, int direction, float& variance, float& median)
{
	float mean = 0;

	vector<float> splitValues;

	for (int i = 0; i < objects.size(); i++)
	{
		mean += objects[i]->m_position[direction];
		splitValues.push_back(objects[i]->m_position[direction]);
	}

	mean /= objects.size();

	variance = -1;
	for (int i = 0; i < objects.size(); i++)
	{
		float diff = objects[i]->m_position[direction] - mean;
		variance += diff * diff;
	}

	sort(splitValues.begin(), splitValues.end());


	if (direction == 0)
		sort(objects.begin(), objects.end());
	else if (direction == 1)
		sort(objects.begin(), objects.end());
	else
		sort(objects.begin(), objects.end());

	median = splitValues[splitValues.size() / 2];
}
*/



void KDTree::computeSplitInfo(vector<WorldObject*> objects, int direction, float& variance, float& median)
{
	float mean = 0;

	vector<float> splitValues;

	for (int i = 0; i < objects.size(); i++)
	{
		mean += objects[i]->m_maxP[direction];
		mean += objects[i]->m_minP[direction];

		splitValues.push_back(objects[i]->m_maxP[direction]);
		splitValues.push_back(objects[i]->m_minP[direction]);
	}
	
	mean /= objects.size();

	variance = -1;
	for (int i = 0; i < objects.size(); i++)
	{
		float diff = objects[i]->m_maxP[direction] - mean;
		variance += diff * diff;

		diff = objects[i]->m_minP[direction] - mean;
		variance += diff * diff;
	}

	sort(splitValues.begin(), splitValues.end());
	
	median = splitValues[splitValues.size() / 2];
}


bool KDTree::testAABBAABB(glm::vec3 aMax, glm::vec3 aMin, glm::vec3 bMax, glm::vec3 bMin)
{
	if (aMax.x <= bMin.x || aMin.x >= bMax.x)
		return false;

	if (aMax.y <= bMin.y || aMin.y >= bMax.y)
		return false;

	if (aMax.z <= bMin.z || aMin.z >= bMax.z)
		return false;

	return true;
}


void KDTree::visitOverlappedNodes(KDTreeNode* node, Player* player, glm::vec3& volNearPt, vector<WorldObject*>& objects)
{
	if (node == NULL)
		return;
	
	int dir = node->m_splitDirection;
	int val = node->m_splitValue;

	// visiting current node
	if (node->isLeaf())
	{
		for (int i = 0; i<node->m_objects.size(); i++)
			objects.push_back(node->m_objects[i]);
		return;
	}

	int first = player->m_position[dir] > val;

	if (first == 0)
		visitOverlappedNodes(node->m_left, player, volNearPt, objects);
	else
		visitOverlappedNodes(node->m_right, player, volNearPt, objects);

	float oldValue = volNearPt[dir];
	volNearPt[dir] = val;

	if (glm::length2(volNearPt - player->m_position) < (player->m_scale.x * player->m_scale.x))
	{
		first = first ^ 1;
		if (first == 0)
			visitOverlappedNodes(node->m_left, player, volNearPt, objects);
		else
			visitOverlappedNodes(node->m_right, player, volNearPt, objects);
	}


	volNearPt[dir] = oldValue;
	
}



void KDTree::insert(WorldObject* object)
{

}


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




void KDTree::renderWireFrame(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->loadUniformLocations(p);
		renderWireFrame(m_head, r);
	p.popMatrix();
}

void KDTree::renderWireFrame(KDTreeNode* root, Renderer* r)
{
	if (root == NULL)
		return;
	
	root->m_wireFrameModel->render();
	renderWireFrame(root->m_left, r);
	renderWireFrame(root->m_right, r);

}

void KDTree::renderCubeFrame(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->loadUniformLocations(p);
		renderCubeFrame(m_head, r);
	p.popMatrix();
}

void KDTree::renderCubeFrame(KDTreeNode* root, Renderer* r)
{
	if (root == NULL)
		return;

	if (root->isLeaf())
		root->m_containedModel->render();
	
	renderCubeFrame(root->m_left, r);
	renderCubeFrame(root->m_right, r);

}

void KDTree::print()
{

}


/*
KDTreeNode* KDTree::recursiveBuild(vector<WorldObject*> objects, glm::vec3 maxP, glm::vec3 minP, int depth)
{
utl::debug("depth", depth);

KDTreeNode* root = new KDTreeNode(maxP, minP);

if (objects.size() < 2 || depth == 5)
{
root->createWireFrameModel();
return root;
}

glm::vec3 lx, rx;	lx = rx = maxP;
glm::vec3 ln, rn;	ln = rn = minP;

glm::vec3 diff = maxP - minP;
float maxDiff = max(diff.x, max(diff.y, diff.z));


// Splits are chosen along the single dimension with the greatest spread of points,
// with the sets partitioned by the median value of all points along that dimension

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
root->m_right = recursiveBuild(rightObjects, rx, rn, depth + 1);


return root;
}

*/