
#include "renderer_manager.h"


RendererManager::RendererManager()
{

}

RendererManager::~RendererManager()
{

}


void RendererManager::init(int width, int height)
{
	m_stringToDPTypeEnum =
		unordered_map<string, DATA_PAIR_TYPE>
		({
			{ "DP_BOOL", DP_BOOL },
			{ "DP_INT", DP_INT },
			{ "DP_FLOAT", DP_FLOAT },
			{ "DP_VEC2", DP_VEC2 },
			{ "DP_VEC3", DP_VEC3 },
			{ "DP_VEC4", DP_VEC4 },
			{ "DP_MAT3", DP_MAT3 },
			{ "DP_MAT4", DP_MAT4 }
	});

	char* filename = "renderer/renderer_data.json";

	Value vContent = utl::readJsonFileToVector(filename);
	const Array& vArray = vContent.get_array();

	initRenderer(vArray, &r_fullVertexColor, "r_fullVertexColor");
	initRenderer(vArray, &r_fullColor, "r_fullColor");
	initRenderer(vArray, &r_fullTexture, "r_fullTexture");
	initRenderer(vArray, &r_playerTarget, "r_playerTarget");
	initRenderer(vArray, &r_skybox, "r_skybox");

	initRenderer(vArray, &r_smokeEffectUpdate, "r_smokeEffectUpdate");
	r_smokeEffectUpdate.enableShader();
		r_smokeEffectUpdate.setData(R_SMOKE_EFFECT_UPDATE::u_launcherLifeTime, 100.0f);
		r_smokeEffectUpdate.setData(R_SMOKE_EFFECT_UPDATE::u_shellLifeTime, 10000.0f);
		r_smokeEffectUpdate.setData(R_SMOKE_EFFECT_UPDATE::u_secondaryShellLifeTime, 1000.0f);
	r_smokeEffectUpdate.disableShader();


	initRenderer(vArray, &r_smokeEffectRender, "r_smokeEffectRender");
	r_smokeEffectRender.enableShader();
		r_smokeEffectRender.setData(R_SMOKE_EFFECT_RENDER::u_billBoardSize, 4.0f);
		r_smokeEffectRender.setData(R_SMOKE_EFFECT_RENDER::u_inverseScreenSize, glm::vec2(1.0f / utl::SCREEN_WIDTH, 1.0f / utl::SCREEN_HEIGHT));
		r_smokeEffectRender.setData(R_SMOKE_EFFECT_RENDER::u_zNear, utl::Z_NEAR);
		r_smokeEffectRender.setData(R_SMOKE_EFFECT_RENDER::u_zFar, utl::Z_FAR);
	r_smokeEffectRender.disableShader();


	r_fullVertexColor.printDataPairs();
	r_fullColor.printDataPairs();
	r_fullTexture.printDataPairs();
	r_playerTarget.printDataPairs();

	r_smokeEffectUpdate.printDataPairs();
	r_smokeEffectRender.printDataPairs();
	/*
	Shader* s;

    s = new Shader("full_vertex_color.vs", "full_vertex_color.fs");
    r_fullVertexColor.addShader(s);

    s = new Shader("full_color.vs", "full_color.fs");
    r_fullColor.addShader(s);
    r_fullColor.addDataPair("u_color",  DP_VEC3);

    s = new Shader("full_texture.vs", "full_texture.fs");
    r_fullTexture.addShader(s);
    r_fullTexture.addDataPair("u_texture", DP_INT);

	s = new Shader("player_target.vs", "player_target.fs");
	r_playerTarget.addShader(s);
	r_playerTarget.addDataPair("u_texture", DP_INT);
	

	Renderer r_healthBar;
	*/

	/*
	s = new Shader("particle_effect_update.vs", "particle_effect_update.gs", "particle_effect_update.fs", true);
	r_fireWorkEffectUpdate.addShader(s);

	const GLchar* varyings[4];
	varyings[0] = "type1";
	varyings[1] = "position1";
	varyings[2] = "velocity1";
	varyings[3] = "age1";

	glTransformFeedbackVaryings(s->getProgramId(), 4, varyings, GL_INTERLEAVED_ATTRIBS);

	s->linkShader();
	s->useShader();
	r_fireWorkEffectUpdate.addDataPair("u_deltaTimeMillis", DP_FLOAT);
	r_fireWorkEffectUpdate.addDataPair("u_time", DP_FLOAT);
	r_fireWorkEffectUpdate.addDataPair("u_randomTexture", DP_INT);
	r_fireWorkEffectUpdate.addDataPair("u_launcherLifeTime", DP_FLOAT);
	r_fireWorkEffectUpdate.addDataPair("u_shellLifeTime", DP_FLOAT);
	r_fireWorkEffectUpdate.addDataPair("u_secondaryShellLifeTime", DP_FLOAT);


	r_fireWorkEffectUpdate.enableShader();
		r_fireWorkEffectUpdate.setData("u_launcherLifeTime", 100.0f);
		r_fireWorkEffectUpdate.setData("u_shellLifeTime", 10000.0f);
		r_fireWorkEffectUpdate.setData("u_secondaryShellLifeTime", 100.0f);
	r_fireWorkEffectUpdate.disableShader();




	s = new Shader("particle_effect_render2.vs", "particle_effect_render2.gs", "particle_effect_render2.fs");
	r_fireWorkEffectRender.addShader(s);
	// vs
	r_fireWorkEffectRender.addDataPair("u_time", DP_FLOAT);
	r_fireWorkEffectRender.addDataPair("u_fadeRate", DP_FLOAT);

	// gs
	r_fireWorkEffectRender.addDataPair("u_billBoardSize", DP_FLOAT);
	r_fireWorkEffectRender.addDataPair("u_centerPosition", DP_VEC3);

	// fs
	r_fireWorkEffectRender.addDataPair("u_texture", DP_INT);

	r_fireWorkEffectRender.enableShader();
		r_fireWorkEffectRender.setData("u_billBoardSize", 0.1f);
	r_fireWorkEffectRender.disableShader();



	
	// smoke renderer
	s = new Shader("smoke_effect_update.vs", "smoke_effect_update.gs", "smoke_effect_update.fs", true);
	r_smokeEffectUpdate.addShader(s);

	varyings[0] = "type1";
	varyings[1] = "position1";
	varyings[2] = "velocity1";
	varyings[3] = "age1";

	glTransformFeedbackVaryings(s->getProgramId(), 4, varyings, GL_INTERLEAVED_ATTRIBS);

	s->linkShader();
	s->useShader();
	r_smokeEffectUpdate.addDataPair("u_deltaTimeMillis", DP_FLOAT);
	r_smokeEffectUpdate.addDataPair("u_time", DP_FLOAT);
	r_smokeEffectUpdate.addDataPair("u_randomTexture", DP_INT);
	r_smokeEffectUpdate.addDataPair("u_launcherLifeTime", DP_FLOAT);
	r_smokeEffectUpdate.addDataPair("u_shellLifeTime", DP_FLOAT);
	r_smokeEffectUpdate.addDataPair("u_secondaryShellLifeTime", DP_FLOAT);


	r_smokeEffectUpdate.enableShader();
		r_smokeEffectUpdate.setData("u_launcherLifeTime", 100.0f);
		r_smokeEffectUpdate.setData("u_shellLifeTime", 10000.0f);
		r_smokeEffectUpdate.setData("u_secondaryShellLifeTime", 1000.0f);
	r_smokeEffectUpdate.disableShader();




	s = new Shader("smoke_effect_render.vs", "smoke_effect_render.gs", "smoke_effect_render.fs");
	r_smokeEffectRender.addShader(s);
	// vs
	r_smokeEffectRender.addDataPair("u_time", DP_FLOAT);
	r_smokeEffectRender.addDataPair("u_fadeRate", DP_FLOAT);

	// gs
	r_smokeEffectRender.addDataPair("u_angle", DP_FLOAT);
	r_smokeEffectRender.addDataPair("u_billBoardSize", DP_FLOAT);

	// fs
	r_smokeEffectRender.addDataPair("u_texture", DP_INT);
	r_smokeEffectRender.addDataPair("u_depthTexture", DP_INT);
	r_smokeEffectRender.addDataPair("u_inverseScreenSize", DP_VEC2);
	r_smokeEffectRender.addDataPair("u_zNear", DP_FLOAT);
	r_smokeEffectRender.addDataPair("u_zFar", DP_FLOAT);

	r_smokeEffectRender.enableShader();
		r_smokeEffectRender.setData("u_billBoardSize", 4.0f);
		r_smokeEffectRender.setData("u_inverseScreenSize", glm::vec2(1.0f / utl::SCREEN_WIDTH, 1.0f / utl::SCREEN_HEIGHT));
		r_smokeEffectRender.setData("u_zNear", utl::Z_NEAR);
		r_smokeEffectRender.setData("u_zFar", utl::Z_FAR);
	r_smokeEffectRender.disableShader();






	
	s = new Shader("background_and_particle_composite.vs", "background_and_particle_composite.fs");
	r_composite.addShader(s);
	r_composite.addDataPair("u_backgroundTexutre", DP_INT);
	r_composite.addDataPair("u_particlesTexture", DP_INT);
	*/

	// scene renderers


/*
	s = new Shader("scene_shaders/multitexture_terrain.vs", "scene_shaders/multitexture_terrain.fs");
	r_multiTexTerrain.addShader(s);
	r_multiTexTerrain.addDataPair("u_blendMapTexture", DP_INT);
	r_multiTexTerrain.addDataPairArray("u_textures", DP_INT, 4);

	s = new Shader("scene_shaders/textured_object.vs", "scene_shaders/textured_object.fs");
	r_texturedObject.addShader(s);
	r_texturedObject.addDataPair("u_texture", DP_INT);

	s = new Shader("scene_shaders/billboarding.vs", 
				   "scene_shaders/billboarding_one_quad.gs", 
				   "scene_shaders/billboarding.fs");
	r_billboardOneQuad.addShader(s);
	r_billboardOneQuad.addDataPair("u_texture", DP_INT);
	r_billboardOneQuad.addDataPair("u_centerPosition", DP_VEC3);
	r_billboardOneQuad.addDataPair("u_billboardWidthScale", DP_FLOAT);
	r_billboardOneQuad.addDataPair("u_billboardHeightScale", DP_FLOAT);

	s = new Shader("scene_shaders/billboarding.vs", 
				   "scene_shaders/billboarding_two_quad.gs", 
				   "scene_shaders/billboarding.fs");
	r_billboardTwoQuad.addShader(s);
	r_billboardTwoQuad.addDataPair("u_texture", DP_INT);
	r_billboardTwoQuad.addDataPair("u_centerPosition", DP_VEC3);
	r_billboardTwoQuad.addDataPair("u_billboardWidthScale", DP_FLOAT);
	r_billboardTwoQuad.addDataPair("u_billboardHeightScale", DP_FLOAT);
	*/





	m_backGroundLayerFBO = utl::createFrameBufferObject(width, height);
	m_particleLayerFBO = utl::createFrameBufferObject(width, height);

}

