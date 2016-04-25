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


#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "SDL.h"
#include "SDL_image.h"



#include "json_spirit.h"
#include "json_spirit_reader_template.h"
#include "json_spirit_writer_template.h"
#include <cassert>
#include <fstream>

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


namespace utl
{
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;

	const float DEGREE_TO_RADIAN = 0.0174;    /// pi/180
	const float RADIAN_TO_DEGREE = 57.32;     /// 180/pi

	template<typename T>
	vector<T> reserveVector(int size);

	/// utl_Math.cpp
	string floatToStr(float value);
	int randInt(int min = 0, int max = 100);
	float randFloat(float min = 0, float max = 1);
	template<class T>
	inline T sqr(const T &x)
	{
		return x*x;
	}

	float barycentricInterpolation(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec2 coord);


	int createUniqueObjectID();


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

	GLuint loadTexture(string filename, bool mipmapFlag = false);
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

	/// utl_Debug.cpp
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
	void debug(string s, unsigned int i);
	void debug(string s, int i);
	void debug(string s, float f);
	void debug(string s, glm::vec2 v);
	void debug(string s, glm::vec3 v);
	void debug(string s, glm::vec4 v);
	void debug(string s, glm::mat3 m);
	void debug(string s, glm::mat4 m);
	template <class T>
	void debug(string s, vector<T> v);

	template <class T>
	void debug(string s, vector< vector<T> > v);

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


	/// utl_network.cpp
	void readBitStream(RakNet::BitStream& bsIn, glm::vec3& v);
	void setBitStream(RakNet::BitStream& bsOut, glm::vec3& v);


	/// utl_json.cpp
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
