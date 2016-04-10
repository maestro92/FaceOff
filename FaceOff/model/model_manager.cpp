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


	m_healthBar = new QuadModel(1, 1);



	m_xyzAxis = new XYZAxisModel();


}



