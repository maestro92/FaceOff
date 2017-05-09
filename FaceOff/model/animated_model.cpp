#include "animated_model.h"








AnimatedModel::AnimatedModel()
{
	init();
}



AnimatedModel::~AnimatedModel()
{



}


AnimatedModel::AnimatedModel(string filename)
{
	init();
	vector<string> textures;
	load(filename, textures);
}

AnimatedModel::AnimatedModel(string filename, vector<string> textures)
{
	init();
	load(filename, textures);
}

void AnimatedModel::init()
{
	m_numBones = 0;
	m_isAnimated = true;
	m_rootBoneNode = NULL;
}

/*
bool AnimatedModel::load(string filename)
{
	return true;
}
*/

bool AnimatedModel::load(string filename, vector<string> textureFiles)
{
	bool ret = false;

	const aiScene* scene = aiImportFile(filename.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

	if (scene)
	{
		m_aabb.min = glm::vec3(INT_MAX);
		m_aabb.max = glm::vec3(INT_MIN);

		m_globalTransform = utl::toGlmMat(scene->mRootNode->mTransformation);
		m_globalInverseTransform = glm::inverse(m_globalTransform);

		ret = initFromAiScene(scene, filename);

		printAiSceneHiearchy(scene->mRootNode, 0);
		printBoneNodesHiearchy(m_rootBoneNode, 0);
		int a = 1;

	}
	else
		cout << "Error parsing '" << filename.c_str() << "': '" << aiGetErrorString() << endl;

	setTextures(textureFiles);

	return ret;
}

void Model::printAiSceneHiearchy(const aiNode* node, int level)
{
	if (node == NULL)
	{
		return;
	}

	string prefix = " ";
	for (int i = 0; i < level; i++)
	{
		prefix += "	";
	}

	cout << prefix << node->mName.data << endl;
	utl::debug("transformation", utl::toGlmMat(node->mTransformation));
	for (int i = 0; i < node->mNumChildren; i++)
	{
		aiNode* child = node->mChildren[i];
		printAiSceneHiearchy(child, level + 1);
	}
}



bool AnimatedModel::initFromAiScene(const aiScene* scene, const string& filename)
{

	initBoneIds(scene);

	aiNode* rootBoneNode = findStartingBoneNode(scene->mRootNode);

//	cout << rootBoneNode->mName.data << endl;

//	utl::debug("rootNode", scene->mRootNode->of)

	m_rootBoneNode = initBoneTree(rootBoneNode);
	cout << m_rootBoneNode->name << endl;
//	printBoneNodesHiearchy(m_rootBoneNode, 0);

	initBoneNodeAnimKeys(scene);

	if (scene->mAnimations)
	{
		 m_ticksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
		 m_durationInTicks = (float)(scene->mAnimations[0]->mDuration);
	}

	m_meshes2.resize(scene->mNumMeshes);
	m_textures.resize(scene->mNumMaterials);		// for some reason Assimp always creates one extra mNumMaterials

	// bool b = initMaterials2(scene, filename);
	// initBonesArray


	for (unsigned int i = 0; i<m_meshes2.size(); i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		initMesh(i, mesh, scene);
	}

	return true;
}



void AnimatedModel::initBoneIds(const aiScene* scene)
{
	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* anim = scene->mAnimations[i];

		for (int j = 0; j < anim->mNumChannels; j++)
		{
			aiNodeAnim* nodeAnim = anim->mChannels[j];
	
			string boneName(nodeAnim->mNodeName.data);
			m_boneIdMapping[boneName] = j;		
			m_numBones++;
		}
	}

	cout << "numBones " << m_numBones << endl;
	int a = 1;
/*

*/
}

