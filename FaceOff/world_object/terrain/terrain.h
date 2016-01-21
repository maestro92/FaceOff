#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "world_object.h"
#include "terrain_model.h"

const float MAX_PIXEL_COLOUR = 256 * 256 * 256;

class Terrain : public WorldObject
{
	public:
		int m_vertexCount;
		float m_size;
		float m_maxHeight;

		Model* m_model;
		GLuint m_textureID;

		static Renderer r_renderer;

	public:
		Terrain();
		Terrain(string heightMap);
		void render(Pipeline& p);

};


#endif