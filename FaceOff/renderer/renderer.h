#ifndef RENDERER_H_
#define RENDERER_H_

#include "define.h"
#include <unordered_map>
#include "pipeline.h"
#include "shader.h"
#include "utility_json.h"
#include "utility_gl.h"
#include "utility_gl_uni_loc.h"
#include <stack>


using namespace std;



enum RenderPass
{
	RENDER_PASS1 = 0,
	RENDER_PASS2,
	RENDER_PASS3,
	RENDER_PASS4,
	RENDER_PASS5,
	RENDER_PASS6
};

const int SHADOW_RENDER = 0;
const int TWOPASS_RAYCASTING_RENDER = 1;
const int DEPTH_TEXTURE_RENDER = 2;
const int DEFERRED_SHADING_RENDER = 3;


struct MatricesUniLoc
{
	GLuint modelViewProjection;
	GLuint modelView;
	GLuint projection;
	GLuint view;
	GLuint model;
	GLuint normal;
	GLuint eyePoint;
};

// http://blogs.msdn.com/b/oldnewthing/archive/2004/05/07/127826.aspx
// http://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
/*
always make the base classes' destructors virtual when they're meant to be manipulated polymorphically
*/
struct DataPair
{
	GLuint uniLoc;
	string name;
	virtual ~DataPair(){};

	virtual void setUniLoc(){};
	virtual void printValue(){};

	void printError()
	{
		//		utl::debug(name, "unmatched type");
		cout << name << " unmatched type" << endl;
		exit(1);
	}
	virtual void setValue(bool value){ printError(); };
	virtual void setValue(int value){ printError(); };
	virtual void setValue(float value){ printError(); };
	virtual void setValue(glm::vec2 value){ printError(); };
	virtual void setValue(glm::vec3 value){ printError(); };
	virtual void setValue(glm::vec4 value){ printError(); };
	virtual void setValue(glm::mat3 value){ printError(); };
	virtual void setValue(glm::mat4 value){ printError(); };
};


struct BoolDataPair : public DataPair
{
	bool value;

	BoolDataPair(){};
	BoolDataPair(string n)
	{
		name = n;
	};

	~BoolDataPair(){};

	virtual void setValue(bool value)
	{
		this->value = value;
	}

	virtual void setUniLoc()
	{
		utl::setUniLoc(uniLoc, value);
	}

	virtual void printValue()
	{
		utl::debug("value is", value);
	}
};

struct IntDataPair : public DataPair
{
	int value;

	IntDataPair(){};
	IntDataPair(string n)
	{
		name = n;
	};

	~IntDataPair(){};

	virtual void setValue(int value)
	{
		this->value = value;
	}

	virtual void setUniLoc()
	{
		utl::setUniLoc(uniLoc, value);
	}

	virtual void printValue()
	{
		utl::debug("value is", value);
	}
};

struct FloatDataPair : public DataPair
{
	float value;

	FloatDataPair(){};
	FloatDataPair(string n)
	{
		name = n;
	};

	~FloatDataPair(){};

	void setValue(float value)
	{
		this->value = value;
	}

	void setUniLoc()
	{
		utl::setUniLoc(uniLoc, value);
	}

	void printValue()
	{
		utl::debug("value is", value);
	}
};

struct Vec2DataPair : public DataPair
{
	glm::vec2 value;

	Vec2DataPair(){};
	Vec2DataPair(string n)
	{
		name = n;
	};

	~Vec2DataPair(){};

	void setValue(glm::vec2 value)
	{
		this->value = value;
	}

	void setUniLoc()
	{
		utl::setUniLoc(uniLoc, value);
	}

	void printValue()
	{
		utl::debug("value is", value);
	}
};

struct Vec3DataPair : public DataPair
{
	glm::vec3 value;

	Vec3DataPair(){};
	Vec3DataPair(string n)
	{
		name = n;
	};

	~Vec3DataPair(){};

	void setValue(glm::vec3 value)
	{
		this->value = value;
	}

