#include "main.h"

#define NETWORK_FLAG 0
#define SERVER_RENDER_FLAG 0


#define SERVER_NETWORK_THREAD 0
#define CLIENT_NETWORK_THREAD 0

#include <stdio.h>
#include <string.h>
#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"

#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "network_manager.h"


#define RENDER_DEBUG_FLAG 0

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

bool incrFlag = true;
bool incrFlag2 = true;

bool incrAngleFlag = true;
bool incrAngleFlag2 = true;

float tempPitch = 0;
float tempYaw = 0;

float tempPitch2 = 0;
float tempYaw2 = 0;

// the server simluates the game in descirete time steps called ticks

glm::vec3 oldPos;

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


// frame rate
// https://sites.google.com/site/sdlgamer/intemediate/lesson-7
// FPS is 50
// So Interval is 1000/50 which is 20ms
// meaning my while loop runs 50 frames per second
// which is every other 20 ms
const int FRAMES_PER_SECOND = 60;
const int INTERVAL = 1000 / FRAMES_PER_SECOND;

//display surface
SDL_Surface* pDisplaySurface = NULL;
//event structure
SDL_Event event;

static float runningTime = 0.0f;


// 15 ms, 66.6 ticks per sec are simulated
const int SERVER_SIMLUATION_FRAMES_PER_SECOND = 66;
const int SERVER_SIMLUATION_TIME_STEP = 1000 / SERVER_SIMLUATION_FRAMES_PER_SECOND;


// 50 ms, 20 snapshots per second
const int SERVER_SNAPSHOT_PER_SECOND = 20;
const int SERVER_SNAPSHOT_TIME_STEP = 1000 / SERVER_SNAPSHOT_PER_SECOND;

// But instead of sending a new packet to the server for each user command, the client sends command packets at a certain rate of packets per second (usually 30).
// This means two or more user commands are transmitted within the same packet.
const int CLIENT_INPUT_SENT_PER_SECOND = 33;
const int CLIENT_INPUT_SENT_TIME_STEP = 1000 / SERVER_SNAPSHOT_PER_SECOND;

RendererManager FaceOff::m_rendererMgr;
ModelManager	FaceOff::m_modelMgr;

FaceOff::FaceOff()
{
	m_isServer = false;
}

FaceOff::~FaceOff()
{



	if (m_networkThread.joinable())
	{
		m_networkThread.join();
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);
}


void FaceOff::init()
{
	isRunning = true;

	containedFlag = false;
	hitNode = NULL;

	initObjects();
	initRenderers();
   
	// initAudio();
#if NETWORK_FLAG == 1
	initNetworkLobby();
#else
	m_defaultPlayerID = 0;
#endif

	// initGUI depends on the m_defaultPlayerID, so initNetworkLobby needs to run first
	initGUI();


	//Initialize clear color
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

	m_zoomFactor = 1.0f;
	m_pipeline.setMatrixMode(PROJECTION_MATRIX);
	m_pipeline.loadIdentity();
	m_pipeline.perspective(45 * m_zoomFactor, utl::SCREEN_WIDTH / utl::SCREEN_HEIGHT, utl::Z_NEAR, utl::Z_FAR);

	Model::enableVertexAttribArrays();

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	SDL_WM_SetCaption("FaceOff", NULL);
}

GLuint tempTexture;




