#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "world_object.h"
#include "terrain_model.h"


class Terrain : public WorldObject
{
	public:
		int m_vertexCount;
		float m_size;
		float m_maxHeight;

		TerrainModel* m_model;
		GLuint m_textureID;



		static Renderer r_renderer;

	public:
		Terrain();
		Terrain(string heightMap);
		
		float getHeightAt(float worldX, float worldZ);
		
		void render(Pipeline& p);

};


#endif