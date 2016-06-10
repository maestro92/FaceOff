#include "collision_detection.h"
/*
void CollisionDetection::closestPtPointAABB(glm::vec3 p, AABB b, glm::vec3& q);
float CollisionDetection::sqDistPointAABB(glm::vec3 p, AABB b);
int CollisionDetection::testSphereAABB(Sphere& s, AABB& b, glm::vec3& q);
int CollisionDetection::testSphereAABB(Sphere& s, AABB& b, ContactData& contact);
*/


// collision detection
void CollisionDetection::closestPtPointAABB(glm::vec3 p, AABB b, glm::vec3& q)
{
	for (int i = 0; i < 3; i++)
	{
		float v = p[i];
		v = max(v, b.min[i]);
		v = min(v, b.max[i]);
		q[i] = v;
	}
}

float CollisionDetection::sqDistPointAABB(glm::vec3 p, AABB b)
{
	float sqDist = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		float v = p[i];
		if (v < b.min[i])
			sqDist += (b.min[i] - v) * (b.min[i] - v);
		if (v > b.max[i])
			sqDist += (v - b.max[i]) * (v - b.max[i]);
	}
	return sqDist;
}

int CollisionDetection::testSphereAABB(Sphere& s, AABB& b, glm::vec3& q)
{
	// find point q on AABB closest to sphere center
	closestPtPointAABB(s.center, b, q);

	// Sphere and AABB intersect if (squared) distance from sphere
	// center to point q is less than (squared) sphere radius
	glm::vec3 v = q - s.center;
	float dist = glm::dot(v, v);
	return dist <= s.radius * s.radius;
}

int CollisionDetection::testSphereAABB(Sphere& s, AABB& b, ContactData& contact)
{
	// find point q on AABB closest to sphere center
	glm::vec3 q = glm::vec3(0.0);
	closestPtPointAABB(s.center, b, q);

	// Sphere and AABB intersect if (squared) distance from sphere
	// center to point q is less than (squared) sphere radius
	glm::vec3 v = q - s.center;
	float dist = glm::dot(v, v);
	if (dist > s.radius * s.radius)
		return 0;

	contact.point = q;
	contact.normal = s.center - q;
	contact.normal = glm::normalize(contact.normal);
	contact.penetrationDepth = s.radius - glm::length(v);

	return 1;
}


// http://gamedev.stackexchange.com/questions/32807/collision-resolve-sphere-aabb-and-aabb-aabb
// http://gamedev.stackexchange.com/questions/32545/what-is-the-mtv-minimum-translation-vector-in-sat-seperation-of-axis
int CollisionDetection::testAABBAABB(AABB& a, AABB& b, ContactData& contact)
{
	// minimum translation vector
	// the vector which objects can move away each other
	// most of the time, this is the normal upon first contact
	glm::vec3 mtvAxis;
	float mtvDistance = FLT_MAX; 


	if (!textAABBAABBAxis(a, b, contact, X_UNIT_AXIS, X_AXIS_DIRECTION, mtvAxis, mtvDistance))
	{
		return 0;
	}

	if (!textAABBAABBAxis(a, b, contact, Y_UNIT_AXIS, Y_AXIS_DIRECTION, mtvAxis, mtvDistance))
	{
		return 0;
	}

	if (!textAABBAABBAxis(a, b, contact, Z_UNIT_AXIS, Z_AXIS_DIRECTION, mtvAxis, mtvDistance))
	{
		return 0;
	}
	
	contact.normal = glm::normalize(mtvAxis);
	contact.penetrationDepth = (float) sqrt(mtvDistance) * 1.001f;

	return 1;
}

bool CollisionDetection::textAABBAABBAxis(AABB& a, AABB& b, ContactData& contact, glm::vec3 axis, int direction, glm::vec3 & mtvAxis, float & mtvDistance)
{
	float d0 = b.max[direction] - a.min[direction];
	float d1 = a.max[direction] - b.min[direction];

	if (d0 <= 0.0f || d1 <= 0.0f)
	{
		return false;
	}

	float overlap = (d0 < d1) ? d0 : -d1;

	glm::vec3 sep = axis * overlap;

	float sepLenSquared = glm::dot(sep, sep);

	if (sepLenSquared < mtvDistance)
	{
		mtvDistance = sepLenSquared;
		mtvAxis = sep;
	}
	return true;
}

bool CollisionDetection::testAABBAABB(glm::vec3 aMax, glm::vec3 aMin, glm::vec3 bMax, glm::vec3 bMin)
{
	if (aMax.x < bMin.x || aMin.x > bMax.x)
		return false;

	if (aMax.y < bMin.y || aMin.y > bMax.y)
		return false;

	if (aMax.z < bMin.z || aMin.z > bMax.z)
		return false;

	return true;
}

bool CollisionDetection::testAABBAABB(AABB a, AABB b)
{
	if (a.max.x < b.min.x || a.min.x > b.max.x)
		return false;

	if (a.max.y < b.min.y || a.min.y > b.max.y)
		return false;

	if (a.max.z < b.min.z || a.min.z > b.max.z)
		return false;

	return true;
}
