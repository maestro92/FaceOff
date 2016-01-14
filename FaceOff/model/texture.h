#ifndef TEXTURE_H_
#define TEXTURE_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream> //You'll need to include this header to use 'std::stringstream'.
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include "SDL.h"
#include "SDL_image.h"

using namespace std;

class Texture
{
    public:
        Texture(GLenum TextureTarget, const string& FileName);

        void Bind(GLenum TextureUnit);

        bool Load(bool generate=false);

    private:
        string m_fileName;
        GLenum m_textureTarget;
        GLuint m_textureObj;



};

#endif
