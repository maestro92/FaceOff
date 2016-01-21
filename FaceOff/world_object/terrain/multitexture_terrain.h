#ifndef MULTI_TEXTURE_TERRAIN_H_
#define MULTI_TEXTURE_TERRAIN_H_

#include "terrain\terrain.h"

class MultiTextureTerrain : public Terrain
{
	private:
		vector<GLuint> m_textureIDs;
		GLuint m_blendMapTextureID;

		static Renderer r_multiTexturedTerrain;
	public:
		MultiTextureTerrain();
		MultiTextureTerrain(int gridX, int gridZ);
		void render(Pipeline& p);

};


#endif