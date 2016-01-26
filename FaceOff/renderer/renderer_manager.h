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

		static Renderer r_multiTexturedTerrain;
		
		
		static SceneRenderer r_multiTexTerrain;
		static SceneRenderer r_texturedObject;
		static SceneRenderer r_billboardOneQuad;
		static SceneRenderer r_billboardTwoQuad;


        static void init();
		static void initSceneRendererStaticLightsData(LightManager lightManager);
};

#endif

