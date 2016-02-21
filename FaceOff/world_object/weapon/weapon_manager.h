#ifndef WEAPON_MANAGER_H_
#define WEAPON_MANAGER_H_


#include "weapon_enum.h"
#include "weapon.h"
#include "json_spirit.h"
#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"
#include <cassert>
#include <fstream>


#ifndef JSON_SPIRIT_MVALUE_ENABLED
#error Please define JSON_SPIRIT_MVALUE_ENABLED for the mValue type to be enabled 
#endif

using namespace std;
using namespace json_spirit;

struct WeaponData
{
	string name;
	int damage;
	int magazineCapacity;
	int maxMagazineCount;
	WeaponTypeEnum weaponType;
	ImportedModel* model;
};

class WeaponManager
{
	public:


		static unordered_map<string, WeaponTypeEnum> s_weaponTypeStringToEnum;
		static unordered_map<string, WeaponData> s_weaponData;

		static const mValue& findValue(const mObject& obj, const string& name);
		static void writeWeaponJsonFiles();
		static void readWeaponData(const mObject& obj);
		static void initWeaponData(WeaponData& data);
		static void initWeaponsData();
		static Weapon createWeaponByName(string weaponName);
		
};

#endif
