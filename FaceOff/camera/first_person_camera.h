#ifndef FIRST_PERSON_CAMERA_H_
#define FIRST_PERSON_CAMERA_H_

#include "utility.h"
#include "camera.h"


class FirstPersonCamera : public Camera
{
    public:
        FirstPersonCamera();
        ~FirstPersonCamera();

        void control(Pipeline& p);
		void control(Pipeline& p, Terrain* terrain);


    private:

        int m_screenMidX;
        int m_screenMidY;

        void restrain();
        void updatePosXZ(float dir);
        void updatePosY(float dir);


		void updatePipeline(Pipeline& p);
		void updatePipelineTranslation(Pipeline& p);
		void updatePipelineRotation(Pipeline& p);


    //    bool m_mouseIn;
};
#endif
