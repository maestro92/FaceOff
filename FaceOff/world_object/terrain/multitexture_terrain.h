#ifndef MULTI_TEXTURE_TERRAIN_H_
#define MULTI_TEXTURE_TERRAIN_H_

#include "renderer_manager.h"
#include "terrain\terrain.h"

class MultiTextureTerrain : public Terrain
{
	private:
		vector<GLuint> m_textureIDs;
		GLuint m_blendMapTextureID;

	//	static Renderer r_multiTexturedTerrain;
	public:
		MultiTextureTerrain();
		MultiTextureTerrain(string heightMap);
		void render(Pipeline& p);

};


#endif