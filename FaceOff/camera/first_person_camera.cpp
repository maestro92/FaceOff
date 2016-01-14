#include "first_person_camera.h"

const float CAMERA_ROTATION_SPEED = 0.1;
const float CAMERA_FORWARD_SPEED = 0.1;

FirstPersonCamera::FirstPersonCamera()
{

}

FirstPersonCamera::FirstPersonCamera(int midX, int midY) : Camera()
{

    m_mouseIn = false;
    m_screenMidX = midX;
    m_screenMidY = midY;
}

FirstPersonCamera::~FirstPersonCamera()
{

}


void FirstPersonCamera::updatePosXZ(float dir)
{
	float rad = (m_yaw + dir) * DEGREE_TO_RAD;
	m_eye.x -= sin(rad) * CAMERA_FORWARD_SPEED;
	m_eye.z -= cos(rad) * CAMERA_FORWARD_SPEED;
}

void FirstPersonCamera::updatePosY(float dir)
{
	float rad = (m_pitch + dir) * DEGREE_TO_RAD;
	m_eye.y += sin(rad) * CAMERA_FORWARD_SPEED;
}


void FirstPersonCamera::control(Pipeline& m_pipeline)
{
    if(m_mouseIn)
    {
        int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);

		m_yaw += CAMERA_ROTATION_SPEED * (m_screenMidX - tmpx);
		m_pitch += CAMERA_ROTATION_SPEED * (m_screenMidY - tmpy);

		restrain();
        SDL_WarpMouse(m_screenMidX, m_screenMidY);
		Uint8* state = SDL_GetKeyState(NULL);

		if(state[SDLK_w])
		{
            updatePosXZ(0.0);
			updatePosY(0.0);
		}

		else if(state[SDLK_s])
		{
            updatePosXZ(180.0);
			updatePosY(180.0);
		}

		if(state[SDLK_a])
			updatePosXZ(90.0);

		else if(state[SDLK_d])
			updatePosXZ(270);
    }

 //   Utility::debug("position", m_eye);

    updatePipeline(m_pipeline);
}

void FirstPersonCamera::updatePipeline(Pipeline& m_pipeline)
{
    m_pipeline.setMatrixMode(VIEW_MATRIX);
    m_pipeline.rotateX(m_pitch);
    m_pipeline.rotateY(m_yaw);
    m_pipeline.translate(m_eye.x, m_eye.y, m_eye.z);
}

void FirstPersonCamera::updateTranslation(Pipeline& m_pipeline)
{
    m_pipeline.setMatrixMode(VIEW_MATRIX);
    m_pipeline.translate(m_eye.x, m_eye.y, m_eye.z);
}

void FirstPersonCamera::updateRotation(Pipeline& m_pipeline)
{
    m_pipeline.setMatrixMode(VIEW_MATRIX);
    m_pipeline.rotateX(m_pitch);
    m_pipeline.rotateY(m_yaw);
}


void FirstPersonCamera::restrain()
{
	if(m_pitch > 90)
		m_pitch = 90;

	if(m_pitch < -90)
		m_pitch = -90;

	if(m_yaw < 0.0)
		m_yaw += 360.0;

	if(m_yaw > 360.0)
		m_yaw -= 360;
}

void FirstPersonCamera::setMouseIn(bool b)
{
    m_mouseIn = b;
    if(m_mouseIn)
        SDL_ShowCursor(SDL_DISABLE);
    else
        SDL_ShowCursor(SDL_ENABLE);
}
