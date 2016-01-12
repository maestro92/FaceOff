#ifndef FIRST_PERSON_CAMERA_H_
#define FIRST_PERSON_CAMERA_H_

#include "utility.h"
#include "camera.h"


class FirstPersonCamera : public Camera
{
    public:
        FirstPersonCamera();
        FirstPersonCamera(int midX, int midY);
        ~FirstPersonCamera();

        void setMouseIn(bool b);
        void control(Pipeline& m_pipeline);
    private:

        int m_screenMidX;
        int m_screenMidY;

        void restrain();
        void updatePosXZ(float dir);
        void updatePosY(float dir);


		void updatePipeline(Pipeline& m_pipeline);
		void updateTranslation(Pipeline& m_pipeline);
        void updateRotation(Pipeline& m_pipeline);


        bool m_mouseIn;
};
#endif
