#if 1

#include "pipeline.h"
#include "glm/gtx/quaternion.hpp"

Pipeline::Pipeline()
{
	m_modelMatrix.push_back(glm::mat4(1.0));
	m_viewMatrix.push_back(glm::mat4(1.0));
	m_projectionMatrix.push_back(glm::mat4(1.0));

	m_modelViewMatrix = glm::mat4(1.0);
	m_modelViewProjectionMatrix = glm::mat4(1.0);
	m_normalMatrix = glm::mat3(1.0);
	matricesReady = true;
	m_curMatrixMode = PROJECTION_MATRIX;

	m_viewPosition = glm::vec3(0.0, 0.0, 0.0);
}

void Pipeline::loadIdentity()
{
	if(m_curMatrixMode==MODEL_MATRIX || m_curMatrixMode==VIEW_MATRIX)
	{
		m_modelMatrix[m_modelMatrix.size()-1]=glm::mat4(1.0);
		m_viewMatrix[m_viewMatrix.size()-1]=glm::mat4(1.0);
		m_viewPosition = glm::vec3(0.0, 0.0, 0.0);
	}
	else
		m_projectionMatrix[m_projectionMatrix.size()-1] = glm::mat4(1.0);
	matricesReady=false;
}


void Pipeline::reset()
{
    m_modelMatrix.clear();
	m_viewMatrix.clear();
	m_viewPosition = glm::vec3(0.0, 0.0, 0.0);
	m_projectionMatrix.clear();

	m_modelMatrix.push_back(glm::mat4(1.0));
	m_viewMatrix.push_back(glm::mat4(1.0));
	m_projectionMatrix.push_back(glm::mat4(1.0));

	m_modelViewMatrix=glm::mat4(1.0);
	m_modelViewProjectionMatrix=glm::mat4(1.0);
	m_normalMatrix=glm::mat3(1.0);
	matricesReady=true;
	m_curMatrixMode=0;

}


bool Pipeline::setMatrixMode(int m)
{
	if(m == MODEL_MATRIX || m == VIEW_MATRIX || m == PROJECTION_MATRIX)
		m_curMatrixMode = m;
	return true;
}




void Pipeline::setViewPosition(glm::vec3 pos)
{
	m_viewPosition = pos;
}

glm::vec3 Pipeline::getViewPosition()
{
	return m_viewPosition;
}

		//modelview
void Pipeline::translate(float x,float y,float z)
{
	if(m_curMatrixMode==MODEL_MATRIX)
		m_modelMatrix[m_modelMatrix.size()-1] *= glm::translate(x, y, z);
	
	else if (m_curMatrixMode == VIEW_MATRIX)
		m_viewMatrix[m_viewMatrix.size() - 1] *= glm::translate(-x, -y, -z);
	
	matricesReady=false;
}


void Pipeline::translate(glm::vec3 transMatrix)
{
	if(m_curMatrixMode==MODEL_MATRIX)
		m_modelMatrix[m_modelMatrix.size() - 1] *= glm::translate(transMatrix.x, transMatrix.y, transMatrix.z);

	else if(m_curMatrixMode==VIEW_MATRIX)
		m_viewMatrix[m_viewMatrix.size() - 1] *= glm::translate(-transMatrix.x, -transMatrix.y, -transMatrix.z);
	
	matricesReady=false;
}


void Pipeline::scale(float v)
{
    scale(glm::vec3(v,v,v));
}

void Pipeline::scale(float x,float y,float z)
{
    scale(glm::vec3(x,y,z));
}

void Pipeline::scale(glm::vec3 v)
{
	if(m_curMatrixMode==MODEL_MATRIX)
		m_modelMatrix[m_modelMatrix.size()-1] *= glm::scale(v.x, v.y, v.z);

	else if(m_curMatrixMode==VIEW_MATRIX)
		m_viewMatrix[m_viewMatrix.size()-1] *= glm::scale(v.x, v.y, v.z);
	
	matricesReady=false;
}


void Pipeline::rotateX(float angle)
{
    rotate(angle, 1.0f, 0.0f, 0.0f);
}

void Pipeline::rotateY(float angle)
{
    rotate(angle, 0.0f, 1.0f, 0.0f);
}

void Pipeline::rotateZ(float angle)
{
    rotate(angle, 0.0f, 0.0f, 1.0f);
}


void Pipeline::rotate(float angle, float x, float y, float z)
{
	if(m_curMatrixMode==MODEL_MATRIX)
		m_modelMatrix[m_modelMatrix.size()-1] *= glm::rotate(angle,x,y,z);

	else if(m_curMatrixMode==VIEW_MATRIX)
		m_viewMatrix[m_viewMatrix.size()-1] *= glm::rotate(-angle,x,y,z);

	matricesReady=false;
}


void Pipeline::rotate(glm::quat q_rotation)
{
    glm::mat4 RotationMatrix = glm::toMat4(q_rotation);

	if(m_curMatrixMode==MODEL_MATRIX)
		m_modelMatrix[m_modelMatrix.size()-1] *= RotationMatrix;

	else if(m_curMatrixMode==VIEW_MATRIX)
		m_viewMatrix[m_viewMatrix.size()-1] *= RotationMatrix;

	matricesReady=false;
}


