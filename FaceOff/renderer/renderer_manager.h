#ifndef RENDERER_MANAGER_H_
#define RENDERER_MANAGER_H_

#include "quad_model.h"
#include "renderer_constants.h"
#include "renderer.h"
#include "scene_renderer.h"
#include "light/light_manager.h"
class RendererManager
{
    public:
        RendererManager();
        ~RendererManager();

        Renderer		r_fullVertexColor;
		Renderer		r_fullColor;
		Renderer		r_fullTexture;
		Renderer		r_playerTarget;
		Renderer		r_skybox;

		
		Renderer		r_healthBar;

		Renderer		r_fireWorkEffectUpdate;
		Renderer		r_fireWorkEffectRender;

		Renderer		r_smokeEffectUpdate;
		Renderer		r_smokeEffectRender;


		Renderer		r_composite;

		
		Renderer r_multiTexturedTerrain;
		SceneRenderer r_multiTexTerrain;
		SceneRenderer r_texturedObject;
		SceneRenderer r_billboardOneQuad;
		SceneRenderer r_billboardTwoQuad;
		

		FrameBufferObject m_backGroundLayerFBO;
		FrameBufferObject m_particleLayerFBO;

		void init(int width, int height);

		/*
		void initRenderer(const Array arr, Renderer* r, string name);
		Object findRendererObject(const Array arr, string name);
		void initRenderer(const Object obj, Renderer* r);
		*/



		void initSceneRendererStaticLightsData(LightManager lightManager);

		
		// unordered_map<string, DATA_PAIR_TYPE> m_stringToDPTypeEnum;
};

#endif



/*

#ifndef RENDERER_MANAGER_H_
#define RENDERER_MANAGER_H_

#include "quad_model.h"
#include "renderer.h"
#include "scene_renderer.h"
#include "light/light_manager.h"
class RendererManager
{
public:
RendererManager();
~RendererManager();


static Renderer		r_fullVertexColor;
static Renderer		r_fullColor;
static Renderer		r_fullTexture;
static Renderer		r_playerTarget;



Renderer r_healthBar;


static Renderer r_multiTexturedTerrain;
static SceneRenderer r_multiTexTerrain;
static SceneRenderer r_texturedObject;
static SceneRenderer r_billboardOneQuad;
static SceneRenderer r_billboardTwoQuad;


static void init();
static void initSceneRendererStaticLightsData(LightManager lightManager);
};

#endif



*/