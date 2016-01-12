#include "camera.h"



Camera::Camera()
{
    m_eye = glm::vec3(0.0, 0.0, 0.0);
    m_pitch = 0.0;
    m_yaw = 0.0;

    RAD_TO_DEGREE = 180.0 / M_PI;
    DEGREE_TO_RAD = M_PI / 180.0;
}

Camera::~Camera()
{

}

void Camera::setEyePoint(glm::vec3 eye)
{
    m_eye = eye;
}

