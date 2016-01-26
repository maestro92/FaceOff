#include "multitexture_terrain.h"

// Renderer MultiTextureTerrain::r_multiTexturedTerrain;

MultiTextureTerrain::MultiTextureTerrain() : MultiTextureTerrain("")
{}

MultiTextureTerrain::MultiTextureTerrain(string heightMap)
{
	if (heightMap == "")
		m_model = new TerrainModel();
	else
		m_model = new TerrainModel(heightMap);

	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/grassy.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/dirt.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/mud.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true));
	m_textureIDs.push_back(utl::loadTexture("Assets/Images/terrain/path.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true));

	m_blendMapTextureID = utl::loadTexture("Assets/Images/terrain/blendMap.png", GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true);
	
	/*
	Shader* s = new Shader("terrain/multitexture_terrain.vs", "terrain/multitexture_terrain.fs");
	r_multiTexturedTerrain.addShader(s);
	r_multiTexturedTerrain.addDataPair("u_blendMapTexture", DP_INT);
	r_multiTexturedTerrain.addDataPairArray("u_textures", DP_INT, 4);
	*/
}



void MultiTextureTerrain::render(Pipeline& p)
{
	RendererManager::r_multiTexTerrain.enableShader();
		RendererManager::r_multiTexTerrain.setData("u_blendMapTexture", 0, GL_TEXTURE_2D, m_blendMapTextureID);
		RendererManager::r_multiTexTerrain.setDataArray("u_textures", 1, GL_TEXTURE_2D, m_textureIDs);

		WorldObject::renderGroup(p, &RendererManager::r_multiTexTerrain, m_model);
	RendererManager::r_multiTexTerrain.disableShader();


	/*
	r_multiTexturedTerrain.enableShader();
		r_multiTexturedTerrain.setData("u_blendMapTexture", 0, GL_TEXTURE_2D, m_blendMapTextureID);
		r_multiTexturedTerrain.setDataArray("u_textures", 1, GL_TEXTURE_2D, m_textureIDs);

		WorldObject::renderGroup(p, &r_multiTexturedTerrain, m_model);
	r_multiTexturedTerrain.disableShader();
	*/
}


