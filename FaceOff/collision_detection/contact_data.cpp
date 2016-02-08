
#include "contact_data.h"

ContactData::ContactData()
{
	pair.resize(2, NULL);
	point = glm::vec3(1.0);
	normal = glm::vec3(1.0);
	penetrationDepth = 1.0;
	restitution = 0.001;
}


void ContactData::resolve()
{
	resolveVelocity();
	resolveInterpenetration();
}

void ContactData::resolveVelocity()
{

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



