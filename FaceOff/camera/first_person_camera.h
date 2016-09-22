#ifndef FIRST_PERSON_CAMERA_H_
#define FIRST_PERSON_CAMERA_H_

#include "utility.h"
#include "camera.h"


class FirstPersonCamera : public Camera
{
    public:
        FirstPersonCamera();
		FirstPersonCamera(glm::vec3 pos);
		~FirstPersonCamera();

		virtual void control();
		virtual void control(glm::vec3& vel, bool canJump);

		void setFreeMode(bool b);
		void updateViewMatrix(Pipeline& p);
		inline void setEyePoint(glm::vec3 eye);
		virtual void processInput(Move move);
		virtual void processInput(Move move, glm::vec3& vel, bool canJump);

	private:
        int m_screenMidX;
        int m_screenMidY;

		bool m_freeMode;

        void restrain();
        void updatePosXZ(float dir);
        void updatePosY(float dir);

		void updateVelXZ(float dir, glm::vec3 & vel);
		void updateVelY(float dir, glm::vec3 & vel);
		
		glm::vec3 getFirePosition();
		
		virtual CameraType getCameraType();

};








#endif


