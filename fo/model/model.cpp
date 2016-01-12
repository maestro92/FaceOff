
#include <assert.h>
#include "Model.h"

using namespace std;



Model::Model()
{
    m_modelGeometry = GL_TRIANGLES;
}


Model::~Model()
{
    clear();
}


void Model::clear()
{

}


void Model::addMesh(EG_Mesh m)
{
    m_meshes.push_back(m);
}


void Model::render()
{
    glEnableVertexAttribArray(POSITION);
    glEnableVertexAttribArray(NORMAL);
    glEnableVertexAttribArray(COLOR);
    glEnableVertexAttribArray(UV);

    /// this means we're going to use these two buffer object
    for (int i=0; i<m_meshes.size(); i++)
    {
        EG_Mesh m = m_meshes[i];

        glBindBuffer(GL_ARRAY_BUFFER, m.m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.m_ind);

        glVertexAttribPointer(POSITION,   3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
        glVertexAttribPointer(NORMAL,   3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(3*sizeof(float)));
        glVertexAttribPointer(COLOR,    3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(6*sizeof(float)));
        glVertexAttribPointer(UV,       2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(9*sizeof(float)));


        const unsigned int texIndex = m_meshes[i].m_textureIndex;


        if (texIndex < m_textures.size() && m_textures[texIndex].m_id != -1)
        {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textures[texIndex].m_id);
        }


        /// glDrawElements, you need to supply an index buffer
        /// glDrawArrays submits the vertices in linear order
        glDrawElements(m_modelGeometry, m.m_numIndices, GL_UNSIGNED_INT, 0);
    }


    glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glDisableVertexAttribArray(POSITION);
	glDisableVertexAttribArray(NORMAL);
	glDisableVertexAttribArray(COLOR);
	glDisableVertexAttribArray(UV);

}

