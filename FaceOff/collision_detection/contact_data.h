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

const float VEL_THRESHOLD = 0.0005;



struct ContactData
{
	vector<WorldObject*> pair;
	glm::vec3 point;
	glm::vec3 normal;
	float penetrationDepth;
	float restitution;
	float friction;

	ContactData();

	float calculateSeparatingVelocity();

	void resolve();
	void resolveVelocity();
	void resolveVelocity1();
	void resolveInterpenetration();
};


struct CollisionDetectionTestPairs
{
	unordered_set<uint64_t> pairs;
	
	uint64_t getHash(uint32_t id0, uint32_t id1)
	{
		uint64_t h0, h1;

		if (id0 < id1)
		{
			h0 = id0;
			h0 = (h0 << 32);
			h1 = id1;
		}
		else
		{
			h0 = id1;
			h0 = (h0 << 32);
			h1 = id0;
		}

		return h0 | h1;
	}

	void addPairs(uint32_t id0, uint32_t id1)
	{
		uint64_t hash = getHash(id0, id1);
		// cout << id0 << " " << id1 << " " << hash << endl;
		pairs.insert(hash);
	}

	bool alreadyTested(uint32_t id0, uint32_t id1)
	{
		uint64_t hash = getHash(id0, id1);
		return (pairs.find(hash) != pairs.end());
	}

	void clear()
	{
		pairs.clear();
	}
};


#endif