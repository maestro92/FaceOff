#ifndef CAMERA_H_
#define CAMERA_H_


#include "define.h"
#include "utility.h"
#include "pipeline.h"
using namespace std;


class Camera
{
    public:
        inline glm::vec3 getEyePoint();
		virtual void control(Pipeline& p);
		Pipeline m_pipeline;
		void setMouseIn(bool b);

    protected:
        Camera();
        virtual ~Camera();
        void setEyePoint(glm::vec3 eye);


        float RAD_TO_DEGREE;
        float DEGREE_TO_RAD;

        glm::vec3 m_eye;

        /// expressed in degrees
        float m_pitch;
        float m_yaw;

		bool m_mouseIn;
};

inline glm::vec3 Camera::getEyePoint()
{
    return m_eye;
}
#endif
