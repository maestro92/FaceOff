#include "imported_model.h"

ImportedModel::ImportedModel()
{
	m_isAnimated = false;
}

ImportedModel::ImportedModel(string filename, ModelType type)
{
	m_type = type;
    load(filename);
}

ImportedModel::ImportedModel(string filename, vector<string> textures, ModelType type)
{
	m_type = type;
	load(filename, textures);
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
		m_aabb.min = glm::vec3(INT_MAX);
		m_aabb.max = glm::vec3(INT_MIN);

	//	cout << "Loading " << filename << endl;
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
		m_aabb.min = glm::vec3(INT_MAX);
		m_aabb.max = glm::vec3(INT_MIN);

		ret = initFromAiScene(scene, filename);
	}
	else
		cout << "Error parsing '" << filename.c_str() << "': '" << aiGetErrorString() << endl;

	setTextures(textureFiles);

	return ret;
}

#if 0
bool ImportedModel::load2(string filename)
{
	return true;
}
bool ImportedModel::load2(string filename, vector<string> textureFiles)
{
	bool ret = false;

	const aiScene* scene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

	if (scene)
	{
		m_aabb.min = glm::vec3(INT_MAX);
		m_aabb.max = glm::vec3(INT_MIN);

		ret = initFromAiScene2(scene, filename);

		printAiSceneHiearchy(scene->mRootNode, 0);

		/*
		if (scene->mAnimations)
		{
			m_TicksPerSecond = (float)(s->mAnimations[0]->mTicksPerSecond != 0 ? s->mAnimations[0]->mTicksPerSecond : 25.0f);
			m_AnimFrameDuration = (float)s->mAnimations[0]->mDuration;
		}
		*/
		cout << "num animations " << scene->mNumAnimations << endl;
		for (int i = 0; i < scene->mNumAnimations; i++)
		{
			aiAnimation* anim = scene->mAnimations[i];
			cout << "	anim name " << anim->mName.data << endl;

			for (int j = 0; j < anim->mNumChannels; j++)
			{
				aiNodeAnim* nodeAnim = anim->mChannels[j];
				cout << "		nodeAnim name " << nodeAnim->mNodeName.data << endl;
				cout << "			mNumPositionKeys " << nodeAnim->mNumPositionKeys << endl;
				cout << "			mNumRotationKeys " << nodeAnim->mNumRotationKeys << endl;
			}
		}

	}
	else
		cout << "Error parsing '" << filename.c_str() << "': '" << aiGetErrorString() << endl;

	setTextures(textureFiles);

	return ret;
}
#endif

bool ImportedModel::initFromAiScene(const aiScene* scene, const string& filename)
{
    m_meshes.resize(scene->mNumMeshes);
    m_textures.resize(scene->mNumMaterials);		// for some reason Assimp always creates one extra mNumMaterials

	bool b = initMaterials2(scene, filename);


    for (unsigned int i=0; i<m_meshes.size(); i++)
    {
        const aiMesh* mesh = scene->mMeshes[i];
        initMesh(i, mesh, scene);
    }

//	return true;
    return b;
}

/*
bool ImportedModel::initFromAiScene2(const aiScene* scene, const string& filename)
{
	m_meshes.resize(scene->mNumMeshes);
	m_textures.resize(scene->mNumMaterials);		// for some reason Assimp always creates one extra mNumMaterials

//	bool b = initMaterials2(scene, filename);


	for (unsigned int i = 0; i<m_meshes.size(); i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		initMesh2(i, mesh, scene);
	}

	return true;
}
*/





void ImportedModel::initMesh(unsigned int index, const aiMesh* m, const aiScene* scene)
{
    vector<VertexData> Vertices;
    vector<unsigned int> Indices;

	aiColor4D col;
	aiMaterial* mat = scene->mMaterials[m->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);

	
	glm::vec3 defaultColor(col.r,col.g,col.b);

	if (m_type == ModelType::sun)
	{
		defaultColor = COLOR_WHITE;
	}
	else
	{
		defaultColor = COLOR_BLACK;
	}
	
	// glm::vec3 defaultColor = COLOR_BLUE;

    for (unsigned int i = 0 ; i < m->mNumVertices ; i++)
    {
        VertexData v;

        /// position
        v.m_position = utl::toGlmVec(m->mVertices[i]);

		m_aabb.min.x = min(m_aabb.min.x, v.m_position.x);
		m_aabb.max.x = max(m_aabb.max.x, v.m_position.x);

		m_aabb.min.y = min(m_aabb.min.y, v.m_position.y);
		m_aabb.max.y = max(m_aabb.max.y, v.m_position.y);

		m_aabb.min.z = min(m_aabb.min.z, v.m_position.z);
		m_aabb.max.z = max(m_aabb.max.z, v.m_position.z);

		
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



/*
void ImportedModel::initMesh2(unsigned int index, const aiMesh* m, const aiScene* scene)
{
	vector<VertexData> Vertices;
	vector<unsigned int> Indices;

	aiColor4D col;
	aiMaterial* mat = scene->mMaterials[m->mMaterialIndex];
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &col);
	glm::vec3 defaultColor(col.r, col.g, col.b);

	cout << "	mesh name " << m->mName.data << endl;

	for (unsigned int i = 0; i < m->mNumVertices; i++)
	{
		VertexData v;

		/// position
		v.m_position = utl::toGlmVec(m->mVertices[i]);

		m_aabb.min.x = min(m_aabb.min.x, v.m_position.x);
		m_aabb.max.x = max(m_aabb.max.x, v.m_position.x);

		m_aabb.min.y = min(m_aabb.min.y, v.m_position.y);
		m_aabb.max.y = max(m_aabb.max.y, v.m_position.y);

		m_aabb.min.z = min(m_aabb.min.z, v.m_position.z);
		m_aabb.max.z = max(m_aabb.max.z, v.m_position.z);


		/// normal
		v.m_normal = utl::toGlmVec(m->mNormals[i]);

		/// colors
		if (m->mColors[0])
			v.m_color = utl::toGlmVec(m->mColors[0][i]);
		else
			v.m_color = defaultColor;

		/// UV
		if (m->mTextureCoords[0])
			v.m_UV = glm::vec2(m->mTextureCoords[0][i].x, m->mTextureCoords[0][i].y);
		else
			v.m_UV = glm::vec2(0.0f, 0.0f);

		Vertices.push_back(v);



	}

	for (int z = 0; z < m->mNumBones; z++)
	{
		aiBone* bone = m->mBones[z];
		cout << "		bone name " << bone->mName.data << endl;
	}

	for (unsigned int i = 0; i < m->mNumFaces; i++)
	{
		const aiFace& Face = m->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_meshes[index] = Mesh(Vertices, Indices, m->mMaterialIndex);
}
*/


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
   
		
	//     if (!success)
   //         m_textures[i].m_id = utl::loadTexture("Assets/models/weapons/Ak_47/ak-47.jpg");

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
