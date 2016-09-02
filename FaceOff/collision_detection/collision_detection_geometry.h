#ifndef COLLISION_DETECTION_GEOMETRY_H_
#define COLLISION_DETECTION_GEOMETRY_H_

#include "utility/utility.h"



// Collision Detection Geometry
enum CDEnum
{
	CD_NONE = 0,
	CD_AABB,
	CD_SPHERE
};

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