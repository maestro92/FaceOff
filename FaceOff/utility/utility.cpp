
#include "utility.h"





void utl::checkGLError()
{
    GLenum err (glGetError());

    while( err != GL_NO_ERROR )
    {
        string error;

        switch(err)
        {
                case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
        }

        cout << "GL_" << error.c_str() << endl;
        err=glGetError();
    }
}


uint32_t utl::createUniqueObjectID()
{
	static uint32_t counter = 50;
	counter++;
	uint32_t temp = counter;
	return temp;
}