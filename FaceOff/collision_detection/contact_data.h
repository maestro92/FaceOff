#ifndef CONTACT_DATA_H_
#define CONTACT_DATA_H_



using namespace std;

#include "utility.h"
#include "world_object.h"
/*
1.	work out the impulse we need, impulse -> change in velocity
2.	for each object in the collision, we can now find the change in
	velocity of the contact point per unit impulse

	collision with two objects, four values
	- linear velocity
	- angular velocity

	collision with static object, two values
	- linear velocity
	- angular velocity
*/


struct ContactData
{
	vector<WorldObject*> pair;
	glm::vec3 point;
	glm::vec3 normal;
	float penetrationDepth;
	float restitution;

	ContactData();
	void resolve();
	void resolveVelocity();
	void resolveInterpenetration();
};



#endif