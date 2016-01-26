#ifndef SCENE_RENDERER_H_
#define SCENE_RENDERER_H_

#include "renderer.h"
#include "light/base_light.h"

class SceneRenderer : public Renderer
{
	public:
		
		virtual void addShader(Shader* s);

		bool addDirLightUniLocs();
		bool addPointLightUniLocs();
		bool addSpotLightUniLocs();

		void setDirLightData(DirectionalLight light);
		void setPointLightData(PointLight light);
		void setSpotLightData(SpotLight light);

		void setDirLightsData(vector<DirectionalLight> lights);
		void setPointLightsData(vector<PointLight> lights);
		void setSpotLightsData(vector<SpotLight> lights);
};


#endif