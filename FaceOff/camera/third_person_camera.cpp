#include "third_person_camera.h"


ThirdPersonCamera::ThirdPersonCamera()
{
    m_springConstant  = DEFAULT_SPRING_CONSTANT;
    m_dampingConstant = DEFAULT_DAMPING_CONSTANT;
    m_enableSpringSystem = true;
	m_mouseIn = true;

	m_eyeOffset = glm::vec3(0.0, 5.0f, 0.0);

	m_target    = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis     = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis     = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis     = glm::vec3(0.0f, 0.0f, 1.0f);
    m_viewMatrix = glm::mat4(1.0);

    glm::vec3 eye_p     = glm::vec3(80.0, 320.0, 0.0);
    glm::vec3 target_p  = glm::vec3(0.0, 5.0, 0.0);
    glm::vec3 up_p      = glm::vec3(0.0, 1.0, 0.0);

    glm::vec2 hori = glm::vec2(eye_p.x, eye_p.z);

    m_pitch = atan( (eye_p.y - target_p.y) / glm::length(hori) );
 //	m_pitch = atan2((eye_p.y - target_p.y), glm::length(hori));
	m_pitch *= utl::RADIAN_TO_DEGREE;

    m_yaw = atan( -eye_p.x / eye_p.z );
//	m_yaw = atan2(-eye_p.x, eye_p.z);
	m_yaw *= utl::RADIAN_TO_DEGREE;

    lookAt(eye_p, target_p, up_p);

	m_targetXAxis = m_xAxis;
	m_targetYAxis = glm::vec3(0.0, 1.0, 0.0);
	m_targetZAxis = glm::cross(m_targetXAxis, m_targetYAxis);

	m_idealViewMatrix = m_viewMatrix;
	setMouseIn(false);
}



ThirdPersonCamera::~ThirdPersonCamera()
{

}

void ThirdPersonCamera::lookAt(glm::vec3& eye, glm::vec3& target, glm::vec3& up)
{
    m_eye = eye;
    m_target = target;

    m_zAxis = eye - target;
    m_offsetDistance = glm::length(m_zAxis);
    m_zAxis = glm::normalize(m_zAxis);

    m_xAxis = glm::cross(up, m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);

    float q_x = -glm::dot(m_xAxis, m_eye);
    float q_y = -glm::dot(m_yAxis, m_eye);
    float q_z = -glm::dot(m_zAxis, m_eye);

    /// this is the transpose of our view matrix, since openGL is column based
    float temp[16] = {m_xAxis.x, m_yAxis.x, m_zAxis.x, 0,
                      m_xAxis.y, m_yAxis.y, m_zAxis.y, 0,
                      m_xAxis.z, m_yAxis.z, m_zAxis.z, 0,
                      q_x,       q_y,       q_z,       1};

    m_viewMatrix = glm::make_mat4(temp);
}



void ThirdPersonCamera::controlCD()
{
	float pitchChange = 0.0f;
	float yawChange = 0.0f;
	m_forwardSpeed = 0.0f;


	// SDL_ShowCursor(SDL_DISABLE);
	Uint8* state = SDL_GetKeyState(NULL);

	if (state[SDLK_w])
		m_forwardSpeed = BALL_FORWARD_SPEED;

	if (state[SDLK_s])
		m_forwardSpeed = -BALL_FORWARD_SPEED;

	if (state[SDLK_a])
		yawChange = BALL_HEADING_SPEED;

	if (state[SDLK_d])
		yawChange = -BALL_HEADING_SPEED;

	if (state[SDLK_m])
		m_offsetDistance += 1;

	if (state[SDLK_n])
		m_offsetDistance -= 1;


	if (state[SDLK_l])
		pitchChange += 0.1;

	if (state[SDLK_k])
		pitchChange -= 0.1;


	m_pitch += pitchChange;


//	utl::debug("m_pitch", m_pitch);

	if (m_forwardSpeed < 0)
		m_yaw -= yawChange;
	else
		m_yaw += yawChange;

	if (m_yaw > 360)
		m_yaw -= 360;

	if (m_yaw < -360)
		m_yaw += 360;

	updateTarget();
}


