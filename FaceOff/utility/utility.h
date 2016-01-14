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


#include "SDL.h"
#include "SDL_image.h"


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


class Utility
{
    public:
        Utility();
        ~Utility();



        template<typename T>
            static vector<T> reserveVector(int size);

        /// Utility_Math.cpp
        static float DEGREE_TO_RADIAN;
        static float RADIAN_TO_DEGREE;
        static string floatToStr(float value);
        static float randFloat(float min=0, float max=1);
        template<class T>
        static inline T sqr(const T &x)
        {
            return x*x;
        }


        /// Utility_SDL.cpp
        static void initSDL(int w, int h, SDL_Surface* & m_displaySurface);
        static void exitSDL(SDL_Surface* & m_displaySurface);
        static SDL_Surface* loadRawImage(string filename);
        static SDL_Surface* loadSDLImage(string filename);


        /// Utility_GL.cpp
        static void initGLEW();
        static void errorCheck();
        static GLuint createFBO();
        static void errorCheckFBO();
        static void bindFBO(GLuint target);

        static TextureDataBuffer createEmptyBuffer(int w, int h);

        static GLuint loadTexture(string filename);
        static GLuint loadTexture(string filename, GLuint filteringParam);
        static GLuint loadTexture(vector<vector<vector<GLubyte>>> data, GLuint filteringParam);

        static GLuint createNewTexture(int w, int h);
        static GLuint createNew3DTexture(int w, int h, int d);


        static GLuint createNewDepthTexture(int w, int h);
        static GLuint createNewCubemapTexture();

        static GLuint loadCubemapTexture(string* filenames);
        static void setTextureParameters(int w, int h, int internal_format, int format);
        static void setCubemapTextureParameters();
        static FrameBufferObject createFrameBufferObject(int width, int height);
        static DoubleFrameBufferObject createDoubleFrameBufferObject(int width, int height);

        static void setupFrameBuffer();
        static void setupFrameBuffer(GLuint target);


        /// utility_assimp.cpp
        static glm::vec3 toGlmVec(aiVector3D& v);
        static glm::vec3 toGlmVec(aiColor3D& c);
        static glm::vec3 toGlmVec(aiColor4D& c);


        static glm::mat4 toGlmMat(aiMatrix3x3 m);
        static glm::mat4 toGlmMat(aiMatrix4x4& m);
        static glm::mat4 toGlmMat(const aiMatrix4x4& m);

        static glm::vec2 scaleGlmVec(const glm::vec2 v, float s);
        static glm::vec2 scaleGlmVec(const glm::vec2 v, float s1, float s2);
        static glm::vec3 scaleGlmVec(const glm::vec3 v, float s);
        static glm::vec3 scaleGlmVec(const glm::vec3 v, float s1, float s2, float s3);


        /// Utility_Debug.cpp
        static void debugLn(int l=1);
        static void debugLn(string s, int l=1);
        static void debugLn(string s, bool b, int l=1);
        static void debugLn(string s, char c, int l=1);
        static void debugLn(string s, string s2, int l=1);
        static void debugLn(string s, unsigned int i, int l=1);
        static void debugLn(string s, int i, int l=1);
        static void debugLn(string s, float f, int l=1);
        static void debugLn(string s, glm::vec2 v, int l=1);
        static void debugLn(string s, glm::vec3 v, int l=1);
        static void debugLn(string s, glm::vec4 v, int l=1);
        static void debugLn(string s, glm::mat3 m, int l=1);
        static void debugLn(string s, glm::mat4 m, int l=1);


        static void debug(string s);
        static void debug(string s, bool b);
        static void debug(string s, char c);
        static void debug(string s, string s2);
        static void debug(string s, unsigned int i);
        static void debug(string s, int i);
        static void debug(string s, float f);
        static void debug(string s, glm::vec2 v);
        static void debug(string s, glm::vec3 v);
        static void debug(string s, glm::vec4 v);
        static void debug(string s, glm::mat3 m);
        static void debug(string s, glm::mat4 m);
        template <class T>
        static void debug(string s, vector<T> v);

        template <class T>
        static void debug(string s, vector< vector<T> > v);

        static void checkGLError();


        /// Utility_UniLoc.cpp
        static void setUniLoc(GLuint location, int value);
        static void setUniLoc(GLuint location, float value);
        static void setUniLoc(GLuint location, float x, float y);
        static void setUniLoc(GLuint location, glm::vec2 value);
        static void setUniLoc(GLuint location, glm::vec3 value);
        static void setUniLoc(GLuint location, glm::vec4 value);
        static void setUniLoc(GLuint location, glm::mat3 value);
        static void setUniLoc(GLuint location, glm::mat4 value);
        static void setUniLocTranspose(GLuint location, glm::mat4 value);
};



template<typename T>
vector<T> Utility::reserveVector(int size)
{
    vector<T> v;
    v.reserve(size);
    return v;
}

template <class T>
void Utility::debug(string s, vector<T> v)
{
    cout << s << endl;
    for(int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

template <class T>
void Utility::debug(string s, vector< vector<T> > v)
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
