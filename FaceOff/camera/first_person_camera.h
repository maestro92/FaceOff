#ifndef FIRST_PERSON_CAMERA_H_
#define FIRST_PERSON_CAMERA_H_

#include "utility.h"
#include "camera.h"


class FirstPersonCamera : public Camera
{
    public:
        FirstPersonCamera();
        ~FirstPersonCamera();

		void control();
		void setFreeMode(bool b);
		void updateViewMatrix(Pipeline& p);

private:

        int m_screenMidX;
        int m_screenMidY;

		bool m_freeMode;

        void restrain();
        void updatePosXZ(float dir);
        void updatePosY(float dir);

		
		glm::vec3 getFirePosition();
		
		virtual CameraType getCameraType();
};
#endif
