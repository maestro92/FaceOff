#include "camera.h"



Camera::Camera()
{
    m_eye = glm::vec3(0.0, 0.0, 0.0);
    m_pitch = 0.0;
    m_yaw = 0.0;

    RAD_TO_DEGREE = 180.0 / M_PI;
    DEGREE_TO_RAD = M_PI / 180.0;

	m_mouseIn = false;
}

Camera::~Camera()
{

}

void Camera::control(Pipeline& p)
{

}

void Camera::setEyePoint(glm::vec3 eye)
{
    m_eye = eye;
}

void Camera::setMouseIn(bool b)
{
	m_mouseIn = b;
	if (m_mouseIn)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_ENABLE);
}


glm::mat4 Camera::getViewMatrix()
{
	return m_viewMatrix;
}


glm::mat4 Camera::getModelMatrix()
{
	return m_modelMatrix;
}

glm::vec3 Camera::getViewDirection()
{
	return -m_zAxis;
}