void FaceOff::initObjects()
{
	m_modelMgr.init();
	m_nm.init(&m_modelMgr, &m_objects, &m_players);

	if (m_isServer)
	{
		m_serverCamera = FirstPersonCamera(glm::vec3(49.83, 200.67, 59.87));
		m_serverCamera.setPitch(-63.5);
		m_serverCamera.setYaw(56.5);
		m_serverCamera.setFreeMode(true);

	//	utl::debug("m_serverCamera eye", m_serverCamera.getEyePoint());
	}

	float scale = 100.0;
	o_worldAxis.setScale(scale);
	o_worldAxis.setModelEnum(ModelEnum::xyzAxis);
	o_worldAxis.setModel(m_modelMgr.get(ModelEnum::xyzAxis));

	
	o_sampleBullet.setScale(1.0, 5.0, 1.0);

	o_skybox = SkyBox();




	//	WeaponManager::initWeaponsData();
	//	Weapon::initGameWeapons();

	/*
	float xbound = 150;
	float ybound = 50;
	float zbound = 150;
	*/

	float wallWidth = 10.0f;

	float xbound = 150;
	float ybound = 50;
	float zbound = 150;

	float xboundWithWalls = 150 + wallWidth * 2;
	float zboundWithWalls = 150 + wallWidth * 2;

	scale = 150;
	WorldObject* o_temp = new WorldObject();
	o_temp->setScale(xbound, zbound, 1.0);
	o_temp->setRotation(glm::rotate(90.0f, 1.0f, 0.0f, 0.0f));
	o_temp->setModelEnum(ModelEnum::ground);
	o_temp->setModel(m_modelMgr.get(ModelEnum::ground));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	o_temp->m_name = "ground";
	m_objects.push_back(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "south wall";
	o_temp->setScale(xbound, ybound / 2, wallWidth);
	o_temp->setPosition(0, ybound / 2, zbound + wallWidth);
//	o_temp->setModel(m_modelMgr.m_cube);
	o_temp->setModelEnum(ModelEnum::cube);
	o_temp->setModel(m_modelMgr.get(ModelEnum::cube));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "north wall";
	o_temp->setPosition(0, ybound / 2, -zbound - wallWidth);
	o_temp->setScale(xbound, ybound / 2, wallWidth);
	o_temp->setRotation(glm::rotate(180.0f, 0.0f, 1.0f, 0.0f));
//	o_temp->setModel(m_modelMgr.m_cube);
	o_temp->setModelEnum(ModelEnum::cube);
	o_temp->setModel(m_modelMgr.get(ModelEnum::cube));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "west wall";
	o_temp->setPosition(-xbound - wallWidth, ybound / 2, 0.0);
	o_temp->setScale(wallWidth, ybound / 2, zbound);
//	o_temp->setModel(m_modelMgr.m_cube);
	o_temp->setModelEnum(ModelEnum::cube);
	o_temp->setModel(m_modelMgr.get(ModelEnum::cube));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "east wall";
	o_temp->setPosition(xbound + wallWidth, ybound / 2, 0.0);
	o_temp->setScale(wallWidth, ybound / 2, zbound);
//	o_temp->setModel(m_modelMgr.m_cube);
	o_temp->setModelEnum(ModelEnum::cube);
	o_temp->setModel(m_modelMgr.get(ModelEnum::cube));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	/*
	scale = 15.0f;
	o_temp = new WorldObject();
	o_temp->setScale(scale);
	o_temp->setPosition(0, 0, 20);
	o_temp->setModel(&m_stairs);
	o_temp->updateAABB();
	o_temp->m_name = "stairs 20";
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->setScale(scale);
	o_temp->setPosition(0, 0, -20);
	o_temp->setRotation(glm::rotate(180.0f, 0.0f, 1.0f, 0.0f));
	o_temp->setModel(&m_stairs);
	o_temp->updateAABB();
	o_temp->m_name = "stairs -20";
	m_objects.push_back(o_temp);
	*/

	scale = 10.0f;
	float gap = 10.0;

	//	for (int i = 0; i < 2; i++)
	for (int i = 0; i < 4; i++)
	{
		o_temp = new WorldObject();
		float x = 10 + i * 2 * gap + 50;
		float y = 0;
		float z = -10 + i * gap;

		o_temp->setPosition(x, scale / 2, z);
		o_temp->setScale(scale);
		o_temp->m_name = "woodenBox " + utl::floatToStr(x);
//		o_temp->setModel(m_modelMgr.m_woodenBox);
		o_temp->setModelEnum(ModelEnum::woodenBox);
		o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
		o_temp->setCollisionDetectionGeometry(CD_AABB);
		m_objects.push_back(o_temp);
	}


	//	for (int i = 0; i < 1; i++)
	for (int i = 0; i < 4; i++)
	{
		o_temp = new WorldObject();
		float x = -10 - i * 2 * gap - 50;
		float y = 0;
		float z = -10 + i * gap;

		o_temp->setPosition(x, scale / 2, z);
		o_temp->setScale(scale);
		o_temp->m_name = "woodenBox " + utl::floatToStr(x);
//		o_temp->setModel(m_modelMgr.m_woodenBox);
		o_temp->setModelEnum(ModelEnum::woodenBox);
		o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
		o_temp->setCollisionDetectionGeometry(CD_AABB);
		m_objects.push_back(o_temp);
	}



	float thinGap = 45, thickGap = 80;
	float halfThinGap = thinGap / 2, halfThickGap = thickGap / 2;

	float pillarXScale = (xbound * 2 - thinGap * 3) / 2;
	float pillarYScale = 100;
	float pillarZScale = (zbound * 2 - thickGap * 2 - thinGap) / 2;

	float halfPosXMag = halfThinGap + pillarXScale / 2;
	float halfPosZMag = halfThinGap + pillarZScale / 2;

	utl::debug("scale x is", pillarXScale);
	utl::debug("scale z is", pillarZScale);


	utl::debug("x is", halfPosXMag);
	utl::debug("z is", halfPosZMag);
	
	o_temp = new WorldObject();
	o_temp->m_name = "NW Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(-halfPosXMag, pillarYScale / 2, -halfPosZMag);
//	o_temp->setModel(m_modelMgr.m_woodenBox);
	o_temp->setModelEnum(ModelEnum::woodenBox);
	o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "NE Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(halfPosXMag, pillarYScale / 2, -halfPosZMag);
//	o_temp->setModel(m_modelMgr.m_woodenBox);
	o_temp->setModelEnum(ModelEnum::woodenBox);
	o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "SW Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(-halfPosXMag, pillarYScale / 2, halfPosZMag);
//	o_temp->setModel(m_modelMgr.m_woodenBox);
	o_temp->setModelEnum(ModelEnum::woodenBox);
	o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "SE Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(halfPosXMag, pillarYScale / 2, halfPosZMag);
//	o_temp->setModel(m_modelMgr.m_woodenBox);
	o_temp->setModelEnum(ModelEnum::woodenBox);
	o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	utl::debug("id is", o_temp->m_instanceId);
	float formationGap = 40.0f;
	



	o_temp = new Weapon(m_modelMgr.getWeaponData(MAC_11));
	o_temp->m_name = "MAC_11";
	o_temp->setPosition(-3 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(AWM));
	o_temp->m_name = "AWM";
	o_temp->setPosition(-2 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);



	o_temp = new Weapon(m_modelMgr.getWeaponData(MINIGUN));
	o_temp->m_name = "MINIGUN";
	o_temp->setPosition(-1 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);



	o_temp = new Weapon(m_modelMgr.getWeaponData(KNIFE));
	o_temp->m_name = "knife";
	o_temp->setPosition(formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	/*
	o_temp = new Weapon(m_modelMgr.getWeaponData(FRAG_GRENADE));
	o_temp->m_name = "FRAG_GRENADE";
	o_temp->setAABBByPosition(2 * formationGap, 5, -110);
	m_objects.push_back(o_temp);
	*/




	o_temp = new Weapon(m_modelMgr.getWeaponData(MP5));
	o_temp->m_name = "MP5";
	o_temp->setPosition(-3 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(MG42));
	o_temp->m_name = "MG42";
	o_temp->setPosition(-2 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	// init weapons for the map
	o_temp = new Weapon(m_modelMgr.getWeaponData(AK_47));
	o_temp->m_name = "AK 47";
	o_temp->setPosition(-formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(M16));
	o_temp->m_name = "M16";
	o_temp->setPosition(formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(KATANA));
	o_temp->m_name = "katana";
	o_temp->setPosition(2 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);



	o_temp = new Weapon(m_modelMgr.getWeaponData(PISTOL_SHOTGUN));
	o_temp->m_name = "shotgun";
	o_temp->setPosition(3 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);




	vector<WorldObject*> objectsForKDTree;

	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* obj = m_objects[i];

		if (obj->getObjectType() == WEAPON && ((Weapon*)(obj))->hasOwner == true)
		{
			continue;
		}
		objectsForKDTree.push_back(obj);
	}


	m_objectKDtree.build(objectsForKDTree, glm::vec3(xboundWithWalls + 1, ybound + 1, zboundWithWalls + 1), glm::vec3(-xboundWithWalls - 1, -1, -zboundWithWalls - 1));




#if NETWORK_FLAG == 0

	Weapon* w = new Weapon(m_modelMgr.getWeaponData(M16));
	w->setCollisionDetectionGeometry(CD_AABB);
	w->m_name = "player0 mainWeapon";

	//	Weapon* pistol = new Weapon(m_modelMgr.getWeaponData());
	Weapon* k = new Weapon(m_modelMgr.getWeaponData(KNIFE));
	k->setCollisionDetectionGeometry(CD_AABB);
	k->m_name = "player0 knife";

	Weapon* g = new Weapon(m_modelMgr.getWeaponData(FRAG_GRENADE));
	g->setMass(0.4);
	g->setMaterialEnergyRestitution(0.6);
	g->setMaterialSurfaceFriction(0.3);
	g->setCollisionDetectionGeometry(CD_AABB);
	g->m_name = "player0 grenade";



	Weapon* w1 = new Weapon(m_modelMgr.getWeaponData(M16));
	w1->setCollisionDetectionGeometry(CD_AABB);
	w1->m_name = "player1 mainWeapon";
	
	Weapon* k1 = new Weapon(m_modelMgr.getWeaponData(KNIFE));
	k1->setCollisionDetectionGeometry(CD_AABB);
	k1->m_name = "player1 knife";

	Weapon* g1 = new Weapon(m_modelMgr.getWeaponData(FRAG_GRENADE));
	g1->setMass(0.4);
	g1->setMaterialEnergyRestitution(0.6);
	g1->setMaterialSurfaceFriction(0.3);
	g1->setCollisionDetectionGeometry(CD_AABB);
	g1->m_name = "player1 grenade";



	Weapon* w2 = new Weapon(m_modelMgr.getWeaponData(M16));
	w2->setCollisionDetectionGeometry(CD_AABB);
	w2->m_name = "player2 mainWeapon";

	Weapon* k2 = new Weapon(m_modelMgr.getWeaponData(KNIFE));
	k2->setCollisionDetectionGeometry(CD_AABB);
	k2->m_name = "player2 knife";

	Weapon* g2 = new Weapon(m_modelMgr.getWeaponData(FRAG_GRENADE));
	g2->setMass(0.4);
	g2->setMaterialEnergyRestitution(0.6);
	g2->setMaterialSurfaceFriction(0.3);
	g2->setCollisionDetectionGeometry(CD_AABB);
	g2->m_name = "player2 grenade";

	m_objects.push_back(w);
	m_objects.push_back(k);
	m_objects.push_back(g);
	
	
	m_objects.push_back(w1);
	m_objects.push_back(k1);
	m_objects.push_back(g1);

	m_objects.push_back(w2);
	m_objects.push_back(k2);
	m_objects.push_back(g2);
	

	m_defaultPlayerID = 0;
	Player* p = new Player(m_defaultPlayerID);
	p->m_name = "player 0";
	p->setDefaultPlayerFlag(true);
	p->setModelEnum(ModelEnum::player);
	p->setModel(m_modelMgr.get(ModelEnum::player));
	p->setMass(80);
	p->setCollisionDetectionGeometry(CD_SPHERE);

	p->pickUp(w);
	p->pickUp(k);
	p->pickUp(g);

	m_players.push_back(p);

	/*
	p = new Player(1);
	p->m_name = "player 1";
	p->setPosition(0, 5, 25);
	p->setModelEnum(ModelEnum::player);
	p->setModel(m_modelMgr.get(ModelEnum::player));
	p->setMass(80);
	p->setCollisionDetectionGeometry(CD_SPHERE);

	p->pickUp(w1);
	p->pickUp(k1);
	p->pickUp(g1);

	m_players.push_back(p);


	p = new Player(2);
	p->m_name = "player 2";
	p->setPosition(0, 5, -40);
	p->setModelEnum(ModelEnum::player);
	p->setModel(m_modelMgr.get(ModelEnum::player));
	p->setMass(80);
	p->setCollisionDetectionGeometry(CD_SPHERE);

	p->pickUp(w2);
	p->pickUp(k2);
	p->pickUp(g2);

	m_players.push_back(p);
	*/



#endif

	for (int i = 0; i < m_players.size(); i++)
		m_objectKDtree.insert(m_players[i]);




	
	// Grenade particle effect
	FireWorkEffect* fwEffect = new FireWorkEffect();
	fwEffect->setPosition(glm::vec3(15.0, 0.0, 13.0));
	fwEffect->setScale(50.0);

	fwEffect->init();
	fwEffect->setTexture("Assets/fireworks_red.jpg");

	m_fireWorkEffects.push_back(fwEffect);
	

	
	SmokeEffect* smEffect = new SmokeEffect();
	smEffect->setPosition(glm::vec3(15.0, 5.0, -53.0));
	smEffect->setScale(5.0);

	smEffect->init();
	m_smokeEffects.push_back(smEffect);
	
//	m_objectKDtree.print();
}





void FaceOff::initRenderers()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_rendererMgr.init(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
	m_rendererMgr.initSceneRendererStaticLightsData(m_lightManager);
}


void FaceOff::initNetworkServerClient()
{
	bool invalidInput = true;
	utl::debugLn(5);
	while (invalidInput)
	{
		char str[512];
		printf("(C)lient or (S)erver?\n");
		gets(str);

		if ((str[0] == 's') || (str[0] == 'S'))
		{
			m_isServer = true;
			invalidInput = false;
		}
		else if (str[0] == 'c' || str[0] == 'C')
		{
			m_isServer = false;
			invalidInput = false;
		}
		else
		{
			printf("InvalidInput!");
		}
	}
}

void FaceOff::initNetworkLobby()
{
	if (m_isServer)
	{
		peer = RakNet::RakPeerInterface::GetInstance();

		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);

		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		// Sets how many incoming connections are allowed. 
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else
	{
		connected = false;
		peer = RakNet::RakPeerInterface::GetInstance();

		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);

		char str[512];

		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0] == 0)
			strcpy(str, "127.0.0.1");

		printf("Starting the client.\n");
		peer->Connect(str, SERVER_PORT, 0, 0);
	}




	// we're waiting in the lobby
	bool waitingInLobby = true;

	// we're gonna settle the player list
	printf("Waiting in Lobby.\n");

	if (m_isServer)
	{
		m_defaultPlayerID = -1;

		while (waitingInLobby)
		{
			//			printf("In the lobby waiting loop.\n");			
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					case SDL_KEYDOWN:
						switch (event.key.keysym.sym)
						{
							case SDLK_a:
								utl::debug("pressed A, ending waitingInLobby");
								waitingInLobby = false;
								break;
						}
				}
			}


			// iterate over each message received
			for (packet = peer->Receive();
				packet;
				peer->DeallocatePacket(packet), packet = peer->Receive())
			{
				// we first initalized bitStream with the packet->data
				RakNet::BitStream bsIn(packet->data, packet->length, false);

				// we ignore the first part of each message (due to RakNet convention)
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));



				cout << endl << "	****	New Packet from:" << packet->guid.g << endl;

				cout << "Player List" << endl;
				for (int i = 0; i < (int)m_players.size(); i++)
					cout << i << " - " << m_players[i]->m_guid.g << endl;

				// Handle message here 
				switch (packet->data[0])
				{
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					printf("Another client has disconnected.\n");
					break;

				case ID_REMOTE_CONNECTION_LOST:
					printf("Another client has lost the connection.\n");
					break;

				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					printf("Another client has connected.\n");
					break;


				case ID_NEW_INCOMING_CONNECTION:
				{
					printf("A connection is incoming.\n");

					int newPlayerId = m_players.size();
					float newSpawnX = newPlayerId * 30;
					float newSpawnY = 5;
					float newSpawnZ = newPlayerId * 30;

					// Add Player
					Player* p = new Player(newPlayerId);
					p->m_guid = packet->guid;
					p->m_name = "player " + utl::intToStr(newPlayerId);
					p->setPosition(newSpawnX, newSpawnY, newSpawnZ);
					p->setRotation(0, 0);
					p->setModelEnum(ModelEnum::player);
					p->setModel(m_modelMgr.get(ModelEnum::player));
					p->setMass(80);
					p->setCollisionDetectionGeometry(CD_SPHERE);


					Weapon* mainWeapon = new Weapon(m_modelMgr.getWeaponData(M16));
					mainWeapon->setCollisionDetectionGeometry(CD_AABB);
					mainWeapon->m_name = "player mainWeapon";

					Weapon* knife = new Weapon(m_modelMgr.getWeaponData(KNIFE));
					knife->setCollisionDetectionGeometry(CD_AABB);
					knife->m_name = "player knife";

					Weapon* grenade = new Weapon(m_modelMgr.getWeaponData(FRAG_GRENADE));
					grenade->setMass(0.4);
					grenade->setMaterialEnergyRestitution(0.6);
					grenade->setMaterialSurfaceFriction(0.3);
					grenade->setCollisionDetectionGeometry(CD_AABB);
					grenade->m_name = "player grenade";

					p->pickUp(mainWeapon);
					p->pickUp(knife);
					p->pickUp(grenade);


					m_players.push_back(p);
					m_objects.push_back(mainWeapon);
					m_objects.push_back(knife);
					m_objects.push_back(grenade);


					utl::debug("new_player_id", newPlayerId);
					utl::debug("player position", m_players[newPlayerId]->getPosition());

					if (m_players.size() > 0)
					{
						// send new client notification to existing clients
						cout << "Signaling arrival of new clients, Sending new client's spaw position to each client" << endl;
						p->toBitStream((RakNet::MessageID)NEW_CLIENT, bsOut);

						for (int i = 0; i < m_players.size(); i++)
						{
							if (i == newPlayerId)
								continue;

							cout << " To: " << i << " - " << m_players[i]->m_guid.g << endl;
							peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(m_players[i]->m_guid), false);
						}
						
	

						// Sending each client's position to new client
						utl::debug("m_players size", m_players.size());
						cout << "Sending each client's position to new client" << endl;

						for (int i = 0; i < m_players.size(); i++)
						{
							if (i == newPlayerId)
								continue;

							cout << "sending for " << i << endl;
							p->toBitStream((RakNet::MessageID)NEW_CLIENT, bsOut);
							peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
						}
					}
					else
					{
						cout << "No clients yet, didn't send spawn pos to existing " << "nor each existing pos to new " << endl;
					}



					// Use a BitStream to write a custom user message
					// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically

					// write a WELCOME message and include the clients index + 1
					p->toBitStream((RakNet::MessageID)SPAWN_INFORMATION, bsOut);

					// send the message back to the same address the current packet came from (the new client)
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

					cout << "Player List" << endl;
					for (int i = 0; i < m_players.size(); i++)
					{
						cout << i << " - " << m_players[i]->getId() << " position " << m_players[i]->m_position.x << " "
							<< m_players[i]->m_position.y << " "
							<< m_players[i]->m_position.z << endl;
					}

					break;
				}


				case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");
					break;
				case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
					break;

				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
				}
				bsOut.Reset();
			}
		}
	}
	else
	{
		while (waitingInLobby)
		{
			// iterate over each message received
			for (packet = peer->Receive();
				packet;
				peer->DeallocatePacket(packet), packet = peer->Receive())
			{

				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

				switch (packet->data[0])
				{
					case SPAWN_INFORMATION:
					{
						Player* newPlayer = m_nm.spawnClientPlayer(bsIn, true);
						newPlayer->setDefaultPlayerFlag(true);
						m_defaultPlayerID = newPlayer->getId();

						server_address = packet->systemAddress;

						printf("Server said I'm client number %d at %f, %f, %f\n", newPlayer->getId(), newPlayer->getPosition().x, newPlayer->getPosition().y, newPlayer->getPosition().z);
						break;
					}


					case NEW_CLIENT:
					{
						Player* newPlayer = m_nm.spawnClientPlayer(bsIn, false);
						printf("Received new client info for %d: %f, %f, %f", newPlayer->getId(), newPlayer->getPosition().x, newPlayer->getPosition().y, newPlayer->getPosition().z);

						cout << "Player List" << endl;
						for (int i = 0; i < m_players.size(); i++)
						{

							if (m_players[i] == NULL)
							{
								cout << "player " << i << " is null" << endl;
							}
							else
							{
								cout << i << " - " << m_players[i]->getId() << " position " << m_players[i]->m_position.x << " "
									<< m_players[i]->m_position.y << " "
									<< m_players[i]->m_position.z << endl;
							}
						}
						break;
					}

					case LOBBY_WAIT_END:
						waitingInLobby = false;
						break;

					case ID_CONNECTION_REQUEST_ACCEPTED:
						connected = true;
						break;
					}

			}
		}
	}



	if (m_isServer)
	{
		// send the end termination signal to client

		for (int i = 0; i < m_players.size(); i++)
		{
			if (i == m_defaultPlayerID)
				continue;

			cout << "sending each client lobby wait end signal" << i << endl;
			bsOut.Reset();
			bsOut.Write((RakNet::MessageID)LOBBY_WAIT_END);
			bsOut.Write(i);

			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(m_players[i]->m_guid), false);
		}

	}

	utl::debug("m_defaultPlayerId", m_defaultPlayerID);

	cout << "Player List" << endl;
	for (int i = 0; i < m_players.size(); i++)
	{

		if (m_players[i] == NULL)
		{
			cout << "player " << i << " is null" << endl;
		}
		else
		{
			cout << i << " - " << m_players[i]->getId() << " position " << m_players[i]->m_position.x << " "
				<< m_players[i]->m_position.y << " "
				<< m_players[i]->m_position.z << endl;
		}
	}


	printf("Done waiting in the Lobby.\n");

}




