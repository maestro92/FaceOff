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
		void setEyePoint(glm::vec3 eye);

		glm::vec3 getViewDirection();
		glm::mat4 getViewMatrix();
		glm::mat4 getModelMatrix();



        Camera();
        virtual ~Camera();

		glm::vec3 m_xAxis;
		glm::vec3 m_yAxis;
		glm::vec3 m_zAxis;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_modelMatrix;

        float RAD_TO_DEGREE;
        float DEGREE_TO_RAD;

        glm::vec3 m_eye;

        /// expressed in degrees
        float m_pitch;
        float m_yaw;	
	
	protected:
		bool m_mouseIn;
};

inline glm::vec3 Camera::getEyePoint()
{
    return m_eye;
}
#endif
