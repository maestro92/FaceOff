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
		

		virtual void control() = 0;
		virtual void control(glm::vec3& vel, bool canJump) = 0;

		virtual void updateViewMatrix(Pipeline& p) = 0;

		virtual void processInput(Move move) = 0;
		virtual void processInput(Move move, glm::vec3 & vel, bool canJump) = 0;

		void setMouseIn(bool b);
		bool getMouseIn();

		void setEyePoint(glm::vec3 eye);

		void setPitch(float pitch);
		void setYaw(float yaw);
		void setRoll(float roll);

		float getPitch();
		float getYaw();
		float getRoll();

		// First POV:	both direction will be the same
		// Third POV:	TargetDirection: aim direction of the player
		//				ViewDirection: view direction of the camera
		glm::vec3 getTargetDirection();
		glm::vec3 getViewDirection();

		glm::mat4 getViewMatrix();
		glm::mat4 getModelMatrix();


		bool hasMoved();
		Move getMoveState();
		


		virtual CameraType getCameraType() = 0;

        Camera();
        virtual ~Camera();

		// camera's three axes
		glm::vec3 m_xAxis;
		glm::vec3 m_yAxis;
		glm::vec3 m_zAxis;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_modelMatrix;

		// the target's three axes
		glm::vec3 m_targetXAxis;
		glm::vec3 m_targetYAxis;
		glm::vec3 m_targetZAxis;
		glm::mat4 m_targetRotation;

		glm::vec3 m_target;
        glm::vec3 m_eye;
		glm::vec3 m_eyeOffset;
		glm::vec3 m_velocity;

		bool m_moved;
		Move m_moveState;

        /// expressed in degrees
        float m_pitch;
        float m_yaw;	
		float m_roll;

		glm::vec3 getFirePosition();

	protected:
		bool m_mouseIn;
};

inline glm::vec3 Camera::getEyePoint()
{
    return m_eye;
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


inline void Camera::setEyePoint(glm::vec3 eye)
{
	m_eye = eye;
}


inline bool Camera::getMouseIn()
{
	return m_mouseIn;
}


inline void Camera::setPitch(float pitch)
{
	m_pitch = pitch;
}

inline void Camera::setYaw(float yaw)
{
	m_yaw = yaw;
}

inline void Camera::setRoll(float roll)
{
	m_roll = roll;
}

inline float Camera::getPitch()
{
	return m_pitch;
}

inline float Camera::getYaw()
{
	return m_yaw;
}

inline float Camera::getRoll()
{
	return m_roll;
}

#endif
