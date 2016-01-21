#ifndef TERRAIN_MODEL_H_
#define TERRAIN_MODEL_H_


#include "model.h"

using namespace std;

class TerrainModel : public Model
{
	public:
		// constructor
		TerrainModel();

		TerrainModel(string heightMap);

		float getHeight(int x, int y, SDL_Surface* image, int maxGray, int minGray);
		void getMinMax(SDL_Surface* image, int* maxGray, int* minGray);


		glm::vec3 computeNormal(int x, int y, SDL_Surface* image, int maxGray, int minGray);


		// Destructor
		~TerrainModel();
};


#endif
