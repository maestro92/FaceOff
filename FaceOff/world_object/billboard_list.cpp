#include "billboard_list.h"

Renderer BillboardList::r_billboard;

BillboardList::BillboardList() : BillboardList("Assets/Images/billboardgrass01.png")
{ }

BillboardList::BillboardList(string textureFile)
{
	m_model = BillboardModel();
	setTexture(textureFile);

	Shader* s = new Shader("billboarding.vs", "billboarding.gs" ,"billboarding.fs");
	r_billboard.addShader(s);
	r_billboard.addDataPair("u_texture", DP_INT);
	r_billboard.addDataPair("u_cameraPosition", DP_VEC3);
	r_billboard.addDataPair("u_billboardWidthScale", DP_FLOAT);
	r_billboard.addDataPair("u_billboardHeightScale", DP_FLOAT);
}

BillboardList::~BillboardList()
{

}


void BillboardList::setTexture(string textureFile)
{
	m_textureID = utl::loadTexture(textureFile);
}

void BillboardList::render(Pipeline& p)
{
	glDisable(GL_CULL_FACE);
	r_billboard.enableShader();
	
		r_billboard.setData("u_texture", 0, GL_TEXTURE_2D, m_textureID);
		r_billboard.setData("u_cameraPosition", p.getViewPosition());
		r_billboard.setData("u_billboardWidthScale", (float)2.0);
		r_billboard.setData("u_billboardHeightScale", (float)2.0);
		WorldObject::renderGroup(p, &r_billboard, &m_model);

	r_billboard.disableShader();
	glEnable(GL_CULL_FACE);
}


