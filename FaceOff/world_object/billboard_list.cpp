#include "billboard_list.h"

//Renderer BillboardList::r_billboardOneQuad;
//Renderer BillboardList::r_billboardTwoQuad;

BillboardList::BillboardList() : BillboardList("Assets/Images/billboardgrass01.png")
//BillboardList::BillboardList() : BillboardList("Assets/Images/bush01.png")
{ }

BillboardList::BillboardList(string textureFile)
{
	m_mode = UNIFORM;

	m_model = BillboardModel(100, 100, 0.5);
	setTexture(textureFile);
//	p_renderer = &RendererManager::r_billboardOneQuad;
	/*
	Shader* s = new Shader("billboarding.vs", 
						   "billboarding_one_quad.gs",
						   "billboarding.fs");
	r_billboardOneQuad.addShader(s);
	r_billboardOneQuad.addDataPair("u_texture", DP_INT);
	r_billboardOneQuad.addDataPair("u_centerPosition", DP_VEC3);
	r_billboardOneQuad.addDataPair("u_billboardWidthScale", DP_FLOAT);
	r_billboardOneQuad.addDataPair("u_billboardHeightScale", DP_FLOAT);

	p_renderer = &r_billboardOneQuad;

	s = new Shader("billboarding.vs", 
				   "billboarding_two_quad.gs", 
				   "billboarding.fs");
	r_billboardTwoQuad.addShader(s);
	r_billboardTwoQuad.addDataPair("u_texture", DP_INT);
	r_billboardTwoQuad.addDataPair("u_centerPosition", DP_VEC3);
	r_billboardTwoQuad.addDataPair("u_billboardWidthScale", DP_FLOAT);
	r_billboardTwoQuad.addDataPair("u_billboardHeightScale", DP_FLOAT);
	*/
}

BillboardList::~BillboardList()
{

}


void BillboardList::setTexture(string textureFile)
{
	m_textureID = utl::loadTexture(textureFile, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, true);
//	m_textureID = utl::loadTexture(textureFile);
}


void BillboardList::setUniormationFormation(int w, int h, int gap)
{
	m_mode = UNIFORM;
	m_model.createUniformFormation(w, h, gap);
}


void BillboardList::setUniormationFormation(int w, int h, int gap, Terrain* terrain)
{

}


void BillboardList::setRandomFormation(int maxW, int maxH, int count)
{
	m_mode = RANDOM;
	m_model.createRandFormation(maxW, maxH, count);
//	p_renderer = &RendererManager::r_billboardTwoQuad;
}

void BillboardList::setRandomFormation(int maxW, int maxH, int count, Terrain* terrain)
{
	m_mode = RANDOM;
	m_model.createRandFormation(maxW, maxH, count, terrain);
//	p_renderer = &RendererManager::r_billboardTwoQuad;
}



void BillboardList::render(Pipeline& p)
{
	/*
	glDisable(GL_CULL_FACE);
	p_renderer->enableShader();
	
		p_renderer->setData("u_texture", 0, GL_TEXTURE_2D, m_textureID);
		p_renderer->setData("u_centerPosition", m_position);
		p_renderer->setData("u_billboardWidthScale", (float)2.0);
		p_renderer->setData("u_billboardHeightScale", (float)2.0);
		WorldObject::renderGroup(p, p_renderer, &m_model);

	p_renderer->disableShader();
	glEnable(GL_CULL_FACE);
*/
	
}


