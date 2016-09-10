
#include "contact_data.h"

ContactData::ContactData()
{
	pair.resize(2, NULL);
	point = glm::vec3(1.0);
	normal = glm::vec3(1.0);
	penetrationDepth = 1.0;
	restitution = 0;
	friction = 0;
}


void ContactData::resolve()
{
	resolveVelocity();
	resolveInterpenetration();
}

float ContactData::calculateSeparatingVelocity()
{
	glm::vec3 relativeVelocity = pair[0]->getVelocity();
	if (pair[1])
		relativeVelocity -= pair[1]->getVelocity();
	return glm::dot(relativeVelocity, normal);
}


// Game Physics Engine Development
// P.108 - P.114
void ContactData::resolveVelocity()
{
	// Find the velocity in the direction of the contact
	float separatingVelocity = calculateSeparatingVelocity();
	
	// Check whether it needs to be resolved
	if (separatingVelocity > 0)
	{
		// the contact is either separating or stationary - there's no impulse required
		return;
	}

	float frictionRestitution = 0;

	restitution = pair[0]->getHalfMaterialEnergyRestitution();
	frictionRestitution = pair[0]->getHalfMaterialSurfaceFriction();

	if (pair[1])
	{
		restitution += pair[1]->getHalfMaterialEnergyRestitution();
		frictionRestitution += pair[1]->getHalfMaterialSurfaceFriction();
	}
	
	// calculate the new separating velocity
	float newSeparatingVelocity = -separatingVelocity * restitution;

	float dv = newSeparatingVelocity - separatingVelocity;
	float totalInverseMass = pair[0]->getInvMass();

	if (pair[1])
		totalInverseMass += pair[1]->getInvMass();

	// if all particles have infinite mass, then impulses have no effect
	if (totalInverseMass <= 0)
		return;

	float impulse = dv / totalInverseMass;

	// Find the amount of impulse per unit of inverse mass
	glm::vec3 impulsePerIMass = normal * impulse;

	glm::vec3 p0Vel = pair[0]->getVelocity();
	glm::vec3 impVel = impulsePerIMass * pair[0]->getInvMass();
	glm::vec3 totVel = p0Vel + impVel;

	
	totVel.x *= frictionRestitution;
	totVel.z *= frictionRestitution;

	if (abs(totVel.x) < VEL_THRESHOLD)
		totVel.x = 0;
	if (abs(totVel.z) < VEL_THRESHOLD)
		totVel.z = 0;
	

	pair[0]->setVelocity(totVel);
	
	// opposite direction for pair[1]
	if (pair[1])
		pair[1]->setVelocity(pair[1]->getVelocity() - impulsePerIMass * pair[1]->getInvMass());
}



void ContactData::resolveInterpenetration()
{
	if (penetrationDepth <= 0)
		return;
	
	float totalInverseMass = pair[0]->getInvMass(); 
	if (pair[1])
		totalInverseMass += pair[1]->getInvMass();

	if (totalInverseMass <= 0)
		return;

	// recall normal is in direction where pair[0] is separating, so negative for pair[1]
	glm::vec3 movePerInvMass = normal * (penetrationDepth / totalInverseMass);

	// Apply the penetration resolution.
	glm::vec3 temp = pair[0]->getPosition() + 
					 pair[0]->getInvMass() * movePerInvMass;
	pair[0]->setPosition(temp);

	if (pair[1])
	{
		temp = pair[1]->getPosition() -
			   pair[1]->getInvMass() * movePerInvMass;

		pair[1]->setPosition(temp);
	}
}



