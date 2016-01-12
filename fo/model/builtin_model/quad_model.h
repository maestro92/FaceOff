#ifndef QUAD_MODEL_H_
#define QUAD_MODEL_H_


#include "Model.h"

using namespace std;

class QuadModel : public Model
{
    public:
        // constructor
        QuadModel();
        QuadModel(int w, int h);

        QuadModel(int w, int h,
                  float uv_x, float uv_y, float uv_w);

        QuadModel(float l, float r,
                  float b, float t);

        QuadModel(float l, float r,
                  float b, float t,
                  float uv_x, float uv_y, float uv_w);

        // Destructor
        ~QuadModel();

        /// textures
        void initIndices(vector<unsigned int>& indices);
        void init(float l, float r,
                  float b, float t,
                  float cx, float cy, float cw);
};


#endif
