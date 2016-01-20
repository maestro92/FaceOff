#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "world_object.h"
#include "terrain_model.h"

class Terrain : public WorldObject
{
	private:
		int m_vertexCount;
		float m_size;

		Model* m_model;
		GLuint m_textureID;

		static Renderer r_terrain;

	public:
		Terrain();
		Terrain(int gridX, int gridZ);
		void render(Pipeline& p);

};


#endif