#include "third_person_camera.h"


ThirdPersonCamera::ThirdPersonCamera()
{
    m_springConstant  = DEFAULT_SPRING_CONSTANT;
    m_dampingConstant = DEFAULT_DAMPING_CONSTANT;
    m_enableSpringSystem = true;

	m_target    = glm::vec3(0.0f, 0.0f, 0.0f);
	m_xAxis     = glm::vec3(1.0f, 0.0f, 0.0f);
	m_yAxis     = glm::vec3(0.0f, 1.0f, 0.0f);
	m_zAxis     = glm::vec3(0.0f, 0.0f, 1.0f);
    m_viewMatrix = glm::mat4(1.0);

    glm::vec3 eye_p     = glm::vec3(-80.0, 30.0, 0.0);
    glm::vec3 target_p  = glm::vec3(0.0, 4.0, 0.0);
    glm::vec3 up_p      = glm::vec3(0.0, 1.0, 0.0);

    glm::vec2 hori = glm::vec2(eye_p.x, eye_p.z);

    m_pitch = atan( (eye_p.y - target_p.y) / glm::length(hori) );
    m_pitch *= RAD_TO_DEGREE;

    m_yaw = atan( -eye_p.x / eye_p.z );
    m_yaw *= RAD_TO_DEGREE;

    lookAt(eye_p, target_p, up_p);
    m_idealViewMatrix = m_viewMatrix;
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


float ThirdPersonCamera::computeHoriDist()
{
    return m_offsetDistance * cos(m_pitch * DEGREE_TO_RAD);
}

float ThirdPersonCamera::computeVertDist()
{
    return m_offsetDistance * sin(m_pitch * DEGREE_TO_RAD);
}

glm::vec3 ThirdPersonCamera::computeEyePos()
{
    glm::vec3 pos;
    float horiDist = computeHoriDist();
    float vertDist = computeVertDist();

    pos.y = m_target.y + vertDist;

	float rad = m_yaw * DEGREE_TO_RAD;
    pos.x = m_target.x - horiDist * sin(rad);
    pos.z = m_target.z - horiDist * cos(rad);

    return pos;
}

void ThirdPersonCamera::control(Pipeline& m_pipeline)
{
    float pitchChange = 0.0f;
    float yawChange = 0.0f;
    float forwardSpeed = 0.0f;


    SDL_ShowCursor(SDL_DISABLE);
    Uint8* state=SDL_GetKeyState(NULL);

    if(state[SDLK_w])
        forwardSpeed = BALL_FORWARD_SPEED;

    if(state[SDLK_s])
        forwardSpeed = -BALL_FORWARD_SPEED;

    if(state[SDLK_a])
        yawChange = BALL_HEADING_SPEED;

    if(state[SDLK_d])
        yawChange = -BALL_HEADING_SPEED;


    /// update the character first
    /// When moving backwards invert rotations to match direction of travel.
    /// When we drive backwards, our car actually turn leftwards when we steer rightwards
//    m_characterObject.setVelocity(0.0f, 0.0f, forwardSpeed);
//    m_characterObject.setAngularVelocity(0.0f, yawChange, 0.0f);

//    updateCharacterObject(0.0f, yawChange, 0.0f);

//    setTarget(m_characterObject.getPosition());

    m_pitch += pitchChange;


    if(forwardSpeed < 0)
        m_yaw -= yawChange;
    else
        m_yaw += yawChange;

    if(m_yaw > 360)
        m_yaw -= 360;

    if(m_yaw < -360)
        m_yaw += 360;

    updateViewMatrix(m_pipeline);

}


void ThirdPersonCamera::updateViewMatrix(Pipeline& m_pipeline)
{
    m_eye = computeEyePos();

    glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
    lookAt(m_eye, m_target, up);

    m_viewMatrix = m_viewMatrix * glm::translate(0.0f, -4.0f, 0.0f);

    m_pipeline.setMatrixMode(VIEW_MATRIX);
    m_pipeline.addMatrix(m_viewMatrix);
}
