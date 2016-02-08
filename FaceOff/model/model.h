#ifndef MODEL_H_
#define	MODEL_H_


#define NO_SDL_GLEXT
#include <GL/glew.h>

#include "define.h"
#include "utility.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "bounding_volume.h"
#include "texture.h"

using namespace std;

#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_GL_VALUE -1

#define POS_VB 0
#define NORMAL_VB 1
#define TANGENT_VB 2
#define COLOR_VB 3
#define UV_VB 4
#define INDEX_BUFFER 5
#define BONE_VB 6

#define POS_UNI_LOC 0
#define NORMAL_UNI_LOC 1
#define TANGENT_UNI_LOC 2
#define COLOR_UNI_LOC 3
#define UV_UNI_LOC 4
#define BONE_ID_UNI_LOC 5
#define BONE_WEIGHT_UNI_LOC 6


/*
http://gamedev.stackexchange.com/questions/19560/what-is-the-best-way-to-store-meshes-or-3d-models-in-a-class

This basically means that the models are stored in GPU memory instead of CPU memory, minimizing GPU to CPU transfers and greatly improving rendering performance.
*/

#include "Mesh.h"

class Model
{
    public:
        enum
        {
            POSITION = 0,
            NORMAL,
            COLOR,
            UV,
        };

        Model();
        ~Model();

        void addMesh(Mesh m);
		void setTextures(vector<string> textureFiles);
		void setMeshRandTextureIdx();
		virtual void render();

        void clear();
        GLuint m_modelGeometry;


		// glm::vec3 m_maxP, m_minP;

		AABB m_aabb;

        vector<Mesh> m_meshes;
        vector<TextureData> m_textures;
};


#endif // MESH37_H
