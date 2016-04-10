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


        Renderer		r_fullVertexColor;
		Renderer		r_fullColor;
		Renderer		r_fullTexture;
		Renderer		r_playerTarget;


		
		Renderer		r_healthBar;


		Renderer r_multiTexturedTerrain;
		SceneRenderer r_multiTexTerrain;
		SceneRenderer r_texturedObject;
		SceneRenderer r_billboardOneQuad;
		SceneRenderer r_billboardTwoQuad;


        void init();
		void initSceneRendererStaticLightsData(LightManager lightManager);
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