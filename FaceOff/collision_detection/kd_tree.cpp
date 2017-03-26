
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

// Reference !!!!
// https://graphics.stanford.edu/wikis/cs248-11-winter/Assignment_4/kdtree

KDTreeNode* KDTree::recursiveBuild(vector<WorldObject*> objects, glm::vec3 maxP, glm::vec3 minP, int depth, int& count)
{
	KDTreeNode* root = new KDTreeNode(maxP, minP);
	/*
	if (objects.size() == 3 && 
		objects[0]->m_name == "ground" &&
		objects[1]->m_name == "stairs 20" && 
		objects[2]->m_name == "stairs -20")
	{
		int a = 1;
	}
	*/

	/*
	utl::debug("depth", depth);
	for (int i = 0; i < objects.size(); i++)
	{
		utl::debug("obj name", objects[i]->m_name);
	}
	*/
	

//	if (objects.size() < 3 || depth == 6)
	if (objects.size() < 3)
	{
		vector <glm::vec3> colors = { RED, GREEN, BLUE };

		int rem = count % 3;
		root->createWireFrameModel(colors[rem]);
		root->createCubeFrameModel();

		// root->m_objects = objects;
		root->setObjects(objects);
		/*
		utl::debug("depth", depth);
		utl::debug("count", count);
		utl::debug("max", root->m_aabb.max);
		utl::debug("min", root->m_aabb.min);
		*/
		root->id = count;
		count++;
		
		utl::debug("root id", root->id);
		utl::debug("depth", depth);
		for (int i = 0; i < root->m_objects.size(); i++)
		{
			utl::debug("obj name", root->m_objects[i]->m_name);
		}

		/*
		for (int i = 0; i < root->m_objects2.size(); i++)
		{
			utl::debug("obj name", root->m_objects2[i]->m_name);
		}
		*/
		/*
		for (auto it = root->m_objects2.begin(); it != root->m_objects2.end(); it++)
		{
			utl::debug("obj name", it->second->m_name);
		}
		*/
		
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

	AABB leftAABB(lx, ln);
	AABB rightAABB(rx, rn);

	for (int i = 0; i < objects.size(); i++)
	{
		WorldObject* obj = objects[i];

		if (obj->getGeometryType() == CD_AABB)
		{
			if (CollisionDetection::testAABBAABB(*(obj->m_aabb), leftAABB))
				leftObjects.push_back(obj);

			if (CollisionDetection::testAABBAABB(*(obj->m_aabb), rightAABB))
				rightObjects.push_back(obj);
		}

		else if (obj->getGeometryType() == CD_SPHERE)
		{
			if (CollisionDetection::testSphereAABB(*(obj->m_sphere), leftAABB))
				leftObjects.push_back(obj);

			if (CollisionDetection::testSphereAABB(*(obj->m_sphere), rightAABB))
				rightObjects.push_back(obj);
		}

		/*
		if (CollisionDetection::testAABBAABB(leftAABB, obj->m_aabb->max, obj->m_aabb->min))
			leftObjects.push_back(obj);

		if (CollisionDetection::testAABBAABB(rightAABB, obj->m_aabb->max, obj->m_aabb->min))
			rightObjects.push_back(obj);
		*/
	}
	
	if (leftObjects.size() == objects.size() ||
		rightObjects.size() == objects.size() ||
		leftObjects.size() == rightObjects.size() == objects.size())
	{
		vector <glm::vec3> colors = { RED, GREEN, BLUE };

		int rem = count % 3;
		root->createWireFrameModel(colors[rem]);
		root->createCubeFrameModel();

//		root->m_objects = objects;
		root->setObjects(objects);
	//	utl::debug("depth", depth);
	//	utl::debug("count", count);
	//	utl::debug("max", root->m_aabb.max);
	//	utl::debug("min", root->m_aabb.min);
		
	
		root->id = count;


		utl::debug("root id", root->id);
		utl::debug("depth", depth);
		for (int i = 0; i < root->m_objects.size(); i++)
		{
			utl::debug("obj name", root->m_objects[i]->m_name);
		}

		count++;

		/*
		for (auto it = root->m_objects2.begin(); it != root->m_objects2.end(); it++)
		{
			utl::debug("obj name", it->second->m_name);
		}
		*/

		/*
		for (int i = 0; i < root->m_objects.size(); i++)
		{
			utl::debug("obj name", root->m_objects[i]->m_name);
		}
		*/
		utl::debugLn(2);
	
		// root->m_left = NULL;
		// root->m_right = NULL;
		root->m_child[0] = NULL;
		root->m_child[1] = NULL;


		return root;
	}
/*
	utl::debug("root->m_splitDirection", root->m_splitDirection);
	utl::debug("root->m_splitValue", root->m_splitValue);

	for (int i = 0; i < leftObjects.size(); i++)
		utl::debug("obj name", leftObjects[i]->m_name);
*/

	// root->m_left = recursiveBuild(leftObjects, lx, ln, depth + 1, count);
	// root->m_right = recursiveBuild(rightObjects, rx, rn, depth + 1, count);
	root->m_child[0] = recursiveBuild(leftObjects, lx, ln, depth + 1, count);
	root->m_child[1] = recursiveBuild(rightObjects, rx, rn, depth + 1, count);

	return root;
}


void KDTree::reInsert(WorldObject* object)
{
	// first: remove the player from player's kdtree parent nodes
	for (int i = 0; i < object->m_parentNodes.size(); i++)
	{
		KDTreeNode* kNode = object->m_parentNodes[i];

		if (kNode == NULL)
			continue;

		kNode->remove(object);
	}


	// then: we clear player's stored parentNodes
	while (!object->m_emptyIndexPool.empty())
	{
		object->m_emptyIndexPool.pop();
	}

	for (int i = 0; i < object->m_parentNodes.size(); i++)
	{
		object->m_parentNodes[i] = NULL;
		object->m_emptyIndexPool.push(i);
	}

	// last: we re-insert the player back into the kdtree
	insert(m_head, object);
}

void KDTree::insert(WorldObject* object)
{
	insert(m_head, object);
}

void KDTree::insert(KDTreeNode* node, WorldObject* object)
{
	if (node == NULL)
		return;



//	utl::debug("max", node->m_aabb.max);
//	utl::debug("min", node->m_aabb.min);

//	if (node->m_left == NULL && node->m_right == NULL)
	if (node->isLeaf())
	{
//		node->m_objects.push_back(object);
		node->addObject(object);
		return;
	}

	/*
	if (CollisionDetection::testAABBAABB(node->m_child[0]->m_aabb, object->m_aabb))
		insert(node->m_child[0], object);

	if (CollisionDetection::testAABBAABB(node->m_child[1]->m_aabb, object->m_aabb))
		insert(node->m_child[1], object);
	*/

	if (object->getGeometryType() == CD_AABB)
	{
		if (CollisionDetection::testAABBAABB(*(object->m_aabb), node->m_child[0]->m_aabb))
			insert(node->m_child[0], object);

		if (CollisionDetection::testAABBAABB(*(object->m_aabb), node->m_child[1]->m_aabb))
			insert(node->m_child[1], object);
	}

	else if (object->getGeometryType() == CD_SPHERE)
	{
		if (CollisionDetection::testSphereAABB(*(object->m_sphere), node->m_child[0]->m_aabb))
			insert(node->m_child[0], object);

		if (CollisionDetection::testSphereAABB(*(object->m_sphere), node->m_child[1]->m_aabb))
			insert(node->m_child[1], object);
	}
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
		mean += objects[i]->m_position[direction];
		splitValues.push_back(objects[i]->m_position[direction]);
	}

	mean /= objects.size();

	variance = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		float diff = objects[i]->m_position[direction] - mean;
		variance += diff * diff;
	}

	sort(splitValues.begin(), splitValues.end());

	median = splitValues[splitValues.size() / 2];
}


