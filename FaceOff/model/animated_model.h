#ifndef ANIMATED_MODEL_H_
#define	ANIMATED_MODEL_H_

#include <vector>
#include "imported_model.h"
#include <unordered_map>

// refer to assimp's specs whenever you have questions
// http://www.assimp.org/lib_html/data.html
using namespace std;



class AnimatedModel : public ImportedModel
{



	public:
		AnimatedModel();
		AnimatedModel(string filename);
		AnimatedModel(string filename, vector<string> textures);
		virtual ~AnimatedModel();
		
		void init();
		// virtual bool load(string filename);
		virtual bool load(string filename, vector<string> textures);
		virtual bool initFromAiScene(const aiScene* scene, const string& filename);

		virtual void initMesh(unsigned int index, const aiMesh* m, const aiScene* scene);
		// why do we need this?
		// glm::mat4 m_globalInverseTransform;

		

		// find the tranformation of the bones at time
		void getAnimBoneTranforms(double animationTick, vector<glm::mat4>& boneTransforms);



		virtual void render();

		void printBoneNodesHiearchy();



	private:
		// we preprocess as much as possible, changing all ai-Matrix to glm-matrix 
		struct BoneAnimKey
		{
			double time;
			
			glm::vec3 pos;
			aiQuaternion quat;
			glm::vec3 scale;
		};

		struct VecKey
		{
			double time;
			glm::vec3 value;
		};

		struct QuatKey
		{
			double time;
			aiQuaternion value;
		};

		struct BoneNode
		{
			int id;
			string name; // mainly for debugging, not actually used				
			glm::mat4 localTransform;	// relative to parent		



			glm::mat4 offsetMatrix;
			vector<BoneNode*> children;

			// Note: from assimp spec 
			// The transformation matrix computed from animationKey values replaces the node's original transformation matrix at a specific time. 
			// vector<BoneAnimKey> animationKeys;

			vector<VecKey> posKeys;
			vector<QuatKey> rotKeys;
			vector<VecKey> scaleKeys;
		};

		void printBoneNodesHiearchy(BoneNode* node, int level);

		BoneNode* m_rootBoneNode;
		vector<BoneAnimKey> m_boneAnimationKeys;

		void initBoneIds(const aiScene* scene);
		BoneNode* initBoneTree(aiNode* node);
		void initBoneNodeAnimKeys(const aiScene* scene);
		aiNode* findStartingBoneNode(aiNode* node);
		
		// int findAnimKeyIndex(double nowTime_s);

		int findAnimKeyIndex(vector<VecKey>& keys, double nowTime_s);
		int findAnimKeyIndex(vector<QuatKey>& keys, double nowTime_s);

		BoneNode* findBoneNode(BoneNode* node, string boneName);

		void getAnimBoneTranforms(double nowTime_s, BoneNode* boneNode, vector<glm::mat4>& boneTransforms, glm::mat4& curTransform);
		unordered_map<string, int> m_boneIdMapping;

//		glm::mat4 m_bindPoseInverseTransform;
		glm::mat4 m_globalTransform;
		glm::mat4 m_globalInverseTransform;
		int m_numBones;


		double m_durationInTicks;
		double m_ticksPerSecond;

		vector<Mesh2> m_meshes2;
		
};


#if 0
#define NUM_BONES_PER_VERTEX 4
struct BoneInfo
{
	glm::mat4 boneOffset;
	glm::mat4 finalTransformation;

	BoneInfo()
	{
		boneOffset = glm::mat4(0.0f);
		finalTransformation = glm::mat4(0.0f);
	}
};

/// The Bone Influence information for each Vertex
struct VertexBoneData
{
	/// number of bones that will influence this Vertex
	/// Bone IDs are indices into an array of bone transformations
	unsigned int ids[NUM_BONES_PER_VERTEX];
	float weights[NUM_BONES_PER_VERTEX];

	VertexBoneData()
	{
		Reset();
	};

	void Reset()
	{
		ZERO_MEM(ids);
		ZERO_MEM(weights);
	}



	/// called in initMesh->loadBones
	/// it finds a free slot in the IDs and Weights and adds the bone in it
	/// it's initialized to zero, therefore it finds a free slot by
	/// going through "if(Weights[i] == 0.0)"

	/// BoneID is the index of the bone in m_BoneInfo
	/// so we can get the transformation information from there
	void AddBoneData(unsigned int BoneID, float Weight)
	{
		/*
		cout << "BoneID" << BoneID << endl;

		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++)
		{
		cout << Weights[i] << " ";
		}
		cout << endl;
		*/

		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(ids); i++)
		{
			if (weights[i] < 0.00001)
			{
				ids[i] = BoneID;
				weights[i] = Weight;
				return;
			}
		}
		//    cout << "here" << endl;

		assert(0);
	}
};



class AnimatedModel : public ImportedModel
{
	public:
		AnimatedModel();
		~AnimatedModel();

		bool virtual load();


		float m_ticksPerFrame;
		float m_animFrameDuration;

		glm::mat4 m_globalInverseTransform;
};
#endif




#endif

