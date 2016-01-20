
#include "billboard_model.h"


BillboardModel::BillboardModel() : BillboardModel(10, 10, 1.0)
{

}

// constructor
BillboardModel::BillboardModel(int w, int h, float gap)
{
	m_modelGeometry = GL_POINTS;

	vector<VertexData> vertices;
	vector<unsigned int> indices;
	VertexData v;
	int vertexPointer = 0;

	for (int z = 0; z < h; z++)
	{
		for (int x = 0; x < w; x++)
		{
			VertexData v;
			// v.m_position = glm::vec3((float)x, 0.0f, (float)z);
			v.m_position = glm::vec3((float)(x * gap), 0.0f, (float)(z * gap));

			vertices.push_back(v);
		}
	}

	int index = 0;
	for (int gz = 0; gz < w; gz++)
	{
		for (int gx = 0; gx < h; gx++)
		{
			indices.push_back(index++);
		}
	}
	Mesh m(vertices, indices);
	m_meshes.push_back(m);
}

// Destructor
BillboardModel::~BillboardModel()
{

}
