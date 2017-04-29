
#include "animation_model_renderer.h"


void AnimationModelRenderer::init()
{
	m_boneTransforms.resize(MAX_BONES, glm::mat4(0.0));
	
	boneUniLockIndexStart = m_dataPairs.size();
	for (unsigned int i = 0; i < MAX_BONES; i++)
	{
	//	char name[128];
	//	printf(name, sizeof(name), "u_bone[%d]", i);

		string name = "u_bones[" + utl::intToStr(i) + "]";
	//	cout << name << " " << sName.size() << endl;;
		int varEnum = boneUniLockIndexStart + i;
		addDataPair(name, DP_MAT4);

		if (m_dataPairs[varEnum]->uniLoc == -1)
		{
			cout << m_shader->getVertexShaderName() << " " << name << " is not active" << endl;
		}
	}

	

}


void AnimationModelRenderer::setUniLocs(Pipeline& p)
{
	for (int i = 0; i < m_boneTransforms.size(); i++)
	{
		int varEnum = boneUniLockIndexStart + i;
		m_dataPairs[varEnum]->setValue(m_boneTransforms[i]);
		m_dataPairs[varEnum]->setUniLoc();
//		m_dataPairs[varEnum]->setUniLocTranspose();
	}

	Renderer::setUniLocs(p);
}