void AnimatedModel::initBoneNodeAnimKeys(const aiScene* scene)
{
//	cout << "num animations " << scene->mNumAnimations << endl;
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
			cout << "			mNumScalingKeys " << nodeAnim->mNumScalingKeys << endl;



			string boneName(nodeAnim->mNodeName.data);
			BoneNode* boneNode = findBoneNode(m_rootBoneNode, boneName);


			// assuming position, rotation and scale keys have the same number of keys
			for (int z = 0; z < nodeAnim->mNumPositionKeys; z++)
			{
				VecKey key;
				key.time = nodeAnim->mPositionKeys[z].mTime;				
				key.value = utl::toGlmVec(nodeAnim->mPositionKeys[z].mValue);
				boneNode->posKeys.push_back(key);
			}

			for (int z = 0; z < nodeAnim->mNumRotationKeys; z++)
			{
				QuatKey key;
				key.time = nodeAnim->mRotationKeys[z].mTime;
				key.value = nodeAnim->mRotationKeys[z].mValue;
				boneNode->rotKeys.push_back(key);
			}
			for (int z = 0; z < nodeAnim->mNumScalingKeys; z++)
			{
				VecKey key;
				key.time = nodeAnim->mScalingKeys[z].mTime;
				key.value = utl::toGlmVec(nodeAnim->mScalingKeys[z].mValue);
				boneNode->scaleKeys.push_back(key);
			}
		}
	}

}

AnimatedModel::BoneNode* AnimatedModel::findBoneNode(BoneNode* node, string boneName)
{
	if (node == NULL)
	{
		return node;
	}

	if (node->name == boneName)
	{
	//	cout << node->name << endl;
		return node;
	}

	for (int i = 0; i< node->children.size(); i++)
	{
		BoneNode* result = findBoneNode(node->children[i], boneName);
		if (result != NULL)
		{
			return result;
		}
		
	}

	return NULL;
}


aiNode* AnimatedModel::findStartingBoneNode(aiNode* node)
{
	if (node == NULL)
	{
		return NULL;
	}

	string name(node->mName.data);
	cout << name << endl;
	if (name != "<MD5_Root>" && name != "<MD5_Mesh>" && name != "<MD5_Hierarchy>")
	{
		return node;
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		aiNode* child = node->mChildren[i];
		
		aiNode* temp = findStartingBoneNode(child);

		if (temp != NULL)
		{
			return temp;
		}
	}

	return NULL;
}


AnimatedModel::BoneNode* AnimatedModel::initBoneTree(aiNode* node)
{
	if (node == NULL)
	{
		return NULL;
	}

	string name(node->mName.data);
	BoneNode* boneNode = new BoneNode();
	boneNode->name = name;
	boneNode->id = m_boneIdMapping[name];
	boneNode->localTransform = utl::toGlmMat(node->mTransformation);

	for (int i = 0; i < node->mNumChildren; i++)
	{
		aiNode* child = node->mChildren[i];
		boneNode->children.push_back(initBoneTree(child));
	}

	return boneNode;
}