void Pipeline::addMatrix(glm::mat4 m)
{
    if(m_curMatrixMode==MODEL_MATRIX)
		m_modelMatrix[m_modelMatrix.size()-1] *= m;

	else if(m_curMatrixMode==VIEW_MATRIX)
		m_viewMatrix[m_viewMatrix.size()-1] *= m;

	matricesReady=false;
}


//projection
void Pipeline::ortho(float left,float right,float bottom,float top,float near,float far) //==glOrtho
{
	m_projectionMatrix[m_projectionMatrix.size()-1]=glm::ortho(left,right,bottom,top,near,far);
	matricesReady=false;
}

void Pipeline::perspective(float angle,float aRatio,float near,float far) //==gluPerspective
{
	m_projectionMatrix[m_projectionMatrix.size()-1]=glm::perspective(angle,aRatio,near,far);
	matricesReady=false;
}

//getters
glm::mat4 Pipeline::getModelMatrix()
{
	return m_modelMatrix[m_modelMatrix.size()-1];
}

glm::mat4 Pipeline::getViewMatrix()
{
	return m_viewMatrix[m_viewMatrix.size()-1];
}

glm::mat4 Pipeline::getModelViewMatrix()
{
	if(!matricesReady)
		return getViewMatrix()*getModelMatrix();
	else
		return m_modelViewMatrix;
}

glm::mat4 Pipeline::getProjectionMatrix()
{
	return m_projectionMatrix[m_projectionMatrix.size()-1];
}

glm::mat4 Pipeline::getModelViewProjectionMatrix()
{
	if(!matricesReady)
		m_modelViewProjectionMatrix = getProjectionMatrix() * getViewMatrix()* getModelMatrix();

    return m_modelViewProjectionMatrix;
}


glm::mat4 Pipeline::getModelViewProjectionMatrixForInstancedRendering()
{
	if(!matricesReady)
		m_modelViewProjectionMatrix = getProjectionMatrix()*getViewMatrix()*getModelMatrix();
    return m_modelViewProjectionMatrix;
}


void Pipeline::pushMatrix()      // glPushMatrix()
{
    glm::mat4 matrix = glm::mat4(1.0);

    if ( m_curMatrixMode == MODEL_MATRIX )
    {
        if(m_modelMatrix.size() < 1)
            return;
        matrix = m_modelMatrix[m_modelMatrix.size()-1];
        m_modelMatrix.push_back(matrix);
    }

    else if ( m_curMatrixMode == VIEW_MATRIX )
    {
        if(m_viewMatrix.size() < 1)
            return;
        matrix = m_viewMatrix[m_modelMatrix.size()-1];
        m_viewMatrix.push_back(matrix);
    }
    else
    {
        if(m_projectionMatrix.size() < 1)
            return;
        matrix = m_projectionMatrix[m_projectionMatrix.size()-1];
        m_projectionMatrix.push_back(matrix);
    }

}

void Pipeline::popMatrix()       // glPopMatrix()
{
    if ( m_curMatrixMode == MODEL_MATRIX )
    {
        if(m_modelMatrix.size()>1)
            m_modelMatrix.pop_back();
    }

    else if ( m_curMatrixMode == VIEW_MATRIX)
    {
        if(m_viewMatrix.size()>1)
            m_viewMatrix.pop_back();
    }
    else
    {
        if(m_projectionMatrix.size()>1)
            m_projectionMatrix.pop_back();
    }
}



void Pipeline::updateLightMatrix(glm::mat4 lightModel, glm::mat4 lightView, glm::mat4 lightProjection)
{
    m_lightModelMatrix = lightModel;
    m_lightViewMatrix = lightView;
    m_lightProjectionMatrix = lightProjection;

}

//GLSL

void Pipeline::updateShadowMatrix()
{
    m_shadowMatrix = LIGHT_BIAS_MATRIX * m_lightProjectionMatrix * m_lightViewMatrix * getModelMatrix();
}


glm::mat4 Pipeline::getShadowMatrix()
{
    updateShadowMatrix();
    return m_shadowMatrix;
}

void Pipeline::RotateForReflection(int face)
{
    switch (face)
    {
        case POSITIVE_X:
            RotatePositiveX();
            break;

        case NEGATIVE_X:
            RotateNegativeX();
            break;

        case POSITIVE_Y:
            RotatePositiveY();
            break;

        case NEGATIVE_Y:
            RotateNegativeY();
            break;

        case POSITIVE_Z:
            RotatePositiveZ();
            break;

        case NEGATIVE_Z:
            RotateNegativeZ();
            break;
        default:
            break;
    };
}


void Pipeline::RotatePositiveX()
{
    rotateZ(180);
    rotateY(-90);
}

void Pipeline::RotateNegativeX()
{
    rotateZ(180);
    rotateY(90);
}

void Pipeline::RotatePositiveY()
{
    rotateX(90);
}

void Pipeline::RotateNegativeY()
{
    rotateX(-90);
}

void Pipeline::RotatePositiveZ()
{
    rotateZ(180);
    rotateY(180);
}

void Pipeline::RotateNegativeZ()
{
    rotateZ(180);
}

#endif
