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
	{
	//	m_minX = m_minY = m_minZ = INT_MAX;
	//	m_maxX = m_maxY = m_maxZ = INT_MIN;

		m_minP = glm::vec3(INT_MAX);
		m_maxP = glm::vec3(INT_MIN);

		cout << "Loading " << filename << endl;
		ret = initFromAiScene(scene, filename);
	}
	else
        cout << "Error parsing '"<< filename.c_str() << "': '" << aiGetErrorString() << endl;


    return ret;
}



bool ImportedModel::load(string filename, vector<string> textureFiles)
{
	bool ret = false;

	const aiScene* scene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

	if (scene)
	{

		m_minP = glm::vec3(INT_MAX);
		m_maxP = glm::vec3(INT_MIN);

		cout << "Loading " << filename << endl;
		ret = initFromAiScene(scene, filename);

	}
	else
		cout << "Error parsing '" << filename.c_str() << "': '" << aiGetErrorString() << endl;

	setTextures(textureFiles);
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

	
    utl::debug("m_meshes size", m_meshes.size());
    utl::debug("m_textures size", m_textures.size());
	/*
	for(int i=0; i<m_meshes.size(); i++)
    {
        utl::debug("mesh texture Index", m_meshes[i].m_textureIndex);
    }

    for(int i=0; i<m_textures.size(); i++)
    {
        utl::debug("textures Id", m_textures[i].m_id);
    }
	*/

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
        v.m_position = utl::toGlmVec(m->mVertices[i]);

		/*
		m_minX = min(m_minX, v.m_position.x);	m_maxX = max(m_maxX, v.m_position.x);
		m_minY = min(m_minY, v.m_position.y);	m_maxY = max(m_maxY, v.m_position.y);
		m_minZ = min(m_minZ, v.m_position.z);	m_maxZ = max(m_maxZ, v.m_position.z);
		*/
		m_minP.x = min(m_minP.x, v.m_position.x);	m_maxP.x = max(m_maxP.x, v.m_position.x);
		m_minP.y = min(m_minP.y, v.m_position.y);	m_maxP.y = max(m_maxP.y, v.m_position.y);
		m_minP.z = min(m_minP.z, v.m_position.z);	m_maxP.z = max(m_maxP.z, v.m_position.z);

		
        /// normal
        v.m_normal = utl::toGlmVec(m->mNormals[i]);

        /// colors
        if(m->mColors[0])
            v.m_color = utl::toGlmVec(m->mColors[0][i]);
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
		/*
        utl::debug("i", i);
        utl::debug("aiTextureType_NONE", pMaterial->GetTextureCount(aiTextureType_NONE));

        utl::debug("aiTextureType_DIFFUSE", pMaterial->GetTextureCount(aiTextureType_DIFFUSE));
        utl::debug("aiTextureType_SPECULAR", pMaterial->GetTextureCount(aiTextureType_SPECULAR));
        utl::debug("aiTextureType_AMBIENT", pMaterial->GetTextureCount(aiTextureType_AMBIENT));
        utl::debug("aiTextureType_EMISSIVE", pMaterial->GetTextureCount(aiTextureType_EMISSIVE));
        utl::debug("aiTextureType_NORMALS", pMaterial->GetTextureCount(aiTextureType_NORMALS));
        utl::debug("aiTextureType_DIFFUSE", pMaterial->GetTextureCount(aiTextureType_DIFFUSE));
        utl::debug("aiTextureType_UNKNOWN", pMaterial->GetTextureCount(aiTextureType_UNKNOWN));
		*/

        /// we're only intersted in teh diffuse texture
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                fullPath = Dir + "/" + path.data;

         //       utl::debug("fullPath", fullPath);
                m_textures[i].m_id = utl::loadTexture(fullPath);

                success = true;
            }
        }

   //     utl::debug(fullPath, m_textures[i].m_id);
        if (!success)
            m_textures[i].m_id = utl::loadTexture("Assets/models/weapons/Ak_47/ak-47.jpg");
//            m_textures[i].m_id = utl::loadTexture("Assets/models/weapons/M9/Tex_0009_1.jpg");
    //    utl::debugLn("final: " + fullPath, m_textures[i].m_id);
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
        utl::debug("i", i);
        utl::debug("aiTextureType_DIFFUSE", pMaterial->GetTextureCount(aiTextureType_DIFFUSE));
*/
        /// we're only intersted in teh diffuse texture
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                fullPath = Dir + "/" + path.data;

           //     utl::debug("fullPath", fullPath);
                m_textures[i-1].m_id = utl::loadTexture(fullPath);

                success = true;
            }
        }

        // utl::debug(fullPath, m_textures[i-1].m_id);
        // Load a white texture in case the model does not include its own texture
		if (!success)
			m_textures[i].m_id = utl::loadTexture("Assets/models/weapons/Ak_47/ak-47.jpg");

        // utl::debugLn("final: " + fullPath, m_textures[i-1].m_id);


    }

    return Ret;
}
