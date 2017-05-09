/*
#include "scene_renderer.h"

void SceneRenderer::init()
{
	dirLightUniLocStartIndex = m_dataPairs.size();
	for (unsigned int i = 0; i < global.lightMgr->getNumDirLight(); i++)
	{
		addDirLightUniLocs();
	}

	pointLightUniLocStartIndex = m_dataPairs.size();
	for (unsigned int i = 0; i < global.lightMgr->getNumPointLight(); i++)
	{
		addPointLightUniLocs();
	}

	spotLightUniLocStartIndex = m_dataPairs.size();
	for (unsigned int i = 0; i < global.lightMgr->getNumSpotLight(); i++)
	{
		addSpotLightUniLocs();
	}
}


void SceneRenderer::addDirLightUniLocs()
{
	addDataPair("u_dirLight.base.color", DP_VEC3);
	addDataPair("u_dirLight.base.ambientIntensity", DP_FLOAT);
	addDataPair("u_dirLight.base.diffuseIntensity", DP_FLOAT);
	addDataPair("u_dirLight.direction", DP_VEC3);
}

void SceneRenderer::addPointLightUniLocs()
{

}

void SceneRenderer::addSpotLightUniLocs()
{

}


void SceneRenderer::setDirLightsData(const vector<DirectionalLight>& lights)
{
	for (int i = 0; i < lights.size(); i++)
	{
		auto light = lights[i];
		int index = dirLightUniLocStartIndex + i * NUM_DIR_LIGHT_UNILOCS;
		setDirLightData(light, index);
	}
}


void SceneRenderer::setPointLightsData(const vector<PointLight>& lights)
{

}

void SceneRenderer::setSpotLightsData(const vector<SpotLight>& lights)
{

}



void SceneRenderer::setDirLightData(const DirectionalLight& light, int lightIndex)
{
	setData(lightIndex, light.color);
	setData(lightIndex + 1, light.ambientIntensity);
	setData(lightIndex + 2, light.diffuseIntensity);
	setData(lightIndex + 3, light.direction);
}

void SceneRenderer::setPointLightData(const PointLight& light, int lightIndex)
{

}

void SceneRenderer::setSpotLightData(const SpotLight& light, int lightIndex)
{

}


*/

