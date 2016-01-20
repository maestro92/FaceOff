
#include "terrain_model.h"


// constructor
TerrainModel::TerrainModel(int vertexCount, float size)
{
	int count = vertexCount * vertexCount;

	vector<VertexData> vertices;
	vector<unsigned int> indices;
	VertexData v;
	int vertexPointer = 0;

	for (int z = 0; z < vertexCount; z++)
	{
		for (int x = 0; x < vertexCount; x++)
		{
			VertexData v;
			v.m_position.x = (float)x / ((float)vertexCount - 1) * size;
			v.m_position.y = 0;
			v.m_position.z = (float)z / ((float)vertexCount - 1) * size;

			v.m_normal.x = 0;
			v.m_normal.y = 1;
			v.m_normal.z = 0;

			v.m_UV.x = (float)x / ((float)vertexCount - 1);
			v.m_UV.y = (float)z / ((float)vertexCount - 1);
			
			vertices.push_back(v);
		}
	}

	for (int gz = 0; gz < vertexCount - 1; gz++)
	{
		for (int gx = 0; gx < vertexCount - 1; gx++)
		{
			int topLeft = (gz * vertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * vertexCount) + gx;
			int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
	Mesh m(vertices, indices);
	m_meshes.push_back(m);
}

// Destructor
TerrainModel::~TerrainModel()
{

}
