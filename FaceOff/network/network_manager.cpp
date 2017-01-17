#include "network_manager.h"





void NetworkManager::init(ModelManager* mm, vector<WorldObject*>* objects, vector<Player*>* players)
{
	m_modelMgr = mm;
	m_objects = objects;
	m_players = players;
}


Player* NetworkManager::spawnClientPlayer(RakNet::BitStream& bs, bool defaultFlag)
{
	Player* player = new Player();
	if (defaultFlag)
	{
		player->setDefaultPlayerFlag(true);
	}
	player->spawnInfoFromBitStream(bs, m_modelMgr);

	int id = player->getId();

	if (id + 1 >= m_players->size())
	{
		m_players->resize(id + 1);
	}

	(*m_players)[id] = player;


	for (int i = 0; i < NUM_WEAPON_SLOTS; i++)
	{
		Weapon* weapon = player->getWeapons()[i];

		if (weapon != NULL)
		{
			(*m_objects).push_back(weapon);
		}
	}

	return player;

	/*
	Player* player = new Player();
	player->setFromBitStream(bs);
	

		// the message id is already ignored
		bs.Read(m_id);
		bs.ReadVector(m_position.x, m_position.y, m_position.z);
		bs.Read(m_camera->m_pitch);
		bs.Read(m_camera->m_yaw);

		bs.Read(m_geometryType);	setCollisionDetectionGeometry(m_geometryType);
		float mass = 0;
		bs.Read(mass);	setMass(mass);

		float restitution = 0;
		bs.Read(restitution);		setMaterialEnergyRestitution(restitution);

		float friction = 0;
		bs.Read(friction);			setMaterialSurfaceFriction(friction);

		// the weapons are read somewhere else
	




	// set the model
	player->setModel(m_modelMgr->get());
	
	// set the weapon
	for (int i = 0; i < NUM_WEAPON_SLOTS; i++)
	{
		int weaponEnum = 0;
		bs.Read(weaponEnum);
		if (weaponEnum != -1)
		{
			Weapon* weapon = new Weapon(m_modelMgr.getWeaponData((WeaponNameEnum)weaponEnum));
			newPlayer->pickUp(weapon);
			m_objects.push_back(weapon);

			utl::debug("weaponEnum is ", weaponEnum);
		}
		else
		{
			utl::debug("weaponEnum is None");
		}
	}


	m_defaultPlayerID = newPlayer->getId();

	if (m_defaultPlayerID + 1 >= m_players.size())
	{
		m_players.resize(m_defaultPlayerID + 1);
	}

	m_players[m_defaultPlayerID] = newPlayer;
	
	return player;
	*/

}


