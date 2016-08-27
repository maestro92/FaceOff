#ifndef CAMERA_H_
#define CAMERA_H_


#include "define.h"
#include "utility.h"
#include "pipeline.h"
#include "terrain/terrain.h"
using namespace std;

enum CameraType
{
	FIRST_PERSON_CAMERA,
	THIRD_PERSON_CAMERA
};

class Camera
{
    public:
        inline glm::vec3 getEyePoint();
		inline glm::vec3 getTargetPoint();
		
		// virtual void control(Pipeline& p);
		// virtual void control(Pipeline& p, Terrain* terrain);
		virtual void control();

		// virtual void computeNewTargetTransform(glm::vec3& pos, glm::mat4& rot);

		virtual void updateViewMatrix(Pipeline& p);

		void setMouseIn(bool b);
		bool getMouseIn();

		void setEyePoint(glm::vec3 eye);

		// First POV:	both direction will be the same
		// Third POV:	TargetDirection: aim direction of the player
		//				ViewDirection: view direction of the camera
		glm::vec3 getTargetDirection();
		glm::vec3 getViewDirection();

		glm::mat4 getViewMatrix();
		glm::mat4 getModelMatrix();

		virtual CameraType getCameraType() = 0;

        Camera();
        virtual ~Camera();

		glm::vec3 m_xAxis;
		glm::vec3 m_yAxis;
		glm::vec3 m_zAxis;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_modelMatrix;

		glm::vec3 m_targetXAxis;
		glm::vec3 m_targetYAxis;
		glm::vec3 m_targetZAxis;
		glm::mat4 m_targetRotation;

		glm::vec3 m_target;
        glm::vec3 m_eye;
		glm::vec3 m_eyeOffset;

        /// expressed in degrees
        float m_pitch;
        float m_yaw;	
	
		glm::vec3 getFirePosition();

	protected:
		bool m_mouseIn;
};

inline glm::vec3 Camera::getEyePoint()
{
    return m_eye;
}


inline void Camera::setEyePoint(glm::vec3 eye)
{
	m_eye = eye;
}


inline glm::vec3 Camera::getTargetPoint()
{
	return m_target;
}

inline void Camera::setMouseIn(bool b)
{
	m_mouseIn = b;
	if (m_mouseIn)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_ENABLE);
}


inline bool Camera::getMouseIn()
{
	return m_mouseIn;
}




#endif
