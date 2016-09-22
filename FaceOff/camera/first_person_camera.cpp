#include "first_person_camera.h"

#include "utility.h"

const float CAMERA_ROTATION_SPEED = 0.5;
const float CAMERA_FORWARD_SPEED = 0.4;

FirstPersonCamera::FirstPersonCamera() : FirstPersonCamera(glm::vec3(0.0, 5.0, 0.0))
{ }

/*
Note that everything (xAxis, yAxis, zAxis and the viewMatrix)
derives from m_eye, m_pitch and m_yaw, so these three things are the most important three

refer to updateViewMatrix() and see how xAxis, yAxis, zAxis and viewMatrix are calculated from
m_eye, m_pitch and m_yaw
*/

FirstPersonCamera::FirstPersonCamera(glm::vec3 pos)
{
	m_xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewMatrix = glm::mat4(1.0);


	m_screenMidX = utl::SCREEN_WIDTH / 2;
	m_screenMidY = utl::SCREEN_HEIGHT / 2;
	m_target = pos;


	m_eyeOffset = glm::vec3(0.0, 5.0f, 0.0);

	m_eye = m_target + m_eyeOffset;

	m_freeMode = false;
	

	Pipeline p;
	updateViewMatrix(p);
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



void FirstPersonCamera::updateVelXZ(float dir, glm::vec3 & vel)
{
	float rad = (m_yaw + dir) * utl::DEGREE_TO_RADIAN;

	vel.x -= sin(rad) * CAMERA_FORWARD_SPEED;
	vel.z -= cos(rad) * CAMERA_FORWARD_SPEED;
}

void FirstPersonCamera::updateVelY(float dir, glm::vec3 & vel)
{
	float rad = (m_pitch + dir) * utl::DEGREE_TO_RADIAN;

	vel.y += sin(rad) * CAMERA_FORWARD_SPEED;
}




void FirstPersonCamera::control()
{
	if (m_mouseIn)
	{
		m_moveState.input.reset();
		m_moved = false;


		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy);

		float deltaYaw = CAMERA_ROTATION_SPEED * (m_screenMidX - tmpx);
		float deltaPitch = CAMERA_ROTATION_SPEED * (m_screenMidY - tmpy);

		m_moved = (deltaYaw != 0 || deltaPitch != 0);

		m_yaw += deltaYaw;
		m_pitch += deltaPitch;

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


void FirstPersonCamera::control(glm::vec3 & vel, bool canJump)
{
	if (m_mouseIn)
	{
		m_moveState.input.reset();
		m_moved = false;


		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy);

		float deltaYaw = CAMERA_ROTATION_SPEED * (m_screenMidX - tmpx);
		float deltaPitch = CAMERA_ROTATION_SPEED * (m_screenMidY - tmpy);

		m_moved = (deltaYaw != 0 || deltaPitch != 0);

		m_yaw += deltaYaw;
		m_pitch += deltaPitch;

		restrain();
		SDL_WarpMouse(m_screenMidX, m_screenMidY);
		Uint8* state = SDL_GetKeyState(NULL);

		if (state[SDLK_w])
		{
			updateVelXZ(0.0, vel);
			if (m_freeMode)
			{
				updateVelY(0.0, vel);
			}

			m_moveState.input.forward = true;
			m_moved = true;
		}

		else if (state[SDLK_s])
		{
			updateVelXZ(180.0, vel);
			if (m_freeMode)
			{
				updateVelY(180.0, vel);
			}

			m_moveState.input.back = true;
			m_moved = true;
		}

		if (state[SDLK_a])
		{
			updateVelXZ(90.0, vel);

			m_moveState.input.left = true;
			m_moved = true;
		}

		else if (state[SDLK_d])
		{
			updateVelXZ(270, vel);

			m_moveState.input.right = true;
			m_moved = true;
		}
		
		if (state[SDLK_SPACE] && canJump)
		{
			vel += glm::vec3(0.0, 175.0, 0.0) * utl::GRAVITY_CONSTANT;

			m_moveState.input.jump = true;
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



void FirstPersonCamera::processInput(Move move, glm::vec3 & vel, bool canJump)
{
	m_pitch = move.state.pitch;
	m_yaw = move.state.yaw;

	if (move.input.forward)
	{
		updateVelXZ(0.0, vel);
		if (m_freeMode)
		{
			updateVelY(0.0, vel);
		}
	}

	else if (move.input.back)
	{
		updateVelXZ(180.0, vel);
		if (m_freeMode)
		{
			updateVelY(180.0, vel);
		}
	}

	if (move.input.left)
	{
		updateVelXZ(90.0, vel);
	}

	else if (move.input.right)
	{
		updateVelXZ(270, vel);
	}

	if (move.input.jump && canJump)
	{
		vel += glm::vec3(0.0, 175.0, 0.0) * utl::GRAVITY_CONSTANT;
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

	// we want the model matrix for the target (well techinically, the transpose of it, because of how glsl works)
	float temp[16] = { m_targetXAxis[0], m_targetXAxis[1], m_targetXAxis[2], 0,
					   m_targetYAxis[0], m_targetYAxis[1], m_targetYAxis[2], 0,
					   m_targetZAxis[0], m_targetZAxis[1], m_targetZAxis[2], 0,
					   0, 0, 0, 1 };
	m_targetRotation = glm::make_mat4(temp);

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

