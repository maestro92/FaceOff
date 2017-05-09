/*
#include "renderer.h"
#include "animation_model_renderer.h"


AnimationModelRenderer::AnimationModelRenderer()
{

}

void AnimationModelRenderer::init()
{
//	m_boneTransforms.resize(MAX_BONES, glm::mat4(0.0));
	
	boneUniLockIndexStart = m_dataPairs.size();
	for (unsigned int i = 0; i < MAX_BONES; i++)
	{
		string name = "u_bones[" + utl::intToStr(i) + "]";
		int varEnum = boneUniLockIndexStart + i;
		addDataPair(name, DP_MAT4);
	}
}

 
void AnimationModelRenderer::setUniLocs(Pipeline& p)
{
	if (m_boneTransforms != nullptr)
	{
		for (int i = 0; i < m_boneTransforms->size(); i++)
		{
			int varEnum = boneUniLockIndexStart + i;
			m_dataPairs[varEnum]->setValue((*m_boneTransforms)[i]);
			m_dataPairs[varEnum]->setUniLoc();
		}
	}
	Renderer::setUniLocs(p);
}
*/
