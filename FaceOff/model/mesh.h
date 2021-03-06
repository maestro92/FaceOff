#ifndef MESH_H_
#define MESH_H_

#include "vertex_data.h"

#include <GL/glew.h>

#include <vector>
#include <string>

using namespace std;

struct Mesh
{
    GLuint m_vbo;
    GLuint m_ind;
    int m_numIndices;
    unsigned int m_textureIndex;

    Mesh()
    {}

    Mesh(vector<VertexData>& Vertices, vector<unsigned int>& Indices) : Mesh(Vertices, Indices, -1)
    { }


    Mesh(vector<VertexData>& Vertices, vector<unsigned int>& Indices, unsigned int texIndex)
    {
        m_numIndices = Indices.size();

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &m_ind);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ind);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_numIndices, &Indices[0], GL_STATIC_DRAW);

        /// we unbind the buffers, so that no one accidentally unbind the buffers
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        m_textureIndex = texIndex;
    }


};



#endif // Mesh_H_
