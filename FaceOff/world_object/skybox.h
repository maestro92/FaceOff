#ifndef SKY_BOX_H_
#define SKY_BOX_H_

#include "utility.h"
#include "world_object.h"
#include "cube_model.h"
#include "camera.h"

class SkyBox : public WorldObject
{
    public:
        SkyBox();
        SkyBox(string* files);
        void init(string* files);
        CubeModel m_cubeModel;

        virtual void render(Pipeline& m_pipeline);

        virtual inline void setRotation(glm::mat4 rot);
      //  virtual inline void setMatrix(Camera* camera);

    private:
        GLuint m_staticCubeMapID;
        Pipeline p;

        Renderer r_skybox;
};


inline void SkyBox::setRotation(glm::mat4 rot)
{

    float temp[16] = {rot[0][0], rot[0][1], rot[0][2], 0.0,
                      rot[1][0], rot[1][1], rot[1][2], 0.0,
                      rot[2][0], rot[2][1], rot[2][2], 0.0,
                      0.0,       0.0,       0.0,       1.0};
    m_rotation = glm::make_mat4(temp);

}

#endif