/*
while running
{
	lock
		check incoming packets
			put in queue
	unlock


	check time
	send outgoing packets

}



-render thread

while running
{

	process stuff in queue
		lock
			temp = queue.top
		unlock
		queue.pop

	render as many frames as possible
}
*/

void FaceOff::serverNetworkThread()
{
	Uint32 lastSnapShotSendTime = 0;
	Uint32 curTime = 0;

	while (isRunning)
	{
		
		// iterate over each message received
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			// we first initalized bitStream with the packet->data
			RakNet::BitStream bsIn(packet->data, packet->length, false);

			// we ignore the first part of each message (due to RakNet convention)
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			// Handle message here 
			switch (packet->data[0])
			{
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					printf("Another client has disconnected.\n");
					break;
				case ID_REMOTE_CONNECTION_LOST:
					printf("Another client has lost the connection.\n");
					break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					printf("Another client has connected.\n");
					break;
				case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");
					break;
				case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
					break;

				case CLIENT_INPUT:
				{
//					utl::debug(">>>>>> incoming client input");
					m_inputQueue.setFromBitStream(bsIn);

					/*
					utl::debug("m_inputQueue size ", m_inputQueue.size());
					
					if (!m_inputQueue.empty())
					{
						int size = m_inputQueue.buffer.size();
						for (int i = 0; i < size; i++)
						{
							Move move = m_inputQueue.front();
							m_inputQueue.pop();
							move.print();
						}

						cout << endl << endl;
					}
					*/

					break;
				}

				case PLAYER_UPDATE:
				{
					// received new position from client    
					int player_id = 0;

					m_players[player_id]->setFromBitStream(bsIn);

					printf("Player %d sent new position ", player_id);			utl::debug("", m_players[player_id]->getPosition());
					printf("Player %d sent new weapon position ", player_id);	
					printf("Player %d sent new pitch ", player_id);				utl::debug("", m_players[player_id]->getCameraPitch());
					printf("Player %d sent new yaw ", player_id);				utl::debug("", m_players[player_id]->getCameraYaw());
				


					// sending new position value to each client
					cout << "sending new position value to each client" << endl;
					m_players[player_id]->toBitStream((RakNet::MessageID)PLAYER_UPDATE, bsOut);

					for (int i = 0; i < m_players.size(); i++)
					{
						if (player_id != i)
						{
							cout << "	To: " << " - " << m_players[i]->m_guid.g << endl;
							peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(m_players[i]->m_guid), false);
						}
						else
							cout << "	Not Sending to own client: " << player_id << endl;
					}
					break;
				}



				default:
				{
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
				}

			}
		}
		
		
		curTime = SDL_GetTicks();

		if (curTime - lastSnapShotSendTime > SERVER_SNAPSHOT_TIME_STEP)
		{
		//	utl::debug("	>>>> Sending snapshot to shits at time", (int)curTime);

			lastSnapShotSendTime = curTime;
		}
	}
}



