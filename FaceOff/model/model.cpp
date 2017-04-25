
#include <assert.h>
#include "model.h"

using namespace std;



Model::Model()
{
	m_isAnimated = false;
    m_modelGeometry = GL_TRIANGLES;

	m_aabb.max = glm::vec3(1.0);
	m_aabb.min = glm::vec3(-1.0);
}


Model::~Model()
{
    clear();
}


void Model::clear()
{

}


void Model::addMesh(Mesh m)
{
    m_meshes.push_back(m);
}


void Model::enableVertexAttribArrays()
{
	glEnableVertexAttribArray(POSITION_VERTEX_ATTRIB);
	glEnableVertexAttribArray(NORMAL_VERTEX_ATTRIB);
	glEnableVertexAttribArray(COLOR_VERTEX_ATTRIB);
	glEnableVertexAttribArray(UV_VERTEX_ATTRIB);
	glEnableVertexAttribArray(BONE_IDS_ATTRIB);
	glEnableVertexAttribArray(BONE_WEIGHTS_ATTRIB);
}



void Model::disableVertexAttribArrays()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(POSITION_VERTEX_ATTRIB);
	glDisableVertexAttribArray(NORMAL_VERTEX_ATTRIB);
	glDisableVertexAttribArray(COLOR_VERTEX_ATTRIB);
	glDisableVertexAttribArray(UV_VERTEX_ATTRIB);
	glDisableVertexAttribArray(BONE_IDS_ATTRIB);
	glDisableVertexAttribArray(BONE_WEIGHTS_ATTRIB);
}


void Model::render()
{
    /// this means we're going to use these two buffer object
    for (int i=0; i<m_meshes.size(); i++)
    {
        Mesh m = m_meshes[i];

        glBindBuffer(GL_ARRAY_BUFFER, m.m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.m_ind);

        glVertexAttribPointer(POSITION_VERTEX_ATTRIB,   3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
        glVertexAttribPointer(NORMAL_VERTEX_ATTRIB,   3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(3*sizeof(float)));
        glVertexAttribPointer(COLOR_VERTEX_ATTRIB,    3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(6*sizeof(float)));
        glVertexAttribPointer(UV_VERTEX_ATTRIB,       2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(9*sizeof(float)));


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
}



void Model::renderSingle()
{
	enableVertexAttribArrays();
	render();
	/*
		/// this means we're going to use these two buffer object
	for (int i = 0; i<m_meshes.size(); i++)
	{
		Mesh m = m_meshes[i];

		glBindBuffer(GL_ARRAY_BUFFER, m.m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.m_ind);

		glVertexAttribPointer(POSITION_VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
		glVertexAttribPointer(NORMAL_VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(COLOR_VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(6 * sizeof(float)));
		glVertexAttribPointer(UV_VERTEX_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(9 * sizeof(float)));


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
	*/
	disableVertexAttribArrays();
}


void Model::setTextures(vector<string> textureFiles)
{
	m_textures.clear();
	for (int i = 0; i < textureFiles.size(); i++)
	{
		TextureData tex;
		tex.m_id = utl::loadTexture(textureFiles[i], GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true);
		m_textures.push_back(tex);
	}
}


void Model::setMeshRandTextureIdx()
{
	int texCount = m_textures.size();

	if (texCount <= 0)
		return;

	for (int i = 0; i < m_meshes.size(); i++)
		m_meshes[i].m_textureIndex = 0;
	
}

bool Model::isAnimated()
{
	return m_isAnimated;
}

