#include "light_manager.h";

LightManager::LightManager()
{
	/*
	DirectionalLight light = DirectionalLight();
	light.color = WHITE;
	light.ambientIntensity = 0.5;
	light.diffuseIntensity = 0.9;

	m_dirLights.push_back(light);
	*/
}

LightManager::~LightManager()
{

}



void LightManager::init()
{
	DirectionalLight dirLight = DirectionalLight();
	dirLight.ambientIntensity = 0.05f;
	dirLight.color = COLOR_WHITE;
	dirLight.diffuseIntensity = 0.5;
//	dirLight.direction = glm::vec3(19.1004, -28.881, -40.5246) * 2.0f;
	dirLight.direction = glm::normalize(glm::vec3(1, -1, 1));
	m_dirLights.push_back(dirLight);
}

vector<DirectionalLight>& LightManager::getDirLights()
{
	return m_dirLights;
}

vector<PointLight>& LightManager::getPointLights()
{
	return m_pointLights;
}

vector<SpotLight>& LightManager::getSpotLights()
{
	return m_spotLights;
}

int LightManager::getNumDirLight()
{
	return m_dirLights.size();
}

int LightManager::getNumPointLight()
{
	return m_pointLights.size();
}

int LightManager::getNumSpotLight()
{
	return m_spotLights.size();
}

DirectionalLight& LightManager::getDirLight(int index)
{
	return m_dirLights[index];
}

PointLight& LightManager::getPointLight(int index)
{
	return m_pointLights[index];
}

SpotLight& LightManager::getSpotLight(int index)
{
	return m_spotLights[index];
}
