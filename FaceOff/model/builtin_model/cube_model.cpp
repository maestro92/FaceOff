#include "cube_model.h"


CubeModel::CubeModel() : CubeModel(glm::vec3(1.0, 1.0, 1.0), glm::vec3(-1.0, -1.0, -1.0))
{}

CubeModel::CubeModel(float xmax, float xmin,
					float ymax, float ymin,
					float zmax, float zmin) : CubeModel(glm::vec3(xmax, ymax, zmax), glm::vec3(xmin, ymin, zmin))
{}

CubeModel::CubeModel(glm::vec3 maxP, glm::vec3 minP)
{
	vector<unsigned int> indices;
	vector<VertexData> vertices;
	VertexData tmp;
	
	tmp.m_position = glm::vec3(minP.x, maxP.y, maxP.z); tmp.m_color = glm::vec3(1.0, 0.0, 0.0);
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(minP.x, minP.y, maxP.z);	tmp.m_color = glm::vec3(0.0, 1.0, 0.0);
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(maxP.x, minP.y, maxP.z);	tmp.m_color = glm::vec3(0.0, 0.0, 1.0);
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(maxP.x, maxP.y, maxP.z);	tmp.m_color = glm::vec3(1.0, 1.0, 0.0);
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(minP.x, maxP.y, minP.z); tmp.m_color = glm::vec3(1.0, 0.0, 1.0);
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(minP.x, minP.y, minP.z);	tmp.m_color = glm::vec3(1.0, 0.0, 0.0);
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(maxP.x, minP.y, minP.z);	tmp.m_color = glm::vec3(1.0, 1.0, 1.0);
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(maxP.x, maxP.y, minP.z);	tmp.m_color = glm::vec3(0.0, 1.0, 1.0);
	vertices.push_back(tmp);
	
	//front face
	indices.push_back(0);   indices.push_back(2);	indices.push_back(1);
	indices.push_back(0);  	indices.push_back(3);   indices.push_back(2);

	//left face
	indices.push_back(2);	indices.push_back(3);	indices.push_back(6);
	indices.push_back(3);	indices.push_back(7);	indices.push_back(6);

	//back face
	indices.push_back(4);	indices.push_back(5);	indices.push_back(6);
	indices.push_back(4);	indices.push_back(7);	indices.push_back(6);

	//right face
	indices.push_back(0);	indices.push_back(1);	indices.push_back(5);
	indices.push_back(0);	indices.push_back(4);	indices.push_back(5);

	//top face
	indices.push_back(0);	indices.push_back(3);	indices.push_back(4);
	indices.push_back(3);	indices.push_back(4);	indices.push_back(7);

	//bottom face
	indices.push_back(1);	indices.push_back(2);	indices.push_back(6);
	indices.push_back(1);	indices.push_back(5);	indices.push_back(6);

	Mesh m(vertices, indices);
	m_meshes.push_back(m);
}

CubeModel::CubeModel(glm::vec3 maxP, glm::vec3 minP, glm::vec3 color)
{
	vector<unsigned int> indices;
	vector<VertexData> vertices;
	VertexData tmp;

	tmp.m_color = color;
	tmp.m_position = glm::vec3(minP.x, maxP.y, maxP.z); 
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(minP.x, minP.y, maxP.z);	
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(maxP.x, minP.y, maxP.z);	
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(maxP.x, maxP.y, maxP.z);	
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(minP.x, maxP.y, minP.z); 
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(minP.x, minP.y, minP.z);	
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(maxP.x, minP.y, minP.z);	
	vertices.push_back(tmp);

	tmp.m_position = glm::vec3(maxP.x, maxP.y, minP.z);	
	vertices.push_back(tmp);

	//front face
	indices.push_back(0);   indices.push_back(2);	indices.push_back(1);
	indices.push_back(0);  	indices.push_back(3);   indices.push_back(2);

	//left face
	indices.push_back(2);	indices.push_back(3);	indices.push_back(6);
	indices.push_back(3);	indices.push_back(7);	indices.push_back(6);

	//back face
	indices.push_back(4);	indices.push_back(5);	indices.push_back(6);
	indices.push_back(4);	indices.push_back(7);	indices.push_back(6);

	//right face
	indices.push_back(0);	indices.push_back(1);	indices.push_back(5);
	indices.push_back(0);	indices.push_back(4);	indices.push_back(5);

	//top face
	indices.push_back(0);	indices.push_back(3);	indices.push_back(4);
	indices.push_back(3);	indices.push_back(4);	indices.push_back(7);

	//bottom face
	indices.push_back(1);	indices.push_back(2);	indices.push_back(6);
	indices.push_back(1);	indices.push_back(5);	indices.push_back(6);

	Mesh m(vertices, indices);
	m_meshes.push_back(m);
}

CubeModel::~CubeModel()
{

}
/*
void CubeModel::init()
{

    vector<unsigned int> indices;
    vector<VertexData> vertices;
    VertexData tmp;

	tmp.m_position = glm::vec3(-1.0,1.0,1.0);   tmp.m_color = glm::vec3(1.0,0.0,0.0);
    vertices.push_back(tmp);

    tmp.m_position = glm::vec3(-1.0,-1.0,1.0);	tmp.m_color = glm::vec3(0.0,1.0,0.0);
    vertices.push_back(tmp);

    tmp.m_position = glm::vec3(1.0,-1.0,1.0);	tmp.m_color = glm::vec3(0.0,0.0,1.0);
    vertices.push_back(tmp);

    tmp.m_position = glm::vec3(1.0,1.0,1.0);	tmp.m_color = glm::vec3(1.0,1.0,0.0);
    vertices.push_back(tmp);

    tmp.m_position = glm::vec3(-1.0,1.0,-1.0); 	tmp.m_color = glm::vec3(1.0,0.0,1.0);
    vertices.push_back(tmp);

    tmp.m_position = glm::vec3(-1.0,-1.0,-1.0);	tmp.m_color = glm::vec3(1.0,0.0,0.0);
    vertices.push_back(tmp);

    tmp.m_position = glm::vec3(1.0,-1.0,-1.0);	tmp.m_color = glm::vec3(1.0,1.0,1.0);
    vertices.push_back(tmp);

    tmp.m_position = glm::vec3(1.0,1.0,-1.0);	tmp.m_color = glm::vec3(0.0,1.0,1.0);
    vertices.push_back(tmp);

    //front face
    indices.push_back(0);   indices.push_back(2);	indices.push_back(1);
    indices.push_back(0);  	indices.push_back(3);   indices.push_back(2);

    //left face
    indices.push_back(2);	indices.push_back(3);	indices.push_back(6);
    indices.push_back(3);	indices.push_back(7);	indices.push_back(6);

    //back face
    indices.push_back(4);	indices.push_back(5);	indices.push_back(6);
    indices.push_back(4);	indices.push_back(7);	indices.push_back(6);

    //right face
    indices.push_back(0);	indices.push_back(1);	indices.push_back(5);
    indices.push_back(0);	indices.push_back(4);	indices.push_back(5);

    //top face
    indices.push_back(0);	indices.push_back(3);	indices.push_back(4);
    indices.push_back(3);	indices.push_back(4);	indices.push_back(7);

    //bottom face
    indices.push_back(1);	indices.push_back(2);	indices.push_back(6);
    indices.push_back(1);	indices.push_back(5);	indices.push_back(6);

    Mesh m(vertices, indices);
    m_meshes.push_back(m);
}


*/

