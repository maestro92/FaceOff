#include "renderer.h"
#include "scene_light_render_helper.h"


/*
SceneLightRenderHelper::SceneLightRenderHelper(Renderer* r)
{
	init(r);
}
*/

void SceneLightRenderHelper::init(Renderer* r)
{
	m_renderer = r;

	dirLightUniLocStartIndex = m_renderer->m_dataPairs.size();
	for (unsigned int i = 0; i < global.lightMgr->getNumDirLight(); i++)
	{
		addDirLightUniLocs();
	}

	pointLightUniLocStartIndex = m_renderer->m_dataPairs.size();
	for (unsigned int i = 0; i < global.lightMgr->getNumPointLight(); i++)
	{
		addPointLightUniLocs();
	}

	spotLightUniLocStartIndex = m_renderer->m_dataPairs.size();
	for (unsigned int i = 0; i < global.lightMgr->getNumSpotLight(); i++)
	{
		addSpotLightUniLocs();
	}
}


void SceneLightRenderHelper::addDirLightUniLocs()
{
	m_renderer->addDataPair("u_dirLight.base.color", DP_VEC3);
	m_renderer->addDataPair("u_dirLight.base.ambientIntensity", DP_FLOAT);
	m_renderer->addDataPair("u_dirLight.base.diffuseIntensity", DP_FLOAT);
	m_renderer->addDataPair("u_dirLight.direction", DP_VEC3);
}

void SceneLightRenderHelper::addPointLightUniLocs()
{

}

void SceneLightRenderHelper::addSpotLightUniLocs()
{

}


void SceneLightRenderHelper::setDirLightsData(const vector<DirectionalLight>& lights)
{
	for (int i = 0; i < lights.size(); i++)
	{
		auto light = lights[i];
		int index = dirLightUniLocStartIndex + i * NUM_DIR_LIGHT_UNILOCS;
		setDirLightData(light, index);
	}
}


void SceneLightRenderHelper::setPointLightsData(const vector<PointLight>& lights)
{

}

void SceneLightRenderHelper::setSpotLightsData(const vector<SpotLight>& lights)
{

}



void SceneLightRenderHelper::setDirLightData(const DirectionalLight& light, int lightIndex)
{
	m_renderer->setData(lightIndex, light.color);
	m_renderer->setData(lightIndex + 1, light.ambientIntensity);
	m_renderer->setData(lightIndex + 2, light.diffuseIntensity);
	m_renderer->setData(lightIndex + 3, light.direction);
}

void SceneLightRenderHelper::setPointLightData(const PointLight& light, int lightIndex)
{

}

void SceneLightRenderHelper::setSpotLightData(const SpotLight& light, int lightIndex)
{

}


