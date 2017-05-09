#ifndef ANIMATION_MODEL_RENDER_HELPER_H_
#define ANIMATION_MODEL_RENDER_HELPER_H_


#include "utility.h"
#include "pipeline.h"
class Renderer;

class AnimationModelRenderHelper
{	
	static const unsigned int MAX_BONES = 100;

	public:
//		AnimationModelRenderHelper(Renderer* r);

		void init(Renderer* r);
		int boneUniLockIndexStart;

		void setUniLocs();

		// the renderer class shouldn't mess with this
		// should be assigned from outside
		vector<glm::mat4>* m_boneTransforms;

		Renderer* m_renderer;
};


#endif