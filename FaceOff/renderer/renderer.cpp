
#include "Renderer.h"


#define ERROR_CHECK 0

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::setShader(Shader* s)
{
    m_shader = s;
	initMatricesUniLocs(s, m_matricesUniLocs);
}


/*
void Renderer::addDataPairArray(const char* name, int dataType, int count)
{
	for (int i = 0; i < count; i++)
	{
		char Name[128];
		int j = sprintf(Name, "%s", name);
		j += sprintf(Name + j, "[%d]", i);

//		cout << Name << endl;
		addDataPair(RENDER_PASS1, Name, dataType);
	}
}
*/


void Renderer::addDataPair(string name, int dataType)
{	
	switch (dataType)
	{
		case DP_BOOL:
		{
			m_dataPairs.push_back(new BoolDataPair(name));
			break;
		}
		case DP_INT:
		{
			m_dataPairs.push_back(new IntDataPair(name));
			break;
		}
		case DP_FLOAT:
		{
			m_dataPairs.push_back(new FloatDataPair(name));
			break;
		}
		case DP_VEC2:
		{
			m_dataPairs.push_back(new Vec2DataPair(name));
			break;
		}
		case DP_VEC3:
		{
			m_dataPairs.push_back(new Vec3DataPair(name));
			break;
		}
		case DP_VEC4:
		{
			m_dataPairs.push_back(new Vec4DataPair(name));
			break;
		}
		case DP_MAT3:
		{
			m_dataPairs.push_back(new Mat3DataPair(name));
			break;
		}
		case DP_MAT4:
		{
			m_dataPairs.push_back(new Mat4DataPair(name));
			break;
		}
	}

	int varEnum = m_dataPairs.size() - 1;
	m_dataPairs[varEnum]->uniLoc = getUniLoc(m_shader, name.c_str());
	if (m_dataPairs[varEnum]->uniLoc == -1)
	{
		cout << m_shader->getVertexShaderName() << " " << name << " is not active" << endl;
	}
}





