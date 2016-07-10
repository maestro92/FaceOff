#ifndef SHADER_H_
#define SHADER_H_

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

/*
enum AttributeSlot {
    SlotPosition,
    SlotTexCoord,
};
*/

enum AttributeSlot {
    SlotPosition,
    SlotTexCoord,
    SlotNormal,
    SlotBirthTime,
    SlotVelocity,
};



using namespace std;
class Shader
{
    unsigned int vs, fs, program;
    unsigned int gs;
    string path;
    void loadFile(const char* fn, string & str);
    bool loadIncludeFiles(string & str, string newLine);
    unsigned int loadShader(string& source, unsigned int ShaderType);

	string m_vsName;
	string m_gsName;
	string m_fsName;


    public:

		Shader(const char* vs_source);
        Shader(const char* vs_source, const char* fs_source);
		Shader(const char* vs_source, const char* gs_source, const char* fs_source, bool feedBack = false);


        Shader(const char* vs_source, bool feedBack);
        Shader(const char* vs_source, const char* gs_source, bool feedBack);
     //   Shader(const char* vs_source, const char* gs_source, const char* fs_source, bool feedBack);
		/*
        Shader(string path, string vs_source, string fs_source);
        Shader(string path, string vs_source, string gs_source, string fs_source);
		*/

        ~Shader();

		string getVertexShaderName();
		string getGeometryShaderName();
		string getFragmentShaderName();

        void linkShader();
        void useShader();
        unsigned int getProgramId();
        void delShader();

    protected:

    private:

};


#endif // Shader_H_

