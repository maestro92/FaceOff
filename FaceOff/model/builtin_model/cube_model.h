#ifndef CUBE_MODEL_H_
#define CUBE_MODEL_H_


#include "Model.h"

using namespace std;

class CubeModel : public Model
{
    public:
        CubeModel();
        ~CubeModel();

        /// textures
        void init();
};




#endif // EG_CUBE_MODEL_H_
