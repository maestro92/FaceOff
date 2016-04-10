#include "terrain.h"

Renderer Terrain::r_renderer;

Terrain::Terrain() : Terrain("")
{}

Terrain::Terrain(string heightMap)
{
	if (heightMap == "")
		m_model = new TerrainModel();
	else
		m_model = new TerrainModel(heightMap);

	m_textureID = utl::loadTexture("Assets/Images/terrain/grass.jpg", GL_LINEAR_MIPMAP_NEAREST, GL_REPEAT, true);
	
	Shader* s = new Shader("terrain/terrain.vs", "terrain/terrain.fs");
	r_renderer.addShader(s);
	r_renderer.addDataPair("u_texture", DP_INT);
}

void Terrain::render(Pipeline& p)
{
	r_renderer.enableShader();
		r_renderer.setData("u_texture", 0, GL_TEXTURE_2D, m_textureID);
//		WorldObject::renderGroup(p, &r_renderer, m_model);
	r_renderer.disableShader();
}


float Terrain::getHeightAt(float worldX, float worldZ)
{
	// we first want to find out where the gridsquare is
	float terrainX = worldX - m_position.x;
	float terrainZ = worldZ - m_position.z;

	terrainX /= m_scale.x;
	terrainZ /= m_scale.z;

	return m_model->getHeightAt(terrainX, terrainZ);
}