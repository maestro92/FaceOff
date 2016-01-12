#ifndef VERTEX_DATA_H_
#define VERTEX_DATA_H_

#include "define.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

struct VertexData
{
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec3 m_color;
    glm::vec2 m_UV;
};

struct TextureData
{
    GLuint m_id;

    TextureData()
    {
        m_id = -1;
    }
};


#endif // EG_VERTEX_DATA
