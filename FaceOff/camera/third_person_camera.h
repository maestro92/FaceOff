#ifndef THIRD_PERSON_CAMERA_H_
#define THIRD_PERSON_CAMERA_H_

#include "camera.h"
#include "renderer.h"

const float BALL_FORWARD_SPEED = 0.25f;
const float BALL_HEADING_SPEED = 0.25f;
const float BALL_ROLLING_SPEED = 10.0f;
const float MAX_CAMERA_OFFSET = 200.0f;
const float MIN_CAMERA_OFFSET = 20.0f;

const float DEFAULT_SPRING_CONSTANT = 16.0f;
const float DEFAULT_DAMPING_CONSTANT = 8.0f;

class ThirdPersonCamera : public Camera
{
    public:
        ThirdPersonCamera();
        ThirdPersonCamera(Pipeline& m_pipeline);
        ~ThirdPersonCamera();

        void lookAt(glm::vec3& eye, glm::vec3& target, glm::vec3& up);

        void control(Pipeline& p);

		void updateTarget();
		void updateEyePos();
		void updateViewMatrix(Pipeline& p);


        void increaseOffsetDistance();
        void decreaseOffsetDistance();


        void enableSpringSystem(bool enableSpringSystem);
        void setSpringConstant(float springConstant);

        void render(Pipeline& m_pipeline, Renderer* renderer, int pass);

        float computeHoriDist();
        float computeVertDist();

		virtual CameraType getCameraType();

    private:
//        glm::vec3 m_target;
        float m_offsetDistance;
		

		
		float m_forwardSpeed;


        /// spring system
        glm::mat4 m_idealViewMatrix;
        float m_springConstant;
        float m_dampingConstant;
        bool  m_enableSpringSystem;
        const float CAMERA_ZOOM_DEGREE = 3.0f;


        /// everything related to the character
        float m_characterHeight;
        glm::vec3 c_right;
        glm::vec3 c_up;
        glm::vec3 c_forward;
        glm::vec3 m_velocity;

        void setViewMatrixRotation(glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis);
        void setViewMatrixPosition(glm::vec3 eye_pos);
};

#endif
