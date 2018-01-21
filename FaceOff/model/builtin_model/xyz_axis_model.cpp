
#include "xyz_axis_model.h"

XYZAxisModel::XYZAxisModel()
{
    m_modelGeometry = GL_LINES;
	m_type = ModelType::xyzAxis;

    std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
	VertexData v;

    float scale = 50;

    /// X axis
    v.m_position = glm::vec3(0.0,0.0,0.0);
	v.m_color = COLOR_RED;
    vertices.push_back(v);

    v.m_position = glm::vec3(scale,0.0,0.0);
    v.m_color = COLOR_RED;
    vertices.push_back(v);

    /// Y axis
    v.m_position = glm::vec3(0.0,0.0,0.0);
	v.m_color = COLOR_GREEN;
    vertices.push_back(v);;

    v.m_position = glm::vec3(0.0,scale,0.0);
    v.m_color = COLOR_GREEN;
    vertices.push_back(v);;

    /// Z axis
    v.m_position = glm::vec3(0.0,0.0,0.0);
    v.m_color = COLOR_BLUE;
    vertices.push_back(v);;

    v.m_position = glm::vec3(0.0,0.0,scale);
    v.m_color = COLOR_BLUE;
    vertices.push_back(v);;


    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);

 //   worldAxis = new mesh(&axisVertices, &axisIndices);
    Mesh m(vertices, indices);
    m_meshes.push_back(m);
}

XYZAxisModel::~XYZAxisModel()
{
}
