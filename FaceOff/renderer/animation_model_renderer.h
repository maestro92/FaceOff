#ifndef ANIMATION_MODEL_RENDERER_H_
#define ANIMATION_MODEL_RENDERER_H_

#include "renderer.h"
#include "utility.h"
class AnimationModelRenderer : public Renderer
{



public:
	static const unsigned int MAX_BONES = 100;
	
	void init();
	int boneUniLockIndexStart;

	virtual void setUniLocs(Pipeline& p);

	vector<glm::mat4> m_boneTransforms;
};


#endif