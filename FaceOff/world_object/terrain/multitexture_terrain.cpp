#include "multitexture_terrain.h"

Renderer MultiTextureTerrain::r_multiTexturedTerrain;

MultiTextureTerrain::MultiTextureTerrain() : MultiTextureTerrain(0, 0)
{}

MultiTextureTerrain::MultiTextureTerrain(int gridX, int gridZ)
{
	m_vertexCount = 128;
	m_size = 400;

	setPosition(gridX, 0, gridZ);
	m_model = new TerrainModel(m_vertexCount, m_size);

	/*
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/grassy.png", GL_LINEAR_MIPMAP_NEAREST, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/dirt.png", GL_LINEAR_MIPMAP_NEAREST, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/mud.png", GL_LINEAR_MIPMAP_NEAREST, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/path.png", GL_LINEAR_MIPMAP_NEAREST, GL_REPEAT, true));
	*/

	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/grassy.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/dirt.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/mud.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/path.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true));

	m_blendMapTextureID = utl::loadTexture("Assets/Images/terrain/blendMap.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true);
	Shader* s = new Shader("terrain/multitexture_terrain.vs", "terrain/multitexture_terrain.fs");
	r_multiTexturedTerrain.addShader(s);
	r_multiTexturedTerrain.addDataPair("u_blendMapTexture", DP_INT);
	r_multiTexturedTerrain.addDataPairArray("u_textures", DP_INT, 4);
//	r_multiTexturedTerrain.addDataPair("u_texture0", DP_INT);
//	r_multiTexturedTerrain.addDataPair("u_textures1", DP_INT);



}



void MultiTextureTerrain::render(Pipeline& p)
{
	r_multiTexturedTerrain.enableShader();
		r_multiTexturedTerrain.setData("u_blendMapTexture", 0, GL_TEXTURE_2D, m_blendMapTextureID);
		r_multiTexturedTerrain.setDataArray("u_textures", 1, GL_TEXTURE_2D, m_textureIDs);
//		r_multiTexturedTerrain.setData("u_textures0", 0, GL_TEXTURE_2D, m_textureIDs[0]);
//		r_multiTexturedTerrain.setData("u_textures1", 1, GL_TEXTURE_2D, m_textureIDs[1]);


		WorldObject::renderGroup(p, &r_multiTexturedTerrain, m_model);
	r_multiTexturedTerrain.disableShader();
}

