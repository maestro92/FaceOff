#ifndef SCENE_LIGHT_RENDER_HELPER_H_
#define SCENE_LIGHT_RENDER_HELPER_H_

#include "light/base_light.h"

#include "global.h"
#include "light\light_manager.h"
class Renderer;

class SceneLightRenderHelper
{
	const int NUM_DIR_LIGHT_UNILOCS = 4;
	const int NUM_PT_LIGHT_UNILOCS = 4;
	const int NUM_SPOT_LIGHT_UNILOCS = 4;

	public:

		void init(Renderer* r);
		void setDirLightsData(const vector<DirectionalLight>& lights);
		void setPointLightsData(const vector<PointLight>& lights);
		void setSpotLightsData(const vector<SpotLight>& lights);

	private:
		int dirLightUniLocStartIndex;
		int pointLightUniLocStartIndex;
		int spotLightUniLocStartIndex;


		void addDirLightUniLocs();
		void addPointLightUniLocs();
		void addSpotLightUniLocs();


		void setDirLightData(const DirectionalLight& light, int lightIndex);
		void setPointLightData(const PointLight& light, int lightIndex);
		void setSpotLightData(const SpotLight& light, int lightIndex);

		Renderer* m_renderer;
};

#endif