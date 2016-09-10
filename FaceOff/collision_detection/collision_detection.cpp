#include "collision_detection.h"






// Real Time Collision Detection P.130
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

// Real Time Collision Detection P.131
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



bool CollisionDetection::testSphereSphere(Sphere a, Sphere b)
{
	glm::vec3 d = a.center - b.center;
	float distSquared = glm::dot(d, d);

	float radiusSum = a.radius + b.radius;
	return distSquared <= radiusSum * radiusSum;
}


bool CollisionDetection::testSphereSphere(Sphere a, Sphere b, ContactData& contact)
{
	glm::vec3 d = a.center - b.center;
	float distSquared = glm::dot(d, d);

	float radiusSum = a.radius + b.radius;
	float radiusSumSquared = radiusSum * radiusSum;

	if (distSquared > radiusSumSquared)
		return false;

	contact.point = b.center + d * 0.5f;
	contact.normal = d;
	contact.normal = glm::normalize(contact.normal);
	contact.penetrationDepth = (a.radius + b.radius) - sqrt(distSquared);

	return true;
}



bool CollisionDetection::testSphereAABB(Sphere s, AABB b)
{
	// find point q on AABB closest to sphere center
	glm::vec3 q = glm::vec3(0.0);
	closestPtPointAABB(s.center, b, q);

	// Sphere and AABB intersect if (squared) distance from sphere
	// center to point q is less than (squared) sphere radius
	glm::vec3 v = s.center - q;
	float dist = glm::dot(v, v);
	return (dist <= s.radius * s.radius);
}


// Real Time Collision Detection P.165
bool CollisionDetection::testSphereAABB(Sphere s, AABB b, ContactData& contact)
{
	// find point q on AABB closest to sphere center
	glm::vec3 q = glm::vec3(0.0);
	closestPtPointAABB(s.center, b, q);

	// Sphere and AABB intersect if (squared) distance from sphere
	// center to point q is less than (squared) sphere radius
	glm::vec3 v = s.center - q;
	float dist = glm::dot(v, v);
	if (dist > s.radius * s.radius)
		return false;

	contact.point = q;
	contact.normal = v;
	contact.normal = glm::normalize(contact.normal);
	contact.penetrationDepth = s.radius - glm::length(v);

	return true;
}





// http://gamedev.stackexchange.com/questions/32807/collision-resolve-sphere-aabb-and-aabb-aabb
// http://gamedev.stackexchange.com/questions/32545/what-is-the-mtv-minimum-translation-vector-in-sat-seperation-of-axis
bool CollisionDetection::testAABBAABB(AABB a, AABB b, ContactData& contact)
{
	// minimum translation vector
	// the vector which objects can move away each other
	// most of the time, this is the normal upon first contact
	glm::vec3 mtvAxis;				// Axis along which to travel with the minimum distance
	float mtvDistance = FLT_MAX;	// Set current minimum distance (max float value so next value is always less)

	// [Axes of potential separation]
	// • Each shape must be projected on these axes to test for intersection:
	//          
	// (1, 0, 0)                    A0 (= B0) [X Axis]
	// (0, 1, 0)                    A1 (= B1) [Y Axis]
	// (0, 0, 1)                    A1 (= B2) [Z Axis]

	// [X Axis]
	if (!textAABBAABBAxis(a, b, contact, X_UNIT_AXIS, X_AXIS_DIRECTION, mtvAxis, mtvDistance))
	{
		return false;
	}

	// [Y Axis]
	if (!textAABBAABBAxis(a, b, contact, Y_UNIT_AXIS, Y_AXIS_DIRECTION, mtvAxis, mtvDistance))
	{
		return false;
	}

	// [Z Axis]
	if (!textAABBAABBAxis(a, b, contact, Z_UNIT_AXIS, Z_AXIS_DIRECTION, mtvAxis, mtvDistance))
	{
		return false;
	}
	
	// Calculate Minimum Translation Vector (MTV) [normal * penetration]
	contact.normal = glm::normalize(mtvAxis);

	// Multiply the penetration depth by itself plus a small increment
	// When the penetration is resolved using the MTV, it will no longer intersect
	contact.penetrationDepth = (float) sqrt(mtvDistance) * 1.001f;

	return true;
}

