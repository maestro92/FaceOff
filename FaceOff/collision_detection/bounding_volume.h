#ifndef BOUNDING_VOLUME_H_
#define BOUNDING_VOLUME_H_

#include "utility/utility.h"

struct AABB
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

};


struct Sphere
{
	glm::vec3 center;
	float radius;
};

#endif