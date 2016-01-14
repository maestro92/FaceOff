
#include "renderer_manager.h"

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

}

