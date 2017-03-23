#ifndef UTILITY_H_
#define UTILITY_H_

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
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

#include "SDL.h"
#include "SDL_image.h"

#include "json_spirit.h"
#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"
#include <cassert>
#include <fstream>
#include <mutex>

#ifndef JSON_SPIRIT_MVALUE_ENABLED
#error Please define JSON_SPIRIT_MVALUE_ENABLED for the mValue type to be enabled 
#endif

using namespace std;
using namespace json_spirit;




#define DEBUG_FLAG 1


using namespace std;


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


struct FrameBufferObject
{
    GLuint FBO;
    GLuint colorTexture;
    GLuint depthTexture;

    void clear()
    {
        clear(glm::vec4(0,0,0,1));
    }

    void clear(glm::vec4 v)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClearColor(v.x, v.y, v.z, v.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};


typedef vector<vector<vector<GLubyte>>> TextureDataBuffer;


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


struct Input
{
	bool left;
	bool right;
	bool forward;
	bool back;
	bool jump;
	bool weaponFired;

	void toBitStream(RakNet::BitStream& bs)
	{
		bs.Write(left);
		bs.Write(right);
		bs.Write(forward);
		bs.Write(back);
		bs.Write(jump);
		bs.Write(weaponFired);
	}

	void setFromBitStream(RakNet::BitStream& bs)
	{
		bs.Read(left);
		bs.Read(right);
		bs.Read(forward);
		bs.Read(back);
		bs.Read(jump);
		bs.Read(weaponFired);
	}

	void reset()
	{
		left = false;
		right = false;
		forward = false;
		back = false;
		jump = false;
		weaponFired = false;
	}

	void print()
	{
		if (left)
			cout << "left: true" << endl;
		else
			cout << "left: false" << endl;

		if (right)
			cout << "right: true" << endl;
		else
			cout << "right: false" << endl;

		if (forward)
			cout << "forward: true" << endl;
		else
			cout << "forward: false" << endl;

		if (back)
			cout << "back: true" << endl;
		else
			cout << "back: false" << endl;

		if (jump)
			cout << "jump: true" << endl;
		else
			cout << "jump: false" << endl;

		if (weaponFired)
			cout << "weaponFired: true" << endl;
		else
			cout << "weaponFired: false" << endl;
	}
};

struct State
{
	float pitch;
	float yaw;

	glm::vec3 position;
	glm::vec3 velocity;

	void toBitStream(RakNet::BitStream& bs)
	{
		bs.Write(pitch);
		bs.Write(yaw);
		bs.WriteVector(position.x, position.y, position.z);
		bs.WriteVector(velocity.x, velocity.y, velocity.z);
	}

	void setFromBitStream(RakNet::BitStream& bs)
	{
		bs.Read(pitch);
		bs.Read(yaw);
		bs.ReadVector(position.x, position.y, position.z);
		bs.ReadVector(velocity.x, velocity.y, velocity.z);
	}

	void reset()
	{
		pitch = 0.0;
		yaw = 0.0;
		position = glm::vec3(0.0, 0.0, 0.0);
		velocity = glm::vec3(0.0, 0.0, 0.0);
	}

	void print()
	{
		cout << "State position: " << position.x << " " << position.y << " " << position.z << ", velocity: " << velocity.x << " " << velocity.y << " " << velocity.z << endl;
	}
};


struct Move
{
	double time;
	int playerId;
	Input input;
	State state;

	Move()
	{
		reset();
	}

	Move(RakNet::BitStream& bs)
	{
		setFromBitStream(bs);
	}

	void toBitStream(RakNet::BitStream& bs)
	{
		bs.Write(time);
		bs.Write(playerId);
		input.toBitStream(bs);
		state.toBitStream(bs);
	}

	void setFromBitStream(RakNet::BitStream& bs)
	{
		bs.Read(time);
		bs.Read(playerId);
		input.setFromBitStream(bs);
		state.setFromBitStream(bs);
	}

	void reset()
	{
		input.reset();
		state.reset();
	}

	void print()
	{
		cout << "playerId" << endl;
		input.print();
		state.print();
	}
};

struct MoveQueue
{
	std::mutex mtx;
	queue<Move> buffer;

	MoveQueue()
	{

	}

	void toBitStream(RakNet::BitStream& bs)
	{
		int size = buffer.size();

		if (size > 0)
		{
			bs.Reset();
			bs.Write((RakNet::MessageID)CLIENT_INPUT);

			bs.Write(size);
		//	cout << "client has " << size << " size " << endl;

			for (int i = 0; i < size; i++)
			{
				mtx.lock();
				Move move = buffer.front();
				buffer.pop();
				mtx.unlock();

				move.toBitStream(bs);
			}
		}
	}

