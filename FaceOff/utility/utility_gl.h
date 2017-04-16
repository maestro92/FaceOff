#ifndef UTILITY_GL_H_
#define UTILITY_GL_H_
#include "define.h"
#include <vector>
#include "utility_debug.h"
#include "utility_sdl.h"

#include <GL/glew.h>
using namespace std;

typedef vector<vector<vector<GLubyte>>> TextureDataBuffer;

struct FrameBufferObject
{
	GLuint FBO;
	GLuint colorTexture;
	GLuint depthTexture;

	void clear()
	{
		clear(glm::vec4(0, 0, 0, 1));
	}

	void clear(glm::vec4 v)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(v.x, v.y, v.z, v.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

// double buffering
struct DoubleFrameBufferObject
{
	FrameBufferObject ping;   // buffer 1
	FrameBufferObject pong;   // buffer 2

	void swapFrontBack()
	{
		FrameBufferObject temp = ping;
		ping = pong;
		pong = temp;
	}

	void clear()
	{
		ping.clear();
		pong.clear();
	}

	void clear(glm::vec4 v)
	{
		ping.clear(v);
		pong.clear(v);
	}
};


namespace utl
{
	/// utl_GL.cpp
	void initGLEW();
	void errorCheck();
	GLuint createFBO();
	void errorCheckFBO();
	void bindFBO(GLuint target);

	TextureDataBuffer createEmptyBuffer(int w, int h);

	GLuint loadTexture(string filename, bool mipmapFlag = true);
	GLuint loadTexture(string filename, GLuint filteringParam, GLuint edgeParam, bool mipmapFlag = false);
	GLuint loadTexture(vector<vector<vector<GLubyte>>> data, GLuint filteringParam, GLuint edgeParam, bool mipmapFlag = false);
	void setTexture2DParams(GLuint target, GLuint filteringParam, GLuint edgeParam, bool mipmapFlag);

	GLuint createNewTexture(int w, int h);
	GLuint createNew3DTexture(int w, int h, int d);
	GLuint createNoiseTexture(int w, int h, int min, int max);

	GLuint createNewDepthTexture(int w, int h);
	GLuint createNewCubemapTexture();

	GLuint loadCubemapTexture(string* filenames);
	void setTextureParameters(int w, int h, int internal_format, int format);
	void setCubemapTextureParameters();
	FrameBufferObject createFrameBufferObject(int width, int height);
	DoubleFrameBufferObject createDoubleFrameBufferObject(int width, int height);

	void setupFrameBuffer();
	void setupFrameBuffer(GLuint target);
}
#endif 

