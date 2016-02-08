#ifndef COLLISION_DETECTION_H_
#define COLLISION_DETECTION_H_

#include "utility.h"
#include "contact_data.h"
#include "bounding_volume.h"

#include "world_object.h"

using namespace std;

class CollisionDetection
{
	public:
		// collision detection
		static void closestPtPointAABB(glm::vec3 p, AABB b, glm::vec3& q);
		static float sqDistPointAABB(glm::vec3 p, AABB b);
		static int testSphereAABB(Sphere& s, AABB& b, glm::vec3& q);
		static int testSphereAABB(Sphere& s, AABB& b, ContactData& contact);

};



#endif