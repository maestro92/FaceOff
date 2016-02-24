#ifndef BOUNDING_VOLUME_H_
#define BOUNDING_VOLUME_H_

#include "utility/utility.h"


enum BVEnum
{
	BV_AABB = 0,
	BV_SPHERE
};

struct BoundingVolume
{
	virtual BVEnum getType() = 0;
};

struct AABB : public BoundingVolume
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

	BVEnum getType()
	{
		return BV_AABB;
	}
};

struct Sphere : public BoundingVolume
{
	glm::vec3 center;
	float radius;

	BVEnum getType()
	{
		return BV_SPHERE;
	}
};

#endif