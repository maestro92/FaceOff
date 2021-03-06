#ifndef LIGHT_MANAGER_H_
#define LIGHT_MANAGER_H_


#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include "define.h"

using namespace std;

const static float SPECULAR_INTENSITY = 1.0f;
const static float SPECULAR_POWER = 32.0f;

#include "base_light.h"

class LightManager
{
	public:
		LightManager(); 
		~LightManager();


//		vector<BaseLight*> getLights();
//		BaseLight* getLight(int index);

		DirectionalLight getDirLight(int index);
		PointLight getPointLight(int index);
		SpotLight getSpotLight(int index);

	private:

		vector<DirectionalLight> m_dirLights;
		vector<PointLight> m_pointLights;
		vector<SpotLight> m_spotLights;

//		vector<BaseLight*> m_lights;
		int m_dirLightCount;
		int m_pointLightCount;
		int m_spotLightCount;
};


#endif