	void setUniLoc()
	{
		utl::setUniLoc(uniLoc, value);
	}

	void printValue()
	{
		utl::debug("value is", value);
	}
};

struct Vec4DataPair : public DataPair
{
	glm::vec4 value;

	Vec4DataPair(){};
	Vec4DataPair(string n)
	{
		name = n;
	};

	~Vec4DataPair(){};


	void setValue(glm::vec4 value)
	{
		this->value = value;
	}

	void setUniLoc()
	{
		utl::setUniLoc(uniLoc, value);
	}

	void printValue()
	{
		utl::debug("value is", value);
	}
};

struct Mat3DataPair : public DataPair
{
	glm::mat3 value;

	Mat3DataPair(){};
	Mat3DataPair(string n)
	{
		name = n;
	};

	~Mat3DataPair(){};

	void setValue(glm::mat3 value)
	{
		this->value = value;
	}

	void setUniLoc()
	{
		utl::setUniLoc(uniLoc, value);
	}

	void printValue()
	{
		utl::debug("value is", value);
	}
};

struct Mat4DataPair : public DataPair
{
	glm::mat4 value;
	Mat4DataPair(){};
	Mat4DataPair(string n)
	{
		name = n;
	};

	~Mat4DataPair(){};

	void setValue(glm::mat4 value)
	{
		this->value = value;
	}

	void setUniLoc()
	{
		utl::setUniLoc(uniLoc, value);
	}

	void printValue()
	{
		utl::debug("value is", value);
	}
};



enum DATA_PAIR_TYPE
{
	DP_BOOL = 0,
	DP_INT,
	DP_FLOAT,
	DP_VEC2,
	DP_VEC3,
	DP_VEC4,
	DP_MAT3,
	DP_MAT4,
};





class Renderer
{
	public:

		static unordered_map<string, DATA_PAIR_TYPE> m_stringToDPTypeEnum;

		Renderer();
		virtual ~Renderer();

		virtual void setShader(Shader* s);

		void enableShader();
		void disableShader();


		virtual void setUniLocs();
		virtual void setUniLocs(Pipeline& p);


		void addDataPair(string name, int dataType);
		void addDataPairArray(const char* name, int dataType, int count);

		GLuint getUniLoc(Shader* s, const char* name);

		bool initMatricesUniLocs(Shader* s, MatricesUniLoc& Mat);

		void setData(int varEnum, bool value);
		void setData(int varEnum, int value);
		void setData(int varEnum, int value, GLuint textureId, GLuint unit);
		void setData(int varEnum, float value);
		void setData(int varEnum, glm::vec2 value);
		void setData(int varEnum, glm::vec3 value);
		void setData(int varEnum, glm::vec4 value);
		void setData(int varEnum, glm::mat3 value);
		void setData(int varEnum, glm::mat4 value);
		void setDataArray(const char* name, int value, GLuint target, vector<GLuint> textureIDs);
		void errorCheck(int varEnum);

		void setUniLoc(BoolDataPair& dp);
		void setUniLoc(IntDataPair& dp);
		void setUniLoc(FloatDataPair& dp);
		void setUniLoc(Vec2DataPair& dp);
		void setUniLoc(Vec3DataPair& dp);
		void setUniLoc(Vec4DataPair& dp);
		void setUniLoc(Mat3DataPair& dp);
		void setUniLoc(Mat4DataPair& dp);
		void setUniLoc(Mat4DataPair& dp, GLint transpose = GL_FALSE);
		void printDataPairs();


		static void initRendererWrapper(const Array arr, Renderer* r, string name, string path);
		static void initRendererWrapper(const Array arr, Renderer* r, string name);
		static Object findRendererObject(const Array arr, string rName);
		static void initRenderer(const Object obj, Renderer* r, string path);
		static void initRenderer(const Object obj, Renderer* r);

	public:
		MatricesUniLoc m_matricesUniLocs;
		Shader* m_shader;
		vector<DataPair*> m_dataPairs;
		stack<GLuint> m_textureUnitStack;

};


#endif
