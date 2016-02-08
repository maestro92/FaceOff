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


