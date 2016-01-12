#ifndef IMPORTED_MODEL_H_
#define	IMPORTED_MODEL_H_


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
#include "texture.h"

#include "Model.h"

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






class ImportedModel : public Model
{

    public:
        ImportedModel();
        ImportedModel(string filename);
        ~ImportedModel();



        void transferDataToBuffer(vector<glm::vec3>& vec, unsigned int bufferIndex, unsigned int location);
        void transferDataToBuffer(vector<glm::vec2>& vec, unsigned int bufferIndex, unsigned int location);
        void transferDataToBuffer(vector<unsigned int>& vec, unsigned int bufferIndex);

        virtual bool load(string filename);
      //  void render();


        /// instead of having VB/IB (Vertex Buffer and Index Buffer) Objects, we
        /// have four buffers - index, position, normal and texture coordinates
        /// also a new member variable called m_VAO that stores the vertex array object
        const aiScene* m_scene;


    protected:
        bool initFromAiScene(const aiScene* pScene, const std::string& Filename);

        void initMesh(const aiMesh* m, const aiScene* s,
                      vector<glm::vec3>& positions,
                      vector<glm::vec3>& normals,
                      vector<glm::vec3>& colors,
                      vector<glm::vec2>& UVs,
                      vector<unsigned int>& Indices);

        void initMesh(unsigned int Index, const aiMesh* m, const aiScene* scene);

        void initVertexVectors(const aiMesh* m,
                          vector<glm::vec3>& positions,
                          vector<glm::vec3>& normals,
                          vector<glm::vec3>& colors,
                          vector<glm::vec2>& UVs,
                          glm::vec3 defaultColor);

        void initIndexVectors(const aiMesh* m, vector<unsigned int>& Indices);

        bool initMaterials(const aiScene* pScene, const std::string& Filename);

        bool initMaterials2(const aiScene* pScene, const std::string& Filename);
        void clear();
        glm::vec3 getMaterialColor(const aiMesh* m, const aiScene* s);


};


#endif // MESH37_H
