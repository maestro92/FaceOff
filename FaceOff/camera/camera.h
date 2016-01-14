#ifndef CAMERA_H_
#define CAMERA_H_


#include "define.h"
#include "utility"
#include "pipeline.h"
using namespace std;


class Camera
{
    public:
        inline glm::vec3 getEyePoint();

    protected:
        Camera();
        virtual ~Camera();
        void setEyePoint(glm::vec3 eye);


        float RAD_TO_DEGREE;
        float DEGREE_TO_RAD;

        glm::vec3 m_eye;

        /// expressed in degrees
        float m_pitch;
        float m_yaw;
};

inline glm::vec3 Camera::getEyePoint()
{
    return m_eye;
}
#endif
