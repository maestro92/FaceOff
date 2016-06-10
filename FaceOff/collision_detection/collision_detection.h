#ifndef COLLISION_DETECTION_H_
#define COLLISION_DETECTION_H_

#include "utility.h"
#include "contact_data.h"
#include "bounding_volume.h"

#include "world_object.h"

using namespace std;


enum DIRECTION
{
	X_AXIS_DIRECTION = 0,
	Y_AXIS_DIRECTION,
	Z_AXIS_DIRECTION
};


const glm::vec3 X_UNIT_AXIS = glm::vec3(1.0, 0.0, 0.0);
const glm::vec3 Y_UNIT_AXIS = glm::vec3(0.0, 1.0, 0.0);
const glm::vec3 Z_UNIT_AXIS = glm::vec3(0.0, 0.0, 1.0);

class CollisionDetection
{
	public:
		// collision detection
		static void closestPtPointAABB(glm::vec3 p, AABB b, glm::vec3& q);
		static float sqDistPointAABB(glm::vec3 p, AABB b);
		static int testSphereAABB(Sphere& s, AABB& b, glm::vec3& q);
		static int testSphereAABB(Sphere& s, AABB& b, ContactData& contact);

		static int testAABBAABB(AABB& a, AABB& b, ContactData& contact);
		static bool textAABBAABBAxis(AABB& a, AABB& b, ContactData& contact, glm::vec3 axis, int direction, glm::vec3 & mtvAxis, float & mtvDistance);

		static bool testAABBAABB(glm::vec3 aMax, glm::vec3 aMin, glm::vec3 bMax, glm::vec3 bMin);
		static bool testAABBAABB(AABB a, AABB b);

};



#endif