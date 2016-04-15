#ifndef MODEL_MANAGER_H_
#define MODEL_MANAGER_H_

using namespace std;

#include <unordered_map>



#include "model.h"
#include "quad_model.h"
#include "imported_model.h"
#include "xyz_axis_model.h"

#include "weapon_enum.h"

class ModelManager
{
	public:
	
		ModelManager();
		~ModelManager();


	

		void init();

		Model* m_player;
		Model* m_healthBar;
		Model* m_xyzAxis;

		Model* m_tree;

		unordered_map<WeaponNameEnum, Model*> m_weapons;

};



#endif