#include "cube_model.h"


CubeModel::CubeModel()
{
    init();
}

CubeModel::~CubeModel()
{

}


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
    indices.push_back(0);   indices.push_back(1);	indices.push_back(2);
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

    EG_Mesh m(vertices, indices);
    m_meshes.push_back(m);
}




