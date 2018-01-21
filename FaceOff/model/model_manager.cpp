#include "model_manager.h"

/*

when working with models in blender
the -z axis in game is the y axis in blender, 

so to be sure to orient the models in the right direction


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
	m_player = new ImportedModel("./Assets/models/sphere_player.obj", textures, ModelType::player);
	m_player->setMeshRandTextureIdx();


	m_healthBar = new QuadModel(1, 1);

	m_xyzAxis = new XYZAxisModel();

	textures.clear();  textures.push_back("Assets/tree.png");
	m_tree = new ImportedModel("Assets/tree.obj", textures, ModelType::tree);

	m_cube = new ImportedModel("Assets/models/unit_cube1.obj", ModelType::cube);

//	m_woodenBox = new ImportedModel("Assets/models/wooden box/WoodenBoxOpen02.obj");

	m_woodenBox = new ImportedModel("Assets/models/wooden_box.obj", ModelType::woodenBox);

	m_ground = new ImportedModel("Assets/models/quad.obj", textures, ModelType::ground);

	textures.clear();  textures.push_back("Assets/Images/chess.png");
	m_legoMan = new ImportedModel("Assets/models/lego_man.obj", ModelType::legoMan);
	


//	m_animatedLegoMan = new AnimatedModel("Assets/models/animations/walking_lego.md5mesh", textures);

//	m_animatedLampBob = new AnimatedModel("Assets/models/animations/boblampclean.md5mesh");

	m_animatedLegoMan = AnimatedModel("Assets/models/animations/walking_lego.md5mesh", textures);
//	m_animatedLegoMan = AnimatedModel("Assets/models/animations/boblampclean.md5mesh", textures);

	m_animatedLegoMan.printBoneNodesHiearchy();


	m_sun = new ImportedModel("Assets/models/sphere10.obj", ModelType::sun);

//	m_animatedLampBob = new AnimatedModel("Assets/models/animations/boblampclean.md5mesh");

//	while(1)
//	{ }

	m_models.resize(ModelType::NUM_MODELS);
	m_models[ModelType::player] = m_player;
	m_models[ModelType::healthbar] = m_healthBar;
	m_models[ModelType::xyzAxis] = m_xyzAxis;
	m_models[ModelType::tree] = m_tree;
	m_models[ModelType::cube] = m_cube;
	m_models[ModelType::woodenBox] = m_woodenBox;
	m_models[ModelType::ground] = m_ground;
	m_models[ModelType::legoMan] = m_legoMan;
	m_models[ModelType::animatedLegoMan] = &m_animatedLegoMan;
	m_models[ModelType::sun] = m_sun;


	/*
	m_models[ModelType::player] = ImportedModel;
	m_models[ModelType::healthbar] = QuadModel(1, 1);
	m_models[ModelType::xyzAxis] = XYZAxisModel();
	m_models[ModelType::tree] = ImportedModel("Assets/tree.obj");
	m_models[ModelType::cube] = ImportedModel("Assets/models/unit_cube1.obj");
	m_models[ModelType::woodenBox] = ImportedModel("Assets/models/wooden box/WoodenBoxOpen02.obj");
	m_models[ModelType::ground] = ImportedModel("Assets/models/quad.obj");
	m_models[ModelType::legoMan] = ImportedModel("Assets/models/lego_man.obj");
	*/
	initWeaponsData();

}

void ModelManager::shutDown()
{


}

Model* ModelManager::get(int modelEnum)
{
	return m_models[modelEnum];
}

WeaponData ModelManager::getWeaponData(WeaponNameEnum name)
{
	return m_weaponDatas[name];
}

// http://strike-counter.com/cs-go-stats/weapons-stats
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
		string weaponSlotStr =			utl::findValue(obj, "slot").get_str();
		WeaponSlotEnum slotEnum =		m_weaponSlotToEnum[weaponSlotStr];
		
		int damage =					utl::findValue(obj, "damage").get_int();
		int magazineCapacity =			utl::findValue(obj, "magazineCapacity").get_int();
		int maxMagazineCount =			utl::findValue(obj, "maxMagazineCount").get_int();
		int m_rpm =						utl::findValue(obj, "rpm").get_int();

		float modelScale =				utl::findValue(obj, "modelScale").get_real();
		float fPOVScale =				utl::findValue(obj, "firstPOVScale").get_real();
		glm::vec3 fPOVOffset =			utl::findVec3(obj, "firstPOVOffset");

		float thirdPOVScale =			utl::findValue(obj, "thirdPOVScale").get_real();
		glm::vec3 thirdPOVOffset =		utl::findVec3(obj, "thirdPOVOffset");

		string modelFileName =			modelPath + utl::findValue(obj, "model").get_str();		
		ImportedModel* model = new ImportedModel(modelFileName, ModelType::unspecified);

		m_weaponDatas[nameEnum] = { name, 
									nameEnum,
									slotEnum,
									damage, 
									magazineCapacity, 
									maxMagazineCount, 
									m_rpm,
									modelScale, 
									fPOVScale,
									fPOVOffset,
									thirdPOVScale,
									thirdPOVOffset,
									model };
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
Model* ModelManager::createXYZAxisModel()
{

}

Model* ModelManager::createQuadModel()
{

}
Model* ModelManager::cubeWireFrameModel()
{

}

Model* ModelManager::cubeModel()
{

}
*/



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
