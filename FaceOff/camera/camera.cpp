#include "camera.h"



Camera::Camera()
{
    m_eye = glm::vec3(0.0, 0.0, 0.0);
    m_pitch = 0.0;
    m_yaw = 0.0;

 //   RAD_TO_DEGREE = 180.0 / M_PI;
 //   DEGREE_TO_RAD = M_PI / 180.0;

	m_mouseIn = false;
	m_targetRotation = glm::mat4(1.0);
}

Camera::~Camera()
{

}

void Camera::control(Pipeline& p)
{

}

void Camera::control(Pipeline& p, Terrain* terrain)
{

}


void Camera::controlCD()
{

}

/*
void Camera::computeNewTargetTransform(glm::vec3& pos, glm::mat4& rot)
{

}
*/

void Camera::updateViewMatrix(Pipeline& p)
{

}


glm::mat4 Camera::getViewMatrix()
{
	return m_viewMatrix;
}


glm::mat4 Camera::getModelMatrix()
{
	return m_modelMatrix;
}


glm::vec3 Camera::getTargetDirection()
{
	return -m_targetZAxis;
}
glm::vec3 Camera::getViewDirection()
{
	return -m_zAxis;
}


glm::vec3 Camera::getFirePosition()
{
	return m_eye;
}