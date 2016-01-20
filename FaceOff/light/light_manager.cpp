#include "light_manager.h"

LightManager::LightManager()
{
	BaseLight* light;


}

LightManager::~LightManager()
{

}


vector<BaseLight*> LightManager::getLights()
{
	return m_lights;
}


BaseLight* LightManager::getLight(int index)
{
	return m_lights[index];
}		

