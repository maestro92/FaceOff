
#include "renderer_manager.h"


RendererManager::RendererManager()
{

}

RendererManager::~RendererManager()
{

}


void RendererManager::init(int width, int height)
{
	char* filename = "renderer/renderer_data.json";

	Value vContent = utl::readJsonFileToVector(filename);
	const Array& vArray = vContent.get_array();

	Renderer::initRendererWrapper(vArray, &r_fullVertexColor, "r_fullVertexColor");
	Renderer::initRendererWrapper(vArray, &r_fullColor, "r_fullColor");
	Renderer::initRendererWrapper(vArray, &r_fullTexture, "r_fullTexture");
	Renderer::initRendererWrapper(vArray, &r_playerTarget, "r_playerTarget");
	Renderer::initRendererWrapper(vArray, &r_skybox, "r_skybox");
	Renderer::initRendererWrapper(vArray, &r_dynamicModel, "r_dynamicModel");
	r_dynamicModel.init();

	Renderer::initRendererWrapper(vArray, &r_smokeEffectUpdate, "r_smokeEffectUpdate");
	r_smokeEffectUpdate.enableShader();
		r_smokeEffectUpdate.setData(R_SMOKE_EFFECT_UPDATE::u_launcherLifeTime, 100.0f);
		r_smokeEffectUpdate.setData(R_SMOKE_EFFECT_UPDATE::u_shellLifeTime, 10000.0f);
		r_smokeEffectUpdate.setData(R_SMOKE_EFFECT_UPDATE::u_secondaryShellLifeTime, 1000.0f);
	r_smokeEffectUpdate.disableShader();


	Renderer::initRendererWrapper(vArray, &r_smokeEffectRender, "r_smokeEffectRender");
	r_smokeEffectRender.enableShader();
		r_smokeEffectRender.setData(R_SMOKE_EFFECT_RENDER::u_billBoardSize, 4.0f);
		r_smokeEffectRender.setData(R_SMOKE_EFFECT_RENDER::u_inverseScreenSize, glm::vec2(1.0f / utl::SCREEN_WIDTH, 1.0f / utl::SCREEN_HEIGHT));
		r_smokeEffectRender.setData(R_SMOKE_EFFECT_RENDER::u_zNear, utl::Z_NEAR);
		r_smokeEffectRender.setData(R_SMOKE_EFFECT_RENDER::u_zFar, utl::Z_FAR);
	r_smokeEffectRender.disableShader();


//	SceneRenderer r_sceneColor;
	Renderer::initRendererWrapper(vArray, &r_sceneTexture, "r_sceneTexture");
	r_sceneTexture.init();
	r_sceneTexture.enableShader();
		r_sceneTexture.setDirLightsData(global.lightMgr->getDirLights());
		r_sceneTexture.setPointLightsData(global.lightMgr->getPointLights());
		r_sceneTexture.setSpotLightsData(global.lightMgr->getSpotLights());
	r_sceneTexture.disableShader();


	/*
	r_fullVertexColor.printDataPairs();
	r_fullColor.printDataPairs();
	r_fullTexture.printDataPairs();
	r_playerTarget.printDataPairs();

	r_smokeEffectUpdate.printDataPairs();
	r_smokeEffectRender.printDataPairs();
	*/

	m_backGroundLayerFBO = utl::createFrameBufferObject(width, height);
	m_particleLayerFBO = utl::createFrameBufferObject(width, height);
}

void RendererManager::initSceneRendererStaticLightsData(LightManager lightManager)
{
	/*
	r_multiTexTerrain.enableShader();
		r_multiTexTerrain.setDirLightData(lightManager.getDirLight(0));
	r_multiTexTerrain.disableShader();

	r_texturedObject.enableShader();
		r_texturedObject.setDirLightData(lightManager.getDirLight(0));
	r_texturedObject.disableShader();

	r_texturedObject.enableShader();
		r_billboardOneQuad.setDirLightData(lightManager.getDirLight(0));
	r_texturedObject.disableShader();

	r_texturedObject.enableShader();
		r_billboardTwoQuad.setDirLightData(lightManager.getDirLight(0));
	r_texturedObject.disableShader();
	*/
}