#if 0
void KDTree::computeSplitInfo(vector<WorldObject*> objects, int direction, float& variance, float& median)
{
	float mean = 0;

	vector<float> splitValues;

	for (int i = 0; i < objects.size(); i++)
	{
		/*
		mean += objects[i]->m_maxP[direction];
		mean += objects[i]->m_minP[direction];

		splitValues.push_back(objects[i]->m_maxP[direction]);
		splitValues.push_back(objects[i]->m_minP[direction]);
		*/

		mean += objects[i]->m_aabb.max[direction];
		mean += objects[i]->m_aabb.min[direction];

		splitValues.push_back(objects[i]->m_aabb.max[direction]);
		splitValues.push_back(objects[i]->m_aabb.min[direction]);
	}
	
	mean /= objects.size();

	variance = -1;
	for (int i = 0; i < objects.size(); i++)
	{
		//float diff = objects[i]->m_maxP[direction] - mean;
		//variance += diff * diff;

		//diff = objects[i]->m_minP[direction] - mean;
		//variance += diff * diff;

		float diff = objects[i]->m_aabb.max[direction] - mean;
		variance += diff * diff;

		diff = objects[i]->m_aabb.min[direction] - mean;
		variance += diff * diff;
	}

	sort(splitValues.begin(), splitValues.end());
	
	median = splitValues[splitValues.size() / 2];
}
#endif


