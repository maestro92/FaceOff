#ifndef BILLBOARD_MODEL_H_
#define BILLBOARD_MODEL_H_


#include "model.h"
#include "terrain/terrain.h"

using namespace std;

class BillboardModel : public Model
{
	public:
		// constructor
		BillboardModel();

		BillboardModel(int w, int h, float gap);
		
		void createUniformFormation(int w, int h, float gap);
		void createUniformFormation(int w, int h, float gap, Terrain* terrain);

		void createRandFormation(int maxW, int maxH, int count);
		void createRandFormation(int maxW, int maxH, int count, Terrain* terrain);


		// Destructor
		~BillboardModel();

};


#endif
