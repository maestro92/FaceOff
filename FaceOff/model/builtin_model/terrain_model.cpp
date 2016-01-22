
#include "terrain_model.h"


float TerrainModel::DEFAULT_SIZE = 800;
float TerrainModel::VERTEX_COUNT = 128;
float TerrainModel::MAX_HEIGHT = 40;
float TerrainModel::MAX_PIXEL_COLOUR = 256 * 256 * 256;

// constructor
TerrainModel::TerrainModel()
{
	m_heights.resize(VERTEX_COUNT, vector<float>(VERTEX_COUNT, 0));

	vector<VertexData> vertices;
	vector<unsigned int> indices;
	VertexData v;

	m_gridSquareSize = DEFAULT_SIZE / ((float)VERTEX_COUNT - 1);
	m_sideVertexCount = VERTEX_COUNT;

	for (int z = 0; z < VERTEX_COUNT; z++)
	{
		for (int x = 0; x < VERTEX_COUNT; x++)
		{
			VertexData v;
			v.m_position.x = (float)x / ((float)VERTEX_COUNT - 1) * DEFAULT_SIZE;
			v.m_position.y = 0;
			v.m_position.z = (float)z / ((float)VERTEX_COUNT - 1) * DEFAULT_SIZE;

			v.m_normal.x = 0;
			v.m_normal.y = 1;
			v.m_normal.z = 0;

			v.m_UV.x = (float)x / ((float)VERTEX_COUNT - 1);
			v.m_UV.y = (float)z / ((float)VERTEX_COUNT - 1);
			
			vertices.push_back(v);
		}
	}

	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
	{
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
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



TerrainModel::TerrainModel(string heightMap)
{
	vector<VertexData> vertices;
	vector<unsigned int> indices;
	VertexData v;

	SDL_Surface* image = utl::loadRawImage(heightMap);
		
	int vertexCount = image->h;
	m_heights.resize(vertexCount, vector<float>(vertexCount, 0));
	m_gridSquareSize = DEFAULT_SIZE / ((float)vertexCount - 1);
	m_sideVertexCount = vertexCount;

	int maxGray, minGray;
	getMinMax(image, &maxGray, &minGray);

	for (int z = 0; z < vertexCount; z++)
	{
		for (int x = 0; x < vertexCount; x++)
		{
			VertexData v;
			v.m_position.x = (float)x / ((float)vertexCount - 1) * DEFAULT_SIZE;
			
			float height = getHeight(x, z, image, maxGray, minGray);
			m_heights[z][x] = height;
			v.m_position.y = height;
			v.m_position.z = (float)z / ((float)vertexCount - 1) * DEFAULT_SIZE;

			// http://stackoverflow.com/questions/13983189/opengl-how-to-calculate-normals-in-a-terrain-height-grid
			
			v.m_normal = computeNormal(x, z, image, maxGray, minGray);
			
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



glm::vec3 TerrainModel::computeNormal(int x, int y, SDL_Surface* image, int maxGray, int minGray)
{
	float heightL = getHeight(x - 1, y, image, maxGray, minGray);
	float heightR = getHeight(x + 1, y, image, maxGray, minGray);
	float heightD = getHeight(x, y - 1, image, maxGray, minGray);
	float heightU = getHeight(x, y + 1, image, maxGray, minGray);

	glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);
	normal = glm::normalize(normal);
	return normal;
}

float TerrainModel::getHeight(int x, int y, SDL_Surface* image, int maxGray, int minGray)
{
	if (x < 0 || x >= image->w || y < 0 || y >= image->h)
		return 0;

	Uint32 height = utl::getpixel(image, x, y);

	Uint8 r, g, b, a;

	SDL_GetRGBA(height, image->format, &r, &g, &b, &a);
	
	float h = (float)((int)r);

	h = (((float)(h - minGray) / (float)(maxGray - minGray))-0.5) * MAX_HEIGHT;

	return h;
}




void TerrainModel::getMinMax(SDL_Surface* image, int* maxGray, int* minGray)
{
	int maxG = 0;
	int minG = 256;

	for (int y = 0; y < image->h; y++)
	{
		for (int x = 0; x < image->w; x++)
		{
			Uint32 pixel = utl::getpixel(image, x, y);
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixel, image->format, &r, &g, &b, &a);


			minG = min(minG, (int)r);
			maxG = max(maxG, (int)r);
		}
	}
	(*maxGray) = maxG;
	(*minGray) = minG;
}

// Destructor
TerrainModel::~TerrainModel()
{

}


float TerrainModel::getGridSquareSize()
{
	return m_gridSquareSize;
}


float TerrainModel::getHeightAt(int x, int z)
{
	return m_heights[z][x];
}


int TerrainModel::getSideGridCount()
{
	return m_heights.size() - 1;
}


float TerrainModel::getHeightAt(float posX, float posZ)
{
	int gridX = (int)(floor(posX / m_gridSquareSize));
	int gridZ = (int)(floor(posZ / m_gridSquareSize));

	if (gridX < 0 || gridX >= m_sideVertexCount || gridZ < 0 || gridZ >= m_sideVertexCount)
		return 0;

	// find out where inside the gird
	float xCoord = fmod(posX, m_gridSquareSize) / m_gridSquareSize;
	float zCoord = fmod(posZ, m_gridSquareSize) / m_gridSquareSize;

	float h = 0;
	if (xCoord <= (1 - zCoord))
	{
		h = utl::barycentricInterpolation(
			glm::vec3(0, m_heights[gridZ][gridX], 0),
			glm::vec3(1, m_heights[gridZ][gridX + 1], 0),
			glm::vec3(0, m_heights[gridZ + 1][gridX], 1),
			glm::vec2(xCoord, zCoord));
	}
	else
	{
		h = utl::barycentricInterpolation(
			glm::vec3(1, m_heights[gridZ][gridX + 1], 0),
			glm::vec3(1, m_heights[gridZ + 1][gridX + 1], 1),
			glm::vec3(0, m_heights[gridZ + 1][gridX], 1),
			glm::vec2(xCoord, zCoord));
	}
	return h;
}