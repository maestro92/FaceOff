
#include "renderer_manager.h"

/*
Renderer RendererManager::r_fullVertexColor;
Renderer RendererManager::r_fullColor;
Renderer RendererManager::r_fullTexture;
Renderer RendererManager::r_playerTarget;


SceneRenderer RendererManager::r_multiTexTerrain;
SceneRenderer RendererManager::r_texturedObject;
SceneRenderer RendererManager::r_billboardOneQuad;
SceneRenderer RendererManager::r_billboardTwoQuad;
*/


RendererManager::RendererManager()
{

}

RendererManager::~RendererManager()
{

}


void RendererManager::init()
{
    Shader* s;

    s = new Shader("full_vertex_color.vs", "full_vertex_color.fs");
    r_fullVertexColor.addShader(s);

    s = new Shader("full_color.vs", "full_color.fs");
    r_fullColor.addShader(s);
    r_fullColor.addDataPair("u_color",  DP_VEC3);

    s = new Shader("full_texture.vs", "full_texture.fs");
    r_fullTexture.addShader(s);
    r_fullTexture.addDataPair("u_texture", DP_INT);

	s = new Shader("player_target.vs", "player_target.fs");
	r_playerTarget.addShader(s);
	r_playerTarget.addDataPair("u_texture", DP_INT);


	Renderer r_healthBar;

	
	s = new Shader("particle_effect_update.vs", "particle_effect_update.gs", "particle_effect_update.fs");
	r_particleEffectUpdate.addShader(s);

	const GLchar* varyings[4];
	varyings[0] = "type1";
	varyings[1] = "position1";
	varyings[2] = "velocity1";
	varyings[3] = "age1";

	glTransformFeedbackVaryings(s->getProgramId(), 4, varyings, GL_INTERLEAVED_ATTRIBS);

	s->linkShader();
	s->useShader();
	r_particleEffectUpdate.addDataPair("u_deltaTimeMillis", DP_FLOAT);
	r_particleEffectUpdate.addDataPair("u_time", DP_FLOAT);
	r_particleEffectUpdate.addDataPair("u_randomTexture", DP_INT);
	r_particleEffectUpdate.addDataPair("u_launcherLifeTime", DP_FLOAT);
	r_particleEffectUpdate.addDataPair("u_shellLifeTime", DP_FLOAT);
	r_particleEffectUpdate.addDataPair("u_secondaryShellLifeTime", DP_FLOAT);


	r_particleEffectUpdate.enableShader();
		r_particleEffectUpdate.setData("u_launcherLifeTime", 100.0f);
		r_particleEffectUpdate.setData("u_shellLifeTime", 10000.0f);
		r_particleEffectUpdate.setData("u_secondaryShellLifeTime", 100.0f);
	r_particleEffectUpdate.disableShader();




	s = new Shader("particle_effect_render.vs", "particle_effect_render.gs", "particle_effect_render.fs");
	r_particleEffectRender.addShader(s);
	r_particleEffectRender.addDataPair("u_texture", DP_INT);
	r_particleEffectRender.addDataPair("u_billBoardSize", DP_FLOAT);
	r_particleEffectRender.addDataPair("u_centerPosition", DP_VEC3);

	r_particleEffectRender.enableShader();
		r_particleEffectRender.setData("u_billBoardSize", 0.1f);
	r_particleEffectUpdate.disableShader();



	// scene renderers
	s = new Shader("scene_shaders/multitexture_terrain.vs", "scene_shaders/multitexture_terrain.fs");
	r_multiTexTerrain.addShader(s);
	r_multiTexTerrain.addDataPair("u_blendMapTexture", DP_INT);
	r_multiTexTerrain.addDataPairArray("u_textures", DP_INT, 4);

	s = new Shader("scene_shaders/textured_object.vs", "scene_shaders/textured_object.fs");
	r_texturedObject.addShader(s);
	r_texturedObject.addDataPair("u_texture", DP_INT);

	s = new Shader("scene_shaders/billboarding.vs", 
				   "scene_shaders/billboarding_one_quad.gs", 
				   "scene_shaders/billboarding.fs");
	r_billboardOneQuad.addShader(s);
	r_billboardOneQuad.addDataPair("u_texture", DP_INT);
	r_billboardOneQuad.addDataPair("u_centerPosition", DP_VEC3);
	r_billboardOneQuad.addDataPair("u_billboardWidthScale", DP_FLOAT);
	r_billboardOneQuad.addDataPair("u_billboardHeightScale", DP_FLOAT);

	s = new Shader("scene_shaders/billboarding.vs", 
				   "scene_shaders/billboarding_two_quad.gs", 
				   "scene_shaders/billboarding.fs");
	r_billboardTwoQuad.addShader(s);
	r_billboardTwoQuad.addDataPair("u_texture", DP_INT);
	r_billboardTwoQuad.addDataPair("u_centerPosition", DP_VEC3);
	r_billboardTwoQuad.addDataPair("u_billboardWidthScale", DP_FLOAT);
	r_billboardTwoQuad.addDataPair("u_billboardHeightScale", DP_FLOAT);


	char* filename = "Assets/renderer_data.json";

}

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