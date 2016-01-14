#include "imported_model.h"

ImportedModel::ImportedModel()
{

}

ImportedModel::ImportedModel(string filename)
{
    load(filename);
}

ImportedModel::~ImportedModel()
{

}

bool ImportedModel::load(string filename)
{
    bool ret = false;

	const aiScene* scene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (scene)
        ret = initFromAiScene(scene, filename);
    else
        cout << "Error parsing '"<< filename.c_str() << "': '" << aiGetErrorString() << endl;


    return ret;
}


bool ImportedModel::initFromAiScene(const aiScene* scene, const string& filename)
{
    m_meshes.resize(scene->mNumMeshes);

    /// for some reason Assimp always creates one extra mNumMaterials
    m_textures.resize(scene->mNumMaterials);

    for (unsigned int i=0; i<m_meshes.size(); i++)
    {
        const aiMesh* mesh = scene->mMeshes[i];
        initMesh(i, mesh, scene);
    }


    bool b = initMaterials2(scene, filename);


    Utility::debug("m_meshes size", m_meshes.size());
    Utility::debug("m_textures size", m_textures.size());
    for(int i=0; i<m_meshes.size(); i++)
    {
        Utility::debug("mesh texture Index", m_meshes[i].m_textureIndex);
    }

    for(int i=0; i<m_textures.size(); i++)
    {
        Utility::debug("textures Id", m_textures[i].m_id);
    }


    return b;
}




void ImportedModel::initMesh(unsigned int index, const aiMesh* m, const aiScene* scene)
{
    vector<VertexData> Vertices;
    vector<unsigned int> Indices;

	aiColor4D col;
	aiMaterial* mat = scene->mMaterials[m->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	glm::vec3 defaultColor(col.r,col.g,col.b);

    for (unsigned int i = 0 ; i < m->mNumVertices ; i++)
    {
        VertexData v;

        /// position
        v.m_position = Utility::toGlmVec(m->mVertices[i]);

        /// normal
        v.m_normal = Utility::toGlmVec(m->mNormals[i]);

        /// colors
        if(m->mColors[0])
            v.m_color = Utility::toGlmVec(m->mColors[0][i]);
        else
            v.m_color = defaultColor;

        /// UV
        if(m->mTextureCoords[0])
            v.m_UV = glm::vec2(m->mTextureCoords[0][i].x, m->mTextureCoords[0][i].y);
        else
            v.m_UV = glm::vec2(0.0f, 0.0f);

        Vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < m->mNumFaces ; i++)
    {
        const aiFace& Face = m->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_meshes[index] = Mesh(Vertices, Indices, m->mMaterialIndex);
}




bool ImportedModel::initMaterials2(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos)
        Dir = ".";
    else if (SlashIndex == 0)
        Dir = "/";
    else
        Dir = Filename.substr(0, SlashIndex);


    bool Ret = true;

    string fullPath;
    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        bool success = false;

        Utility::debug("i", i);
        Utility::debug("aiTextureType_NONE", pMaterial->GetTextureCount(aiTextureType_NONE));

        Utility::debug("aiTextureType_DIFFUSE", pMaterial->GetTextureCount(aiTextureType_DIFFUSE));
        Utility::debug("aiTextureType_SPECULAR", pMaterial->GetTextureCount(aiTextureType_SPECULAR));
        Utility::debug("aiTextureType_AMBIENT", pMaterial->GetTextureCount(aiTextureType_AMBIENT));
        Utility::debug("aiTextureType_EMISSIVE", pMaterial->GetTextureCount(aiTextureType_EMISSIVE));
        Utility::debug("aiTextureType_NORMALS", pMaterial->GetTextureCount(aiTextureType_NORMALS));
        Utility::debug("aiTextureType_DIFFUSE", pMaterial->GetTextureCount(aiTextureType_DIFFUSE));
        Utility::debug("aiTextureType_UNKNOWN", pMaterial->GetTextureCount(aiTextureType_UNKNOWN));


        /// we're only intersted in teh diffuse texture
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                fullPath = Dir + "/" + path.data;

                Utility::debug("fullPath", fullPath);
                m_textures[i].m_id = Utility::loadTexture(fullPath);

                success = true;
            }
        }

        Utility::debug(fullPath, m_textures[i].m_id);
        if (!success)
            m_textures[i].m_id = Utility::loadTexture("Assets/models/weapons/Ak_47/ak-47.jpg");
//            m_textures[i].m_id = Utility::loadTexture("Assets/models/weapons/M9/Tex_0009_1.jpg");
        Utility::debugLn("final: " + fullPath, m_textures[i].m_id);
    }

    return Ret;
}




bool ImportedModel::initMaterials(const aiScene* pScene, const std::string& Filename)
{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos)
        Dir = ".";
    else if (SlashIndex == 0)
        Dir = "/";
    else
        Dir = Filename.substr(0, SlashIndex);


    bool Ret = true;

    string fullPath;
    // Initialize the materials
    for (unsigned int i = 1 ; i < pScene->mNumMaterials ; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        bool success = false;
/*
        Utility::debug("i", i);
        Utility::debug("aiTextureType_DIFFUSE", pMaterial->GetTextureCount(aiTextureType_DIFFUSE));
*/
        /// we're only intersted in teh diffuse texture
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                fullPath = Dir + "/" + path.data;

                Utility::debug("fullPath", fullPath);
                m_textures[i-1].m_id = Utility::loadTexture(fullPath);

                success = true;
            }
        }

        Utility::debug(fullPath, m_textures[i-1].m_id);
        // Load a white texture in case the model does not include its own texture
        if (!success)
            m_textures[i].m_id = Utility::loadTexture("Assets/Images/bricks.jpg");

        Utility::debugLn("final: " + fullPath, m_textures[i-1].m_id);


    }

    return Ret;
}
