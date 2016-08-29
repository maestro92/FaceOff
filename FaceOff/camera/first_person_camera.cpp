#include "first_person_camera.h"

#include "utility.h"

const float CAMERA_ROTATION_SPEED = 0.5;
const float CAMERA_FORWARD_SPEED = 0.3;

FirstPersonCamera::FirstPersonCamera()
{
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewMatrix = glm::mat4(1.0);


	m_screenMidX = utl::SCREEN_WIDTH/2;
	m_screenMidY = utl::SCREEN_HEIGHT/2;
	m_target = glm::vec3(0.0, 5.0f, 0.0);


	m_eyeOffset = glm::vec3(0.0, 5.0f, 0.0);

	m_eye = m_target + m_eyeOffset; 

	m_freeMode = false;
}


FirstPersonCamera::~FirstPersonCamera()
{

}


void FirstPersonCamera::updatePosXZ(float dir)
{
	float rad = (m_yaw + dir) * utl::DEGREE_TO_RADIAN;

	m_target.x -= sin(rad) * CAMERA_FORWARD_SPEED;
	m_target.z -= cos(rad) * CAMERA_FORWARD_SPEED;

}

void FirstPersonCamera::updatePosY(float dir)
{
	float rad = (m_pitch + dir) * utl::DEGREE_TO_RADIAN;

	m_target.y += sin(rad) * CAMERA_FORWARD_SPEED;
}



void FirstPersonCamera::control()
{
	if (m_mouseIn)
	{
		m_moveState.input.reset();
		m_moved = false;


		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy);

		m_yaw += CAMERA_ROTATION_SPEED * (m_screenMidX - tmpx);
		m_pitch += CAMERA_ROTATION_SPEED * (m_screenMidY - tmpy);

		restrain();
		SDL_WarpMouse(m_screenMidX, m_screenMidY);
		Uint8* state = SDL_GetKeyState(NULL);

		if (state[SDLK_w])
		{
			updatePosXZ(0.0);
			if (m_freeMode)
			{
				updatePosY(0.0);
			}

			m_moveState.input.forward = true;
			m_moved = true;
		}

		else if (state[SDLK_s])
		{
			updatePosXZ(180.0);
			if (m_freeMode)
			{
				updatePosY(180.0);
			}

			m_moveState.input.back = true;
			m_moved = true;
		}

		if (state[SDLK_a])
		{
			updatePosXZ(90.0);
	
			m_moveState.input.left = true;
			m_moved = true;
		}

		else if (state[SDLK_d])
		{
			updatePosXZ(270);

			m_moveState.input.right = true;
			m_moved = true;
		}

		m_moveState.state.pitch = m_pitch;
		m_moveState.state.yaw = m_yaw;
	}
}


void FirstPersonCamera::processInput(Move move)
{
	m_pitch = move.state.pitch;
	m_yaw = move.state.yaw;

	if (move.input.forward)
	{
		updatePosXZ(0.0);
		if (m_freeMode)
		{
			updatePosY(0.0);
		}
	}

	else if (move.input.back)
	{
		updatePosXZ(180.0);
		if (m_freeMode)
		{
			updatePosY(180.0);
		}
	}

	if (move.input.left)
	{
		updatePosXZ(90.0);
	}

	else if (move.input.right)
	{
		updatePosXZ(270);
	}
}


void FirstPersonCamera::setFreeMode(bool b)
{
	m_freeMode = b;
}


void FirstPersonCamera::updateViewMatrix(Pipeline& p)
{
	p.setMatrixMode(VIEW_MATRIX);
	p.loadIdentity();


	m_eye = m_target + m_eyeOffset;


	p.rotateX(m_pitch);
	p.rotateY(m_yaw);
	p.translate(m_eye.x, m_eye.y, m_eye.z);

	glm::mat4 view = p.getViewMatrix();

	m_viewMatrix = p.getViewMatrix();
	m_xAxis = glm::vec3(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
	m_yAxis = glm::vec3(m_viewMatrix[0][1], m_viewMatrix[1][1], m_viewMatrix[2][1]);
	m_zAxis = glm::vec3(m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2]);


	m_targetXAxis = m_xAxis;
	m_targetYAxis = m_yAxis;
	m_targetZAxis = m_zAxis;


	float temp[16] = { m_targetXAxis[0], m_targetXAxis[1], m_targetXAxis[2], 0,
					   m_targetYAxis[0], m_targetYAxis[1], m_targetYAxis[2], 0,
					   m_targetZAxis[0], m_targetZAxis[1], m_targetZAxis[2], 0,
					   0, 0, 0, 1 };
	m_targetRotation = glm::make_mat4(temp);
	m_targetRotation *= glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);

	p.setViewPosition(m_eye);
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


CameraType FirstPersonCamera::getCameraType()
{
	return FIRST_PERSON_CAMERA;
}


glm::vec3 FirstPersonCamera::getFirePosition()
{
	return m_eye;
}

