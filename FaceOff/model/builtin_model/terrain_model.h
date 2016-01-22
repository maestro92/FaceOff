#ifndef TERRAIN_MODEL_H_
#define TERRAIN_MODEL_H_


#include "model.h"

using namespace std;

class TerrainModel : public Model
{

	static float DEFAULT_SIZE; 
	static float VERTEX_COUNT;
	static float MAX_HEIGHT;
	static float MAX_PIXEL_COLOUR; 


	public:
		// constructor
		TerrainModel();

		TerrainModel(string heightMap);

		float getHeightAt(int x, int z);
		float getGridSquareSize();
		int getSideGridCount();


		float getHeightAt(float posX, float posZ);


	private:
		int m_sideVertexCount;
		float m_gridSquareSize;
		vector<vector<float>> m_heights;

		float getHeight(int x, int y, SDL_Surface* image, int maxGray, int minGray);
		void getMinMax(SDL_Surface* image, int* maxGray, int* minGray);

		glm::vec3 computeNormal(int x, int y, SDL_Surface* image, int maxGray, int minGray);



		// Destructor
		~TerrainModel();
};


#endif