void AnimatedModel::initMesh(unsigned int index, const aiMesh* m, const aiScene* scene)
{
	vector<VertexData2> Vertices;
	vector<unsigned int> Indices;

	aiColor4D col;
	aiMaterial* mat = scene->mMaterials[m->mMaterialIndex];
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &col);
	glm::vec3 defaultColor(col.r, col.g, col.b);

	for (unsigned int i = 0; i < m->mNumVertices; i++)
	{
		VertexData2 v;

		/// position
		v.position = utl::toGlmVec(m->mVertices[i]);

		m_aabb.min.x = min(m_aabb.min.x, v.position.x);
		m_aabb.max.x = max(m_aabb.max.x, v.position.x);

		m_aabb.min.y = min(m_aabb.min.y, v.position.y);
		m_aabb.max.y = max(m_aabb.max.y, v.position.y);

		m_aabb.min.z = min(m_aabb.min.z, v.position.z);
		m_aabb.max.z = max(m_aabb.max.z, v.position.z);


		/// normal
		v.normal = utl::toGlmVec(m->mNormals[i]);

		/// colors
		if (m->mColors[0])
			v.color = utl::toGlmVec(m->mColors[0][i]);
		else
			v.color = defaultColor;

		/// UV
		if (m->mTextureCoords[0])
			v.UV = glm::vec2(m->mTextureCoords[0][i].x, m->mTextureCoords[0][i].y);
		else
			v.UV = glm::vec2(0.0f, 0.0f);

		Vertices.push_back(v);



	} 

	for (int i = 0; i < m->mNumBones; i++)
	{
		aiBone* bone = m->mBones[i];

		string boneName(bone->mName.data);
		BoneNode* boneNode = findBoneNode(m_rootBoneNode, boneName);
		glm::mat4 offsetMatrix = utl::toGlmMat(bone->mOffsetMatrix);

		boneNode->offsetMatrix = offsetMatrix;

		int boneId = m_boneIdMapping[boneName];

		for (int j = 0; j < bone->mNumWeights; j++)
		{
			int vertexId = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;

			Vertices[vertexId].addBone(boneId, weight);
		}
	}


	for (int j = 0; j < Vertices.size(); j++)
	{
		float sum = 0;
		for (int z = 0; z < NUM_BONES_PER_VERTEX; z++)
		{
			sum += Vertices[j].weights[z];
		}
		if (sum != 1)
		{
			cout << m->mName.data << ", sum is not 1: " << sum << endl;
		}
		else
		{
	//		cout << m->mName.data << ", sum is 1: " << sum << endl;
		}
	}
	

	for (unsigned int i = 0; i < m->mNumFaces; i++)
	{
		const aiFace& Face = m->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_meshes2[index] = Mesh2(Vertices, Indices, m->mMaterialIndex);
}


/*
we traverse through our tree (nodes), and whenever we arrive at a node, we set the matrix transform in our boneTransformation
*/
void AnimatedModel::getAnimBoneTranforms(double nowTime_s, vector<glm::mat4>& boneTransforms)
{
	if (boneTransforms.size() != m_numBones)
	{
		boneTransforms.resize(m_numBones);
	}

	glm::mat4 parentTransform = glm::mat4(1.0f);

	double timeInTicks = nowTime_s * m_ticksPerSecond;
	double animationTick = fmod(timeInTicks, m_durationInTicks);
	


//	int animKeyIndex = findPosKeyIndex(animationTick);
//	cout << animationTick << endl;
	getAnimBoneTranforms(animationTick, m_rootBoneNode, boneTransforms, parentTransform);

}


// TODO: binary search
int AnimatedModel::findAnimKeyIndex(vector<VecKey>& keys, double nowTime_s)
{
	for (int i = 0; i < keys.size() - 1; i++)
	{
		if (nowTime_s < keys[i + 1].time)
		{
			return i;
		}
	}
	return 0;
}


int AnimatedModel::findAnimKeyIndex(vector<QuatKey>& keys, double nowTime_s)
{
	for (int i = 0; i < keys.size() - 1; i++)
	{
		if (nowTime_s < keys[i + 1].time)
		{
			return i;
		}
	}
	return 0;
}