/*
The client creates user commands from sampling input devices with the same tick rate that the server is running 
with. 

Instead of sending a new packet to the server for each user command, the client sends command packets at a certain rate of packets per second (usually 30).
This means two or more user commands are transmitted within the same packet. The Clients can increase the command rate with c1-cmdrate. This will increase
responsiveness but requires more outgoing bandwidth, too. 

https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking
*/
void FaceOff::clientNetworkThread()
{
	Uint32 lastSnapShotSendTime = 0;
	Uint32 curTime = 0;

#if 1
	while (isRunning)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			cout << "here" << endl;

			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			switch (packet->data[0])
			{
			case NEW_CLIENT:
				cout << "NEW_CLIENT message " << endl;
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				break;

			case SNAPSHOT_FROM_SERVER:
			{
				// snap shot from server
				cout << "SNAPSHOT from server" << endl;

				break;
			}

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}

		// sending server my inputs
		curTime = SDL_GetTicks();

		if (curTime - lastSnapShotSendTime > CLIENT_INPUT_SENT_TIME_STEP)
		{
			if (!m_inputQueue.empty())
			{
		//		cout << "sending server my inputs" << endl;
				m_inputQueue.toBitStream(bsOut);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, server_address, false);
			}
		
			lastSnapShotSendTime = curTime;
		}
	}
#endif
}



void FaceOff::start()
{
	cout << "Start" << endl;
	Uint32 startTime = SDL_GetTicks();
	m_nextGameTick = 0;

#if NETWORK_FLAG == 1
	
	if (m_isServer)
	{
		m_networkThread = thread(&FaceOff::serverNetworkThread, this);
	}
	else
	{
		m_networkThread = thread(&FaceOff::clientNetworkThread, this);
	}
	
#endif

	while (isRunning)
	{
		startTime = SDL_GetTicks();

		if (m_isServer)
		{
			serverHandleDeviceEvents();
		}
		else
		{
			clientHandleDeviceEvents();
		}

		update();
		render();
		SDL_GL_SwapBuffers();
	}
}


void FaceOff::serverHandleDeviceEvents()
{

	while (SDL_PollEvent(&event))
	{
		int tmpx, tmpy;
		switch (event.type)
		{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_MOUSEBUTTONDOWN:

				switch (event.button.button)
				{
					int tmpx, tmpy;
					case SDL_BUTTON_LEFT:
					{
						SDL_GetMouseState(&tmpx, &tmpy);
						m_mouseState.m_leftButtonDown = true;
						m_serverCamera.setMouseIn(true);
						break;
					}
				}
				break;


			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						isRunning = false;
						break;

					case SDLK_0:
						containedFlag = !containedFlag;
						break;


					case SDLK_z:
						if (m_isServer)
							m_serverCamera.setMouseIn(false);
						break;
				}
				break;
		}
	}

}



void FaceOff::clientHandleDeviceEvents()
{
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
				m_mouseState.m_leftButtonDown = false;
				SDL_GetMouseState(&tmpx, &tmpy);

				if (m_players[m_defaultPlayerID]->inGrenadeGatherMode())
				{
					utl::debug("here in Grenade gather mode");
					Weapon* grenade = m_players[m_defaultPlayerID]->throwGrenade();
				}

				hitNode = NULL;

				break;

			case SDL_BUTTON_RIGHT:
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
				SDL_GetMouseState(&tmpx, &tmpy);
				m_mouseState.m_leftButtonDown = true;

				if (m_players[m_defaultPlayerID]->m_camera->getMouseIn())
				{

					m_players[m_defaultPlayerID]->fireWeapon();

					if (m_players[m_defaultPlayerID]->isUsingLongRangedWeapon())
					{

						WorldObject* hitObject = NULL;

						glm::vec3 lineStart = m_players[m_defaultPlayerID]->getFirePosition();
						glm::vec3 lineDir = -m_players[m_defaultPlayerID]->m_camera->m_targetZAxis;


						utl::debug("lineStart", lineStart);
						utl::debug("lineDir", lineDir);

						// m_objectKDtree.visitNodes(m_objectKDtree.m_head, lineStart, lineDir, 500.0f, hitObject, 0, hitNode);

						float hitObjectSqDist = FLT_MAX;
						glm::vec3 hitPoint;

						unordered_set<int> objectsAlreadyTested;

						m_objectKDtree.visitNodes(m_objectKDtree.m_head, m_players[m_defaultPlayerID], lineStart, lineDir, 500.0f, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);

						//	utl::debug("player pos", lineStart);
						//	utl::debug("target z", lineDir);

						if (hitObject != NULL)
						{
							utl::debug("name", hitObject->m_name);
							hitObject->isHit = true;

							WorldObject* hitPointMark = new WorldObject();
							hitPointMark->setPosition(hitPoint);
							hitPointMark->setScale(1.0, 1.0, 1.0);
							hitPointMark->setModelEnum(ModelEnum::cube);
							hitPointMark->setModel(m_modelMgr.get(ModelEnum::cube));
							hitPointMark->m_name = "hitMark";
							//								m_hitPointMarks.push_back(hitPointMark);
						}
						else
							utl::debug("hitObject is NULL");
						// VisitNodes

					}

				}

				m_players[m_defaultPlayerID]->m_camera->setMouseIn(true);

				



				break;


			case SDL_BUTTON_RIGHT:
				cout << "clicking right" << endl;
				SDL_GetMouseState(&tmpx, &tmpy);
				m_mouseState.m_rightButtonDown = true;

				m_zoomedIn = !m_zoomedIn;
				m_gui.setSniperZoomMode(m_zoomedIn);

				if (m_zoomedIn)
				{
					m_zoomFactor = 0.2f;
					m_pipeline.perspective(45 * m_zoomFactor, utl::SCREEN_WIDTH / utl::SCREEN_HEIGHT, utl::Z_NEAR, utl::Z_FAR);
				}
				else
				{
					m_zoomFactor = 1.0f;
					m_pipeline.perspective(45 * m_zoomFactor, utl::SCREEN_WIDTH / utl::SCREEN_HEIGHT, utl::Z_NEAR, utl::Z_FAR);
				}
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

			case SDLK_0:
				containedFlag = !containedFlag;
				break;

				// main gun
			case SDLK_1:
				m_players[m_defaultPlayerID]->switchWeapon(WeaponSlotEnum::MAIN);
				break;

				// pistol
			case SDLK_2:
				m_players[m_defaultPlayerID]->switchWeapon(WeaponSlotEnum::PISTOL);
				break;

				// MELEE
			case SDLK_3:
				m_players[m_defaultPlayerID]->switchWeapon(WeaponSlotEnum::MELEE);
				break;

				// GRENADES
			case SDLK_4:
				m_players[m_defaultPlayerID]->switchWeapon(WeaponSlotEnum::PROJECTILE);
				break;


			case SDLK_8:
			{


			}

			break;



			case SDLK_9:
				//	if (m_players[m_defaultPlayerID]->has
				break;


			case SDLK_r:
			{
				utl::debug("Reloading Weapon");
				m_players[m_defaultPlayerID]->reloadWeapon();
			}
			break;

			case SDLK_g:
			{
				utl::debug("Dropping Weapon");
				Weapon* droppedWeapon = m_players[m_defaultPlayerID]->drop();
				if (droppedWeapon != NULL)
				{
					m_objectKDtree.insert(droppedWeapon);
				}
			}
			break;




			case SDLK_SPACE:
				//		if (m_players[m_defaultPlayerID]->m_velocity.y == 0.0)
				//		utl::debug(">>>> Just Jumped");
				//		utl::debug("m_players[m_defaultPlayerID]->m_velocity", m_players[m_defaultPlayerID]->m_velocity);

				//		if (m_players[m_defaultPlayerID]->isNotJumping())
				//			m_players[m_defaultPlayerID]->m_velocity += glm::vec3(0.0, 150.0, 0.0) * utl::GRAVITY_CONSTANT;

				break;

			case SDLK_z:
				if (m_isServer)
					m_serverCamera.setMouseIn(false);
				else
					m_players[m_defaultPlayerID]->m_camera->setMouseIn(false);
				break;
			}
			break;
		}
	}

}


