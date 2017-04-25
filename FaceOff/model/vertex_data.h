#ifndef VERTEX_DATA_H_
#define VERTEX_DATA_H_

#include "define.h"

#define NO_SDL_GLEXT
#include <GL/glew.h>

const int NUM_BONES_PER_VERTEX = 4;

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


// with bones, for animated_models
struct VertexData2
{
	glm::vec3 position;	//0
	glm::vec3 normal;	//3
	glm::vec3 color;	//6
	glm::vec2 UV;		//9
	unsigned int boneIds[NUM_BONES_PER_VERTEX];	//11
	float weights[NUM_BONES_PER_VERTEX];		//15


	VertexData2()
	{
		memset(boneIds, 0, sizeof(boneIds));
		memset(weights, 0, sizeof(weights));
	}

	void addBone(unsigned int boneId, float weight)
	{
		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			if (weights[i] < 0.00001)
			{
				boneIds[i] = boneId;
				weights[i] = weight;
				return;
			}
		}
	}

	void print()
	{
		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			cout << "boneId " << boneIds[i] << ", weight " << weights[i] << endl;
		}

	}

};


#endif // EG_VERTEX_DATA
