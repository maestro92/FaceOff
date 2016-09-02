#ifndef COLLISION_DETECTION_H_
#define COLLISION_DETECTION_H_

#include "utility.h"
#include "contact_data.h"
#include "collision_detection_geometry.h"

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


namespace CollisionDetection
{
	// collision detection
	void closestPtPointAABB(glm::vec3 p, AABB b, glm::vec3& q);
	float sqDistPointAABB(glm::vec3 p, AABB b);

	bool testSphereSphere(Sphere s, Sphere b);
	bool testSphereSphere(Sphere s, Sphere b, ContactData& contact);

	bool testSphereAABB(Sphere s, AABB b);
	bool testSphereAABB(Sphere s, AABB b, ContactData& contact);
	bool testSphereAABBPlayerVersion(Sphere s, AABB b, ContactData& contact);

	bool testAABBAABB(AABB a, AABB b);
	bool testAABBAABB(AABB a, AABB b, ContactData& contact);
	bool textAABBAABBAxis(AABB a, AABB b, ContactData& contact, glm::vec3 axis, int direction, glm::vec3 & mtvAxis, float & mtvDistance);

	bool testRayAABB(glm::vec3 p, glm::vec3 d, AABB aabb);
	bool testRayAABB(glm::vec3 p, glm::vec3 d, AABB aabb, glm::vec3& q);
	bool testRaySphere(glm::vec3 p, glm::vec3 d, Sphere s);
	bool testRaySphere(glm::vec3 p, glm::vec3 d, Sphere s, glm::vec3& q);

	// bool testSphereAABB(Sphere s, AABB b, glm::vec3& q);
	// bool testAABBAABB(glm::vec3 aMax, glm::vec3 aMin, glm::vec3 bMax, glm::vec3 bMin);
};


#endif