#include "model_manager.h"

/*
unordered_map<string, WeaponTypeEnum> m_weaponTypeToEnum;
unordered_map<string, WeaponNameEnum> m_weaponNameToEnum;
*/

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
	m_weaponTypeToEnum =
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
			{ "MG42", MG42 }
		});

	

	char* filename = "Assets/weapon_data.json";
	ifstream is(filename);

	mValue value;
	read(is, value);

	const mArray& addr_array = value.get_array();

	string modelPath = "./Assets/models/weapons/";

	for (int i = 0; i < addr_array.size(); i++)
	{
		const mObject obj = addr_array[i].get_obj();

		string	name =					findValue(obj, "name").get_str();
		WeaponNameEnum nameEnum =		m_weaponNameToEnum[name];
		int damage =					findValue(obj, "damage").get_int();
		int magazineCapacity =			findValue(obj, "magazineCapacity").get_int();
		int maxMagazineCount =			findValue(obj, "maxMagazineCount").get_int();
		
		float modelScale =				findValue(obj, "modelScale").get_real();

		string weaponTypeStr =			findValue(obj, "weaponType").get_str();
		WeaponTypeEnum typeEnum =		m_weaponTypeToEnum[weaponTypeStr];
		
		glm::vec3 fPOVOffset =			findVec3(obj, "firstPOVOffset");
		
		string modelFileName =			modelPath + findValue(obj, "model").get_str();
		// modelFileName = path + modelFileName;
		utl::debug("fPOVOffset", fPOVOffset);

		if (nameEnum == MP5)
		{
			int a = 1;
		}

		ImportedModel* model = new ImportedModel(modelFileName);

		/*
		if (nameEnum == KATANA)
		{
			model = new ImportedModel();
			vector<string> textures;
			textures.push_back("Assets/models/weapons/katana/Katana_CM_01.jpg");

			model->load(modelFileName, textures);
//			model->setTextures(textures);
	//		model->setMeshRandTextureIdx(); 
		}
		else
		{

		*/
		//	model = new ImportedModel(modelFileName);
		// }


		m_weaponDatas[nameEnum] = { name, 
									nameEnum, 
									damage, 
									magazineCapacity, 
									maxMagazineCount, 
									modelScale, 
									fPOVOffset, 
									typeEnum, 
									model };
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

	assert(it != obj.end());
	assert(it->first == name);

	return it->second;
}




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