	void setFromBitStream(RakNet::BitStream& bs)
	{
		// no need for message id
		//	RakNet::MessageID msgId;
		//	bs.Read(msgId);

		int size = 0;
		bs.Read(size);

		for (int i = 0; i < size; i++)
		{
			Move move(bs);
			mtx.lock();
			buffer.push(move);
			mtx.unlock();
		}
	}

	Move front()
	{
		mtx.lock();
		Move f = buffer.front();
		mtx.unlock();
		return f;
	}

	void push(Move move)
	{
		mtx.lock();
		buffer.push(move);
		mtx.unlock();
	}

	Move pop()
	{
		mtx.lock();
		Move f = buffer.front();
		buffer.pop();
		mtx.unlock();
		return f;
	}

	int size()
	{
		mtx.lock();
		int size = buffer.size();
		mtx.unlock();
		return size;
	}

	bool empty()
	{
		mtx.lock();
		bool b = (buffer.size() == 0);
		mtx.unlock();
		return b;
	}
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


	const string clientDebugPrefix = "									";

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

	uint32_t createUniqueObjectID();

	bool chance(float percent);

	void write(RakNet::BitStream &bitStream, std::string &s);
	void read(RakNet::BitStream &bitStream, std::string &s);

	long long getCurrentTime_ms();
	
	// http://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c
	uint64 GetTimeMs64();


	/// utl_SDL.cpp
	void initSDL(int w, int h, SDL_Surface* & m_displaySurface);
	void exitSDL(SDL_Surface* & m_displaySurface);
	SDL_Surface* loadRawImage(string filename);
	SDL_Surface* loadSDLImage(string filename);
	Uint32 getpixel(SDL_Surface *surface, int x, int y);

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


	/// utl_assimp.cpp
	glm::vec3 toGlmVec(aiVector3D& v);
	glm::vec3 toGlmVec(aiColor3D& c);
	glm::vec3 toGlmVec(aiColor4D& c);


	glm::mat4 toGlmMat(aiMatrix3x3 m);
	glm::mat4 toGlmMat(aiMatrix4x4& m);
	glm::mat4 toGlmMat(const aiMatrix4x4& m);

	/// utl_debug.cpp
	void debugLn(int l = 1);
	void debugLn(string s, int l = 1);
	void debugLn(string s, bool b, int l = 1);
	void debugLn(string s, char c, int l = 1);
	void debugLn(string s, string s2, int l = 1);
	void debugLn(string s, unsigned int i, int l = 1);
	void debugLn(string s, int i, int l = 1);
	void debugLn(string s, float f, int l = 1);
	void debugLn(string s, glm::vec2 v, int l = 1);
	void debugLn(string s, glm::vec3 v, int l = 1);
	void debugLn(string s, glm::vec4 v, int l = 1);
	void debugLn(string s, glm::mat3 m, int l = 1);
	void debugLn(string s, glm::mat4 m, int l = 1);


	void debug(string s);
	void debug(string s, bool b);
	void debug(string s, char c);
	void debug(string s, string s2);
	void debug(string s, const char* s2);
	void debug(string s, unsigned int i);
	void debug(string s, int i);
	void debug(string s, float f);
	void debug(string s, double d);
	void debug(string s, glm::vec2 v);
	void debug(string s, glm::vec3 v);
	void debug(string s, glm::vec4 v);
	void debug(string s, glm::mat3 m);
	void debug(string s, glm::mat4 m);
	template <class T>
	void debug(string s, vector<T> v);

	template <class T>
	void debug(string s, vector< vector<T> > v);



	void clDebug(string s);
	void clDebug(string s, bool b);
	void clDebug(string s, char c);
	void clDebug(string s, string s2);
	// void clDebug(string s, const char* s2);
	void clDebug(string s, unsigned int i);
	void clDebug(string s, int i);
	void clDebug(string s, float f);
	void clDebug(string s, double d);
	void clDebug(string s, glm::vec2 v);
	void clDebug(string s, glm::vec3 v);
	void clDebug(string s, glm::vec4 v);
	void clDebug(string s, glm::mat3 m);
	void clDebug(string s, glm::mat4 m);

	void checkGLError();


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



	// utl_time.cpp
	long long getCurrentTimeMillis();


	/// utl_json.cpp
	mValue readJsonFileToMap(char* file);
	Value readJsonFileToVector(char* file);
	const mValue& findValue(const mObject& obj, const string& name);
	glm::vec3 findVec3(const mObject& obj, const string& name);




};

template<typename T>
vector<T> utl::reserveVector(int size)
{
    vector<T> v;
    v.reserve(size);
    return v;
}

template <class T>
void utl::debug(string s, vector<T> v)
{
    cout << s << endl;
    for(int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

template <class T>
void utl::debug(string s, vector< vector<T> > v)
{
    cout << s << endl;
    for(int y = 0; y < v.size(); y++)
    {
        for(int x = 0; x < v[y].size(); x++)
            cout << setw(20) << left << v[y][x];
        cout << endl;
    }
    cout << endl;
}








#endif
