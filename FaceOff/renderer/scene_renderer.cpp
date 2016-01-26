
#include "scene_renderer.h"

void SceneRenderer::addShader(Shader* s)
{
	Renderer::addShader(s);

	addDirLightUniLocs();
	addPointLightUniLocs();
	addSpotLightUniLocs();
}


bool SceneRenderer::addDirLightUniLocs()
{
	addDataPair("u_dirLight.base.color", DP_VEC3);
	addDataPair("u_dirLight.base.ambientIntensity", DP_FLOAT);
	addDataPair("u_dirLight.base.diffuseIntensity", DP_FLOAT);
	addDataPair("u_dirLight.direction", DP_VEC3);

	return true;
}

bool SceneRenderer::addPointLightUniLocs()
{
	return true;
}

bool SceneRenderer::addSpotLightUniLocs()
{
	return true;
}


void SceneRenderer::setDirLightData(DirectionalLight light)
{
	setData("u_dirLight.base.color", light.color);
	setData("u_dirLight.base.ambientIntensity", light.ambientIntensity);
	setData("u_dirLight.base.diffuseIntensity", light.diffuseIntensity);
	setData("u_dirLight.direction", light.direction);
}

void SceneRenderer::setPointLightData(PointLight light)
{

}

void SceneRenderer::setSpotLightData(SpotLight light)
{

}

void SceneRenderer::setDirLightsData(vector<DirectionalLight> lights)
{

}

void SceneRenderer::setPointLightsData(vector<PointLight> lights)
{

}

void SceneRenderer::setSpotLightsData(vector<SpotLight> lights)
{

}