void AnimatedModel::getAnimBoneTranforms(double animationTick, BoneNode* boneNode, vector<glm::mat4>& boneTransforms, glm::mat4& parentTransfrom)
{
	
	if (boneNode == NULL)
	{
		return;
	}

//	cout << m_rootBoneNode->name << endl;
//	cout << boneNode->name << endl;

	int posKeyIndex = findAnimKeyIndex(boneNode->posKeys, animationTick);
	int rotKeyIndex = findAnimKeyIndex(boneNode->rotKeys, animationTick);
	int scaleKeyIndex = findAnimKeyIndex(boneNode->scaleKeys, animationTick);


	// int posKeyIndex = 0;
	// int rotKeyIndex = 0;
	// int scaleKeyIndex = 0;

	// cout << "posKeyIndex " << posKeyIndex << ", rotKeyIndex " << rotKeyIndex << ", scaleKeyIndex " << scaleKeyIndex << endl;

	glm::mat4 scale = glm::scale(boneNode->scaleKeys[scaleKeyIndex].value);
	glm::mat4 quat = utl::toGlmMat((boneNode->rotKeys[rotKeyIndex].value.Normalize()).GetMatrix());
	glm::mat4 trans = glm::translate(boneNode->posKeys[posKeyIndex].value);

//	cout << scale << endl;

	glm::mat4 transform = trans * quat * scale;

	glm::mat4 newParentTransform = parentTransfrom * transform;


//	boneTransforms[boneNode->id] = m_globalInverseTransform * newParentTransform * transform * boneNode->offsetMatrix;//  *;
	boneTransforms[boneNode->id] = newParentTransform * boneNode->offsetMatrix;//  *;
	/*
	cout << boneNode->name << endl;
	if (boneNode->name == "Leg.L")
	{
		cout << "################################################################################ " << endl;
		utl::debug("translationMat", trans);
		utl::debug("rotationMat", quat);
		utl::debug("scalingMat", scale);

		utl::debug("m_GlobalInverseTransform", m_globalInverseTransform);
		utl::debug("newParentTransform", newParentTransform);
		utl::debug("boneOffset", boneNode->offsetMatrix);
		utl::debug("parentTransform", newParentTransform);
		utl::debug("transform", transform);
		utl::debug("bone finalTransformation", boneTransforms[boneNode->id]);
		int a = 1;
	}
	*/


	for (int i = 0; i < boneNode->children.size(); i++)
	{
		getAnimBoneTranforms(animationTick, boneNode->children[i], boneTransforms, newParentTransform);
	}

}


void AnimatedModel::printBoneNodesHiearchy()
{
	printBoneNodesHiearchy(m_rootBoneNode, 0);
}

void AnimatedModel::printBoneNodesHiearchy(BoneNode* node, int level)
{
	if (node == NULL)
	{
		return;
	}

	string prefix = " ";
	for (int i = 0; i < level; i++)
	{
		prefix += "	";
	}

	cout << prefix << node->id << " " << node->name << endl;

	cout << "numPosKeys " << node->posKeys.size() << ", numQuadKeys " << node->rotKeys.size() << ", numScaleKeys " << node->scaleKeys.size() << endl;

	for (int i = 0; i < node->children.size(); i++)
	{
		BoneNode* child = node->children[i];
		printBoneNodesHiearchy(child, level + 1);
	}
}

/*
void AnimatedModel::updateAnimBoneTranforms_(double time_s, const aiNode* node, const glm::mat4& curTransform)
{
	// set current Node

	const aiNodeAnim* nodeAnim = findNodeAnim();

	if (nodeAnim)
	{

	}


	// traverse through child

}
*/

void AnimatedModel::render()
{
//	enableVertexAttribArrays();
	
	/// this means we're going to use these two buffer object
	for (int i = 0; i<m_meshes2.size(); i++)
	{
		Mesh2 m = m_meshes2[i];

		glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ind);

		glVertexAttribPointer(POSITION_VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData2), 0);
		glVertexAttribPointer(NORMAL_VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData2), (GLvoid*)(3 * sizeof(float)));
		glVertexAttribPointer(COLOR_VERTEX_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData2), (GLvoid*)(6 * sizeof(float)));
		glVertexAttribPointer(UV_VERTEX_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData2), (GLvoid*)(9 * sizeof(float)));
		glVertexAttribIPointer(BONE_IDS_ATTRIB, 4, GL_INT, sizeof(VertexData2), (const GLvoid*)(11 * sizeof(float)));
		glVertexAttribPointer(BONE_WEIGHTS_ATTRIB, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData2), (GLvoid*)(11 * sizeof(float) + 4 * sizeof(int)));
//		glVertexAttribPointer(BONE_WEIGHTS_ATTRIB, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData2), (GLvoid*)(15 * sizeof(float)));

		/*
		const unsigned int texIndex = m_meshes2[i].m_textureIndex;


		if (texIndex < m_textures.size() && m_textures[texIndex].m_id != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textures[texIndex].m_id);
		}
		*/

		/// glDrawElements, you need to supply an index buffer
		/// glDrawArrays submits the vertices in linear order
		glDrawElements(m_modelGeometry, m.numIndices, GL_UNSIGNED_INT, 0);
	}
//	disableVertexAttribArrays();
}
