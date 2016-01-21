#include "first_person_camera.h"

#include "utility.h"

const float CAMERA_ROTATION_SPEED = 0.1;
const float CAMERA_FORWARD_SPEED = 0.1;

FirstPersonCamera::FirstPersonCamera()
{
	m_eye = glm::vec3(0.0, 5.0, 0.0);
	m_screenMidX = utl::SCREEN_WIDTH/2;
	m_screenMidY = utl::SCREEN_HEIGHT/2;
}


FirstPersonCamera::~FirstPersonCamera()
{

}


void FirstPersonCamera::updatePosXZ(float dir)
{
	float rad = (m_yaw + dir) * utl::DEGREE_TO_RADIAN;
	m_eye.x -= sin(rad) * CAMERA_FORWARD_SPEED;
	m_eye.z -= cos(rad) * CAMERA_FORWARD_SPEED;
}

void FirstPersonCamera::updatePosY(float dir)
{
	float rad = (m_pitch + dir) * utl::DEGREE_TO_RADIAN;
	m_eye.y += sin(rad) * CAMERA_FORWARD_SPEED;
}


void FirstPersonCamera::control(Pipeline& p)
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

 //   utl::debug("position", m_eye);

    updatePipeline(p);
}

void FirstPersonCamera::updatePipeline(Pipeline& p)
{
	p.setMatrixMode(VIEW_MATRIX);

	p.rotateX(m_pitch);
	p.rotateY(m_yaw);
	p.translate(m_eye.x, m_eye.y, m_eye.z);

	glm::mat4 view = p.getViewMatrix();

	m_viewMatrix = p.getViewMatrix();
	m_xAxis = glm::vec3(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
	m_yAxis = glm::vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
	m_zAxis = glm::vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);

	// m_pipeline = p;
	// m_pipeline.setViewPosition(m_eye);

	p.setViewPosition(m_eye);
	
}

void FirstPersonCamera::updatePipelineTranslation(Pipeline& p)
{
	p.setMatrixMode(VIEW_MATRIX);
	p.translate(m_eye.x, m_eye.y, m_eye.z);

	// m_pipeline = p;
	// m_pipeline.setViewPosition(m_eye);

	p.setViewPosition(m_eye);
}

void FirstPersonCamera::updatePipelineRotation(Pipeline& p)
{
	p.setMatrixMode(VIEW_MATRIX);
	p.rotateX(m_pitch);
	p.rotateY(m_yaw);

	// m_pipeline = p;


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
/*
void FirstPersonCamera::setMouseIn(bool b)
{
    m_mouseIn = b;
	if (m_mouseIn)
		SDL_ShowCursor(SDL_DISABLE);
    else
        SDL_ShowCursor(SDL_ENABLE);
}
*/