void ThirdPersonCamera::updateTarget()
{
	m_targetXAxis = m_xAxis;
	m_targetYAxis = glm::vec3(0.0, 1.0, 0.0);
	m_targetZAxis = glm::cross(m_targetXAxis, m_targetYAxis);
	m_targetZAxis = glm::normalize(m_targetZAxis);

	float temp[16] = {  m_targetXAxis[0], m_targetXAxis[1], m_targetXAxis[2], 0,
						m_targetYAxis[0], m_targetYAxis[1], m_targetYAxis[2], 0,
						m_targetZAxis[0], m_targetZAxis[1], m_targetZAxis[2], 0,
						0,				  0,				0,				  1 };
	m_targetRotation = glm::make_mat4(temp);
	m_targetRotation *= glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);
	m_target += -m_targetZAxis * m_forwardSpeed;
}


void ThirdPersonCamera::updateEyePos()
{
	float horiDist = computeHoriDist();
	float vertDist = computeVertDist();

	m_eye.y = m_target.y + vertDist;

	float rad = m_yaw * utl::DEGREE_TO_RADIAN;
	m_eye.x = m_target.x - horiDist * sin(rad);
	m_eye.z = m_target.z - horiDist * cos(rad);
}


void ThirdPersonCamera::updateViewMatrix(Pipeline& p)
{
    updateEyePos();

    glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
    lookAt(m_eye, m_target, up);



	m_viewMatrix = m_viewMatrix * glm::translate(0.0f, -m_eyeOffset.y, 0.0f);

	m_eye.y += m_eyeOffset.y;

    p.setMatrixMode(VIEW_MATRIX);
    p.addMatrix(m_viewMatrix);

	p.setViewPosition(m_eye);
}


float ThirdPersonCamera::computeHoriDist()
{
	return m_offsetDistance * cos(m_pitch * utl::DEGREE_TO_RADIAN);
}

float ThirdPersonCamera::computeVertDist()
{
	return m_offsetDistance * sin(m_pitch * utl::DEGREE_TO_RADIAN);
}


CameraType ThirdPersonCamera::getCameraType()
{
	return THIRD_PERSON_CAMERA;
}


glm::vec3 ThirdPersonCamera::getFirePosition()
{
	return m_target;
}



/*
void ThirdPersonCamera::control(Pipeline& p)
{
float pitchChange = 0.0f;
float yawChange = 0.0f;
m_forwardSpeed = 0.0f;


// SDL_ShowCursor(SDL_DISABLE);
Uint8* state=SDL_GetKeyState(NULL);

if(state[SDLK_w])
m_forwardSpeed = BALL_FORWARD_SPEED;

if(state[SDLK_s])
m_forwardSpeed = -BALL_FORWARD_SPEED;

if(state[SDLK_a])
yawChange = BALL_HEADING_SPEED;

if(state[SDLK_d])
yawChange = -BALL_HEADING_SPEED;

if (state[SDLK_m])
m_offsetDistance += 1;

if (state[SDLK_n])
m_offsetDistance -= 1;


if (state[SDLK_l])
pitchChange += 0.1;

if (state[SDLK_k])
pitchChange -= 0.1;


m_pitch += pitchChange;


if (m_forwardSpeed < 0)
m_yaw -= yawChange;
else
m_yaw += yawChange;

if(m_yaw > 360)
m_yaw -= 360;

if(m_yaw < -360)
m_yaw += 360;

/// update the character first
/// When moving backwards invert rotations to match direction of travel.
/// When we drive backwards, our car actually turn leftwards when we steer rightwards
updateTarget();
updateViewMatrix(p);

}
*/



/*
void ThirdPersonCamera::computeNewTargetTransform(glm::vec3& pos, glm::mat4& rot)
{
pos = m_target;

glm::vec3 xAxis = m_xAxis;
glm::vec3 yAxis = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 zAxis = glm::cross(xAxis, yAxis);
zAxis = glm::normalize(zAxis);

float temp[16] = {  m_targetXAxis[0], m_targetXAxis[1], m_targetXAxis[2], 0,
m_targetYAxis[0], m_targetYAxis[1], m_targetYAxis[2], 0,
m_targetZAxis[0], m_targetZAxis[1], m_targetZAxis[2], 0,
0,				  0,				0,				  1 };

rot = glm::make_mat4(temp);
rot *= glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f);
pos += -zAxis * m_forwardSpeed;
}

*/