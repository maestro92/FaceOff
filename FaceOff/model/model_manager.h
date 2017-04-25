#ifndef MODEL_MANAGER_H_
#define MODEL_MANAGER_H_

using namespace std;

#include <unordered_map>
#include "model_enum.h"

#include "utility.h"

#include "model.h"
#include "quad_model.h"
#include "imported_model.h"
#include "animated_model.h"
#include "xyz_axis_model.h"


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

// to resolve circular depenency issue
class Weapon;

class ModelManager
{


	public:

		ModelManager();
		~ModelManager();


		void init();
		void shutDown();

		void writeWeaponJsonFiles();
		void readWeaponData(const mObject& obj);
		void initWeaponData(WeaponData& data);
		void initWeaponsData();
		Weapon createWeapon(WeaponNameEnum name);

		



	//	const mValue& findValue(const mObject& obj, const string& name);
	//	glm::vec3 findVec3(const mObject& obj, const string& name);

		// unordered_map<string, WeaponTypeEnum> m_weaponTypeToEnum;
		unordered_map<string, WeaponSlotEnum> m_weaponSlotToEnum;
		unordered_map<string, WeaponNameEnum> m_weaponNameToEnum;

		WeaponData getWeaponData(WeaponNameEnum name);

		unordered_map<WeaponNameEnum, WeaponData> m_weaponDatas;

		Model* get(int modelEnum);


		Model* getOneBuiltInModel();

	private:
		Model* m_player;
		Model* m_healthBar;
		Model* m_xyzAxis;
		Model* m_tree;
		Model* m_cube;
		Model* m_ground;
		Model* m_woodenBox;
		Model* m_legoMan;

		// Model* m_animatedLegoMan;
		
		AnimatedModel m_animatedLegoMan;
		Model* m_animatedLampBob;


		Model* createXYZAxisModel();
		Model* createQuadModel();
		Model* cubeWireFrameModel();
		Model* cubeModel();

		vector<Model*> m_models;
		

//		vector<Model> m_models;


/*
		XYZAxisModel    m_xyzModel;
		ImportedModel	m_bulletModel;

		ImportedModel	m_lowPolyTree;
		ImportedModel	m_stairs;
		ImportedModel	m_woodenBox;
		*/


};



#endif