void FaceOff::update()
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	m_mouseState.m_pos = glm::vec2(mx, utl::SCREEN_HEIGHT - my);

	/*
	for (int i = 0; i < m_players.size(); i++)
	{
		if (i != m_defaultPlayerID && m_players[i] != NULL)
		m_players[i]->updateModel();

	}
	*/


#if 0
#if NETWORK_FLAG == 1

#if	SERVER_NETWORK_THREAD != 1
	if (m_isServer)
	{

		// iterate over each message received
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			// we first initalized bitStream with the packet->data
			RakNet::BitStream bsIn(packet->data, packet->length, false);

			// we ignore the first part of each message (due to RakNet convention)
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			cout << endl << "New Packet from:" << packet->guid.g << endl;


			// Handle message here 
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;

			case PLAYER_UPDATE:
				// received new position from client       
			{
								  int player_id = 0;
								  glm::vec3 pos, wPos;
								  float camPitch, camYaw;

								  bsIn.Read(player_id);
								  bsIn.ReadVector(pos.x, pos.y, pos.z);
								  bsIn.ReadVector(wPos.x, wPos.y, wPos.z);
								  bsIn.Read(camPitch);
								  bsIn.Read(camYaw);


								  printf("Player %d sent new position ", player_id);			utl::debug("", pos);
								  printf("Player %d sent new weapon position ", player_id);	utl::debug("", wPos);
								  printf("Player %d sent new pitch ", player_id);				utl::debug("", camPitch);
								  printf("Player %d sent new yaw ", player_id);				utl::debug("", camYaw);


								  m_players[player_id]->setPosition(pos);
								  m_players[player_id]->update(wPos, camPitch, camYaw);




								  cout << "sending new position value to each client" << endl;


								  bsOut.Reset();
								  bsOut.Write((RakNet::MessageID)PLAYER_UPDATE);
								  bsOut.Write(player_id);
								  bsOut.WriteVector(pos.x, pos.y, pos.z);
								  bsOut.WriteVector(wPos.x, wPos.y, wPos.z);
								  bsOut.Write(camPitch);
								  bsOut.Write(camYaw);


								  for (int i = 0; i < m_players.size(); i++)
								  {
									  if (player_id != i)
									  {
										  cout << "	To: " << " - " << m_players[i]->m_guid.g << endl;
										  peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(m_players[i]->m_guid), false);
									  }
									  else
										  cout << "	Not Sending to own client: " << player_id << endl;
								  }
								  break;
			}


			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
			bsOut.Reset();
		}
	}
#endif

#if CLIENT_NETWORK_THREAD != 1
	if (!m_isServer)
	{

		//		utl::debug("m_nextGameTick", m_nextGameTick);
		//		utl::debug("curTick", curTick);

		//	m_nextGameTick += DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES;

		//	if (m_nextGameTick > SDL_GetTicks())
		if (m_nextGameTick > DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES)
		{
			cout << "sending each server my location" << endl;
			bsOut.Reset();
			bsOut.Write((RakNet::MessageID)PLAYER_UPDATE);
			bsOut.Write(m_defaultPlayerID);


			utl::setBitStream(bsOut, m_players[m_defaultPlayerID]->m_position);
			utl::setBitStream(bsOut, m_players[m_defaultPlayerID]->getCurWeapon()->m_position);
			bsOut.Write(m_players[m_defaultPlayerID]->getCameraPitch());
			bsOut.Write(m_players[m_defaultPlayerID]->getCameraYaw());


			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, server_address, false);

			m_nextGameTick = 0;
		}
		m_nextGameTick += DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES / 10;


		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			cout << "here" << endl;

			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			switch (packet->data[0])
			{
			case NEW_CLIENT:
				cout << "NEW_CLIENT message " << endl;
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				break;

			case PLAYER_UPDATE:
			{
								  // report the server's new counter value

								  cout << "PLAYER_UPDATE, updating other_player_id's position" << endl;

								  int other_player_id = 0;
								  int player_id = 0;
								  glm::vec3 pos, wPos;
								  float camPitch, camYaw;

								  bsIn.Read(other_player_id);
								  bsIn.ReadVector(pos.x, pos.y, pos.z);
								  bsIn.ReadVector(wPos.x, wPos.y, wPos.z);
								  bsIn.Read(camPitch);
								  bsIn.Read(camYaw);

								  utl::debug("other_player_id", other_player_id);

								  m_players[other_player_id]->setPosition(pos);
								  m_players[other_player_id]->update(wPos, camPitch, camYaw);
								  break;
			}


			case YOUR_TURN:
				printf("My Turn. Sending message.\n");
				bsOut.Write((RakNet::MessageID)PLAYER_UPDATE);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break
			}
		}

	}
#endif
#endif
#endif
}


/*

Frame
	get input, update velocity
	gravity on velocity

	pos = pos + velocity

	resolveVelocity
	resolvePenetration

	update camera
*/


void FaceOff::serverSimulation()
{
	collisionDetectionTestPairs.clear();

	// process client inputs

	int size = m_inputQueue.size();
	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			Move move = m_inputQueue.pop();
			//	move.print();

			int playerId = move.playerId;
			Player* p = m_players[playerId];


			simulatePlayerPhysics(p, playerId, move);


			if (m_isServer)
			{
				if (oldPos != p->getPosition())
				{
					utl::debug("pos", p->getPosition());
					oldPos = p->getPosition();
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < m_players.size() ; i++)
		{
			Player* p = m_players[i];
			simulatePlayerPhysics(p, i);

			if (m_isServer)
			{
				if (oldPos != p->getPosition())
				{
					utl::debug("pos", p->getPosition());
					oldPos = p->getPosition();
				}
			}
		}

	}


	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];
		simulateObjectPhysics(object, i);
	}
}

void FaceOff::clientSimulation()
{
	// it's possible that unodered_set.clear() retains memory, so it may be slightly faster
	// then deallocating memory
	collisionDetectionTestPairs.clear();
	// CollisionDetectionTestPairs collisionDetectionTestPairs;

	for (int i = 0; i < m_players.size(); i++)
	{
		Player* p = m_players[i];
	

		simulatePlayerPhysics(p, i);

		/*
		if (oldPos != p->getPosition())
		{
			utl::debug("pos", p->getPosition());
			utl::debug("vel", p->getVelocity());
			oldPos = p->getPosition();
			utl::debugLn(1);
		}
		*/
	}

	
	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];
		simulateObjectPhysics(object, i);
	}
}

void FaceOff::simulatePlayerPhysics(Player* p, int i, Move move)
{
	if (p == NULL)
	{
		return;
	}

	p->updateGameInfo();

	p->m_velocity += utl::BIASED_HALF_GRAVITY;

	p->processInput(move);

	p->m_position += p->m_velocity;

	p->updateCollisionDetectionGeometry();

	checkNeighborsAfterClientInput(p);

	p->updateCollisionDetectionGeometry();
	m_objectKDtree.reInsert(p);


	p->updateWeaponTransform();

}


void FaceOff::simulatePlayerPhysics(Player* p, int i)
{
	if (p == NULL)
	{
		return;
	}

	p->updateGameInfo();


	p->m_velocity += utl::BIASED_HALF_GRAVITY;

	if (p->isDefaultPlayer())
	{
		p->control();

#if NETWORK_FLAG == 1
		if (m_players[m_defaultPlayerID]->hasMoved())
		{
			m_inputQueue.push(m_players[m_defaultPlayerID]->getMoveState());
		}
#endif		
		p->updateGameStats();
	}

	p->m_position += p->m_velocity;
	p->updateMidAirVelocity();
#if NETWORK_FLAG == 0 
	if (i == 1)
	{
		/*
		float tempDist = 100;

		if (incrFlag)
			p->m_position.z += 0.05;
		else
			p->m_position.z -= 0.05;

		if (p->m_position.z > tempDist)
			incrFlag = false;
		if (p->m_position.z < 20)
			incrFlag = true;
		*/


		/*
		float tempMaxAngle = 150;

		if (incrAngleFlag)
		{
			tempPitch = 0;
			tempYaw += 1;
		}
		else
		{
			tempPitch = 0;
			tempYaw -= 1;
		}

		if (tempPitch > tempMaxAngle)
		{
			incrAngleFlag = false;
		}
		if (tempPitch < -tempMaxAngle)
		{
			incrAngleFlag = true;
		}
		*/
		p->setRotation(tempPitch, tempYaw);
	}

	else if (i == 2)
	{
		/*
		float tempDist = 15;

		if (incrFlag2)
			p->m_position.x += 0.05;
		else
			p->m_position.x -= 0.05;


		if (p->m_position.x > tempDist)
			incrFlag2 = false;
		if (p->m_position.x < -tempDist)
			incrFlag2 = true;
			*/

		
		/*
		float tempMaxAngle = 150;

		if (incrAngleFlag2)
		{
			tempPitch2 += 0.05;
			tempYaw2 += 1;
		}
		else
		{
			tempPitch2 += 0.05;
			tempYaw2 -= 1;
		}

		if (tempPitch2 > tempMaxAngle)
		{
			incrAngleFlag2 = false;
		}
		if (tempPitch2 < -tempMaxAngle)
		{
			incrAngleFlag2 = true;
		}
		*/
	//	tempPitch2 = 0;
	//	tempYaw2 = 0;

		p->setRotation(tempPitch2, tempYaw2);		
	}

#endif



	p->updateCollisionDetectionGeometry();

	checkNeighbors(p);

	p->updateCollisionDetectionGeometry();
	m_objectKDtree.reInsert(p);

	p->updateWeaponTransform();

}


