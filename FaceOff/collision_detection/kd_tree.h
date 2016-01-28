#ifndef KD_TREE_H_
#define KD_TREE_H_

#include "kd_tree_node.h"
#include "world_sphere.h"
#include "cube_wireframe_model.h"
#include <algorithm>
#include "renderer.h"
#include "model.h"

using namespace std;

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
									glm::vec3 maxP, glm::vec3 minP, int depth);

		void insert(WorldObject* object);

		bool testAABBAABB(glm::vec3 aMax, glm::vec3 aMin, glm::vec3 bMax, glm::vec3 bMin);

		void computeSplitInfo(vector<WorldObject*> objects, int direction, float& variance, float& median);

		void visitOverlappedNodes(KDTreeNode* node, WorldSphere* sphere);

		void render(Pipeline& p, Renderer* r);
		void render(KDTreeNode* root, Renderer* r);

		void renderSingle(Pipeline& p, Renderer* r);
		void renderGroup(Pipeline& p, Renderer* r);
};


#endif