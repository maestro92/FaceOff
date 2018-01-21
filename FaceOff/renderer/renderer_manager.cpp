
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


	Renderer::initRendererWrapper(vArray, &r_sceneTexture, "r_sceneTexture");
	r_sceneTexture.lightHelper = new SceneLightRenderHelper();
	r_sceneTexture.lightHelper->init(&r_sceneTexture);
	r_sceneTexture.enableShader();
		r_sceneTexture.lightHelper->setDirLightsData(global.lightMgr->getDirLights());
		r_sceneTexture.lightHelper->setPointLightsData(global.lightMgr->getPointLights());
		r_sceneTexture.lightHelper->setSpotLightsData(global.lightMgr->getSpotLights());
	r_sceneTexture.disableShader();


	Renderer::initRendererWrapper(vArray, &r_occlusionMap, "r_occlusionMap");

	Renderer::initRendererWrapper(vArray, &r_volLightScattering, "r_volLightScattering");


	Renderer::initRendererWrapper(vArray, &r_sceneTextureWithShadowPass1, "r_sceneTextureWithShadowPass1");

	Renderer::initRendererWrapper(vArray, &r_sceneTextureWithShadowPass2, "r_sceneTextureWithShadowPass2");
	r_sceneTextureWithShadowPass2.lightHelper = new SceneLightRenderHelper();
	r_sceneTextureWithShadowPass2.lightHelper->init(&r_sceneTextureWithShadowPass2);
	r_sceneTextureWithShadowPass2.enableShader();
		r_sceneTextureWithShadowPass2.lightHelper->setDirLightsData(global.lightMgr->getDirLights());
		r_sceneTextureWithShadowPass2.lightHelper->setPointLightsData(global.lightMgr->getPointLights());
		r_sceneTextureWithShadowPass2.lightHelper->setSpotLightsData(global.lightMgr->getSpotLights());
	r_sceneTextureWithShadowPass2.disableShader();

	// r_dynamicModel
	Renderer::initRendererWrapper(vArray, &r_dynamicModel, "r_dynamicModel");
	r_dynamicModel.animationHelper = new AnimationModelRenderHelper();
	r_dynamicModel.animationHelper->init(&r_dynamicModel);
	
	// r_dynamicModelWithShadowPass1
	Renderer::initRendererWrapper(vArray, &r_dynamicModelWithShadowPass1, "r_dynamicModelWithShadowPass1");
	r_dynamicModelWithShadowPass1.animationHelper = new AnimationModelRenderHelper();
	r_dynamicModelWithShadowPass1.animationHelper->init(&r_dynamicModelWithShadowPass1);
	

	// r_dynamicModelWithShadowPass2
	Renderer::initRendererWrapper(vArray, &r_dynamicModelWithShadowPass2, "r_dynamicModelWithShadowPass2");
	r_dynamicModelWithShadowPass2.animationHelper = new AnimationModelRenderHelper();
	r_dynamicModelWithShadowPass2.animationHelper->init(&r_dynamicModelWithShadowPass2);

	r_dynamicModelWithShadowPass2.lightHelper = new SceneLightRenderHelper();
	r_dynamicModelWithShadowPass2.lightHelper->init(&r_dynamicModelWithShadowPass2);
	r_dynamicModelWithShadowPass2.enableShader();
		r_dynamicModelWithShadowPass2.lightHelper->setDirLightsData(global.lightMgr->getDirLights());
		r_dynamicModelWithShadowPass2.lightHelper->setPointLightsData(global.lightMgr->getPointLights());
		r_dynamicModelWithShadowPass2.lightHelper->setSpotLightsData(global.lightMgr->getSpotLights());
	r_dynamicModelWithShadowPass2.disableShader();
	
	

//	Renderer::initRendererWrapper(vArray, &r_occlusionMap, "r_occlusionMap");	