// used in testAABBAABB
// testing AABB and AABB examines intersecting in each axis
// which is what this function does
bool CollisionDetection::textAABBAABBAxis(AABB a, AABB b, ContactData& contact, glm::vec3 axis, int direction, glm::vec3 & mtvAxis, float & mtvDistance)
{
	// [Separating Axis Theorem]
	// • Two convex shapes only overlap if they overlap on all axes of separation
	// • In order to create accurate responses we need to find the collision vector (Minimum Translation Vector)   
	// • Find if the two boxes intersect along a single axis 
	// • Compute the intersection interval for that axis
	// • Keep the smallest intersection/penetration value

	// Calculate the two possible overlap ranges
	// Either we overlap on the left or the right sides
	float d0 = b.max[direction] - a.min[direction];
	float d1 = a.max[direction] - b.min[direction];

	// Intervals do not overlap, so no intersection
	if (d0 <= 0.0f || d1 <= 0.0f)
	{
		return false;
	}

	// Find out if we overlap on the 'right' or 'left' of the object.
	float overlap = (d0 < d1) ? d0 : -d1;

	// The mtd vector for that axis
	glm::vec3 sep = axis * overlap;

	// The mtd vector length squared
	float sepLenSquared = glm::dot(sep, sep);

	// If that vector is smaller than our computed Minimum Translation Distance use that vector as our current MTV distance
	if (sepLenSquared < mtvDistance)
	{
		mtvDistance = sepLenSquared;
		mtvAxis = sep;
	}
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


/*
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
*/


// https://tavianator.com/fast-branchless-raybounding-box-intersections/
// also take a look at Real Time Collision Detection P.181 (there's a error in that code, see the link below for correction)
// http://realtimecollisiondetection.net/books/rtcd/errata/

bool CollisionDetection::testRayAABB(glm::vec3 p, glm::vec3 d, AABB aabb)
{
	float tMin = -FLT_MIN;
	float tMax = FLT_MAX;
	float EPSION = 0.00001;

	for (int i = 0; i < 3; i++)
	{

		// divide by zero case, so it's handled separately
		if (abs(d[i]) < EPSION)
		{
			if (p[i] < aabb.min[i] || p[i] > aabb.max[i])
				return false;
		}
		else
		{
			float ood = 1.0f / d[i];
			float t1 = (aabb.min[i] - p[i]) * ood;
			float t2 = (aabb.max[i] - p[i]) * ood;

			if (t1 > t2)
				swap(t1, t2);

			tMin = max(tMin, t1);
			tMax = min(tMax, t2);

			if (tMin > tMax)
				return false;
			
		}
	}

	return true;
}

bool CollisionDetection::testRayAABB(glm::vec3 p, glm::vec3 d, AABB aabb, glm::vec3& q)
{
	float tMin = -FLT_MIN;
	float tMax = FLT_MAX;
	float EPSION = 0.00001;

	for (int i = 0; i < 3; i++)
	{
		// divide by zero case, so it's handled separately
		if (abs(d[i]) < EPSION)
		{
			if (p[i] < aabb.min[i] || p[i] > aabb.max[i])
				return false;
		}
		else
		{
			float ood = 1.0f / d[i];
			float t1 = (aabb.min[i] - p[i]) * ood;
			float t2 = (aabb.max[i] - p[i]) * ood;

			if (t1 > t2)
				swap(t1, t2);

			tMin = max(tMin, t1);
			tMax = min(tMax, t2);

			if (tMin > tMax)
				return false;

		}
	}

	q = p + d * tMin;
	return true;
}




bool CollisionDetection::testRaySphere(glm::vec3 p, glm::vec3 d, Sphere s)
{
	glm::vec3 m = p - s.center;
	float c = glm::dot(m, m) - s.radius * s.radius;

	// if there is definitely at least one real root, there must be an intersection
	if (c <= 0.0f)
		return true;

	float b = glm::dot(m, d);

	// early exit if ray origin outside sphere and ray pointing away form sphere
	if (b > 0.0f)
		return false;

	float disc = b*b - c;

	// a negative discriminant corresponds to ray missing sphere
	if (disc < 0.0f)
		return false;

	return true;
}

bool CollisionDetection::testRaySphere(glm::vec3 p, glm::vec3 d, Sphere s, glm::vec3& q)
{
	float t = 0.0f;

	glm::vec3 m = p - s.center;
	float b = glm::dot(m, d);
	float c = glm::dot(m, m) - s.radius * s.radius;

	// Exit if r's origin outside s (c > 0) and r pointing away from s (b > ))
	if (c > 0.0f && b > 0.0f)
		return false;

	float discr = b*b - c;

	// a negative discriminant corresponds to ray missing sphere
	if (discr < 0.0f)
		return false;

	// Ray now found to intersect sphere, compute smallest t value of intersection
	t = -b - sqrt(discr);

	// if t is negative, ray started inside sphere so clamp t to zero
	if (t < 0.0f)
		t = 0.0f;

	q = p + d * t;

	return true;
}


/*
// Real Time Collision Detection P.165
// instead of having an accurate contact normal, we change the normal only to have entirely Y component, or X or Z component
// this is mainly for cases where a Player (sphere model) makes contact with a corner of AABB cube
// so when the falls down, from the AABB, it will have a contact with the corner, hence giving a horizontal component 
bool CollisionDetection::testSphereAABBPlayerVersion(Sphere s, AABB b, ContactData& contact)
{
	// find point q on AABB closest to sphere center
	glm::vec3 q = glm::vec3(0.0);
	closestPtPointAABB(s.center, b, q);

	// Sphere and AABB intersect if (squared) distance from sphere
	// center to point q is less than (squared) sphere radius
	glm::vec3 v = s.center - q;
	float dist = glm::dot(v, v);
	if (dist > s.radius * s.radius)
		return false;

	contact.point = q;
	contact.normal = s.center - q;

	if (contact.normal.y > 0.0f)
	{
		contact.normal.x = 0.0f;
		contact.normal.z = 0.0f;
	}

	contact.normal = glm::normalize(contact.normal);
	contact.penetrationDepth = s.radius - glm::length(v);

	return true;
}
*/