#ifndef MAIN_H_
#define MAIN_H_

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "SDL.h"
#include "SDL_image.h"

#include "define.h"
#include "utility.h"

#include <GL/glew.h>


#include "gol_model_manager.h"

#include "utility.h"
#include "shader.h"

#include "label.h"
#include "gui_manager.h"


#include "imported_model.h"
#include "pipeline.h"

#include <ft2build.h>

#include "renderer_manager.h"
#include "renderer.h"



#include "first_person_camera.h"

#include "quad_model.h"
#include "xyz_axis_model.h"
#include "world_object.h"
#include "skybox.h"
#define FRAME_VALUES 10

using namespace std;
/*

For style
use http://google-styleguide.googlecode.com/svn/trunk/cppguide.html#Function_Names

C++ style
http://geosoft.no/development/cppstyle.html

http://stackoverflow.com/questions/1228161/why-use-prefixes-on-member-variables-in-c-classes

I use:

m for members
c for constants/readonlys
p for pointer (and pp for pointer to pointer)
v for volatile
s for static
i for indexes and iterators
e for events
*/


class FaceOff
{
	private:
		RendererManager              m_rm;
		Renderer*                    p_renderer;

		/// GUI
		long long m_runningTime;
		MouseState m_mouseState;

		Pipeline m_pipeline;


		float m_fps;
		float m_iterRefreshRate;
		float m_curIter;
		unsigned int m_frameCount;
		unsigned int m_frameTicks[FRAME_VALUES];
		unsigned int m_frameTicksIndex;
		unsigned int m_prevFrameTick;

		bool isRunning;

		FirstPersonCamera m_firstPersonCamera;



		Model*          p_model;
		QuadModel       m_groundModel;
		XYZAxisModel    m_xyzModel;
		ImportedModel   m_playerModel;
		ImportedModel   m_gunModel;

		WorldObject     o_worldAxis;
		WorldObject     o_ground;
		WorldObject     o_player;
		WorldObject     o_gun;
		SkyBox          o_skybox;


		GLuint tempTexture;

		GUIManager m_gui;
		GOLModelManager m_GOLModelManager;
	public:

		FaceOff();
		~FaceOff();

		/// init functions
		void initObjects();
		void initModels();
		void initRenderers();
		void initGUI();

		void start();
		void update();

		void forwardRender();
		void renderScene();

		void renderGUI();


		void startCB();
		void resetGameBoardCB();
		void GOLModelListBoxCB();
};

#endif