void FaceOff::simulateObjectPhysics(WorldObject* object, int i)
{
	if (object == NULL)
	{
		return;
	}

	object->updateGameInfo();

	if (object->ignorePhysics())
	{
		return;
	}

	if (object->getObjectType() == WEAPON)
	{
		Weapon* wObject = (Weapon*)object;

		if (wObject->getWeaponSlot() == PROJECTILE && wObject->shouldExplode())
		{
			utl::debug("Exploding");

			ParticleEffect* effect = wObject->explode();
			m_smokeEffects.push_back((SmokeEffect*)effect);

			destroyWorldObjectByIndex(i);
			return;
		}
	}



	object->m_velocity += glm::vec3(0.0f, -9.81f, 0.0f) * 0.005f * 0.5f;
	object->m_position += object->m_velocity;
	// object->m_velocity += utl::BIASED_HALF_GRAVITY;
	// object->m_position += object->m_velocity;
	object->updateCollisionDetectionGeometry();

	checkNeighbors(object);

	object->updateCollisionDetectionGeometry();
	m_objectKDtree.reInsert(object);


}


void FaceOff::checkNeighbors(WorldObject* obj)
{
	obj->inMidAir = true;

	vector<WorldObject*> neighbors;
	glm::vec3 volNearPoint(obj->getPosition());
	m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, obj, volNearPoint, neighbors);


	for (int j = 0; j < neighbors.size(); j++)
	{
		WorldObject* neighbor = neighbors[j];
		/*
		// this is for debugging, will render it with "collided color"
		if (p->isDefaultPlayer())
		{
			neighbor->isTested = true;
		}
		*/


		if (collisionDetectionTestPairs.alreadyTested(obj->m_instanceId, neighbor->m_instanceId))
			continue;
		else
			collisionDetectionTestPairs.addPairs(obj->m_instanceId, neighbor->m_instanceId);


		if (obj->ignorePhysicsWith(neighbor))
		{
			continue;
		}


		ContactData contactData;
		if (testCollisionDetection(obj, neighbor, contactData))
		{
			if (neighbor->getDynamicType() == STATIC)
			{
				contactData.pair[0] = obj;
				contactData.pair[1] = NULL;
			}
			else
			{
				contactData.pair[0] = obj;
				contactData.pair[1] = neighbor;
			}

			/*
			// this is for debugging, will render it with "collided color"
			if (obj->getObjectType() == PLAYER && ((Player*)(obj))->isDefaultPlayer() && neighbor->getName() == "woodenBox -80")
			{
				utl::debug("With WoodenBox");
				utl::debug("obj ", obj->getName());
				utl::debug("neighbor ", neighbor->getName());
				utl::debug("normal ", contactData.normal);
			}
			
			if (obj->getObjectType() == PLAYER && ((Player*)(obj))->isDefaultPlayer() && neighbor->getName() == "ground")
			{
				utl::debug("With ground");
				utl::debug("obj ", obj->getName());
				utl::debug("neighbor ", neighbor->getName());
				utl::debug("normal ", contactData.normal);
			}
			*/

			contactData.resolveVelocity();
			contactData.resolveInterpenetration();
			neighbor->updateCollisionDetectionGeometry();
		}
	}
}

void FaceOff::checkNeighborsAfterClientInput(WorldObject* obj)
{
	clientInputCollisionDetectionTestPairs.clear();

	vector<WorldObject*> neighbors;
	glm::vec3 volNearPoint(obj->getPosition());
	m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, obj, volNearPoint, neighbors);


	for (int j = 0; j < neighbors.size(); j++)
	{
		WorldObject* neighbor = neighbors[j];
		/*
		// this is for debugging, will render it with "collided color"
		if (p->isDefaultPlayer())
		{
		neighbor->isTested = true;
		}
		*/


		if (collisionDetectionTestPairs.alreadyTested(obj->m_instanceId, neighbor->m_instanceId) == false)
		{
			collisionDetectionTestPairs.addPairs(obj->m_instanceId, neighbor->m_instanceId);
		}

		if (clientInputCollisionDetectionTestPairs.alreadyTested(obj->m_instanceId, neighbor->m_instanceId))
			continue;
		else
			clientInputCollisionDetectionTestPairs.addPairs(obj->m_instanceId, neighbor->m_instanceId);


		if (obj->ignorePhysicsWith(neighbor))
		{
			continue;
		}


		ContactData contactData;
		if (testCollisionDetection(obj, neighbor, contactData))
		{
			if (neighbor->getDynamicType() == STATIC)
			{
				contactData.pair[0] = obj;
				contactData.pair[1] = NULL;
			}
			else
			{
				contactData.pair[0] = obj;
				contactData.pair[1] = neighbor;
			}

			contactData.resolveVelocity();
			contactData.resolveInterpenetration();
			neighbor->updateCollisionDetectionGeometry();
		}
	}
}
/*
void FaceOff::simulatePhysics()
{
	// it's possible that unodered_set.clear() retains memory, so it may be slightly faster
	// then deallocating memory
	collisionDetectionTestPairs.clear();
	// CollisionDetectionTestPairs collisionDetectionTestPairs;

	for (int i = 0; i < m_players.size(); i++)
	{
		Player* p = m_players[i];
		simulatePlayerPhysics(p, i, m_isServer);
	}


	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];
		simulateObjectPhysics(object, i);
	}
}
*/


void FaceOff::render()
{
	m_pipeline.setMatrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();


	if (m_isServer)
	{
		m_serverCamera.control();
		m_serverCamera.updateViewMatrix(m_pipeline);

		o_skybox.setPosition(m_serverCamera.getEyePoint());

		serverSimulation();
	}
	else
	{
		clientSimulation();

		m_players[m_defaultPlayerID]->updateCamera(m_pipeline);

		o_skybox.setPosition(m_players[m_defaultPlayerID]->m_camera->getEyePoint());
	}







	// *******************************************************
	// ************* Rendering *******************************
	// *******************************************************
	m_pipeline.setMatrixMode(MODEL_MATRIX);

	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererMgr.m_backGroundLayerFBO.FBO);
	//	Model::enableVertexAttribArrays();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// the render function disables depth test Cull face already and enables it afterwards
	o_skybox.render(m_pipeline, &m_rendererMgr.r_skybox);
	glClear(GL_DEPTH_BUFFER_BIT);

#if RENDER_DEBUG_FLAG
	p_renderer = &m_rendererMgr.r_fullTexture;
	p_renderer->enableShader();
	p_renderer->setData((int)R_FULL_TEXTURE::u_texture, 0, GL_TEXTURE_2D, tempTexture);

	{
		// render the players
		if (!m_isServer)
		{
			m_players[m_defaultPlayerID]->renderGroup(m_pipeline, p_renderer);
		}


		for (int i = 0; i < m_objects.size(); i++)
		{
			WorldObject* object = m_objects[i];

			if (object == NULL)
				continue;




			if (object->isTested != true && object->isCollided != true && object->isHit != true)
			{
				if (m_isServer && object->getName() == "player mainWeapon")
				{
					utl::debug("object name is", object->getName());
				}
				object->renderGroup(m_pipeline, p_renderer);
			}
		}




		for (int i = 0; i < m_players.size(); i++)
		{
			if (i == m_defaultPlayerID)
				continue;

			Player* player = m_players[i];
			if (player->isTested != true && player->isCollided != true && player->isHit != true)
			{

				m_players[i]->renderGroup(m_pipeline, p_renderer);
			}
		}


	}
	p_renderer->disableShader();


	// Rendering wireframes
	p_renderer = &m_rendererMgr.r_fullVertexColor;
	p_renderer->enableShader();

	o_worldAxis.renderGroup(m_pipeline, p_renderer);


	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];

		if (object == NULL)
			continue;


		object->renderWireFrameGroup(m_pipeline, p_renderer);
	}


	for (int i = 0; i < m_players.size(); i++)
	{
		if (i == m_defaultPlayerID)
			continue;

		Player* player = m_players[i];

		player->renderWireFrameGroup(m_pipeline, p_renderer);

	}

	if (containedFlag)
	{
		m_objectKDtree.renderCubeFrame(m_pipeline, p_renderer);
	}
	else
	{
		//	if (hitNode != NULL)
		//		m_objectKDtree.renderNode(m_pipeline, p_renderer, hitNode);
	}
	p_renderer->disableShader();



	p_renderer = &m_rendererMgr.r_fullColor;
	p_renderer->enableShader();
	p_renderer->setData(R_FULL_COLOR::u_color, GREEN);


		// m_objectKDtree.renderGroup(m_pipeline, p_renderer);
		if (!containedFlag)
			m_objectKDtree.renderWireFrame(m_pipeline, p_renderer);


		for (int i = 0; i < m_objects.size(); i++)
		{
			WorldObject* object = m_objects[i];

			if (object == NULL)
				continue;


			object->alreadyFireTested = false;

			if (object->isHit)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, GREEN);
				object->renderGroup(m_pipeline, p_renderer);

			}
			else if (object->isCollided)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, PURPLE);
				object->renderGroup(m_pipeline, p_renderer);
				object->isCollided = false;
			}
			else if (object->isTested)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, BLUE);
				object->renderGroup(m_pipeline, p_renderer);
				object->isTested = false;
			}





		}


		// rendering players
		for (int i = 0; i < m_players.size(); i++)
		{

			Player* p = m_players[i];

			p->alreadyFireTested = false;
			if (i == m_defaultPlayerID)
			{
				continue;
			}


			if (p->isHit)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, GREEN);
				p->renderGroup(m_pipeline, p_renderer);

			}
			else if (p->isCollided)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, PURPLE);
				p->renderGroup(m_pipeline, p_renderer);
				p->isCollided = false;
			}
			else if (p->isTested)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, BLUE);
				p->renderGroup(m_pipeline, p_renderer);
				p->isTested = false;
			}
		}

		/*
		// rendering hitPointMarks
		for (int i = 0; i < m_hitPointMarks.size(); i++)
		{
			p_renderer->setData(R_FULL_COLOR::u_color, RED);
			m_hitPointMarks[i]->renderGroup(m_pipeline, p_renderer);
		}
		*/

	p_renderer->disableShader();

