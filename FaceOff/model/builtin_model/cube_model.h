#ifndef CUBE_MODEL_H_
#define CUBE_MODEL_H_


#include "Model.h"

using namespace std;

class CubeModel : public Model
{
    public:
        CubeModel();

		CubeModel(float xmax, float xmin,
					float ymax, float ymin,
					float zmax, float zmin);

		CubeModel(glm::vec3 maxP, glm::vec3 minP);

		CubeModel(glm::vec3 maxP, glm::vec3 minP, glm::vec3 color);

        ~CubeModel();

        /// textures
        void init();
};




#endif // EG_CUBE_MODEL_H_
