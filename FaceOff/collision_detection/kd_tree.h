#ifndef KD_TREE_H_
#define KD_TREE_H_

#include "kd_tree_node.h"
#include "cube_wireframe_model.h"
#include <algorithm>
#include "renderer.h"
#include "model.h"
#include "collision_detection.h"
// #include "player.h"
using namespace std;
#include "bounding_volume.h"

/*
enum NODE_OBJ_RELATION
{
	INSIDE = 0,
	PARTIAL,
	OUTSIDE
};
*/



class KDTree
{
	public:
		KDTreeNode* m_head;

		KDTree();

		/*
		void build(vector<WorldObject*> objects, 
					float maxX, float minX, 
					float maxY, float minY, 
					float maxZ, float minZ);
		
		KDTreeNode* recursiveBuild(vector<WorldObject*> objects,
								float maxX, float minX, 
								float maxY, float minY, 
								float maxZ, float minZ);
								*/

		void build(vector<WorldObject*> objects, 
					glm::vec3 maxP, glm::vec3 minP);

		KDTreeNode* recursiveBuild(vector<WorldObject*> objects,
									glm::vec3 maxP, glm::vec3 minP, int depth, int& count);

		void insert(WorldObject* object);
		void insert(KDTreeNode* node, WorldObject* object);

		void computeSplitInfo(vector<WorldObject*> objects, int direction, float& variance, float& median);

		// void visitNodes(KDTreeNode* node, glm::vec3 lineStart, glm::vec3 lineDir, float tmax, WorldObject* & object);
		// void visitNodes(KDTreeNode* node, glm::vec3 lineStart, glm::vec3 lineDir, float tmax, WorldObject* & object, int depth, KDTreeNode*& hitNode);
		void visitNodes(KDTreeNode* node, WorldObject* player, glm::vec3 lineStart, glm::vec3 lineDir, float tmax, WorldObject* & hitObject, float& hitObjectSqDist, glm::vec3& hitPoint);


		// void visitOverlappedNodes(KDTreeNode* node, Player* player, glm::vec3& volNearPt);
		// void visitOverlappedNodes(Player* player, glm::vec3& volNearPt, vector<WorldObject*>& objects);
		void visitOverlappedNodes(KDTreeNode* node, WorldObject* testObject, glm::vec3& volNearPt, vector<WorldObject*>& objects);



		void renderWireFrame(Pipeline& p, Renderer* r);
		void renderWireFrame(KDTreeNode* root, Renderer* r);

		void renderCubeFrame(Pipeline& p, Renderer* r);
		void renderCubeFrame(KDTreeNode* root, Renderer* r);

		void renderNode(Pipeline& p, Renderer* r, KDTreeNode* node);
		
		void renderSingle(Pipeline& p, Renderer* r);
		void renderGroup(Pipeline& p, Renderer* r);


	//	static bool testRayAABB(glm::vec3 p, glm::vec3 d, AABB aabb);
		static bool testAABBAABB(AABB a, AABB b);
		static bool testAABBAABB(glm::vec3 aMax, glm::vec3 aMin, glm::vec3 bMax, glm::vec3 bMin);

		// void copyObjects(KDTreeNode* & node, vector<WorldObject*> & objects);

		void print();
		void print(KDTreeNode* node);
};


#endif