//	Renderer::initRendererWrapper(vArray, &r_volLightScattering, "r_volLightScattering");

	
	/*
	Renderer::initRendererWrapper(vArray, &r_sceneTexture, "r_sceneTexture");
	r_sceneTexture.init();
	r_sceneTexture.enableShader();
		r_sceneTexture.setDirLightsData(global.lightMgr->getDirLights());
		r_sceneTexture.setPointLightsData(global.lightMgr->getPointLights());
		r_sceneTexture.setSpotLightsData(global.lightMgr->getSpotLights());
	r_sceneTexture.disableShader();


	Renderer::initRendererWrapper(vArray, &r_sceneTextureWithShadowPass1, "r_sceneTextureWithShadowPass1");

	Renderer::initRendererWrapper(vArray, &r_sceneTextureWithShadowPass2, "r_sceneTextureWithShadowPass2");
	r_sceneTextureWithShadowPass2.init();
	r_sceneTextureWithShadowPass2.enableShader();
		r_sceneTextureWithShadowPass2.setDirLightsData(global.lightMgr->getDirLights());
		r_sceneTextureWithShadowPass2.setPointLightsData(global.lightMgr->getPointLights());
		r_sceneTextureWithShadowPass2.setSpotLightsData(global.lightMgr->getSpotLights());
	r_sceneTextureWithShadowPass2.disableShader();

	Renderer::initRendererWrapper(vArray, &r_dynamicModel, "r_dynamicModel");
	r_dynamicModel.init();

	Renderer::initRendererWrapper(vArray, &r_dynamicModelWithShadowPass1, "r_dynamicModelWithShadowPass1");
	r_dynamicModelWithShadowPass1.init();
	
	Renderer::initRendererWrapper(vArray, &r_dynamicModelWithShadowPass2, "r_dynamicModelWithShadowPass2");
	r_dynamicModelWithShadowPass2.init();
	*/




	/*
	r_fullVertexColor.printDataPairs();
	r_fullColor.printDataPairs();
	r_fullTexture.printDataPairs();
	r_playerTarget.printDataPairs();

	r_smokeEffectUpdate.printDataPairs();
	r_smokeEffectRender.printDataPairs();
	*/


	shadowMapWidth = utl::SCREEN_WIDTH * SHADOW_MAP_RATIO;
	shadowMapHeight = utl::SCREEN_HEIGHT * SHADOW_MAP_RATIO;

	initShadowMapFBO(shadowMapWidth, shadowMapHeight);
	// m_shadowMapFBO = utl::createFrameBufferObject(shadowMapWidth, shadowMapHeight);

	m_backGroundLayerFBO = utl::createFrameBufferObject(width, height);
	m_particleLayerFBO = utl::createFrameBufferObject(width, height);
	m_occlusionMapFBO = utl::createFrameBufferObject(width, height);
	
}


/*



void RendererManager::initShadowMapFBO(int w, int h)
{
	glGenTextures(1, &m_shadowMapFBO.depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapFBO.depthTexture);

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// Remove artifact on the edges of the shadowmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Create a FBO and attach the depth texture:
	glGenFramebuffers(1, &m_shadowMapFBO.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO.FBO);

	// Instruct openGL that we won't bind a color texture with the currently bound FBO
	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);

	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapFBO.depthTexture, 0);

	// check FBO status
	utl::errorCheck();

	GLenum FBOstatus;
	FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "GL_FRAMEBUFFER_COMPLETE failed. CANNOT use FBO" << endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// don't need the color channel, just the dpeth

}

*/
/*
void RendererManager::initOcclusionMapFBO()
{


}
*/

void RendererManager::initShadowMapFBO(int w, int h)
{
	glGenTextures(1, &m_shadowMapFBO.depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapFBO.depthTexture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Remove artifact on the edges of the shadowmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Create a FBO and attach the depth texture:
	glGenFramebuffers(1, &m_shadowMapFBO.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO.FBO);

	// Instruct openGL that we won't bind a color texture with the currently bound FBO
	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);

	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapFBO.depthTexture, 0);

	// check FBO status
	utl::errorCheck();

	GLenum FBOstatus;
	FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "GL_FRAMEBUFFER_COMPLETE failed. CANNOT use FBO" << endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// don't need the color channel, just the dpeth

}

/*
void RendererManager::initSceneRendererStaticLightsData(LightManager lightManager)
{

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

}
*/










