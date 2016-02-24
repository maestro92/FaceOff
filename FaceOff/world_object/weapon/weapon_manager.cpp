#include "weapon_manager.h"


unordered_map<string, WeaponTypeEnum> WeaponManager::s_weaponTypeStringToEnum;
unordered_map<string, WeaponData> WeaponManager::s_weaponData;

void WeaponManager::writeWeaponJsonFiles()
{

}


void WeaponManager::initWeaponData(WeaponData& data)
{


}


void WeaponManager::readWeaponData(const mObject& obj)
{

}



const mValue& WeaponManager::findValue(const mObject& obj, const string& name)
{
	mObject::const_iterator it = obj.find(name);

	assert(it != obj.end());
	assert(it->first == name);

	return it->second;
}

void WeaponManager::initWeaponsData()
{
	s_weaponTypeStringToEnum =
	unordered_map<string, WeaponTypeEnum>
	({
		{ "MELEE", MELEE },
		{ "PISTOL", PISTOL },
		{ "SHOTGUN", SHOTGUN },
		{ "SUBMACHINE_GUN", SUBMACHINE_GUN },
		{ "ASSULT_RIFLE", ASSULT_RIFLE },
		{ "SNIPER_RIFLE", SNIPER_RIFLE },
		{ "MACHINE_GUN", MACHINE_GUN },
		{ "NUM_WEAPON_TYPE", NUM_WEAPON_TYPE }	
	});


	char* filename = "Assets/weapon_data.json";
	ifstream is(filename);

	mValue value;
	read(is, value);

	const mArray& addr_array = value.get_array();
	
	for (int i = 0; i < addr_array.size(); i++)
	{
		const mObject obj = addr_array[i].get_obj();

		string name =			findValue(obj, "name").get_str();
		int damage =			findValue(obj, "damage").get_int();
		int magazineCapacity =	findValue(obj, "magazineCapacity").get_int();
		int maxMagazineCount =	findValue(obj, "maxMagazineCount").get_int();
		string weaponTypeStr =	findValue(obj, "weaponType").get_str();
		WeaponTypeEnum weaponType = s_weaponTypeStringToEnum[weaponTypeStr];
		ImportedModel* model = new ImportedModel("./Assets/models/weapons/Ak_47/Ak-47.obj");

		s_weaponData[name] = { name, damage, magazineCapacity, maxMagazineCount, weaponType, model };
	}


	/*
	// Pistol
	s_weaponData["AK-47"] =
	{
		"AK-47", 36, 30, 3, ASSULT_RIFLE, new ImportedModel("./Assets/models/weapons/Ak_47/Ak-47.obj")
	};

	// Assult Rifle
	s_weaponData["AK-47"] =
	{
		"AK-47", 36, 30, 3, ASSULT_RIFLE, new ImportedModel("./Assets/models/weapons/Ak_47/Ak-47.obj")
	};
	
	s_weaponData["AK-47"] = 
	{
		"AK-47", 36, 30, 3, ASSULT_RIFLE, new ImportedModel("./Assets/models/weapons/Ak_47/Ak-47.obj")
	};
	*/
}


Weapon WeaponManager::createWeaponByName(string weaponName)
{
	Weapon weapon;
	WeaponData data = s_weaponData[weaponName];

	weapon.m_name = data.name;
	weapon.m_damage = data.damage;
	weapon.m_type = data.weaponType;
	weapon.m_magazine.capacity = data.magazineCapacity;
	weapon.m_magazine.cur = data.magazineCapacity;
	weapon.m_model = data.model;

	return weapon;
}

