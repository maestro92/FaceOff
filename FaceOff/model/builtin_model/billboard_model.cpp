
#include "billboard_model.h"


BillboardModel::BillboardModel() : BillboardModel(10, 10, 1.0)
{

}

// constructor
BillboardModel::BillboardModel(int w, int h, float gap)
{
	m_modelGeometry = GL_POINTS;
	createUniformFormation(w, h, gap);
}



void BillboardModel::createUniformFormation(int w, int h, float gap)
{
	m_meshes.clear();
	vector<VertexData> vertices;
	vector<unsigned int> indices;
	VertexData v;
	int index = 0;

	for (int z = 0; z < h; z++)
	{
		for (int x = 0; x < w; x++)
		{
			VertexData v;
			v.m_position.x = (float)(x * gap) + utl::randFloat(0, gap);
			v.m_position.y = 0.0f;
			v.m_position.z = (float)(z * gap) + utl::randFloat(0, gap);

			vertices.push_back(v);
			indices.push_back(index++);
		}
	}

	Mesh m(vertices, indices);
	m_meshes.push_back(m);
}

void BillboardModel::createRandFormation(int maxW, int maxH, int count)
{
	m_meshes.clear();
	vector<VertexData> vertices;
	vector<unsigned int> indices;
	VertexData v;

	int index = 0;
	for (int i = 0; i < count; i++)
	{
		VertexData v;
		v.m_position.x = utl::randFloat(0, maxW);
		v.m_position.y = 0;
		v.m_position.z = utl::randFloat(0, maxH);

		vertices.push_back(v);
		indices.push_back(index++);
	}


	Mesh m(vertices, indices);
	m_meshes.push_back(m);
}



// Destructor
BillboardModel::~BillboardModel()
{

}
