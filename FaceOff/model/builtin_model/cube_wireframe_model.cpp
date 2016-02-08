#include "cube_wireframe_model.h"


CubeWireFrameModel::CubeWireFrameModel() : CubeWireFrameModel(1, -1, 1, -1, 1, -1)
{}

/*
CubeWireFrameModel::CubeWireFrameModel(glm::vec3 maxXYZ, glm::vec3 minXYZ) : CubeWireFrameModel(maxXYZ.x, minXYZ.x, 
																								maxXYZ.y, minXYZ.y, 
																								maxXYZ.z, minXYZ.z)
{}
*/
CubeWireFrameModel::CubeWireFrameModel(AABB aabb) : CubeWireFrameModel(aabb.max.x, aabb.min.x,
																	   aabb.max.y, aabb.min.y,
																	   aabb.max.z, aabb.min.z)
{}



CubeWireFrameModel::CubeWireFrameModel(float xmax, float xmin, 
										float ymax, float ymin,
										float zmax, float zmin)
{
	m_modelGeometry = GL_LINES;

	vector<unsigned int> indices;
	vector<VertexData> vertices;
	VertexData tmp;



	// posZ's face
	tmp.m_position = glm::vec3(xmin, ymin, zmax);	tmp.m_color = RED;
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(xmax, ymin, zmax);		tmp.m_color = RED;
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(xmin, ymax, zmax);		tmp.m_color = RED;
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(xmax, ymax, zmax);		tmp.m_color = RED;
	vertices.push_back(tmp);

	// negZ's face
	tmp.m_position = glm::vec3(xmin, ymin, zmin);	tmp.m_color = RED;
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(xmax, ymin, zmin);	tmp.m_color = RED;
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(xmin, ymax, zmin);	tmp.m_color = RED;
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(xmax, ymax, zmin);		tmp.m_color = RED;
	vertices.push_back(tmp);


	// posZ's face
	indices.push_back(0);   indices.push_back(1);
	indices.push_back(0);	indices.push_back(2);
	indices.push_back(2);	indices.push_back(3);
	indices.push_back(1);	indices.push_back(3);

	// posX's face
	indices.push_back(1);   indices.push_back(3);
	indices.push_back(1);	indices.push_back(5);
	indices.push_back(3);	indices.push_back(7);
	indices.push_back(5);	indices.push_back(7);

	// posY's face
	indices.push_back(2);   indices.push_back(3);
	indices.push_back(2);	indices.push_back(6);
	indices.push_back(6);	indices.push_back(7);
	indices.push_back(3);	indices.push_back(7);

	// negZ's face
	indices.push_back(4);   indices.push_back(5);
	indices.push_back(4);	indices.push_back(6);
	indices.push_back(6);	indices.push_back(7);
	indices.push_back(7);	indices.push_back(5);

	// negX's face
	indices.push_back(0);   indices.push_back(2);
	indices.push_back(0);	indices.push_back(4);
	indices.push_back(2);	indices.push_back(6);
	indices.push_back(4);	indices.push_back(6);

	// negY's face
	indices.push_back(0);   indices.push_back(4);
	indices.push_back(0);	indices.push_back(1);
	indices.push_back(4);	indices.push_back(5);
	indices.push_back(1);	indices.push_back(5);

	Mesh m(vertices, indices);
	m_meshes.push_back(m);
}


CubeWireFrameModel::~CubeWireFrameModel()
{

}




