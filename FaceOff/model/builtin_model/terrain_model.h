#ifndef TERRAIN_MODEL_H_
#define TERRAIN_MODEL_H_


#include "model.h"

using namespace std;

class TerrainModel : public Model
{
	public:
		// constructor
		TerrainModel();

		TerrainModel(int vertexCount, float size);

		// Destructor
		~TerrainModel();
};


#endif