#else
	p_renderer = &m_rendererMgr.r_fullTexture;
	p_renderer->enableShader();
	p_renderer->setData((int)R_FULL_TEXTURE::u_texture, 0, GL_TEXTURE_2D, tempTexture);

		for (int i = 0; i < m_players.size(); i++)
		{
			Player* p = m_players[i];
			/*
			if (!m_isServer)
			{
				if (oldPos != p->getPosition())
				{
					utl::debug("pos", p->getPosition());

					oldPos = p->getPosition();
				}
			}
			*/
			if (p->isHit == false)
			{
				p->renderGroup(m_pipeline, p_renderer);
			}
		}


		for (int i = 0; i < m_objects.size(); i++)
		{
			WorldObject* object = m_objects[i];

			if (object == NULL)
				continue;

			if (object->isHit == false)
			{
				object->renderGroup(m_pipeline, p_renderer);
			}
		}

	p_renderer->disableShader();


	// Rendering wireframes
	p_renderer = &m_rendererMgr.r_fullVertexColor;
	p_renderer->enableShader();

		o_worldAxis.renderGroup(m_pipeline, p_renderer);
		
		if (containedFlag)
		{
			m_objectKDtree.renderCubeFrame(m_pipeline, p_renderer);
		}
		else
		{
			//	if (hitNode != NULL)
			//		m_objectKDtree.renderNode(m_pipeline, p_renderer, hitNode);
		}
	p_renderer->disableShader();





	p_renderer = &m_rendererMgr.r_fullColor;
	p_renderer->enableShader();
	p_renderer->setData(R_FULL_COLOR::u_color, GREEN);

		if (!containedFlag)
			m_objectKDtree.renderWireFrame(m_pipeline, p_renderer);

		// rendering players
		for (int i = 0; i < m_players.size(); i++)
		{
			Player* p = m_players[i];

			p->alreadyFireTested = false;

			if (p->isHit)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, GREEN);
				p->renderGroup(m_pipeline, p_renderer);
			}
		}


		for (int i = 0; i < m_objects.size(); i++)
		{
			WorldObject* object = m_objects[i];

			if (object == NULL)
				continue;

			object->alreadyFireTested = false;

			if (object->getObjectType() == WEAPON)
			{
				continue;
			}

			if (object->isHit)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, GREEN);
				object->renderGroup(m_pipeline, p_renderer);
			}
		}



		/*
		// rendering hitPointMarks
		for (int i = 0; i < m_hitPointMarks.size(); i++)
		{
			p_renderer->setData(R_FULL_COLOR::u_color, RED);
			m_hitPointMarks[i]->renderGroup(m_pipeline, p_renderer);
		}
		*/

	p_renderer->disableShader();

#endif



	long long timeNowMillis = getCurrentTimeMillis();

	int deltaTimeMillis = (unsigned int)(timeNowMillis - m_currentTimeMillis);
	m_currentTimeMillis = timeNowMillis;
	/*
	if (m_fireWorkEffects.size() > 0)
	{
	p_renderer = &m_rendererMgr.r_fireWorkEffectUpdate;
	p_renderer->enableShader();
	for (int i = 0; i < m_fireWorkEffects.size(); i++)
	{
	FireWorkEffect* effect = m_fireWorkEffects[i];
	effect->m_time += deltaTimeMillis;

	p_renderer->setData("u_randomTexture", 3, GL_TEXTURE_1D, effect->m_randomTextureId);
	p_renderer->setData("u_time", (float)effect->m_time);
	p_renderer->setData("u_deltaTimeMillis", (float)deltaTimeMillis);

	effect->update(m_pipeline, p_renderer);
	}
	p_renderer->disableShader();


	p_renderer = &m_rendererMgr.r_fireWorkEffectRender;
	p_renderer->enableShader();
	for (int i = 0; i < m_fireWorkEffects.size(); i++)
	{
	FireWorkEffect* effect = m_fireWorkEffects[i];

	p_renderer->setData("u_centerPosition", effect->getPosition());
	p_renderer->setData("u_texture", 0, GL_TEXTURE_2D, effect->m_textureId);
	effect->render(m_pipeline, p_renderer);
	}
	p_renderer->disableShader();
	}
	*/



	if (!m_zoomedIn)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RENDER_TO_SCREEN);
	}

	m_gui.initGUIRenderingSetup();
	if (!m_zoomedIn)
	{
		m_gui.renderTextureFullScreen(m_rendererMgr.m_backGroundLayerFBO.colorTexture);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	

	// smoke effects

	
	if (m_smokeEffects.size() > 0)
	{
		p_renderer = &m_rendererMgr.r_smokeEffectUpdate;
		p_renderer->enableShader();
		for (int i = 0; i < m_smokeEffects.size(); i++)
		{
			SmokeEffect* effect = m_smokeEffects[i];
			effect->m_time += deltaTimeMillis;

			p_renderer->setData(R_SMOKE_EFFECT_UPDATE::u_randomTexture, 3, GL_TEXTURE_1D, effect->m_randomTextureId);
			p_renderer->setData(R_SMOKE_EFFECT_UPDATE::u_time, (float)effect->m_time);
			p_renderer->setData(R_SMOKE_EFFECT_UPDATE::u_deltaTimeMillis, (float)deltaTimeMillis);

			effect->update(m_pipeline, p_renderer);
		}
		p_renderer->disableShader();


		p_renderer = &m_rendererMgr.r_smokeEffectRender;
		p_renderer->enableShader();
		for (int i = 0; i < m_smokeEffects.size(); i++)
		{
			SmokeEffect* effect = m_smokeEffects[i];

			p_renderer->setData(R_SMOKE_EFFECT_RENDER::u_angle, effect->m_particleRotation);
			p_renderer->setData(R_SMOKE_EFFECT_RENDER::u_texture, 0, GL_TEXTURE_2D, effect->m_textureId);
			p_renderer->setData(R_SMOKE_EFFECT_RENDER::u_depthTexture, 1, GL_TEXTURE_2D, m_rendererMgr.m_backGroundLayerFBO.depthTexture);
			effect->render(m_pipeline, p_renderer);
		}
		p_renderer->disableShader();
	}
	
	glDisable(GL_BLEND);

	if (m_zoomedIn)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RENDER_TO_SCREEN);
		m_gui.renderSnipeScopeView(m_rendererMgr.m_backGroundLayerFBO.colorTexture);
	}

//	Model::enableVertexAttribArrays();
	m_gui.updateAndRender(m_mouseState);
//	renderGUI();
//	Model::disableVertexAttribArrays();
}


long long FaceOff::getCurrentTimeMillis()
{
#ifdef WIN32
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif
}



bool FaceOff::testCollisionDetection(WorldObject* a, WorldObject* b, ContactData& contactData)
{
	if (a->getGeometryType() == CD_AABB)
	{
		if (b->getGeometryType() == CD_AABB)
		{
			return CollisionDetection::testAABBAABB(*(a->m_aabb), *(b->m_aabb), contactData);
		}
		else if (b->getGeometryType() == CD_SPHERE)
		{
			return CollisionDetection::testSphereAABB(*(b->m_sphere), *(a->m_aabb), contactData);
		}
	}

	else if (a->getGeometryType() == CD_SPHERE)
	{
		if (b->getGeometryType() == CD_AABB)
		{
			return CollisionDetection::testSphereAABB(*(a->m_sphere), *(b->m_aabb), contactData);
		}
		else if (b->getGeometryType() == CD_SPHERE)
		{
			return CollisionDetection::testSphereSphere(*(a->m_sphere), *(b->m_sphere), contactData);
		}
	}

	return false;
}


void FaceOff::destroyWorldObjectByIndex(int i)
{
	WorldObject* object = m_objects[i];

	//remove object from its stored kdTree nodes
	for (int j = 0; j < object->m_parentNodes.size(); j++)
	{
		KDTreeNode* kNode = object->m_parentNodes[j];
		if (kNode == NULL)
			continue;
		kNode->remove(object);
	}

	// remove object from main's object containers
	m_emptyBucketPool.push(i);

	delete m_objects[i];
	m_objects[i] = NULL;
}







