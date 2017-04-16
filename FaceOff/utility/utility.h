#ifndef UTILITY_H_
#define UTILITY_H_

#include <cstdlib>

#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>

#include <cstdio>
#include <iomanip>
#include <queue>
#include <GL/glew.h>

//#define NO_SDL_GLEXT
//#include <GL/glew.h>


#include "define.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <iomanip>

#include <unordered_set>

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID
#include "StringCompressor.h"
//#include "RakNet/StringCompressor.h"



#include <cassert>
#include <fstream>
#include <mutex>




using namespace std;





#define DEBUG_FLAG 1





#define RENDER_TO_SCREEN 0

#define BLACK       glm::vec3(0.0,0.0,0.0)
#define DARK_BLUE   glm::vec3(0.0,0.0,0.75)
#define BLUE        glm::vec3(0.0,0.0,0.5)
#define GREEN       glm::vec3(0.0,1.0,0.0)
#define TEAL        glm::vec3(0.0,1.0,1.0)
#define RED         glm::vec3(1.0,0.0,0.0)
#define PURPLE      glm::vec3(1.0,0.0,1.0)
#define YELLOW      glm::vec3(1.0,1.0,0.0)

#define DARK_GRAY   glm::vec3(0.25,0.25,0.25)
#define GRAY        glm::vec3(0.5,0.5,0.5)
#define LIGHT_GRAY  glm::vec3(0.75, 0.75, 0.75)
#define WHITE       glm::vec3(1.0,1.0,1.0)

typedef long long UniqueId;




#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

/* Remove if already defined */
typedef long long int64; typedef unsigned long long uint64;




struct MouseState
{
    bool m_leftButtonDown;
    bool m_rightButtonDown;
    bool m_middleButtonDown;

    glm::vec2 m_pos;

    MouseState()
    {
        m_leftButtonDown = false;
        m_rightButtonDown = false;
        m_middleButtonDown = false;

        m_pos = glm::vec2(0,0);
    }

};

struct TextureObject
{
    GLuint id;
    int width;
    int height;
    int depth;
};







template<class T>
struct LinkedListNode
{
	T data;
	LinkedListNode<T>* next;

	LinkedListNode(const T& d) : data(d), next() 
	{}

	
};


template<class T>
class LinkedList
{
	LinkedListNode* head;


};





// http://gafferongames.com/game-physics/networked-physics/
enum NetworkGameMessageEnum
{
	// server to client
	SPAWN_INFORMATION = ID_USER_PACKET_ENUM + 1,
	NEW_CLIENT = ID_USER_PACKET_ENUM + 2,
	LOBBY_WAIT_END = ID_USER_PACKET_ENUM + 3,

	SERVER_SNAPSHOT = ID_USER_PACKET_ENUM + 4,
	CLIENT_INPUT = ID_USER_PACKET_ENUM + 5,
	NONE = ID_USER_PACKET_ENUM + 6,
	// client to server
};



namespace utl
{
	const float Z_NEAR = 0.05;
	const float Z_FAR = 2000.0;
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const int SCREEN_WIDTH_MIDDLE = SCREEN_WIDTH / 2;
	const int SCREEN_HEIGHT_MIDDLE = SCREEN_HEIGHT / 2; 

	const float MATH_EPISON = 0.000001f;
	const float GRAVITY_CONSTANT = 0.01f;
	const glm::vec3 BIASED_HALF_GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f) * GRAVITY_CONSTANT * 0.5f;




	const float DEGREE_TO_RADIAN = 0.0174;    /// pi/180
	const float RADIAN_TO_DEGREE = 57.32;     /// 180/pi

	template<typename T>
	vector<T> reserveVector(int size);

	/// utl_Math.cpp
	string intToStr(int value);
	string floatToStr(float value);
	int randInt(int min = 0, int max = 100);
	float randFloat(float min = 0, float max = 1);
	template<class T>
	inline T sqr(const T &x)
	{
		return x*x;
	}

	float barycentricInterpolation(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec2 coord);

	glm::vec3 interpolateEntityPosition(glm::vec3 pos0, glm::vec3 pos1, float interpFactor);
	glm::vec3 interpolateEntityAngles(glm::vec3 pos0, glm::vec3 pos1, float interpFactor);
	float interpolateAngle(float f0, float f1, float interpFactor);

	uint32_t createUniqueObjectID();

	bool chance(float percent);

	void write(RakNet::BitStream& bs, std::string& s);
	void read(RakNet::BitStream& bs, std::string& s);


	long long getCurrentTime_ms();
	
	// http://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c
	uint64 GetTimeMs64();









	void checkGLError();






	// utl_time.cpp
	long long getCurrentTimeMillis();





};




template<typename T>
vector<T> utl::reserveVector(int size)
{
	vector<T> v;
	v.reserve(size);
	return v;
}





#endif
