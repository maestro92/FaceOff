
#include "Renderer.h"


Renderer::Renderer()
{
    m_curShader = -1;
}

Renderer::~Renderer()
{

}

void Renderer::addShader(Shader* s)
{
    m_shaders.push_back(s);

    MatricesUniLoc mat;
    initMatricesUniLocs(s, mat);
    m_matricesUniLocs.push_back(mat);

    unordered_map<string, DataPair*> table;
    m_tables.push_back(table);
}


void Renderer::addDataPair(const char* name, int dataType)
{
    addDataPair(RENDER_PASS1, name, dataType);
}


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



void Renderer::addDataPair(int pass, const char* name, int dataType)
{
    DataPair* p;

    switch(dataType)
    {
        case DP_BOOL:
        {
            m_tables[pass][name] = new BoolDataPair(name);
            break;
        }
		case DP_INT:
        {
            m_tables[pass][name] = new IntDataPair(name);
            break;
        }
        case DP_FLOAT:
        {
            m_tables[pass][name] = new FloatDataPair(name);
            break;
        }
        case DP_VEC2:
        {
            m_tables[pass][name] = new Vec2DataPair(name);
            break;
        }
        case DP_VEC3:
        {
            m_tables[pass][name] = new Vec3DataPair(name);
            break;
        }
        case DP_VEC4:
        {
            m_tables[pass][name] = new Vec4DataPair(name);
            break;
        }
        case DP_MAT3:
        {
            m_tables[pass][name] = new Mat3DataPair(name);
            break;
        }
        case DP_MAT4:
        {
            m_tables[pass][name] = new Mat4DataPair(name);
            break;
        }
    }

    if(m_tables[pass].find(name) == m_tables[pass].end())
    {
        cout << "new DataPair Type" << endl;
        exit(1);
    }
    m_tables[pass][name]->uniLoc = getUniLoc(m_shaders[pass], name);
}



void Renderer::setData(const char* name, bool value)
{
    setData(RENDER_PASS1, name, value);
}

void Renderer::setData(const char* name, int value)
{
    setData(RENDER_PASS1, name, value);
}

void Renderer::setData(const char* name, int value, GLuint target, GLuint textureId)
{
    setData(RENDER_PASS1, name, value, target, textureId);
}

void Renderer::setData(const char* name, float value)
{
    setData(RENDER_PASS1, name, value);
}

void Renderer::setData(const char* name, glm::vec2 value)
{
    setData(RENDER_PASS1, name, value);
}

void Renderer::setData(const char* name, glm::vec3 value)
{
    setData(RENDER_PASS1, name, value);
}

void Renderer::setData(const char* name, glm::vec4 value)
{
    setData(RENDER_PASS1, name, value);
}

void Renderer::setData(const char* name, glm::mat3 value)
{
    setData(RENDER_PASS1, name, value);
}

void Renderer::setData(const char* name, glm::mat4 value)
{
    setData(RENDER_PASS1, name, value);
}




void Renderer::setData(int pass, const char* name, bool value)
{
    errorCheck(pass, name);
    m_tables[pass][name]->setValue(value);
}

void Renderer::setData(int pass, const char* name, int value)
{
    errorCheck(pass, name);
    m_tables[pass][name]->setValue(value);
}

void Renderer::setData(int pass, const char* name, int value, GLuint target, GLuint textureId)
{
    errorCheck(pass, name);

    glActiveTexture(GL_TEXTURE0 + (unsigned int)value);
    glBindTexture(target, textureId);

	m_textureUnitStack.push(GL_TEXTURE0 + (unsigned int)value);
    m_tables[pass][name]->setValue(value);
}

void Renderer::setData(int pass, const char* name, float value)
{
    errorCheck(pass, name);
    m_tables[pass][name]->setValue(value);
}

void Renderer::setData(int pass, const char* name, glm::vec2 value)
{
    errorCheck(pass, name);
    m_tables[pass][name]->setValue(value);
}

void Renderer::setData(int pass, const char* name, glm::vec3 value)
{
    errorCheck(pass, name);
    m_tables[pass][name]->setValue(value);
}

void Renderer::setData(int pass, const char* name, glm::vec4 value)
{
    errorCheck(pass, name);
    m_tables[pass][name]->setValue(value);
}

void Renderer::setData(int pass, const char* name, glm::mat3 value)
{
    errorCheck(pass, name);
    m_tables[pass][name]->setValue(value);
}

void Renderer::setData(int pass, const char* name, glm::mat4 value)
{
    errorCheck(pass, name);
    m_tables[pass][name]->setValue(value);
}

void Renderer::errorCheck(int pass, const char* name)
{
    if(m_tables[pass].find(name) == m_tables[pass].end())
    {
        cout << "glUniLoc " << name << " is not found " << endl;
		cout << m_shaders[pass]->getVertexShaderName() << endl;
        exit(1);
    }
}


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