#if 0
void KDTree::visitNodes(KDTreeNode* node, glm::vec3 lineStart, glm::vec3 lineDir, float tmax, WorldObject* & object, int depth, KDTreeNode*& hitNode)
{
	if (node == NULL)
		return;
	/*
	if (depth == 2)
	{
		hitNode = node;
		return;
	}
	*/

	if (node->isLeaf())
	{


		for (int i = 0; i < node->m_objects.size(); i++)
		{

			if (lineStart.x > 120)
			{
				int a = 1;
			}

//			if (KDTree::testRayAABB(lineStart, lineDir, node->m_objects[i]->m_maxP, node->m_objects[i]->m_minP))
			if (KDTree::testRayAABB(lineStart, lineDir, node->m_objects[i]->m_aabb))
			{
				utl::debug("lineStart", lineStart);
				utl::debug("lineDir", lineDir);
				object = node->m_objects[i];
				hitNode = node;
				return;
			}
		}
		return;
	}


	int dim = node->m_splitDirection;
	int val = node->m_splitValue;

	int first = lineStart[dim] > node->m_splitValue;

	if (lineDir[dim] == 0.0f)
	{
		if (first == 0)
			visitNodes(node->m_left, lineStart, lineDir, tmax, object, depth + 1, hitNode);
		else
			visitNodes(node->m_right, lineStart, lineDir, tmax, object, depth + 1, hitNode);
	}
	else
	{
		float t = (node->m_splitValue - lineStart[dim]) / lineDir[dim];

		if (0.0f <= t && t < tmax)
		{
			if (first == 0)
			{
				visitNodes(node->m_left, lineStart, lineDir, tmax, object, depth + 1, hitNode);
				if (object != NULL)
					return;
			
			//	if (hitNode != NULL)
			//		return;
				visitNodes(node->m_right, lineStart + lineDir * t, lineDir, tmax - t, object, depth + 1, hitNode);
			}
			else
			{
				visitNodes(node->m_right, lineStart, lineDir, tmax, object, depth + 1, hitNode);
				if (object != NULL)
					return;

				//if (hitNode != NULL)
				//	return;

				visitNodes(node->m_left, lineStart + lineDir * t, lineDir, tmax - t, object, depth + 1, hitNode);
			}
		}
		else
		{
			if (first == 0)
				visitNodes(node->m_left, lineStart, lineDir, tmax, object, depth + 1, hitNode);
			else
				visitNodes(node->m_right, lineStart, lineDir, tmax, object, depth + 1, hitNode);
		}
	}
}
#endif




// ray interescting kd tree
// Real Time Collision Detection page 323