Object RendererManager::findRendererObject(const Array arr, string rName)
{
	Object rObj;
	bool found = false;
	for (int i = 0; i < arr.size(); i++)
	{
		const Object obj = arr[i].get_obj();
		for (Object::size_type i = 0; i != obj.size(); i++)
		{
			const Pair& pair = obj[i];
			const string& name = pair.name_;
			const Value&  value = pair.value_;

			if (name == "r" && value.get_str() == rName)
			{
				rObj = obj;
				found = true;
			}

			break;
		}

		if (found)
			break;
	}

	if (found == false)
	{
		cout << rName << " not found" << endl;
		exit(1);
	}

	return rObj;
}

void RendererManager::initRenderer(const Array arr, Renderer* r, string name)
{
	Object obj = findRendererObject(arr, name);
	initRenderer(obj, r);
}

void RendererManager::initRenderer(const Object obj, Renderer* r)
{
	string vs = "";
	string gs = "";
	string fs = "";
	GLchar** varyings = NULL;

	bool hasGs = false;
	bool transform = false;
	int transformSize = 0;

	Object dataObj;

	for (Object::size_type i = 0; i != obj.size(); i++)
	{
		const Pair& pair = obj[i];
		const string& name = pair.name_;
		const Value&  value = pair.value_;

		if (name == "vs")
		{
			vs = value.get_str();
		}

		else if (name == "gs")
		{
			gs = value.get_str();
			hasGs = true;
		}

		else if (name == "fs")
		{
			fs = value.get_str();
		}

		else if (name == "tf_data")
		{
			transform = true;
			const Array& array = value.get_array();

			transformSize = array.size();
			varyings = new GLchar*[transformSize];

			for (int j = 0; j < transformSize; j++)
			{
				varyings[j] = (GLchar*)((array[j].get_str()).c_str());
			}
		}

		else if (name == "data")
		{
			dataObj = value.get_obj();
		}
	}


	Shader* s;
	if (hasGs && transform)
	{
		s = new Shader(vs.c_str(), gs.c_str(), fs.c_str(), true);
		glTransformFeedbackVaryings(s->getProgramId(), transformSize, varyings, GL_INTERLEAVED_ATTRIBS);
		s->linkShader();
	}
	else if (hasGs)
	{
		s = new Shader(vs.c_str(), gs.c_str(), fs.c_str());
	}
	else
	{
		s = new Shader(vs.c_str(), fs.c_str());
	}

	int size = dataObj.size();
	r->setShader(s);

	for (int j = 0; j < size; j++)
	{
		const string& varName = dataObj[j].name_;
		const Value&  varType = dataObj[j].value_;

		DATA_PAIR_TYPE dpType = m_stringToDPTypeEnum[varType.get_str()];
		r->addDataPair(varName, dpType);
	}
}



void RendererManager::initSceneRendererStaticLightsData(LightManager lightManager)
{
	/*
	r_multiTexTerrain.enableShader();
		r_multiTexTerrain.setDirLightData(lightManager.getDirLight(0));
	r_multiTexTerrain.disableShader();

	r_texturedObject.enableShader();
		r_texturedObject.setDirLightData(lightManager.getDirLight(0));
	r_texturedObject.disableShader();

	r_texturedObject.enableShader();
		r_billboardOneQuad.setDirLightData(lightManager.getDirLight(0));
	r_texturedObject.disableShader();

	r_texturedObject.enableShader();
		r_billboardTwoQuad.setDirLightData(lightManager.getDirLight(0));
	r_texturedObject.disableShader();
	*/
}











