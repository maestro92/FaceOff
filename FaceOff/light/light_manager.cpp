#include "light_manager.h"

LightManager::LightManager()
{
	DirectionalLight light = DirectionalLight();
	light.color = WHITE;
	light.ambientIntensity = 0.5;
	light.diffuseIntensity = 0.9;

	m_dirLights.push_back(light);
}

LightManager::~LightManager()
{

}


DirectionalLight LightManager::getDirLight(int index)
{
	return m_dirLights[index];
}

PointLight LightManager::getPointLight(int index)
{
	return m_pointLights[index];
}

SpotLight LightManager::getSpotLight(int index)
{
	return m_spotLights[index];
}
