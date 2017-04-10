
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


void utl::write(RakNet::BitStream &bs, std::string &s)
{
	const char *str = s.c_str();
	RakNet::StringCompressor::Instance()->EncodeString(str, 255, &bs);
}

void utl::read(RakNet::BitStream &bs, std::string &s)
{
	char str[255];
	RakNet::StringCompressor::Instance()->DecodeString(str, 255, &bs);
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




/* Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
* windows and linux. */

uint64 GetTimeMs64()
{
#ifdef _WIN32
	/* Windows */
	FILETIME ft;
	LARGE_INTEGER li;

	/* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
	* to a LARGE_INTEGER structure. */
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	uint64 ret = li.QuadPart;
	ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
	ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

	return ret;
#else
	/* Linux */
	struct timeval tv;

	gettimeofday(&tv, NULL);

	uint64 ret = tv.tv_usec;
	/* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
	ret /= 1000;

	/* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
	ret += (tv.tv_sec * 1000);

	return ret;
#endif
}