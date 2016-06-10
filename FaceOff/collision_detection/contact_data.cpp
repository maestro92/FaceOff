
#include "contact_data.h"

ContactData::ContactData()
{
	pair.resize(2, NULL);
	point = glm::vec3(1.0);
	normal = glm::vec3(1.0);
	penetrationDepth = 1.0;
//	restitution = 0.8;
	restitution = 0.3;
	friction = 0.3;
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


// P.110
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

	/*
	utl::debug("dv", dv);
	utl::debug("totalInverseMass", totalInverseMass);
	utl::debug("impulse", impulse);
	*/

	// Find the amount of impulse per unit of inverse mass
	glm::vec3 impulsePerIMass = normal * impulse;

	glm::vec3 p0Vel = pair[0]->getVelocity();
	glm::vec3 impVel = impulsePerIMass * pair[0]->getInvMass();
	glm::vec3 totVel = p0Vel + impVel;

	pair[0]->setVelocity(totVel);
	
	if (pair[1])
		pair[1]->setVelocity(pair[1]->getVelocity() + impulsePerIMass * -pair[1]->getInvMass());
}




void ContactData::resolveVelocity1()
{
	// Find the velocity in the direction of the contact
	float separatingVelocity = calculateSeparatingVelocity();

	// Check whether it needs to be resolved
	if (separatingVelocity > 0)
	{
		// the contact is either separating or stationary - there's no impulse required
		return;
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

	/*
	utl::debug("dv", dv);
	utl::debug("totalInverseMass", totalInverseMass);
	utl::debug("impulse", impulse);
	*/

	// Find the amount of impulse per unit of inverse mass
	glm::vec3 impulsePerIMass = normal * impulse;

	glm::vec3 p0Vel = pair[0]->getVelocity();
	glm::vec3 impVel = impulsePerIMass * pair[0]->getInvMass();
	glm::vec3 totVel = p0Vel + impVel;

	totVel.x *= friction;
	totVel.z *= friction;

	if (abs(totVel.x) < VEL_THRESHOLD)
		totVel.x = 0;
	if (abs(totVel.z) < VEL_THRESHOLD)
		totVel.z = 0;

//	utl::debug("impVel", impVel);

	pair[0]->setVelocity(totVel);

	if (pair[1])
		pair[1]->setVelocity(pair[1]->getVelocity() + impulsePerIMass * -pair[1]->getInvMass());
}


void ContactData::resolveInterpenetration()
{
	if (penetrationDepth <= 0)
		return;
	
	float totalMass = pair[0]->m_mass;
	if (pair[1])
		totalMass += pair[1]->m_mass;;

	glm::vec3 movePerInvMass = normal * (penetrationDepth / totalMass);

	// Apply the penetration resolution.
	glm::vec3 temp = pair[0]->getPosition() + 
					 pair[0]->m_mass * movePerInvMass;
	pair[0]->setPosition(temp);

	if (pair[1])
	{
		temp =	pair[1]->getPosition() +
				pair[1]->m_mass * movePerInvMass;

		pair[1]->setPosition(temp);
	}
}



