#include "shader.h"

#include <stdio.h>
#include <string.h>

string dir_path = "./shaders/";
string p_str;




Shader::Shader(const char* vs_source, const char* fs_source)
{
    string source;
    cout << "vs: " << vs_source << ", fs: " << fs_source << endl;
	m_vsName = vs_source;
	m_gsName = "";
	m_fsName = fs_source;

	// source code and Mode
	loadFile(vs_source, source);
    vs = loadShader(source, GL_VERTEX_SHADER);

	// load the fragment Shader
    source = "";
    loadFile(fs_source, source);
    fs = loadShader(source, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // links the program object
    glLinkProgram(program);
    // this will use this Shader program
    glUseProgram(program);


}



Shader::Shader(const char* vs_source, const char* gs_source, const char* fs_source, bool feedBack)
{
    string source;
	cout << endl << endl << endl << "vs: " << vs_source << ", gs:" << gs_source << ", fs: " << fs_source << endl << endl;
	m_vsName = vs_source;
	m_gsName = gs_source;
	m_fsName = fs_source;

	
	// source code and Mode
	loadFile(vs_source, source);
    vs = loadShader(source, GL_VERTEX_SHADER);


    // load the geometry Shader
	source = "";
	loadFile(gs_source, source);
    gs = loadShader(source, GL_GEOMETRY_SHADER);


    // load the fragment Shader
	source = "";
	loadFile(fs_source, source);
    fs = loadShader(source, GL_FRAGMENT_SHADER);


    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);


	if (!feedBack)
	{
		// links the program object
		glLinkProgram(program);
		// this will use this Shader program
		glUseProgram(program);
	}

}

/*
Shader::Shader(const char* vs_source, const char* gs_source, const char* fs_source, bool feedBack)
{
    string source;
	cout << endl << endl << endl << "vs: " << vs_source << ", gs:" << gs_source << ", fs: " << fs_source << endl << endl;
    loadFile(vs_source, source);
    /// load the vertex shader
    vs = loadShader(source, GL_VERTEX_SHADER);


    source = "";
    // load the geometry Shader
    loadFile(gs_source, source);
    gs = loadShader(source, GL_GEOMETRY_SHADER);

    source = "";
    // load the fragment shader
    loadFile(fs_source, source);
    fs = loadShader(source, GL_FRAGMENT_SHADER);


    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);
}
*/



Shader::Shader(const char* vs_source, bool feedBack)
{
    string source;
    cout << "vs: " << vs_source << endl << endl;
    loadFile(vs_source, source);
    /// load the vertex shader
    vs = loadShader(source, GL_VERTEX_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vs);
}

Shader::Shader(const char* vs_source, const char* gs_source, bool feedBack)
{
    string source;
    cout << "vs: " << vs_source << ", gs:" << gs_source << endl << endl;

	m_vsName = vs_source;
	m_gsName = gs_source;

    loadFile(vs_source, source);
    /// load the vertex shader
    vs = loadShader(source, GL_VERTEX_SHADER);

    source = "";
    // load the geometry Shader
    loadFile(gs_source, source);
    gs = loadShader(source, GL_GEOMETRY_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, gs);
}

/*
Shader::Shader(string path, string vs_source, string fs_source) : Shader( (path + vs_source).c_str(),  (path + fs_source).c_str())
{

}

Shader::Shader(string path, string vs_source, string gs_source, string fs_source)
{

}
*/

/*
Shader::Shader(string path, string vs_source, bool feedBack) : Shader( (path + vs_source).c_str(), feedBack)
{

}
*/

// deletes the Shader
Shader::~Shader()
{
    glDetachShader(program, vs);
    glDetachShader(program, gs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}


void Shader::loadFile(const char* fn, string & str)
{
    p_str = dir_path;
    string mstr(fn);
    p_str = p_str + mstr;
 //   cout << "p_str is " << endl << p_str << endl;


    ifstream in(p_str);

    if(!in.is_open())
    {
        cout << "The file " << fn << " cannot be openned\n" << endl;
		exit(1);
        return;
    }

    char tmp[300];
    while(!in.eof())
    {
        in.getline(tmp,300);

    //    if(p_str == "./shaders/shadow_SecondRender_dir_pt_include.fs")
     //   {
       //     cout << tmp << endl;
        if(loadIncludeFiles(str, tmp))
            continue;
      //  }

        str+=tmp;
        str+='\n';
    }

/*
    if(p_str == "./shaders/shadow_SecondRender_dir_pt_include.fs")
    {
        cout << str << endl;
    }
*/
}

/*
void Shader::loadFile(const char* fn, string & str)
{
    p_str = dir_path;
    string mstr(fn);
    p_str = p_str + mstr;
 //   cout << "p_str is " << endl << p_str << endl;


    ifstream in(p_str);

    if(!in.is_open())
    {
        cout << "The file " << fn << "cannot be openned\n" << endl;
        return;
    }

    char tmp[300];
    while(!in.eof())
    {
        in.getline(tmp,300);
        str+=tmp;
        str+='\n';
    }

    if(p_str == "./shaders/shadow_SecondRender_dir_pt_include.fs")
    {
        cout << str << endl;
    }
}
*/


bool Shader::loadIncludeFiles(string & str, string newLine)
{

    istringstream iss(newLine);

    int i = 0;
    string includeFileName;

    bool includeFlag = false;

    while(iss)
    {
        string s;
        iss >> s;
        if(s == "#include")
            includeFlag = true;
       //     cout << newLine << endl;

        if(i==1 && includeFlag)
        {
            includeFileName = s.substr(1,s.length()-2);
            cout << includeFileName << endl;

            const char* f = includeFileName.c_str();
            loadFile(f, str);
 //           cout << str << endl;
        }
        i++;
    }


/*
    string delimiter = " ";
    istringstream iss(newLine);
    string s;

    while(getline(iss, s, ' '))
    {
        if(s == "#include")
            cout << newLine << endl;
    }
*/
    return includeFlag;
}

// need glew to run Shader
unsigned int Shader::loadShader(string& source, unsigned int ShaderType)
{
    // id of current Shader
    unsigned int id;

    // Specifices the type of Shader to be created
    id = glCreateShader(ShaderType);

    const char* csource = source.c_str();


    // replace the source code in a Shader object
    // 1: number of elements in the string and length arrays
    glShaderSource(id, 1, &csource, NULL);
    glCompileShader(id);

    char error[1000];
    glGetShaderInfoLog(id, 1000, NULL, error);

    if(strlen(error) != 0)
   {
        cout << "Shader Compile Status: \n" << error << endl << endl;
   }

    return id;
}

void Shader::linkShader()
{
    // links the program object
    glLinkProgram(program);
}

void Shader::useShader()
{
	glUseProgram(program);
}

unsigned int Shader::getProgramId()
{
	return program;
}

void Shader::delShader()
{
	glUseProgram(0);
}



string Shader::getVertexShaderName()
{
	return m_vsName;
}

string Shader::getGeometryShaderName()
{
	return m_gsName;
}

string Shader::getFragmentShaderName()
{
	return m_fsName;
}

