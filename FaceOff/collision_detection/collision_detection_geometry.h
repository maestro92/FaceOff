#ifndef COLLISION_DETECTION_GEOMETRY_H_
#define COLLISION_DETECTION_GEOMETRY_H_

#include "utility/utility.h"


enum GMEnum
{
	GM_AABB = 0,
	GM_SPHERE
};

// Collision Detection Geometry
struct CDGeometry
{
	virtual GMEnum getType() = 0;
};

struct AABB : public CDGeometry
{
	glm::vec3 max;
	glm::vec3 min;
	
	AABB()
	{
		max = glm::vec3(1.0);
		min = glm::vec3(-1.0);
	}

	AABB(glm::vec3 a, glm::vec3 b)
	{
		max = a;
		min = b;
	}

	GMEnum getType()
	{
		return GM_AABB;
	}
};

struct Sphere : public CDGeometry
{
	glm::vec3 center;
	float radius;

	GMEnum getType()
	{
		return GM_SPHERE;
	}
};

#endif