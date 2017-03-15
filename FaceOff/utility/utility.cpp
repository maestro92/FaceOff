
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


void utl::write(RakNet::BitStream &bitStream, std::string &s)
{
	const char *str = s.c_str();
	RakNet::StringCompressor::Instance()->EncodeString(str, 255, &bitStream);
}

void utl::read(RakNet::BitStream &bitStream, std::string &s)
{
	char str[255];
	RakNet::StringCompressor::Instance()->DecodeString(str, 255, &bitStream);
	string new_s(str);
	s = new_s;
}

// https://www.epochconverter.com/
long long utl::getCurrentTime_ms()
{
	using namespace std::chrono;
	std::chrono::milliseconds ms = duration_cast< std::chrono::milliseconds >(
		system_clock::now().time_since_epoch()
		);
	return ms.count();
}