void Renderer::printTables()
{

    DataPair* dp = m_tables[1]["u_color"];

    for (int i=0; i<m_tables.size(); i++)
    {
        utl::debug("Table ", i);

        for ( auto it = m_tables[i].begin(); it != m_tables[i].end(); ++it )
        {
            cout << " " << it->first;
            cout << ":" << it->second << endl;
        }

        utl::debugLn(2);
    }
}


GLuint Renderer::getUniLoc(Shader* s, const char* name)
{
    unsigned int shaderID = s->getProgramId();

    GLuint location = glGetUniformLocation(shaderID, name);

    if(location == -1)
    {
        cout << "Error in Init_Shader with " << name << endl;
    }
    return location;
}

void Renderer::loadUniformLocations()
{
    loadUniformLocations(RENDER_PASS1);
}

void Renderer::loadUniformLocations(int pass)
{
    for ( auto local_it = m_tables[pass].begin(); local_it!= m_tables[pass].end(); ++local_it )
        (local_it->second)->setUniLoc();
}

void Renderer::loadUniformLocations(Pipeline& p)
{
    loadUniformLocations(p, RENDER_PASS1);
}

void Renderer::loadUniformLocations(Pipeline& p, int pass)
{
    loadUniformLocations(pass);

    if(pass != m_curShader)
    {
        utl::debug("######### pass Not Matching");
    }

	if(!p.matricesReady)
	{
		p.m_modelViewMatrix = p.m_viewMatrix[p.m_viewMatrix.size()-1] * p.m_modelMatrix[p.m_modelMatrix.size()-1];
		p.m_modelViewProjectionMatrix = p.m_projectionMatrix[p.m_projectionMatrix.size()-1]*
                                        p.m_viewMatrix[p.m_viewMatrix.size()-1]*
                                        p.m_modelMatrix[p.m_modelMatrix.size()-1];
		p.m_normalMatrix = glm::mat3(p.m_modelViewMatrix);
	}

	glUniformMatrix4fv(m_matricesUniLocs[pass].modelViewProjection, 1, GL_FALSE, &p.m_modelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(m_matricesUniLocs[pass].modelView, 1, GL_FALSE, &p.m_modelViewMatrix[0][0]);
	glUniformMatrix4fv(m_matricesUniLocs[pass].model, 1, GL_FALSE, &p.m_modelMatrix[p.m_modelMatrix.size()-1][0][0]);
    glUniformMatrix4fv(m_matricesUniLocs[pass].view, 1, GL_FALSE, &p.m_viewMatrix[p.m_viewMatrix.size()-1][0][0]);
	glUniformMatrix3fv(m_matricesUniLocs[pass].normal, 1, GL_FALSE, &p.m_normalMatrix[0][0]);
	glUniform3f(m_matricesUniLocs[pass].eyePoint, p.m_viewPosition.x, p.m_viewPosition.y, p.m_viewPosition.z);
}



void Renderer::enableShader()
{
    enableShader(RENDER_PASS1);
}

void Renderer::enableShader(int RenderPassID)
{
    m_shaders[RenderPassID]->useShader();
    m_curShader = RenderPassID;
}


void Renderer::disableShader()
{
    disableShader(RENDER_PASS1);
}

void Renderer::disableShader(int RenderPassID)
{
    m_shaders[RenderPassID]->delShader();
    m_curShader = -1;
    while(!m_textureUnitStack.empty())
    {
        glActiveTexture(m_textureUnitStack.top());
        glBindTexture(GL_TEXTURE_2D, 0);
        m_textureUnitStack.pop();
    }
}

void Renderer::setUniformLocation(GLuint location, bool value)
{
    glUniform1i(location, value);
}

void Renderer::setUniformLocation(GLuint location, int value)
{
    glUniform1i(location, value);
}

void Renderer::setUniformLocation(GLuint location, float value)
{
    glUniform1f(location, value);
}

void Renderer::setUniformLocation(GLuint location, float x, float y)
{
    glUniform2f(location, x, y);
}

void Renderer::setUniformLocation(GLuint location, glm::vec2 value)
{
    glUniform2f(location, value.x, value.y);
}

void Renderer::setUniformLocation(GLuint location, glm::vec3 value)
{
    glUniform3f(location, value.x, value.y, value.z);
}

/// http://glm.g-truc.net/0.9.2/api/a00132_source.html
/// source code of glm::vec4, you see the union
void Renderer::setUniformLocation(GLuint location, glm::vec4 value)
{
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Renderer::setUniformLocation(GLuint location, glm::mat3 value)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void Renderer::setUniformLocation(GLuint location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Renderer::setUniformLocationTranspose(GLuint location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_TRUE, &value[0][0]);
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


void Renderer::setDataPairUniLoc(BoolDataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void Renderer::setDataPairUniLoc(IntDataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void Renderer::setDataPairUniLoc(FloatDataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void Renderer::setDataPairUniLoc(Vec2DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value.x, dp.value.y);
}

void Renderer::setDataPairUniLoc(Vec3DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void Renderer::setDataPairUniLoc(Vec4DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void Renderer::setDataPairUniLoc(Mat3DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}

void Renderer::setDataPairUniLoc(Mat4DataPair& dp)
{
    setUniformLocation(dp.uniLoc, dp.value);
}


