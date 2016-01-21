#ifndef BILLBOARD_MODEL_H_
#define BILLBOARD_MODEL_H_


#include "model.h"

using namespace std;

class BillboardModel : public Model
{
	public:
		// constructor
		BillboardModel();

		BillboardModel(int w, int h, float gap);

		void createUniformFormation(int w, int h, float gap);

		void createRandFormation(int maxW, int maxH, int count);


		// Destructor
		~BillboardModel();

};


#endif
