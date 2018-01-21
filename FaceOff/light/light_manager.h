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

		void init();
		
		vector<DirectionalLight>& getDirLights();
		DirectionalLight& getDirLight(int index);
		int getNumDirLight();

		vector<PointLight>& getPointLights();
		PointLight& getPointLight(int index);
		int getNumPointLight();
		
		vector<SpotLight>& getSpotLights();
		SpotLight& getSpotLight(int index);
		int getNumSpotLight();

	private:

		vector<DirectionalLight> m_dirLights;
		vector<PointLight> m_pointLights;
		vector<SpotLight> m_spotLights;

};


#endif