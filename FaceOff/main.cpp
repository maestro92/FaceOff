#include "main.h"


#include <stdio.h>
#include <string.h>
#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"

#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "network_manager.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000


/*
RakNet Ogre tutorial
http://classes.cs.kent.edu/gpg/trac/wiki/cmarshall

*/


// http://stackoverflow.com/questions/4845410/error-lnk2019-unresolved-external-symbol-main-referenced-in-function-tmainc
#undef main


using namespace std;
//using namespace std::placeholders;
// https://www.youtube.com/watch?v=tlXM8qDOS3U
// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


// frame rate
// https://sites.google.com/site/sdlgamer/intemediate/lesson-7
// FPS is 50
const int FRAMES_PER_SECOND = 60;
// So Interval is 1000/50 which is 20ms
// meaning my while loop runs 50 frames per second
// which is every other 20 ms
const int INTERVAL = 1000 / FRAMES_PER_SECOND;

//display surface
SDL_Surface* pDisplaySurface = NULL;
//event structure
SDL_Event event;

static float runningTime = 0.0f;






FaceOff::FaceOff()
{
	isRunning = true;

	initRenderers();
	initObjects();
	initModels();
	initGUI();


	//Initialize clear color
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

	m_pipeline.setMatrixMode(PROJECTION_MATRIX);
	m_pipeline.loadIdentity();
	m_pipeline.perspective(45, SCREEN_WIDTH / SCREEN_HEIGHT, 0.5, 5000.0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	SDL_WM_SetCaption("GAME OF LIFE", NULL);
}

FaceOff::~FaceOff()
{

}



void FaceOff::initRenderers()
{
	m_rm.init();
	tempTexture = Utility::loadTexture("Assets/Images/tank1B.png");
}


void FaceOff::initObjects()
{
	m_firstPersonCamera = FirstPersonCamera(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	float scale = 20.0;
	o_worldAxis.setScale(scale);
	o_ground.setRotation(glm::rotate(90.0f, 1.0f, 0.0f, 0.0f));
	o_skybox = SkyBox();

	int x = SCREEN_WIDTH - 200;
	int y = 0;
	int w = 200;
	int h = SCREEN_HEIGHT;
//	m_gui.init(SCREEN_WIDTH, SCREEN_HEIGHT, x, y, w, h);
}


void FaceOff::initModels()
{
	float scale = 50.0;

	m_xyzModel = XYZAxisModel();
	m_groundModel = QuadModel(-scale, scale, -scale, scale);

	// m_playerModel.load("./Assets/models/lego_man.obj");
	//    m_gunModel.load("./Assets/models/box.obj");
	//    m_gunModel.load("./Assets/models/phoenix_ugv.md2");
	//    m_gunModel.load("./Assets/models/jeep.obj");
	//    m_gunModel.load("./Assets/models/monkey.obj");


	//    m_gunModel.load("./Assets/models/weapons/deagle.obj");
	//    m_gunModel.load("./Assets/models/weapons/M9/M9.obj");
	m_gunModel.load("./Assets/models/weapons/Ak_47/Ak-47.obj");
	//    m_gunModel.load("./Assets/models/weapons/Flintlock_pistol/Flintlock_pistol.obj");
}


void FaceOff::initGUI()
{
	
	Control::init("", 25, SCREEN_WIDTH, SCREEN_HEIGHT);

	int X_OFFSET = 600;

	int SLIDER_HEIGHT = 35;
	int BUTTON_WIDTH = 200;
	int BUTTON_HEIGHT = 30;

	Control* temp;

	temp = new Label("GAME OF LIFE",
		X_OFFSET, 0,
		BUTTON_WIDTH, 120,
		BLACK);
	temp->setFont(50, WHITE);
	temp->setTextLayout(true, CENTER, CENTER);
	m_gui.addGUIComponent(temp);


	string golDescription = "The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970.";

	temp = new Label(golDescription,
		X_OFFSET, 120,
		BUTTON_WIDTH, 100,
		BLACK);
	temp->setFont(15, WHITE);
	temp->setTextLayout(true, CENTER, TOP_ALIGNED);
	m_gui.addGUIComponent(temp);



	ListBox* lb = new ListBox("", X_OFFSET, 220,
		200, 400,
		WHITE, BLACK, 2,
		std::bind(&FaceOff::GOLModelListBoxCB, this));
	lb->setItemFont(14, GREEN);
//	lb->setContent(m_GOLModelManager.getModels());
	lb->setItemsTextLayout(CENTER, CENTER);
	m_gui.addGUIComponent(lb);



	temp = new Button("Start", X_OFFSET, 535,
		BUTTON_WIDTH, BUTTON_HEIGHT,
		GRAY, BLACK, DARK_BLUE,
		std::bind(&FaceOff::startCB, this));
	temp->setFont(25, GREEN);
	temp->setTextLayout(false, CENTER, CENTER);
	m_gui.addGUIComponent(temp);


	temp = new Button("Reset", X_OFFSET, 570,
		BUTTON_WIDTH, BUTTON_HEIGHT,
		GRAY, BLACK, DARK_BLUE,
		std::bind(&FaceOff::resetGameBoardCB, this));
	temp->setFont(25, GREEN);
	temp->setTextLayout(false, CENTER, CENTER);
	m_gui.addGUIComponent(temp);
	
}


void FaceOff::start()
{
	cout << "Start" << endl;

	Uint32 startTime = SDL_GetTicks();
	Uint32 next_game_tick = 0;
	Uint32 delay_time = 0;

	while (isRunning)
	{
		startTime = SDL_GetTicks();

		while (SDL_PollEvent(&event))
		{
			int tmpx, tmpy;
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
					cout << "clicking Up left" << endl;
					m_mouseState.m_leftButtonDown = false;
					SDL_GetMouseState(&tmpx, &tmpy);
					break;

				case SDL_BUTTON_RIGHT:
					cout << "clicking Up right" << endl;
					m_mouseState.m_rightButtonDown = false;
					SDL_GetMouseState(&tmpx, &tmpy);
					break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:

				switch (event.button.button)
				{
					int tmpx, tmpy;
				case SDL_BUTTON_LEFT:
					cout << "clicking left" << endl;
					SDL_GetMouseState(&tmpx, &tmpy);
					m_mouseState.m_leftButtonDown = true;
					m_firstPersonCamera.setMouseIn(true);
					break;

				case SDL_BUTTON_RIGHT:
					cout << "clicking right" << endl;
					SDL_GetMouseState(&tmpx, &tmpy);
					m_mouseState.m_rightButtonDown = true;
					break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{

				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
					break;

				case SDLK_z:
					m_firstPersonCamera.setMouseIn(false);
					break;

				case SDLK_SPACE:
					break;
				}
				break;
			}
		}
		update();
		forwardRender();
		SDL_GL_SwapBuffers();
	}
}


void FaceOff::update()
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	m_mouseState.m_pos = glm::vec2(mx, SCREEN_HEIGHT - my);

}


void FaceOff::forwardRender()
{
	m_pipeline.setMatrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();

	m_firstPersonCamera.control(m_pipeline);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	o_skybox.setPosition(-m_firstPersonCamera.getEyePoint());
	o_skybox.render(m_pipeline);



	m_pipeline.setMatrixMode(MODEL_MATRIX);
	p_renderer = &m_rm.r_fullVertexColor;
	p_model = &m_xyzModel;
	o_worldAxis.renderSingle(m_pipeline, p_renderer, RENDER_PASS1, p_model);

	p_renderer = &m_rm.r_fullColor;
	p_renderer->setData("u_color", glm::vec3(0.5, 0.0, 0.0));
	p_model = &m_groundModel;
	o_ground.renderSingle(m_pipeline, p_renderer, RENDER_PASS1, p_model);

	p_model = &m_playerModel;
	o_player.renderSingle(m_pipeline, p_renderer, RENDER_PASS1, p_model);


	p_renderer = &m_rm.r_fullTexture;
	p_renderer->setData("u_texture", 0, GL_TEXTURE_2D, 0);
	p_model = &m_gunModel;
	o_gun.renderSingle(m_pipeline, p_renderer, RENDER_PASS1, p_model);


	//   renderGUI();
}





void FaceOff::renderGUI()
{
	
	m_gui.initGUIRenderingSetup();
	/// http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement
	unsigned int getTicks = SDL_GetTicks();

	//    string final_str = "(" + Utility::floatToStr(m_mouseState.m_pos.x) + ", " + Utility::floatToStr(m_mouseState.m_pos.y) + ")";

	m_gui.updateAndRender(m_mouseState);
	
}

int main(int argc, char *argv[])
{
	Utility::initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, pDisplaySurface);
	Utility::initGLEW();

	networkManager();

	FaceOff Martin;
	Martin.start();

	Utility::exitSDL(pDisplaySurface);
	//normal termination
	cout << "Terminating normally." << endl;
	return EXIT_SUCCESS;
}


/// Function CallBacks
void FaceOff::startCB()
{

}

void FaceOff::resetGameBoardCB()
{

}

void FaceOff::GOLModelListBoxCB()
{

}
