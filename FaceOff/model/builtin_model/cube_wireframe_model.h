#ifndef CUBE_WIRE_FRAME_MODEL_H_
#define CUBE_WIRE_FRAME_MODEL_H_


#include "Model.h"

using namespace std;

class CubeWireFrameModel : public Model
{
	public:
		CubeWireFrameModel();
		CubeWireFrameModel(float xmax, float xmin,
			float ymax, float ymin,
			float zmax, float zmin);

		~CubeWireFrameModel();
};




#endif // EG_CUBE_MODEL_H_
