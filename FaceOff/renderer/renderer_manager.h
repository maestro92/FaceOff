#ifndef RENDERER_MANAGER_H_
#define RENDERER_MANAGER_H_

#include "quad_model.h"
#include "renderer.h"


class RendererManager
{
    public:
        RendererManager();
        ~RendererManager();

        static Renderer    r_fullVertexColor;
		static Renderer    r_fullColor;
		static Renderer    r_fullTexture;

        static void init();
};

#endif