// ray interesecting an AABB
// Real Time Collision Detection page 179 - 181
#if 0
void KDTree::visitNodes(KDTreeNode* node, WorldObject* player, glm::vec3 lineStart, glm::vec3 lineDir, float tmax, WorldObject* & hitObject, float& hitObjectSqDist, glm::vec3& hitPoint)
{
	if (node == NULL)
		return;

	if (node->isLeaf())
	{
		for (int i = 0; i < node->m_objects.size(); i++)
		{
			WorldObject* obj = node->m_objects[i];

			if (obj == NULL)
				continue;

			if (obj->alreadyFireTested)
				continue;

//			if (obj->objectId.id == player->objectId.id)
			if (obj->getInstanceId() == player->getInstanceId())
				continue;


			glm::vec3 tempHitPoint;

//			if (CollisionDetection::testRayAABB(lineStart, lineDir, obj->m_geometry, tempHitPoint))
//			if (CollisionDetection::testRayCollisionDetection(lineStart, lineDir, obj->m_geometry, tempHitPoint))

			bool hit = false;
		

			if (obj->getGeometryType() == CD_AABB)
			{
				hit = CollisionDetection::testRayAABB(lineStart, lineDir, *(obj->m_aabb), tempHitPoint);
			}
			else if (obj->getGeometryType() == CD_SPHERE)
			{
				utl::debug("here in sphere");
				cout << "obj name is" << obj->m_name << endl;
				hit = CollisionDetection::testRaySphere(lineStart, lineDir, *(obj->m_sphere), tempHitPoint);
			}

			if (hit)
			{
				obj->alreadyFireTested = true;

				float sqDist = glm::length2(player->m_position - tempHitPoint);
				if (sqDist < hitObjectSqDist)
				{
					hitObjectSqDist = sqDist;
					hitObject = obj;
					hitPoint = tempHitPoint;
				}
			}
		}

		/*
		for (auto it = node->m_objects2.begin(); it != node->m_objects2.end(); it++)
		{
			if ((it->second)->alreadyFireTested)
				continue;

			if (it->second->m_instanceId == player->m_instanceId)
				continue;

			if (KDTree::testRayAABB(lineStart, lineDir, (it->second)->m_aabb))
			{
				float sqDist = glm::length2(player->m_position - (it->second)->m_position);
				utl::debug("THIS NEEDS TO BE FIXED!!!! Wrong distance metric");

				(it->second)->alreadyFireTested = true;

				if (sqDist < hitObjectSqDist)
				{
					utl::debug("lineStart", lineStart);
					utl::debug("lineDir", lineDir);
					hitObjectSqDist = sqDist;
					hitObject = (it->second);

				}
			}
		}
		*/
		return;
	}


	int dim = node->m_splitDirection;
	int val = node->m_splitValue;

	int first = lineStart[dim] > node->m_splitValue;

	if (lineDir[dim] == 0.0f)
	{
		visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint);
	}
	else
	{
		float t = (node->m_splitValue - lineStart[dim]) / lineDir[dim];

		if (0.0f <= t && t < tmax)
		{
			visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint);
		//	if (hitObject != NULL)
			//	return;

			visitNodes(node->m_child[first ^ 1], player, lineStart + lineDir * t, lineDir, tmax - t, hitObject, hitObjectSqDist, hitPoint);
		}
		else
		{
			visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint);
		}
	}
}


void KDTree::visitNodes(KDTreeNode* node, WorldObject* player, glm::vec3 lineStart, glm::vec3 lineDir, float tmax, WorldObject* & hitObject, float& hitObjectSqDist, glm::vec3& hitPoint, unordered_set<int>& objectsAlreadyTested)
{
	if (node == NULL)
		return;

	if (node->isLeaf())
	{
		for (int i = 0; i < node->m_objects.size(); i++)
		{
			WorldObject* obj = node->m_objects[i];

			if (obj == NULL)
				continue;

			if (obj->alreadyFireTested)
				continue;

			// if (obj->objectId.id == player->objectId.id)
			if (obj->getInstanceId() == player->getInstanceId())
				continue;

			glm::vec3 tempHitPoint;

			bool hit = false;

			if (objectsAlreadyTested.find(obj->getInstanceId()) != objectsAlreadyTested.end())
				continue;
			else
				objectsAlreadyTested.insert(obj->getInstanceId());

			if (obj->getGeometryType() == CD_AABB)
			{
				hit = CollisionDetection::testRayAABB(lineStart, lineDir, *(obj->m_aabb), tempHitPoint);
			}
			else if (obj->getGeometryType() == CD_SPHERE)
			{
				cout << "obj name is" << obj->m_name << endl;
				hit = CollisionDetection::testRaySphere(lineStart, lineDir, *(obj->m_sphere), tempHitPoint);
				if (hit == true)
					cout << "true" << endl;				
				else
					cout << "false" << endl;
			}

			if (hit)
			{
				obj->alreadyFireTested = true;

				float sqDist = glm::length2(player->m_position - tempHitPoint);
				if (sqDist < hitObjectSqDist)
				{
					hitObjectSqDist = sqDist;
					hitObject = obj;
					hitPoint = tempHitPoint;
				}
			}
		}

		return;
	}


	int dim = node->m_splitDirection;
	int val = node->m_splitValue;

	int first = lineStart[dim] > node->m_splitValue;

	if (lineDir[dim] == 0.0f)
	{
		visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);
	}
	else
	{
		float t = (node->m_splitValue - lineStart[dim]) / lineDir[dim];

		if (0.0f <= t && t < tmax)
		{
			visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);
			//	if (hitObject != NULL)
			//	return;

			visitNodes(node->m_child[first ^ 1], player, lineStart + lineDir * t, lineDir, tmax - t, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);
		}
		else
		{
			visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);
		}
	}
}

