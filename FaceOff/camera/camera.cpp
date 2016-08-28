#include "camera.h"



Camera::Camera()
{
    m_eye = glm::vec3(0.0, 0.0, 0.0);
    m_pitch = 0.0;
    m_yaw = 0.0;

	m_mouseIn = false;
	m_targetRotation = glm::mat4(1.0);

	m_moved = false;
}

Camera::~Camera()
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

bool Camera::hasMoved()
{
	return m_moved;
}

Move Camera::getMoveState()
{
	return m_moveState;
}


