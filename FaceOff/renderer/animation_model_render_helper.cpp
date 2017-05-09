#include "renderer.h"
#include "animation_model_render_helper.h"

/*
AnimationModelRenderHelper::AnimationModelRenderHelper(Renderer* r)
{
	init(r);
}
*/

void AnimationModelRenderHelper::init(Renderer* r)
{
	m_renderer = r;

	boneUniLockIndexStart = m_renderer->m_dataPairs.size();
	for (unsigned int i = 0; i < MAX_BONES; i++)
	{
		string name = "u_bones[" + utl::intToStr(i) + "]";
		int varEnum = boneUniLockIndexStart + i;
		m_renderer->addDataPair(name, DP_MAT4);
	}
}


void AnimationModelRenderHelper::setUniLocs()
{
	if (m_boneTransforms != nullptr)
	{
		for (int i = 0; i < m_boneTransforms->size(); i++)
		{
			int varEnum = boneUniLockIndexStart + i;
			m_renderer->m_dataPairs[varEnum]->setValue((*m_boneTransforms)[i]);
			m_renderer->m_dataPairs[varEnum]->setUniLoc();
		}
	}
}

