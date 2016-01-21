
#include "terrain_model.h"


const float DEFAULT_SIZE = 800;
const float VERTEX_COUNT = 128;
const float MAX_HEIGHT = 40;
const float MAX_PIXEL_COLOUR = 256 * 256 * 256;

// constructor
TerrainModel::TerrainModel()
{
	vector<VertexData> vertices;
	vector<unsigned int> indices;
	VertexData v;
	int vertexPointer = 0;

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
	
	int maxGray, minGray;
	
	getMinMax(image, &maxGray, &minGray);
	utl::debug("maxGray", maxGray);
	utl::debug("minGray", minGray);

	int vertexCount = image->h;

	for (int z = 0; z < vertexCount; z++)
	{
		for (int x = 0; x < vertexCount; x++)
		{
			VertexData v;
			v.m_position.x = (float)x / ((float)vertexCount - 1) * DEFAULT_SIZE;
			v.m_position.y = getHeight(x, z, image, maxGray, minGray);
			v.m_position.z = (float)z / ((float)vertexCount - 1) * DEFAULT_SIZE;

			// http://stackoverflow.com/questions/13983189/opengl-how-to-calculate-normals-in-a-terrain-height-grid
			
			v.m_normal = computeNormal(x, z, image, maxGray, minGray);
			
			/*
			v.m_normal.x = 0;
			v.m_normal.y = 1;
			v.m_normal.z = 0;
			*/

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

	if (h == minGray)
	{
		cout << "here" << endl;
	}

	return h;

	// cout << "h " << h << ", newh" << newh << endl;

	// utl::debug("h", newh);
	/*
	static int count = 0;
	if (x < 125 && x > 120 && y < 125 && y > 120 && ((int)r)>115 )
//	if (((int)r)<0 && count < 10)
	{
		utl::debug("height", height);	
		cout << (int)r << " " << (int)g << " " << (int)b << " " << (int)a << endl;
		count++;
	}
	

	
//	height += MAX_PIXEL_COLOUR / 2.0f; 
//	height /= MAX_PIXEL_COLOUR / 2.0f;
	if (x < 5 && y < 5)
		utl::debug("final height", h);

	h /= 256.0f;
	
	h *= MAX_HEIGHT;

	if (x < 5 && y < 5)
		utl::debug("final height", h);

	*/

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
