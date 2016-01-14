
#include "utility.h"

/*
static void setUniLoc(GLuint location, int value);
static void setUniLoc(GLuint location, float value);
static void setUniLoc(GLuint location, float x, float y);
static void setUniLoc(GLuint location, glm::vec3 value);
static void setUniLoc(GLuint location, glm::vec4 value);
static void setUniLoc(GLuint location, glm::mat4 value);
*/


void Utility::setUniLoc(GLuint location, int value)
{
    glUniform1i(location, value);
}

void Utility::setUniLoc(GLuint location, float value)
{
    glUniform1f(location, value);
}

void Utility::setUniLoc(GLuint location, float x, float y)
{
    glUniform2f(location, x, y);
}

void Utility::setUniLoc(GLuint location, glm::vec2 value)
{
    glUniform2f(location, value.x, value.y);
}

void Utility::setUniLoc(GLuint location, glm::vec3 value)
{
    glUniform3f(location, value.x, value.y, value.z);
}

/// http://glm.g-truc.net/0.9.2/api/a00132_source.html
/// source code of glm::vec4, you see the union
void Utility::setUniLoc(GLuint location, glm::vec4 value)
{
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Utility::setUniLoc(GLuint location, glm::mat3 value)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void Utility::setUniLoc(GLuint location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Utility::setUniLocTranspose(GLuint location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_TRUE, &value[0][0]);
}