#endif


void KDTree::visitNodes(KDTreeNode* node, WorldObject* player, glm::vec3 lineStart, glm::vec3 lineDir, float tmax, 
	WorldObject* & hitObject, float& hitObjectSqDist, glm::vec3& hitPoint, vector<WorldObject*>& objectsAlreadyTested)
{
	if (node == NULL)
		return;

	if (node->isLeaf())
	{
		for (int i = 0; i < node->m_objects.size(); i++)
		{
			WorldObject* obj = node->m_objects[i];

			if (obj == NULL)
				continue;

			if (obj->alreadyFireTested)
				continue;

			if (obj->getCollisionFlagIndex() == player->getCollisionFlagIndex())
				continue;

			glm::vec3 tempHitPoint;

			bool hit = false;

			objectsAlreadyTested.push_back(obj);
			obj->alreadyFireTested = true;

			if (obj->getGeometryType() == CD_AABB)
			{
				hit = CollisionDetection::testRayAABB(lineStart, lineDir, *(obj->m_aabb), tempHitPoint);
			}
			else if (obj->getGeometryType() == CD_SPHERE)
			{
				cout << "obj name is" << obj->m_name << endl;
				hit = CollisionDetection::testRaySphere(lineStart, lineDir, *(obj->m_sphere), tempHitPoint);
				if (hit == true)
					cout << "true" << endl;
				else
					cout << "false" << endl;
			}

			if (hit)
			{
				float sqDist = glm::length2(player->m_position - tempHitPoint);
				if (sqDist < hitObjectSqDist)
				{
					hitObjectSqDist = sqDist;
					hitObject = obj;
					hitPoint = tempHitPoint;
				}
			}
		}

		return;
	}


	int dim = node->m_splitDirection;
	int val = node->m_splitValue;

	int first = lineStart[dim] > node->m_splitValue;

	if (lineDir[dim] == 0.0f)
	{
		visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);
	}
	else
	{
		float t = (node->m_splitValue - lineStart[dim]) / lineDir[dim];

		if (0.0f <= t && t < tmax)
		{
			visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);
			//	if (hitObject != NULL)
			//	return;

			visitNodes(node->m_child[first ^ 1], player, lineStart + lineDir * t, lineDir, tmax - t, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);
		}
		else
		{
			visitNodes(node->m_child[first], player, lineStart, lineDir, tmax, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);
		}
	}
}












// Real Time Collision Detection page 321
void KDTree::visitOverlappedNodes(KDTreeNode* node, WorldObject* testObject, glm::vec3& volNearPt, vector<WorldObject*>& objects, bool setCollsionFlagsBothWays)
{
	if (node == NULL)
		return;
	
	int dir = node->m_splitDirection;
	int val = node->m_splitValue;

	// visiting current node
	if (node->isLeaf())
	{
		/*
		for (auto it = node->m_objects2.begin(); it != node->m_objects2.end(); it++)
		{
			if (it->second->m_instanceId == testObject->m_instanceId)
				continue;
			objects.push_back(it->second);
		}
		*/

		// utl::debug("size is", node->m_objects.size());

		/*
		utl::debug("node id is", node->id);
		if (node->id = 5)
		{
			int a = 1;
		}
		*/

		for (int i = 0; i < node->m_objects.size(); i++)
		{
			WorldObject* obj = node->m_objects[i];

			if (obj == NULL)
			{
				continue;
			}
			
			/*
			if (testObject->m_name == "player 0")
			{
				utl::debug("		object name is", obj->m_name);
			}
			
			if (testObject->m_name == "player 0" && obj->m_name == "ground")
			{
				int a = 1;
			}
			*/

			if (testObject->ignorePhysicsWith(obj))
			{
				continue;
			}

			if (testObject->alreadyTestedPhysicsWith(obj))
			{
				continue;
			}
//			if (obj->objectId.id == testObject->objectId.id)
//			if (obj->getInstanceId() == testObject->getInstanceId())			
//				continue;
			
//			utl::debug("object name is", obj->m_name);
			objects.push_back(obj);

			testObject->registerCollsionFlag(obj->getCollisionFlagIndex());
			if (setCollsionFlagsBothWays)
			{
				obj->registerCollsionFlag(testObject->getCollisionFlagIndex());
			}


			/*
			if (testObject->m_name == "player 0")
			{				
				int objIndex = obj->getCollisionFlagIndex();
				int arrayIndex = objIndex / ENTITY_COLLISION_ENTRY_SIZE;
				int intIndex = objIndex - arrayIndex * ENTITY_COLLISION_ENTRY_SIZE;

				cout << "		value is " << unsigned(testObject->collisionFlags[arrayIndex]) << endl;
				int a = 1;
			}
			*/

		}

		return;
	}

	int first = testObject->m_position[dir] > val;

	visitOverlappedNodes(node->m_child[first], testObject, volNearPt, objects, setCollsionFlagsBothWays);


	float oldValue = volNearPt[dir];
	volNearPt[dir] = val;



	if (testObject->getGeometryType() == CD_AABB)
	{
		if (CollisionDetection::testAABBAABB(*(testObject->m_aabb), node->m_child[first ^ 1]->m_aabb))
		{
			visitOverlappedNodes(node->m_child[first ^ 1], testObject, volNearPt, objects, setCollsionFlagsBothWays);
		}
	}

	else if (testObject->getGeometryType() == CD_SPHERE)
	{
		if (glm::length2(volNearPt - testObject->m_sphere->center) < (testObject->m_sphere->radius * testObject->m_sphere->radius))
		{
			visitOverlappedNodes(node->m_child[first ^ 1], testObject, volNearPt, objects, setCollsionFlagsBothWays);
		}
	}
	volNearPt[dir] = oldValue;
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
		r->setUniLocs(p);		
		m_head->m_wireFrameModel->render();
	p.popMatrix();
}




