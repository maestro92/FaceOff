#ifndef UTILITY_GL_UNI_LOC_H_
#define UTILITY_GL_UNI_LOC_H_
#include "define.h"

#include <GL/glew.h>

namespace utl
{
	/// utl_UniLoc.cpp
	void setUniLoc(GLuint location, int value);
	void setUniLoc(GLuint location, float value);
	void setUniLoc(GLuint location, float x, float y);
	void setUniLoc(GLuint location, glm::vec2 value);
	void setUniLoc(GLuint location, glm::vec3 value);
	void setUniLoc(GLuint location, glm::vec4 value);
	void setUniLoc(GLuint location, glm::mat3 value);
	void setUniLoc(GLuint location, glm::mat4 value);
	void setUniLocTranspose(GLuint location, glm::mat4 value);
}

#endif