void Renderer::setData(int varEnum, bool value)
{
#if ERROR_CHECK
	errorCheck(name);
#endif
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::setData(int varEnum, int value)
{
#if ERROR_CHECK
	errorCheck(name);
#endif
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::setData(int varEnum, int value, GLuint target, GLuint textureId)
{
#if ERROR_CHECK
	errorCheck(name);
#endif

	glActiveTexture(GL_TEXTURE0 + (unsigned int)value);
	glBindTexture(target, textureId);

	m_textureUnitStack.push(GL_TEXTURE0 + (unsigned int)value);
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::setData(int varEnum, float value)
{
#if ERROR_CHECK
	errorCheck(name);
#endif
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::setData(int varEnum, glm::vec2 value)
{
#if ERROR_CHECK
	errorCheck(name);
#endif
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::setData(int varEnum, glm::vec3 value)
{
#if ERROR_CHECK
	errorCheck(name);
#endif
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::setData(int varEnum, glm::vec4 value)
{
#if ERROR_CHECK
	errorCheck(name);
#endif
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::setData(int varEnum, glm::mat3 value)
{
#if ERROR_CHECK
	errorCheck(name);
#endif
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::setData(int varEnum, glm::mat4 value)
{
#if ERROR_CHECK
	errorCheck(name);
#endif
	m_dataPairs[varEnum]->setValue(value);
}

void Renderer::errorCheck(int varEnum)
{
	if (varEnum > m_dataPairs.size())
	{
		cout << m_shader->getVertexShaderName() << endl;
		cout << "glUniLoc " << m_dataPairs[varEnum]->name << " is not found " << endl;
		exit(1);
	}

	if (m_dataPairs[varEnum] == NULL)
	{
		cout << m_shader->getVertexShaderName() << endl;
		cout << "glUniLoc " << m_dataPairs[varEnum]->name << " is not found " << endl;
		exit(1);
	}
}

/*
void Renderer::setDataArray(const char* name, int value, GLuint target, vector<GLuint> textureIds)
{
	for (int i = 0; i < textureIds.size(); i++)
	{
		char Name[128];
		int j = sprintf(Name, "%s", name);
		j += sprintf(Name + j, "[%d]", i);
		setData(Name, i + value, target, textureIds[i]);
	}
}
*/

void Renderer::printDataPairs()
{
	cout << m_shader->getVertexShaderName() << endl;
    for (int i=0; i<m_dataPairs.size(); i++)
    {
		cout << "	" << i << " : " << m_dataPairs[i]->name << endl;
    }
	
	cout << endl << endl;
}


GLuint Renderer::getUniLoc(Shader* s, const char* name)
{
    unsigned int shaderID = s->getProgramId();

    GLuint location = glGetUniformLocation(shaderID, name);

    if(location == -1)
    {
        cout << "Error in getUniLoc with " << name << endl;
    }
    return location;
}

void Renderer::setUniLocs()
{
	for (int i = 0; i < m_dataPairs.size(); i++)
	{
		m_dataPairs[i]->setUniLoc();
	}
}


void Renderer::setUniLocs(Pipeline& p)
{
	setUniLocs();

	if (!p.matricesReady)
	{
		p.m_modelViewMatrix = p.m_viewMatrix[p.m_viewMatrix.size() - 1] * p.m_modelMatrix[p.m_modelMatrix.size() - 1];
		p.m_modelViewProjectionMatrix = p.m_projectionMatrix[p.m_projectionMatrix.size() - 1] *
			p.m_viewMatrix[p.m_viewMatrix.size() - 1] *
			p.m_modelMatrix[p.m_modelMatrix.size() - 1];
		p.m_normalMatrix = glm::mat3(p.m_modelViewMatrix);
	}

	glUniformMatrix4fv(m_matricesUniLocs.modelViewProjection, 1, GL_FALSE, &p.m_modelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(m_matricesUniLocs.modelView, 1, GL_FALSE, &p.m_modelViewMatrix[0][0]);
	glUniformMatrix4fv(m_matricesUniLocs.model, 1, GL_FALSE, &p.m_modelMatrix[p.m_modelMatrix.size() - 1][0][0]);
	glUniformMatrix4fv(m_matricesUniLocs.view, 1, GL_FALSE, &p.m_viewMatrix[p.m_viewMatrix.size() - 1][0][0]);
	glUniformMatrix3fv(m_matricesUniLocs.normal, 1, GL_FALSE, &p.m_normalMatrix[0][0]);
	glUniform3f(m_matricesUniLocs.eyePoint, p.m_viewPosition.x, p.m_viewPosition.y, p.m_viewPosition.z);
}



void Renderer::enableShader()
{
	m_shader->useShader();
}

void Renderer::disableShader()
{
    m_shader->delShader();
    while(!m_textureUnitStack.empty())
    {
        glActiveTexture(m_textureUnitStack.top());
        glBindTexture(GL_TEXTURE_2D, 0);
        m_textureUnitStack.pop();
    }
}


/// http://glm.g-truc.net/0.9.2/api/a00132_source.html
/// source code of glm::vec4, you see the union
void Renderer::setUniLoc(BoolDataPair& dp)
{
	glUniform1i(dp.uniLoc, dp.value);
}

void Renderer::setUniLoc(IntDataPair& dp)
{
	glUniform1i(dp.uniLoc, dp.value);
}

void Renderer::setUniLoc(FloatDataPair& dp)
{
	glUniform1f(dp.uniLoc, dp.value);
}

void Renderer::setUniLoc(Vec2DataPair& dp)
{
	glUniform2f(dp.uniLoc, dp.value.x, dp.value.y);
}

void Renderer::setUniLoc(Vec3DataPair& dp)
{
	glUniform3f(dp.uniLoc, dp.value.x, dp.value.y, dp.value.z);
}

void Renderer::setUniLoc(Vec4DataPair& dp)
{
	glUniform4f(dp.uniLoc, dp.value.x, dp.value.y, dp.value.z, dp.value.w);
}

void Renderer::setUniLoc(Mat3DataPair& dp)
{
	glUniformMatrix3fv(dp.uniLoc, 1, GL_FALSE, &dp.value[0][0]);
}

void Renderer::setUniLoc(Mat4DataPair& dp, GLint transpose)
{
	glUniformMatrix4fv(dp.uniLoc, 1, transpose, &dp.value[0][0]);
}


bool Renderer::initMatricesUniLocs(Shader* s, MatricesUniLoc& Mat)
{
    Mat.modelViewProjection = getUniLoc( s, "u_modelViewProjMat");
    Mat.modelView           = getUniLoc( s, "u_modelViewMat");
    Mat.view                = getUniLoc( s, "u_viewMat");
    Mat.projection          = getUniLoc( s, "u_projMat");
    Mat.model               = getUniLoc( s, "u_modelMat");
    Mat.normal              = getUniLoc( s, "u_normalMat");
	Mat.eyePoint			= getUniLoc( s, "u_eyePoint");
    if(Mat.modelViewProjection == -1 ||
       Mat.modelView == -1 ||
       Mat.view == -1 ||
       Mat.projection == -1 ||
       Mat.model == -1 ||
       Mat.normal)
    {
        return false;
    }
    return true;
}


