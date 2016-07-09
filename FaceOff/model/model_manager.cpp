#include "model_manager.h"


ModelManager::ModelManager()
{

}

ModelManager::~ModelManager()
{

}

void ModelManager::init()
{

	vector<string> textures;  textures.push_back("Assets/Images/chess.png");
	m_player = new ImportedModel("./Assets/models/sphere/sphere.obj", textures);
	m_player->setMeshRandTextureIdx();


	textures.clear();  textures.push_back("Assets/tree.png");
	m_tree = new ImportedModel("Assets/tree.obj", textures);


	m_cube = new ImportedModel("Assets/models/unit_cube.obj");


	m_healthBar = new QuadModel(1, 1);



	m_xyzAxis = new XYZAxisModel();


	initWeaponsData();

}


WeaponData ModelManager::getWeaponData(WeaponNameEnum name)
{
	return m_weaponDatas[name];
}

void ModelManager::initWeaponsData()
{
	
	m_weaponSlotToEnum =
		unordered_map<string, WeaponSlotEnum>
		({
			{ "MAIN", MAIN },
			{ "PISTOL", PISTOL },
			{ "MELEE", MELEE },
			{ "PROJECTILE", PROJECTILE }
		});


		/*
		unordered_map<string, WeaponTypeEnum>
		({
			{ "MELEE", MELEE },
			{ "PISTOL", PISTOL },
			{ "SHOTGUN", SHOTGUN },
			{ "SUBMACHINE_GUN", SUBMACHINE_GUN },
			{ "ASSULT_RIFLE", ASSULT_RIFLE },
			{ "SNIPER_RIFLE", SNIPER_RIFLE },
			{ "MACHINE_GUN", MACHINE_GUN }
		});
		*/

	m_weaponNameToEnum = 
		unordered_map<string, WeaponNameEnum>
		({
			{ "KNIFE", KNIFE },
			{ "DESERT_EAGLE", DESERT_EAGLE },
			{ "KATANA", KATANA },
			{ "MAC_11", MAC_11 },
			{ "MP5", MP5 },
			{ "AK_47", AK_47 },
			{ "M16", M16 },
			{ "AWM", AWM },
			{ "MINIGUN", MINIGUN },
			{ "PISTOL_SHOTGUN", PISTOL_SHOTGUN },
			{ "MG42", MG42 },
			{ "FRAG_GRENADE", FRAG_GRENADE }
		});

	

	char* filename = "Assets/weapon_data.json";
	mValue content = utl::readJsonFileToMap(filename);

	const mArray& addr_array = content.get_array();

	string modelPath = "./Assets/models/weapons/";

	for (int i = 0; i < addr_array.size(); i++)
	{
		const mObject obj = addr_array[i].get_obj();
		//const mArray& arr = addr_array[i].get_array();
		//cout << "val size is " << arr.size() << endl;

		string name =					utl::findValue(obj, "name").get_str();
		WeaponNameEnum nameEnum =		m_weaponNameToEnum[name];
		int damage =					utl::findValue(obj, "damage").get_int();
		int magazineCapacity =			utl::findValue(obj, "magazineCapacity").get_int();
		int maxMagazineCount =			utl::findValue(obj, "maxMagazineCount").get_int();

		float modelScale =				utl::findValue(obj, "modelScale").get_real();
		string weaponSlotStr =			utl::findValue(obj, "slot").get_str();
		WeaponSlotEnum slotEnum =		m_weaponSlotToEnum[weaponSlotStr];

		float fPOVScale =				utl::findValue(obj, "firstPOVScale").get_real();
		glm::vec3 fPOVOffset =			utl::findVec3(obj, "firstPOVOffset");

		string modelFileName =			modelPath + utl::findValue(obj, "model").get_str();
		
		ImportedModel* model = new ImportedModel(modelFileName);

		m_weaponDatas[nameEnum] = { name, 
									nameEnum, 
									damage, 
									magazineCapacity, 
									maxMagazineCount, 
									modelScale, 
									fPOVScale,
									fPOVOffset,
									slotEnum,
									model };
	}
	


	Value vContent = utl::readJsonFileToVector(filename);

	const Array& vArray = vContent.get_array();

	for (int i = 0; i < vArray.size(); i++)
	{
		const Object obj = vArray[i].get_obj();

		for (Object::size_type j = 0; j != obj.size(); j++)
		{
			const Pair& pair = obj[j];

			const string& name = pair.name_;
			const Value&  value = pair.value_;
			utl::debug("name is", name);
		}
	}




	/*
	ifstream is(filename);

	Value vContent;
	read_or_throw(is, vContent);



	const Array& vArray = vContent.get_array();

	for (int i = 0; i < vArray.size(); i++)
	{
		const Object obj = vArray[i].get_obj();
		//const mArray& arr = addr_array[i].get_array();
		//cout << "val size is " << arr.size() << endl;

		string name = utl::findValue(obj, "name").get_str();
		WeaponNameEnum nameEnum = m_weaponNameToEnum[name];
		int damage = utl::findValue(obj, "damage").get_int();
		int magazineCapacity = utl::findValue(obj, "magazineCapacity").get_int();
		int maxMagazineCount = utl::findValue(obj, "maxMagazineCount").get_int();

		float modelScale = utl::findValue(obj, "modelScale").get_real();
		string weaponSlotStr = utl::findValue(obj, "slot").get_str();
		WeaponSlotEnum slotEnum = m_weaponSlotToEnum[weaponSlotStr];

		float fPOVScale = utl::findValue(obj, "firstPOVScale").get_real();
		glm::vec3 fPOVOffset = utl::findVec3(obj, "firstPOVOffset");


	}

	*/


















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



/*
glm::vec3 ModelManager::findVec3(const mObject& obj, const string& name)
{
	const mObject vec3Obj = findValue(obj, name).get_obj();

	float x = findValue(vec3Obj, "x").get_real();
	float y = findValue(vec3Obj, "y").get_real();
	float z = findValue(vec3Obj, "z").get_real();

	glm::vec3 vec3Value(x, y, z);

	return vec3Value;
}

const mValue& ModelManager::findValue(const mObject& obj, const string& name)
{
	mObject::const_iterator it = obj.find(name);

	
	if (it == obj.end() || it->first != name)
	{
		cout << "error findingValue " << name);
//		assert(it != obj.end());
//		assert(it->first == name);
		exit(1);
	}

	return it->second;
}
*/



/*
Weapon ModelManager::createWeapon(WeaponNameEnum name)
{
	Weapon weapon;
	WeaponData data = m_weaponDatas[name];

	weapon.setData(data);
	
	
	weapon.m_name = data.name;
	weapon.m_damage = data.damage;
	weapon.m_type = data.type;
	weapon.m_magazine.max = data.magazineCapacity;
	weapon.m_magazine.cur = data.magazineCapacity;
	weapon.m_model = data.model;


	return weapon;
}
*/