void KDTree::renderWireFrame(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->setUniLocs(p);
		renderWireFrame(m_head, r);
	p.popMatrix();
}

void KDTree::renderWireFrame(KDTreeNode* root, Renderer* r)
{
	if (root == NULL)
		return;
	
	root->m_wireFrameModel->render();

	renderWireFrame(root->m_child[0], r);
	renderWireFrame(root->m_child[1], r);

	
//	renderWireFrame(root->m_left, r);
//	renderWireFrame(root->m_right, r);

}

void KDTree::renderCubeFrame(Pipeline& p, Renderer* r)
{
	p.pushMatrix();
		r->setUniLocs(p);
		renderCubeFrame(m_head, r);
	p.popMatrix();
}

void KDTree::renderCubeFrame(KDTreeNode* root, Renderer* r)
{
	if (root == NULL)
		return;

	if (root->isLeaf())
		root->m_containedModel->render();
	
	renderCubeFrame(root->m_child[0], r);
	renderCubeFrame(root->m_child[1], r);

//	renderCubeFrame(root->m_left, r);
//	renderCubeFrame(root->m_right, r);

}

void KDTree::renderNode(Pipeline& p, Renderer* r, KDTreeNode* root)
{
	p.pushMatrix();
		r->setUniLocs(p);
		root->m_containedModel->render();
	p.popMatrix();
}


/*
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

bool KDTree::testAABBAABB(AABB a, AABB b)
{
	if (a.max.x < b.min.x || a.min.x > b.max.x)
		return false;

	if (a.max.y < b.min.y || a.min.y > b.max.y)
		return false;

	if (a.max.z < b.min.z || a.min.z > b.max.z)
		return false;

	return true;
}
*/


void KDTree::print()
{
	print(m_head);
}

void KDTree::print(KDTreeNode* node)
{
	if (node == NULL)
		return;

	if (node->isLeaf())
	{
		cout << "	node " << node->id << " has " << node->m_objects.size() << " of objects" << endl;

		for (int i = 0; i < node->m_objects.size(); i++)
		{
			utl::debug("obj name", node->m_objects[i]->m_name);
		}

		cout << endl << endl << endl;

	}

	print(node->m_child[0]);
	print(node->m_child[1]);
}


void KDTree::print(int id)
{
	print(m_head, id);
}

void KDTree::print(KDTreeNode* node, int id)
{
	if (node == NULL)
		return;

	if (node->isLeaf() && node->id == id)
	{
		cout << "	node " << node->id << " has " << node->m_objects.size() << " of objects" << endl;

		for (int i = 0; i < node->m_objects.size(); i++)
		{
			utl::debug("obj name", node->m_objects[i]->m_name);
		}

		cout << endl << endl << endl;

	}

	print(node->m_child[0], id);
	print(node->m_child[1], id);
}