int main(int argc, char *argv[])
{
	utl::debug("Game Starting"); 
	utl::initSDL(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT, pDisplaySurface);
	utl::initGLEW();
	FaceOff Martin;


#if NETWORK_FLAG == 1
	Martin.initNetworkServerClient();
#endif

	Martin.init();
	Martin.start();


	utl::exitSDL(pDisplaySurface);
	//normal termination
	cout << "Terminating normally." << endl;
	return EXIT_SUCCESS;
}


int FaceOff::endWithError(char* msg, int error)
{
	//Display error message in console
	cout << msg << "\n";
	system("PAUSE");
	return error;
}


// http://kcat.strangesoft.net/mpstream.c
void FaceOff::initAudio()
{
	FILE* fp = NULL;
	fp = fopen("Assets/audio/sound1.wav", "rb");


	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	DWORD dataSize;

	//Check that the WAVE file is OK
	fread(type, sizeof(char), 4, fp);                                              //Reads the first bytes in the file
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')            //Should be "RIFF"
	{
		endWithError("No RIFF");                                            //Not RIFF
		exit(1);
	}

	fread(&size, sizeof(DWORD), 1, fp);                                           //Continue to read the file
	fread(type, sizeof(char), 4, fp);                                             //Continue to read the file
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')           //This part should be "WAVE"
	{
		endWithError("not WAVE");                                            //Not WAVE
		exit(1);
	}
		
	fread(type, sizeof(char), 4, fp);                                              //Continue to read the file
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')           //This part should be "fmt "
	{
		endWithError("not fmt ");                                            //Not fmt                                     //Not RIFF
		exit(1);
	}

	//Now we know that the file is a acceptable WAVE file
	//Info about the WAVE data is now read and stored
	fread(&chunkSize, sizeof(DWORD), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(DWORD), 1, fp);
	fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);


	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')           //This part should be "data"
	{
		endWithError("Missing DATA");                                        //not data
		exit(1);
	}

		
	fread(&dataSize, sizeof(DWORD), 1, fp);                                        //The size of the sound data is read

	//Display the info about the WAVE file
	cout << "Chunk Size: " << chunkSize << "\n";
	cout << "Format Type: " << formatType << "\n";
	cout << "Channels: " << channels << "\n";
	cout << "Sample Rate: " << sampleRate << "\n";
	cout << "Average Bytes Per Second: " << avgBytesPerSec << "\n";
	cout << "Bytes Per Sample: " << bytesPerSample << "\n";
	cout << "Bits Per Sample: " << bitsPerSample << "\n";
	cout << "Data Size: " << dataSize << "\n";

	unsigned char* buf = new unsigned char[dataSize];                            //Allocate memory for the sound data
	cout << fread(buf, sizeof(BYTE), dataSize, fp) << " bytes loaded\n";           //Read the sound data and display the 
	//number of bytes loaded.
	//Should be the same as the Data Size if OK

	//Now OpenAL needs to be initialized 
	ALCdevice *device;                                                          //Create an OpenAL Device
	ALCcontext *context;                                                        //And an OpenAL Context
	device = alcOpenDevice(NULL);                                               //Open the device
	if (!device)
	{
		endWithError("no sound device");                         //Error during device oening
		exit(1);
	}

	context = alcCreateContext(device, NULL);                                   //Give the device a context
	alcMakeContextCurrent(context);                                             //Make the context the current
	if (!context)
	{
		endWithError("no sound context");                       //Error during context handeling
		exit(1);
	}
		
	ALuint source;                                                              //Is the name of source (where the sound come from)
	ALuint buffer;                                                           //Stores the sound data
	ALuint frequency = sampleRate;;                                               //The Sample Rate of the WAVE file
	ALenum format = 0;                                                            //The audio format (bits per sample, number of channels)

	alGenBuffers(1, &buffer);                                                    //Generate one OpenAL Buffer and link to "buffer"
	alGenSources(1, &source);                                                   //Generate one OpenAL Source and link to "source"
	if (alGetError() != AL_NO_ERROR)
	{
		endWithError("Error GenSource");     //Error during buffer/source generation
		exit(1);
	}
	

	//Figure out the format of the WAVE file
	if (bitsPerSample == 8)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO8;
		else if (channels == 2)
			format = AL_FORMAT_STEREO8;
	}
	else if (bitsPerSample == 16)
	{
		if (channels == 1)
			format = AL_FORMAT_MONO16;
		else if (channels == 2)
			format = AL_FORMAT_STEREO16;
	}

	if (!format)
	{
		endWithError("Wrong BitPerSample");                      //Not valid format
		exit(1);
	}
		
	alBufferData(buffer, format, buf, dataSize, frequency);                    //Store the sound data in the OpenAL Buffer
	if (alGetError() != AL_NO_ERROR)
	{
		endWithError("Error loading ALBuffer");                              //Error during buffer loading
		exit(1); 
	}
		
	//Sound setting variables
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };                                    //Position of the source sound
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };                                    //Velocity of the source sound
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };                                  //Position of the listener
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };                                  //Velocity of the listener
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };                 //Orientation of the listener
	//First direction vector, then vector pointing up) 
	//Listener                                                                               
	alListenerfv(AL_POSITION, ListenerPos);                                  //Set position of the listener
	alListenerfv(AL_VELOCITY, ListenerVel);                                  //Set velocity of the listener
	alListenerfv(AL_ORIENTATION, ListenerOri);                                  //Set orientation of the listener

	//Source
	alSourcei(source, AL_BUFFER, buffer);                                 //Link the buffer to the source
	alSourcef(source, AL_PITCH, 1.0f);                                 //Set the pitch of the source
	alSourcef(source, AL_GAIN, 1.0f);                                 //Set the gain of the source
	alSourcefv(source, AL_POSITION, SourcePos);                                 //Set the position of the source
	alSourcefv(source, AL_VELOCITY, SourceVel);                                 //Set the velocity of the source
	alSourcei(source, AL_LOOPING, AL_FALSE);                                 //Set if source is looping sound

	//PLAY 
	alSourcePlay(source);                                                       //Play the sound buffer linked to the source
	if (alGetError() != AL_NO_ERROR)
	{
		endWithError("Error playing sound"); //Error when playing sound
		exit(1);
	}

	// system("PAUSE");                                                            //Pause to let the sound play

	//Clean-up
	fclose(fp);                                                                 //Close the WAVE file
	delete[] buf;                                                               //Delete the sound data buffer
	alDeleteSources(1, &source);                                                //Delete the OpenAL Source
	alDeleteBuffers(1, &buffer);                                                 //Delete the OpenAL Buffer
	alcMakeContextCurrent(NULL);                                                //Make no context current
	alcDestroyContext(context);                                                 //Destroy the OpenAL Context
	alcCloseDevice(device);                                                     //Close the OpenAL Device

}



void FaceOff::initGUI()
{

	m_gui.init(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
	Control::init("", 25, utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
	if (!m_isServer)
	{
		int xOffset = 55;
		int yOffset = 570;

		int BAR_WIDTH = 60;
		int BAR_HEIGHT = 10;

		Control* HPBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GREEN, "icon_hp.png");
		m_players[m_defaultPlayerID]->m_healthBarGUI = (Bar*)HPBar;

		xOffset = 175;
		Control* armorBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GRAY, "icon_armor.png");
		m_players[m_defaultPlayerID]->m_armorBarGUI = (Bar*)armorBar;

		xOffset = 700;
		Control* ammoBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GRAY, "icon_ammo.png");
		m_players[m_defaultPlayerID]->m_ammoBarGUI = (Bar*)ammoBar;

		int aimWidth = 20;
		int aimHeight = 20;

		int aimX = utl::SCREEN_WIDTH / 2 - aimWidth / 2;
		int aimY = utl::SCREEN_HEIGHT / 2;

		utl::debug("aimX", aimX);
		utl::debug("aimY", aimY);
		Control* horiAim = new Label("", aimX, aimY - 1, aimWidth, 2, GREEN);

		aimX = utl::SCREEN_WIDTH / 2;
		aimY = utl::SCREEN_HEIGHT / 2 - aimHeight / 2;

		utl::debug("aimX", aimX);
		utl::debug("aimY", aimY);

		Control* vertAim = new Label("", aimX - 1, aimY, 2, aimHeight, GREEN);

		m_gui.addGUIComponent(HPBar);
		m_gui.addGUIComponent(armorBar);
		m_gui.addGUIComponent(ammoBar);
		m_gui.addGUIComponent(horiAim);
		m_gui.setHorAimIndex(m_gui.getNumGUIComponent() - 1);

		m_gui.addGUIComponent(vertAim);
		m_gui.setVerAimIndex(m_gui.getNumGUIComponent() - 1);
	}
}


void FaceOff::renderGUI()
{

	m_gui.initGUIRenderingSetup();
	/// http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement
	//	unsigned int getTicks = SDL_GetTicks();

	//	static string final_str = "(" + utl::floatToStr(m_mouseState.m_pos.x) + ", " + utl::floatToStr(m_mouseState.m_pos.y) + ")";
	m_gui.updateAndRender(m_mouseState);

	// healthbar and text


}

/*
void FaceOff::removeObject(WorldObject* object)
{

}
*/






