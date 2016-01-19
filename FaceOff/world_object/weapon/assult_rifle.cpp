

#include "assult_rifle.h"

AssultRifle::AssultRifle()
{

}

AssultRifle::AssultRifle(string gunType)
{
	m_gunType = gunType;

	for (auto it = Weapon::s_weaponModels.begin(); it != Weapon::s_weaponModels.end(); it++)
	{
		utl::debugLn(it->first);
	}


	/*
	if (Weapon::s_weaponModels.find(m_gunType) == Weapon::s_weaponModels.end())
	{
		cout << "gun model " << m_gunType << " not found" << endl;
		exit(1);
	}
	*/
}


int AssultRifle::getType()
{
	return ASSULT_RIFLE;
}
