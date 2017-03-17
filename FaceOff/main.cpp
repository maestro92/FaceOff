#include "main.h"

#define NETWORK_FLAG 1

// #define SINGLE_PLAYER_MODE 1
#define EMULATE_LATENCY 1

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


// const float GAME_SPEED = 1f;
// const float FIXED_TICK_TIME_s;

// #define MAX_CLIENTS 10
// #define SERVER_PORT 60000

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


// const bool SINGLE_PLAYER_MODE



const float GAME_SPEED = 1.0f;
const float _FIXED_UPDATE_TIME_s = 0.01667f;
const float FIXED_UPDATE_TIME_s = _FIXED_UPDATE_TIME_s / GAME_SPEED;
const float FIXED_UPDATE_TIME_ms = FIXED_UPDATE_TIME_s * 1000;
//const float _FIXED_UPDATE_TIME_s = 


const float SV_FRAMES_PER_SECOND = 20;
const float SV_FIXED_UPATE_TIME_s = 1 / SV_FRAMES_PER_SECOND;

const float CLIENT_INTERP_DELAY = FIXED_UPDATE_TIME_ms * 2;

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


const float SPAWN_POSITION_UNIT_OFFSET = 20.0f;

const int INVALID_OBJECT = 0x7FFFFFFF;

RendererManager FaceOff::m_rendererMgr;
ModelManager	FaceOff::m_modelMgr;



FaceOff::FaceOff()
{
	cout << "default ctr" << endl;
}

FaceOff::FaceOff(int nice)
{
	cout << "assginment version" << endl;
	int a = 1;
}

FaceOff::~FaceOff()
{

}


void FaceOff::init()
{
	isRunning = true;
	singlePlayerMode = true;

	latencyOptions = { 0, 20, 50, 100, 200 };	// millisecond
	latency = 0; 

	packetLossOptions = { 0, 5, 10, 50 };
	packetLoss = 0;

	curLatencyOption = 0;

	predictionOn = true;
	containedFlag = false;
	hitNode = NULL;


	timeProfilerIndex = 0;

	initObjects();
	initRenderers();
   
	// initAudio();
	initNetwork();
	initNetworkLobby();


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



void FaceOff::initMap(FOArray& objects, FOPlayerArray& players, KDTree& tree)
{
	objects.init(NUM_MAX_OBJECTS);
	players.init(NUM_MAX_CLIENTS);
		
	float wallWidth = 10.0f;

	float xbound = 150;
	float ybound = 50;
	float zbound = 150;

	float xboundWithWalls = 150 + wallWidth * 2;
	float zboundWithWalls = 150 + wallWidth * 2;

	float scale = 150;
		
	WorldObject* o_temp = new WorldObject();
	o_temp->setScale(xbound, zbound, 1.0);
	o_temp->setRotation(glm::rotate(90.0f, 1.0f, 0.0f, 0.0f));
	o_temp->setModelEnum(ModelEnum::ground);
	o_temp->setModel(m_modelMgr.get(ModelEnum::ground));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	o_temp->m_name = "ground";
	objects.add(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "south wall";
	o_temp->setScale(xbound, ybound / 2, wallWidth);
	o_temp->setPosition(0, ybound / 2, zbound + wallWidth);
	//	o_temp->setModel(m_modelMgr.m_cube);
	o_temp->setModelEnum(ModelEnum::cube);
	o_temp->setModel(m_modelMgr.get(ModelEnum::cube));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "north wall";
	o_temp->setPosition(0, ybound / 2, -zbound - wallWidth);
	o_temp->setScale(xbound, ybound / 2, wallWidth);
	o_temp->setRotation(glm::rotate(180.0f, 0.0f, 1.0f, 0.0f));
	//	o_temp->setModel(m_modelMgr.m_cube);
	o_temp->setModelEnum(ModelEnum::cube);
	o_temp->setModel(m_modelMgr.get(ModelEnum::cube));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "west wall";
	o_temp->setPosition(-xbound - wallWidth, ybound / 2, 0.0);
	o_temp->setScale(wallWidth, ybound / 2, zbound);
	//	o_temp->setModel(m_modelMgr.m_cube);
	o_temp->setModelEnum(ModelEnum::cube);
	o_temp->setModel(m_modelMgr.get(ModelEnum::cube));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);




	o_temp = new WorldObject();
	o_temp->m_name = "east wall";
	o_temp->setPosition(xbound + wallWidth, ybound / 2, 0.0);
	o_temp->setScale(wallWidth, ybound / 2, zbound);
	//	o_temp->setModel(m_modelMgr.m_cube);
	o_temp->setModelEnum(ModelEnum::cube);
	o_temp->setModel(m_modelMgr.get(ModelEnum::cube));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);


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
		objects.add(o_temp);
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
		objects.add(o_temp);
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
	objects.add(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "NE Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(halfPosXMag, pillarYScale / 2, -halfPosZMag);
	//	o_temp->setModel(m_modelMgr.m_woodenBox);
	o_temp->setModelEnum(ModelEnum::woodenBox);
	o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);




	o_temp = new WorldObject();
	o_temp->m_name = "SW Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(-halfPosXMag, pillarYScale / 2, halfPosZMag);
	//	o_temp->setModel(m_modelMgr.m_woodenBox);
	o_temp->setModelEnum(ModelEnum::woodenBox);
	o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "SE Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(halfPosXMag, pillarYScale / 2, halfPosZMag);
	//	o_temp->setModel(m_modelMgr.m_woodenBox);
	o_temp->setModelEnum(ModelEnum::woodenBox);
	o_temp->setModel(m_modelMgr.get(ModelEnum::woodenBox));
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);

	// making the start of dynamic objects
	objects.preferredIterationStart = objects.maxUsed + 1;

	// utl::debug("id is", o_temp->m_instanceId);
	float formationGap = 40.0f;


	o_temp = new Weapon(m_modelMgr.getWeaponData(MAC_11));
	o_temp->m_name = "MAC_11";
	o_temp->setPosition(-3 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(AWM));
	o_temp->m_name = "AWM";
	o_temp->setPosition(-2 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(MINIGUN));
	o_temp->m_name = "MINIGUN";
	o_temp->setPosition(-1 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);



	o_temp = new Weapon(m_modelMgr.getWeaponData(KNIFE));
	o_temp->m_name = "knife";
	o_temp->setPosition(formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(MP5));
	o_temp->m_name = "MP5";
	o_temp->setPosition(-3 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(MG42));
	o_temp->m_name = "MG42";
	o_temp->setPosition(-2 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);



	// init weapons for the map
	o_temp = new Weapon(m_modelMgr.getWeaponData(AK_47));
	o_temp->m_name = "AK 47";
	o_temp->setPosition(-formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);


	o_temp = new Weapon(m_modelMgr.getWeaponData(M16));
	o_temp->m_name = "M16";
	o_temp->setPosition(formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);



	o_temp = new Weapon(m_modelMgr.getWeaponData(KATANA));
	o_temp->m_name = "katana";
	o_temp->setPosition(2 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);



	o_temp = new Weapon(m_modelMgr.getWeaponData(PISTOL_SHOTGUN));
	o_temp->m_name = "shotgun";
	o_temp->setPosition(3 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	objects.add(o_temp);





	vector<WorldObject*> objectsForKDTree;

	for (int i = 0; i < objects.getIterationEnd(); i++)
	{
		WorldObject* obj = objects.getByIndex(i);

		if (obj == NULL)
		{
			continue;
		}

		if (obj->getObjectType() == WEAPON && ((Weapon*)(obj))->hasOwner())
		{
			continue;
		}

		objectsForKDTree.push_back(obj);
	}


	//	m_objectKDtree.build(objectsForKDTree, glm::vec3(xboundWithWalls + 1, ybound + 1, zboundWithWalls + 1), glm::vec3(-xboundWithWalls - 1, -1, -zboundWithWalls - 1));
	tree.build(objectsForKDTree, glm::vec3(xboundWithWalls + 1, ybound + 1, zboundWithWalls + 1), glm::vec3(-xboundWithWalls - 1, -1, -zboundWithWalls - 1));



}




void FaceOff::initObjects()
{
	m_modelMgr.init();
	// m_nm.init(&m_modelMgr, &m_objects, &m_players);

	/*/
	if (m_isServer)
	{
		m_spectatorCamera = FirstPersonCamera(glm::vec3(49.83, 200.67, 59.87));
		m_spectatorCamera.setPitch(-63.5);
		m_spectatorCamera.setYaw(56.5);
		m_spectatorCamera.setFreeMode(true);

	//	utl::debug("m_spectatorCamera eye", m_spectatorCamera.getEyePoint());
	}
	*/

	float scale = 100.0;
	o_worldAxis.setScale(scale);
	o_worldAxis.setModelEnum(ModelEnum::xyzAxis);
	o_worldAxis.setModel(m_modelMgr.get(ModelEnum::xyzAxis));

	
	o_sampleBullet.setScale(1.0, 5.0, 1.0);

	o_skybox = SkyBox();


	initMap(sv_objects, sv_players, sv_objectKDtree);

	initMap(cl_objects, cl_players, cl_objectKDtree);


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
	
}


/*
void FaceOff::lateInitObjects()
{

	for (auto player : sv_players.objects)
	{
		if (player != NULL)
		{
			sv_objectKDtree.insert(player);
		}
	}

	for (auto player : cl_players.objects)
	{
		if (player != NULL)
		{
			cl_objectKDtree.insert(player);
		}
	}

}
*/


void FaceOff::initRenderers()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_rendererMgr.init(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
	m_rendererMgr.initSceneRendererStaticLightsData(m_lightManager);
}


void FaceOff::initNetwork()
{
	bool invalidInput = true;
	bool m_isServer = false;
	utl::debugLn(5);
	while (invalidInput)
	{
		char str[512];
		printf("Are You Hosting? Y/N \n");
		gets(str);

		if ((str[0] == 'Y') || (str[0] == 'y') || str[0] == '\0')
		{
			m_isServer = true;
			invalidInput = false;
		}
		else if (str[0] == 'N' || str[0] == 'n')
		{
			m_isServer = false;
			invalidInput = false;
		}
		else
		{
			printf("InvalidInput!");
		}
	}

	if (m_isServer)
	{
		m_server.init(SV_FRAMES_PER_SECOND);
	}

	m_client.init();


	// cout << "client address " <<  RakNet::SystemAddress::ToInteger(m_client.systemAddress) << endl;
}

void FaceOff::initNetworkLobby()
{
	// we're waiting in the lobby
	// bool waitingInLobby = true;

	// we're gonna settle the player list
	printf("Waiting in Lobby.\n");


	bool svWaitingInLobby = true;
	bool clWaitingInLobby = true;

	while ((m_server.isInitalized && svWaitingInLobby) || (m_server.isDedicated == false && clWaitingInLobby))
	{
		SDL_PumpEvents();
		Uint8* state = SDL_GetKeyState(NULL);
		if (state[SDLK_a] || state[SDLK_SPACE])
		{ 
			svWaitingInLobby = false;

			if (m_server.isInitalized)
			{
			
				// we add a bot if there is only one player
				if (m_server.getNumConnectedClients() == 1)
				{
					singlePlayerMode = true;
					int newPlayerId = sv_players.nextAvaiableId();
					float newSpawnX = 0;
					float newSpawnY = 5;
					float newSpawnZ = 25;

					// Add Player
					Player* p = new Player(newPlayerId);
					p->m_name = "player " + utl::intToStr(newPlayerId);
					p->setPosition(newSpawnX, newSpawnY, newSpawnZ);
					p->setRotation(0, 0);
					p->setModelEnum(ModelEnum::player);
					p->setModel(m_modelMgr.get(ModelEnum::player));
					p->setMass(80);
					p->setCollisionDetectionGeometry(CD_SPHERE);


					Weapon* mainWeapon = new Weapon(m_modelMgr.getWeaponData(M16));
					mainWeapon->setCollisionDetectionGeometry(CD_AABB);
					mainWeapon->m_name = "player1 mainWeapon";

					Weapon* knife = new Weapon(m_modelMgr.getWeaponData(KNIFE));
					knife->setCollisionDetectionGeometry(CD_AABB);
					knife->m_name = "player1 knife";

					Weapon* grenade = new Weapon(m_modelMgr.getWeaponData(FRAG_GRENADE));
					grenade->setMass(0.4);
					grenade->setMaterialEnergyRestitution(0.6);
					grenade->setMaterialSurfaceFriction(0.3);
					grenade->setCollisionDetectionGeometry(CD_AABB);
					grenade->m_name = "player1 grenade";


					sv_players.add(p);
					sv_objects.add(mainWeapon);
					sv_objects.add(knife);
					sv_objects.add(grenade);


					p->pickUp(mainWeapon);
					p->pickUp(knife);
					p->pickUp(grenade);


					sv_objectKDtree.insert(p);

					RakNet::BitStream bs;
					bs.Write((RakNet::MessageID)NEW_CLIENT);
					p->serialize(bs);

					//	HIGH_PRIORITY, RELIABLE_ORDERED,
					m_server.peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, m_server.getClientAddress(0), false);
				}
				else
				{
					singlePlayerMode = false;
				}


				// send the end termination signal to client
//				for (int i = 0; i < m_server.clients.size(); i++)
				for (int i = 0; i < m_server.getNumClients(); i++)
				{
					RakNet::SystemAddress clientAddress = m_server.getClientAddress(i);

					std::cout << "sending each client lobby wait end signal" << i << endl;
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)LOBBY_WAIT_END);
					bsOut.Write(i);

					m_server.peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, clientAddress, false);
				}
			}
		}

		// running server wait for lobby frame
		if (m_server.isInitalized == true)
		{
			for (sv_packet = m_server.peer->Receive();
				sv_packet;
				m_server.peer->DeallocatePacket(sv_packet), sv_packet = m_server.peer->Receive())
			{

				printf("server reading msg.\n");

				// we first initalized bitStream with the packet->data
				RakNet::BitStream bsIn(sv_packet->data, sv_packet->length, false);

				cout << "sv client address " << RakNet::SystemAddress::ToInteger(sv_packet->systemAddress) << endl;

				// we ignore the first part of each message (due to RakNet convention)
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

				// Handle message here 
				switch (sv_packet->data[0])
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
						m_server.addClient(sv_packet);

						int newPlayerId = sv_players.nextAvaiableId();
						float newSpawnX = newPlayerId * SPAWN_POSITION_UNIT_OFFSET;
						float newSpawnY = 5;
						float newSpawnZ = newPlayerId * SPAWN_POSITION_UNIT_OFFSET;

						// Add Player
						Player* p = new Player(newPlayerId);
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


						sv_players.add(p);
						sv_objects.add(mainWeapon);
						sv_objects.add(knife);
						sv_objects.add(grenade);


						p->pickUp(mainWeapon);
						p->pickUp(knife);
						p->pickUp(grenade);


						sv_objectKDtree.insert(p);


						p->svDebug();

						RakNet::BitStream bsOut;
						broadCastNewClientJoining(newPlayerId, bsOut);
						replyBackToNewClient(newPlayerId, bsOut);
						break;
					}


					case ID_DISCONNECTION_NOTIFICATION:
						printf("A client has disconnected.\n");
						break;
					case ID_CONNECTION_LOST:
						printf("A client lost the connection.\n");
						break;
						
					case NONE:
					{
						printf("A client NONE the connection.\n");

						if (noneCounter < 5)
						{
							RakNet::BitStream bsOut1;
							bsOut1.Write((RakNet::MessageID)NONE);
							bsOut1.Write("Hello world");

							//				m_server.peer->Send(&bsOut1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sv_packet->systemAddress, false);

							RakNet::SystemAddress clientAddress = m_server.getClientAddress(0);
							m_server.peer->Send(&bsOut1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientAddress, false);
							noneCounter++;
						}
						break;
					}
					
					default:
						printf("Message with identifier %i has arrived.\n", sv_packet->data[0]);
						break;
				}
				// bsOut.Reset();
			}
		}


		// running client wait for lobby frame
		if (m_server.isDedicated == false)
		{
			// iterate over each message received
			for (cl_packet = m_client.peer->Receive();
				cl_packet;
				m_client.peer->DeallocatePacket(cl_packet), cl_packet = m_client.peer->Receive())
			{

				printf("client reading msg.\n");

				RakNet::BitStream bsIn(cl_packet->data, cl_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

				switch (cl_packet->data[0])
				{
					case SPAWN_INFORMATION:
					{
						m_client.serverSystemAddress = cl_packet->systemAddress;

						cout << "sv address " << RakNet::SystemAddress::ToInteger(cl_packet->systemAddress) << endl;

						utl::clDebug("Server gave me Spawn Information");

						// the new player
						Player* p = new Player();
						deserializePlayerAndWeaponAndAddToWorld(p, bsIn, true);
						cl_objectKDtree.insert(p);
		
						p->clDebug();


						// existing player
						int existingPlayerCount = 0;
						bsIn.Read(existingPlayerCount);

						for (int i = 0; i < existingPlayerCount; i++)
						{
							Player* p = new Player();
							deserializePlayerAndWeaponAndAddToWorld(p, bsIn, false);
							cl_objectKDtree.insert(p);
						}

						break;
					}


					case NEW_CLIENT:
					{
						utl::clDebug("Received info about new client info for");

						Player* p = new Player();
						deserializePlayerAndWeaponAndAddToWorld(p, bsIn, false);
						cl_objectKDtree.insert(p);

						cout << "Player List" << endl;
						for (int i = 0; i < cl_players.getIterationEnd(); i++)
						{
							Player* p = cl_players.get(i);

							if (p == NULL)
							{
								cout << "player " << i << " is null" << endl;
							}
							else
							{
								cout << i << " - " << p->objectId.id << endl;  utl::debug("position ", p->m_position);
							}
						}
					
						break;
					}

					case LOBBY_WAIT_END:
						clWaitingInLobby = false;
						break;

					case ID_CONNECTION_REQUEST_ACCEPTED:
						m_client.isConnected = true;
						break;
					
					case NONE:
					{
						if (noneCounter < 5)
						{
							RakNet::RakString rs;
							bsIn.Read(rs);
							printf("						%s\n", rs.C_String());

							RakNet::BitStream bsOut1;
							bsOut1.Write((RakNet::MessageID)NONE);
							bsOut1.Write("Hello world");
							m_client.peer->Send(&bsOut1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, cl_packet->systemAddress, false);
							noneCounter++;
						}
						break;
					}				
					
				}

			}
		}
	}









	printf("Done waiting in the Lobby.\n");

}


void FaceOff::broadCastNewClientJoining(int playerId, RakNet::BitStream& bs)
{
	bs.Reset();	// Very important!!

	Player* p = sv_players.get(playerId);

	if (sv_players.getIterationEnd() > 0)
	{
		// send existing clients about the new client notification 
		std::cout << "Signaling arrival of new clients, Sending new client's spaw position to each client" << endl;

		bs.Write((RakNet::MessageID)NEW_CLIENT);
		p->serialize(bs);


		for (int i = 0; i < m_server.getNumClients(); i++)
		{
			if (i == playerId)
				continue;

			m_server.peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_server.getClientAddress(i), false);
		}
	}
}


void FaceOff::deserializePlayerAndWeaponAndAddToWorld(Player* p, RakNet::BitStream& bs, bool curPlayer)
{
	p->deserialize(bs, &m_modelMgr);

	if (curPlayer == true)
	{
		m_defaultPlayerID = p->getId();
		p->setDefaultPlayerFlag(curPlayer);
	}


	
	cl_players.set(p, p->objectId.id);
	cl_objectKDtree.insert(p);

	for (int i = 0; i < p->getWeapons().size(); i++)
	{
		Weapon* weapon = p->getWeapons()[i];
		if (weapon != NULL)
		{
			cl_objects.set(weapon, weapon->objectId);
		}
	}

	/*
	int weaponCount = 0;
	bs.Read(weaponCount);
	
	cout << ">>>> desearializing Players " << endl;
	for (int i = 0; i < weaponCount; i++)
	{
		Weapon* weapon = new Weapon();
		weapon->deserialize(bs, &m_modelMgr);

		p->pickUp(weapon);
		cl_objects.set(weapon, weapon->objectId);
	}
	*/
}


void FaceOff::deserializeEntityAndAddToWorld(WorldObject* obj, RakNet::BitStream& bs)
{
	obj->deserialize(bs, &m_modelMgr);
	/*
	if (obj->objectId.s.index == 27)
	{
		int a = 1;
	}
	*/
	cl_objects.set(obj, obj->objectId);

	if (obj->getObjectType() == WEAPON)
	{
		Weapon* weapon = (Weapon*)obj;
		if (weapon->hasOwner())
		{
			cl_players.get(weapon->ownerId)->pickUp(weapon);
		}
	}

	/*
	if (obj->objectId.s.index == 27)
	{
		cout << "object address is " << obj << endl;
		cout << "player weapons addresses are " << endl;
		for (int i = 0; i < cl_players.get(0)->getWeapons().size(); i++)
		{
			Weapon* weapon = cl_players.get(0)->getWeapons()[i];
			if (weapon != NULL)
			{
				cout << "weapon address is " << weapon << endl;
			}
		}
		int a = 1;
	}
	*/
	cl_objectKDtree.insert(obj);
}


void FaceOff::serverToClientSendPacket(RakNet::SystemAddress& address, RakNet::BitStream& bs)
{
#if EMULATE_LATENCY == 1
	if (!utl::chance(packetLoss))
	{
		DelayedPacket dp(m_server.realTime + (unsigned int)latency, address, bs);
		serverToClient.push(dp);
	}
#else
	m_server.peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, address, false);
#endif
}


// we should use real time here cuz we are using trying to simulate real latency
void FaceOff::clientToServerSendPacket(RakNet::SystemAddress& address, RakNet::BitStream& bs)
{
#if EMULATE_LATENCY == 1
	if (!utl::chance(packetLoss))
	{
		DelayedPacket dp(m_client.realTime + (unsigned int)latency, address, bs);
		clientToServer.push(dp);
	}
#else
	m_client.peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, address, false);
#endif
}


void FaceOff::processPacketQueue(unsigned int curRealTime, PacketQueue& packetQueue, bool isServerToClient)
{
	while (!packetQueue.empty())
	{
		// have to call packetQueue.front() without making a copy
		// cuz making a copy will mess up up the readOffset on the bitstream
		if (packetQueue.front().deliveryTime <= curRealTime)
		{
			DelayedPacket dp = packetQueue.front();

			if (isServerToClient)
			{
				m_server.peer->Send(&dp.bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, dp.address, false);
			}
			else
			{
				m_client.peer->Send(&dp.bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, dp.address, false);
			}
			packetQueue.pop();
		}
		else
		{
			break;
		}

	}
}


void FaceOff::replyBackToNewClient(int playerId, RakNet::BitStream& bs)
{
	bs.Reset();	// Very important!!
	
	bs.Write((RakNet::MessageID)SPAWN_INFORMATION);
	Player* p = sv_players.get(playerId);
	p->serialize(bs);

	// Sending each client's position to new client
	utl::debug("m_players size", sv_players.count);
	std::cout << "Sending each client's position to new client" << endl;


	// excluding yourself
	bs.Write(m_server.getNumConnectedClients()-1);

	for (int i = 0; i < m_server.getNumClients(); i++)
	{
		cout << "in reply, looking at player " << i << endl;

		if (i == playerId)
			continue;

		if (m_server.isClientConnected(i))
		{
			p = sv_players.get(i);

			std::cout << "sending for " << i << endl;
			p->serialize(bs);
		}
	}


//	cout << "client address " << RakNet::SystemAddress::ToInteger(m_server.getClient(playerId).systemAddress) << endl;

//	HIGH_PRIORITY, RELIABLE_ORDERED,
	m_server.peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, m_server.getClientAddress(playerId), false);

	// m_server.peer->Send(&bs, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, m_server.getClient(playerId).systemAddress, false);
}


void FaceOff::serverReadPackets()
{
//	Uint32 lastSnapShotSendTime = 0;
//	Uint32 curTime = 0;

	// iterate over each message received
	for (sv_packet = m_server.peer->Receive(); 
		sv_packet; 
		m_server.peer->DeallocatePacket(sv_packet), sv_packet = m_server.peer->Receive())
	{
	//	cout << "here in serverReadPackets" << endl;

		// we first initalized bitStream with the packet->data
		RakNet::BitStream bsIn(sv_packet->data, sv_packet->length, false);

		// we ignore the first part of each message (due to RakNet convention)
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

		// Handle message here 
		switch (sv_packet->data[0])
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

			case ID_GAME_MESSAGE_1:
				std::cout << "ID_GAME_MESSAGE_1" << endl;
				break;

			case CLIENT_INPUT:
			{
				// http://stackoverflow.com/questions/10381144/error-c2361-initialization-of-found-is-skipped-by-default-label
				// for some reason i need to enclose this case 
				// otherwise i get a initalization skipped by 'default' label error

				int sequence = 0;
				bsIn.Read(sequence);

				// utl::debug("sequence is", sequence);
			//	std::cout << "CLIENT_INPUT" << einndl;
				


				int clientId = 0;
				bsIn.Read(clientId);

				m_server.setClientIncomingSequence(clientId, sequence);
				m_server.setClientLastUserCmdFrame(clientId, sequence);


				int cmdNum = 0;
				bsIn.Read(cmdNum);
			//	utl::debug("server read cmdNum", cmdNum);
				m_server.setClientLastUserCmdNum(clientId, cmdNum);

				// get the command
				UserCmd cmd; 
				cmd.deserialize(bsIn);
				// run the command
				serverRunClientMoveCmd(clientId, cmd);

				break;
			}
			default:
				printf("Message with identifier %i has arrived.\n", sv_packet->data[0]);
				break;

			/*
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
		*/

				

		}
	}

	/*
	curTime = SDL_GetTicks();

	if (curTime - lastSnapShotSendTime > SERVER_SNAPSHOT_TIME_STEP)
	{
		//	utl::debug("	>>>> Sending snapshot to shits at time", (int)curTime);

		lastSnapShotSendTime = curTime;
	}
	*/
}




// i'm making things simple. Im just sending 
void FaceOff::serverFrame(long long dt)
{


	
	serverReadPackets();

	// allow pause if only the local client is connected
	/*
	if (SV_CheckPaused()) 
	{
		return;
	}
	*/

	// this can happen considerably earlier when lots of clients play and the map doesn't change
	/*
	if (m_server.time >= 0x70000000)
	{
		cout << "Restarting server due to time wrapping" << endl;
		return;
	}
	*/

	// this can happen considerably earlier when lots of clients play and the map doesn't change
	if (m_server.nextSnapshotEntityIndex >= 0x7FFFFFFE - m_server.nextSnapshotEntityIndex)
	{
		cout << "Restarting server due to m_server.nextSnapshotEntityStartIndex wrapping" << endl;
		return;
	}

	serverCalculatePing();

//	if (// if time to simulate)
	{

	}


	
	int msPerFrame;

	msPerFrame = 1000 / m_server.m_fps;

	//	m_server.timeResidual += msec;

	bool runServerFrame = false;

	m_server.m_deltaTimeAccumulatorMS += dt;
	while (m_server.m_deltaTimeAccumulatorMS >= msPerFrame)
	{
		serverSimulationTick(msPerFrame);
		m_server.m_deltaTimeAccumulatorMS -= msPerFrame;
		m_server.time += dt;
//		m_server.realTime += dt;
		runServerFrame = true;
	}

	// we only want to send it at a rate that the server is ticking
	if (runServerFrame)
	{
		serverSendClientMessages();
	}

	processPacketQueue(m_server.realTime, serverToClient, true);

}


/*
===================
Updates the client ping variables
===================
*/
void FaceOff::serverCalculatePing()
{





	/*
	for (int i = 0; i < m_server.clients.size(); i++)
	{
		Client* client = m_server.getClient(i);

		if (client == NULL)
		{
			client->ping = INFINITE_PING;
			return;
		}

	}
	*/


}

void FaceOff::serverCheckTimeouts()
{

}


void FaceOff::serverSendClientMessages()
{
	for (int i = 0; i < m_server.getNumClients(); i++)
	{
		if (m_server.isClientConnected(i) == false)
		{
			continue;
		}
		
		/*
		if (m_server.time < client->nextSnapshotTime)
		{
			continue;
		}
		*/
		debugCurClientId = i;
		serverSendClientSnapshot(i);
	}
}


void FaceOff::serverSendClientSnapshot(int clientId)
{
	// this is the snapshot we are creating
	Snapshot *to, *from;
	int toIndex = m_server.getClientOutgoingSequence(clientId) & SV_SNAPSHOT_BUFFER_MASK;
	to = &m_server.getClientSnapshot(clientId, toIndex);


	// check if it has been long since we got a snapshot last time
	//
	//
	//
	//

	int fromIndex = m_server.getClientLastUserCmdFrame(clientId) & SV_SNAPSHOT_BUFFER_MASK;
	from = &m_server.getClientSnapshot(clientId, fromIndex);


	RakNet::BitStream bs;
	serverWritePlayers(from, to, clientId, bs);
	serverWriteEntities(from, to, bs);


	RakNet::BitStream bs1;
	bs1.Write((RakNet::MessageID)SERVER_SNAPSHOT);
	bs1.Write(m_server.getClientOutgoingSequence(clientId));
	m_server.incrementClientOutgoingSequence(clientId);

	bs1.Write(m_server.realTime);


	bs1.Write(m_server.getClientLastUserCmdNum(clientId));

	//	bs1.Write(client->netchan.outgoingSequence);
	// utl::debug("sending snapshot sequence", client->netchan.outgoingSequence);

	// ++(client->netchan.outgoingSequence);




	// utl::debug("sending snapshot sequence", client->netchan.outgoingSequence);

	bs1.Write(bs);	
	// cout << "bs " << bs.GetNumberOfBitsUsed() << endl;
	// cout << "bs1 " << bs1.GetNumberOfBitsUsed() << endl;

//	m_server.peer->Send(&bs1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, client->systemAddress, false);

//	m_server.peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, client.systemAddress, false);



	serverToClientSendPacket(m_server.getClientAddress(clientId), bs1);
//	m_server.peer->Send(&bs1, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, m_server.getClient(clientId).systemAddress, false);
}

void FaceOff::serverWritePlayers(Snapshot* from, Snapshot* to, int clientId, RakNet::BitStream& bs)
{
	for (int i = 0; i < sv_players.getIterationEnd(); i++)
	{
		Player* player = sv_players.getByIndex(i);
		if (player != NULL)
		{
			if (i == clientId)
			{
				serverWriteDefaultPlayer(player, bs);
			}
			else
			{
				serverWriteDefaultPlayer(player, bs);
			}
		}
	}

	
	bs.Write(END_OF_SV_SNAPSHOT_PLAYERS);
}


void FaceOff::serverWriteDefaultPlayer(Player* p, RakNet::BitStream& bs)
{
	int flags;

	// positions
	flags |= U_POSITION0;
	flags |= U_POSITION1;
	flags |= U_POSITION2;

	bs.Write(flags);
	bs.Write(p->objectId.id);

	if (flags & U_POSITION0)		bs.Write(p->m_position[0]);
	if (flags & U_POSITION1)		bs.Write(p->m_position[1]);
	if (flags & U_POSITION2)		bs.Write(p->m_position[2]);
}

/*
void FaceOff::serverWriteOtherPlayer(Player* p, RakNet::BitStream& bs)
{
	int flags = 0;

	bs.Write(flags);
	bs.Write(obj1->objectId.id);

	// positions
	if (0.0 != obj1->position[0])		flags |= U_POSITION0;
	if (0.0 != obj1->position[1])		flags |= U_POSITION1;
	if (0.0 != obj1->position[2])		flags |= U_POSITION2;

	// angles
	if (0.0 != obj1->angles[0])		flags |= U_ANGLE0;
	if (0.0 != obj1->angles[1])		flags |= U_ANGLE1;
	if (0.0 != obj1->angles[2])		flags |= U_ANGLE2;

	if (flags & U_POSITION0)		bs.Write(obj1->position[0]);
	if (flags & U_POSITION1)		bs.Write(obj1->position[1]);
	if (flags & U_POSITION2)		bs.Write(obj1->position[2]);

	if (flags & U_ANGLE0)		bs.Write(obj1->angles[0]);
	if (flags & U_ANGLE1)		bs.Write(obj1->angles[0]);
	if (flags & U_ANGLE2)		bs.Write(obj1->angles[0]);
}
*/

void FaceOff::serverWriteEntities(Snapshot* from, Snapshot* to, RakNet::BitStream& bs)
{
	// these are only dynamic objects, we don't care about static objects
	to->numEntities = 0;
	to->firstEntityIndex = m_server.nextSnapshotEntityIndex;

	/*
	cout << "iterating" << endl;
	for (int i = 0; i < sv_objects.getIterationEnd(); i++)
	{
		if (sv_objects.getByIndex(i) != NULL)
		{
			WorldObject* obj = sv_objects.getByIndex(i);

			cout << "	" << i << endl;
			cout << "		" << obj->objectId.s.tag << endl;
			cout << "		" << obj->objectId.s.index << endl;
		}

	}
	*/


	for (int i = sv_objects.preferredIterationStart; i < sv_objects.getIterationEnd(); i++)
	{
		if (sv_objects.getByIndex(i) != NULL)
		{
			// recall we are using a circular buffer
			int index = m_server.nextSnapshotEntityIndex % m_server.snapshotObjectStatesBufferSize;

			m_server.snapshotObjectStates[index] = sv_objects.getByIndex(i)->getState();
			++(m_server.nextSnapshotEntityIndex);


			// this should never hit, the game should quit in serverFrame before
			if (m_server.nextSnapshotEntityIndex >= 0x7FFFFFFE) 
			{
				cout << "svs.nextSnapshotEntityIndex wrapped" << endl;
			}

			++(to->numEntities);
		}
	}



	int firstIndex0 = from->firstEntityIndex;
	int firstIndex1 = to->firstEntityIndex;

	int numEntities0 = (from == NULL) ? 0 : from->numEntities; 
	int numEntities1 = to->numEntities;


	WorldObjectState* obj0 = NULL; 
	WorldObjectState* obj1 = NULL;


	int i0 = 0;
	int i1 = 0;


	ObjectId id0;
	ObjectId id1;

	id0.id = -1;
	id1.id = -1;

	// cout << "iterating"  << endl;


	// we iterate till both lists are done
	while (i1 < numEntities1 || i0 < numEntities0)
	{
		if (i1 >= numEntities1)
		{
			id1.id = INVALID_OBJECT;
		}
		else
		{
			obj1 = &m_server.snapshotObjectStates[ (firstIndex1 + i1) % m_server.snapshotObjectStatesBufferSize];
			id1 = obj1->objectId;
		}


		if (i0 >= numEntities0)
		{
			id0.id = INVALID_OBJECT;
		}
		else
		{
			obj0 = &m_server.snapshotObjectStates[(firstIndex0 + i0) % m_server.snapshotObjectStatesBufferSize];
			id0 = obj0->objectId;
		}

		/*
		cout << "	id1 " << id1.id << endl;
		cout << "	id0 " << id0.id << endl;

		cout << "	id1 tag " << id1.s.tag << endl;
		cout << "	id0 tag " << id0.s.tag << endl;

		cout << "	id1 index " << id1.s.index << endl;
		cout << "	id0 index " << id0.s.index << endl;
		*/

		// writing objects
		if (id1.s.index == id0.s.index)
		{
			if (id1.s.tag == id0.s.tag)
			{
				serverWriteWorldObjects(obj0, obj1, bs, false);
				++i0;
				++i1;
				continue;
			}
			else
			{
				// remove old object
				serverWriteRemoveWorldObject(obj0, bs);
				++i0;

				// add new object
				serverWriteNewWorldObject(obj1, bs);
				++i1;
				continue;
			}
		}

		// adding new entity
		if (id1.s.index < id0.s.index)
		{
			serverWriteNewWorldObject(obj1, bs);
			++i1;
			continue;

		}

		// the old entity isn't present in the new message
		// removing entity
		if (id1.s.index > id0.s.index)
		{
			serverWriteRemoveWorldObject(obj0, bs);
			++i0;
			continue;
		}
	}

	bs.Write(END_OF_SV_SNAPSHOT_ENTITIES);

	/*
	// skip all the static objects
	for (int i = sv_objects.preferredIterationStart; i < sv_objects.getIterationEnd(); i++)
	{

	}
	*/
}



void FaceOff::serverWriteRemoveWorldObject(WorldObjectState* obj0, RakNet::BitStream& bs)
{
	int flags = 0;

	flags |= U_REMOVE;

	bs.Write(flags);
	bs.Write(obj0->objectId.id);
}



void FaceOff::serverWriteNewWorldObject(WorldObjectState* obj1, RakNet::BitStream& bs)
{
	int flags = 0;

	flags |= U_ADD;

	WorldObject* obj = sv_objects.get(obj1->objectId.id);

	if (obj->getObjectType() == WorldObjectType::SCENE_OBJECT)
	{
		flags |= U_SCENE_OBJECT;
	}
	else if (obj->getObjectType() == WorldObjectType::WEAPON)
	{
		flags |= U_WEAPON;
	}

	bs.Write(flags);
	sv_objects.get(obj1->objectId.id)->serialize(bs);
	if (obj1->objectId.s.index == 27)
	{
		utl::debug("debugCurClientId", debugCurClientId);
		utl::debug("last client Cmd", m_server.getClientLastUserCmdFrame(debugCurClientId));
		utl::debug("	writing 27 new new mainWeapon"); 
	}
}

void FaceOff::serverWriteWorldObjects(WorldObjectState* obj0, WorldObjectState* obj1, RakNet::BitStream& bs, bool force)
{
	int flags = 0;

	// positions
	if (obj0->position[0] != obj1->position[0])		flags |= U_POSITION0;
	if (obj0->position[1] != obj1->position[1])		flags |= U_POSITION1;
	if (obj0->position[2] != obj1->position[2])		flags |= U_POSITION2;

	// angles
	if (obj0->angles[0] != obj1->angles[0])		flags |= U_ANGLE0;
	if (obj0->angles[1] != obj1->angles[1])		flags |= U_ANGLE1;
	if (obj0->angles[2] != obj1->angles[2])		flags |= U_ANGLE2;


	if (!flags && !force)
	{
	//	cout << "nothing to send\n" << endl;
		return;
	}

	flags |= U_DELTA;

	bs.Write(flags);
	bs.Write(obj0->objectId.id);
	if (flags & U_POSITION0)		bs.Write(obj1->position[0]);
	if (flags & U_POSITION1)		bs.Write(obj1->position[1]);
	if (flags & U_POSITION2)		bs.Write(obj1->position[2]);

	if (flags & U_ANGLE0)		bs.Write(obj1->angles[0]);
	if (flags & U_ANGLE1)		bs.Write(obj1->angles[0]);
	if (flags & U_ANGLE2)		bs.Write(obj1->angles[0]);
}


void FaceOff::serverRunClientMoveCmd(int playerId, UserCmd cmd)
{
	Player* player = sv_players.get(playerId);
	/*
	if (cmd.buttons & FORWARD)
	{
		utl::clDebug("pressed forward");
	}

	if (cmd.buttons & BACK)
	{
		utl::clDebug("pressed BACK");
	}

	if (cmd.buttons & LEFT)
	{
		utl::clDebug("pressed LEFT");
	}

	if (cmd.buttons & RIGHT)
	{
		utl::clDebug("pressed RIGHT");
	}
	*/
	player->processUserCmd(cmd);
}


/*
The client creates user commands from sampling input devices with the same tick rate that the server is running 
with. 

Instead of sending a new packet to the server for each user command, the client sends command packets at a certain rate of packets per second (usually 30).
This means two or more user commands are transmitted within the same packet. The Clients can increase the command rate with c1-cmdrate. This will increase
responsiveness but requires more outgoing bandwidth, too. 

https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking
*/
void FaceOff::clientFrame(long long dt)
{
	//clientHandleDeviceEvents(); 

	clientReadPackets();

	clientSendCmd();
	clientCheckForResend();

	processPacketQueue(m_client.realTime, clientToServer, false);

	// I opt to do player prediction collision detection on the interpolated objects
	// so I do this before the clientPrediction
	interpolateEntities();

	clientPrediction();

	render();	
}

// interpolate, also add entities to the collision tree
void FaceOff::interpolateEntities()
{
	// https://developer.valvesoftware.com/wiki/Source_Multpiplayer_Networking

	// cur ClientTime
	long long curClientRenderTime = m_client.realTime - CLIENT_INTERP_DELAY;

	
	ClientSnapshot* from = NULL;
	ClientSnapshot* to = NULL;


	if (m_client.curSnapshot != NULL)
	{
		std::cout << "## cursnapshot serverTime " << m_client.curSnapshot->serverTime << endl;
		std::cout << "## cur renderTime " << curClientRenderTime << endl;

		int start = m_client.curSnapshot->messageSequenceNum;
		int end = std::max(start - 20, 0);
		for (int i = start; i >= end; i--)
		{


			int snapshotIndex = i & CL_SNAPSHOT_BUFFER_MASK;



			ClientSnapshot* temp = &m_client.snapshots[snapshotIndex];

			std::cout << "	snapshotIndex " << i << ", temp snapshot serverTime " << temp->serverTime << endl;
		//	std::cout << "	temp serverTime" << i << endl;

			if (temp->valid() && temp->serverTime > curClientRenderTime);
			{
				to = temp;
			}

			if (temp->valid() && temp->serverTime <= curClientRenderTime)
			{
				if (from != NULL && temp->serverTime < from->serverTime)
				{
					from = temp;
				}
			}

			if (to && from)
			{
				break;
			}
		}

		if (from == NULL)
		{
			from = to;
		}

		std::cout << "to snapshot serverTime " << to->serverTime << endl;
		std::cout << "from snapshot serverTime " << from->serverTime << endl;


		long long numer = curClientRenderTime - from->serverTime;
		long long denom = to->serverTime - from->serverTime;

		std::cout << "numer " << numer << endl;
		std::cout << "denom " << denom << endl;

		double interpFactor = (double)numer / (double)denom;

		std::cout << interpFactor << endl;

		ClientWorldObjectState cState0;
		ClientWorldObjectState cState1;

		for (int i = to->start; i < to->getIterationEnd(); i++)
		{
			cState1 = to->entities[i];
			ObjectId objId1 = cState1.state.objectId;

			if (cState1.flags && U_DELTA)
			{
				cState0 = from->entities[i];
				ObjectId objId0 = cState0.state.objectId;
				WorldObject* obj = cl_objects.get(objId1);
				if (objId0.id == objId1.id)
				{
					// interpolate position
					// interpolate angle

					obj->m_position = utl::interpolateEntityPosition(cState0.state.position, cState1.state.position, interpFactor);
				}
				else
				{
					obj->m_position = cState1.state.position;
				}

				cl_objectKDtree.insert(obj);
			}
		}
	}
}




/*

interpolate



	void InterpoalteEntity(i, Snapshot* from, Snapshot* to, lerpfactor)
	{
		state1 = to.entities[i];
		objectId = state1.objectId;

		state0 = from.entities[objectId.s.index]

		if(state1.objectId != state0.objectId
		{
			state0 = state1;
		}


		interpolatePosition(state0.pos, state1.pos, interFactor);
		interpoalteAngle();

	}

*/


void FaceOff::clientReadPackets()
{
	Uint32 lastSnapShotSendTime = 0;
	Uint32 curTime = 0;

	for (cl_packet = m_client.peer->Receive(); 
		cl_packet; 
		m_client.peer->DeallocatePacket(cl_packet), cl_packet = m_client.peer->Receive())
	{
	//	utl::clDebug("here in clientReadPackets");

		RakNet::BitStream bsIn(cl_packet->data, cl_packet->length, false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

		/*
		int sequence = 0;
		bsIn.Read(sequence);

		utl::debug("sequence is", sequence);

		RakNet::MessageID msgId = NONE;
		bsIn.Read(msgId);
		*/
		switch (cl_packet->data[0])
		{
			case NEW_CLIENT:
				cout << "NEW_CLIENT message " << endl;
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				break;

			case ID_GAME_MESSAGE_1:
				utl::clDebug("ID_GAME_MESSAGE_1");
				break;
			
			case SERVER_SNAPSHOT:
			{
				// snap shot from server
			//	utl::clDebug("SNAPSHOT from server");
				int sequence = 0;
				bsIn.Read(sequence);

				m_client.netchan.incomingSequence = sequence;
			//	utl::clDebug("sequence", sequence);


				clientParseSnapshot(bsIn);
				break;
			}

			case LOBBY_WAIT_END:
				utl::debug("in LOBBY_WAIT_END");
				break;

			default:
				printf("Message with identifier %i has arrived.\n", cl_packet->data[0]);
				break;
		}

		/*
		switch (packet->data[0])
		{
		case NEW_CLIENT:
			cout << "NEW_CLIENT message " << endl;
			break;

		case ID_CONNECTION_REQUEST_ACCEPTED:
			break;

		case SERVER_SNAPSHOT:
		{
			// snap shot from server
			cout << "SNAPSHOT from server" << endl;
			clientParseSnapshot(bsIn);
			break;
		}

		default:
			printf("Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}
		*/
	}

	// sending server my inputs
	curTime = SDL_GetTicks();

	/*
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
	*/
}


void FaceOff::clientCheckForResend()
{

}

void FaceOff::clientParseSnapshot(RakNet::BitStream& bs)
{
	ClientSnapshot* prev = NULL;
	ClientSnapshot* cur = &m_client.snapshots[m_client.netchan.incomingSequence & CL_SNAPSHOT_BUFFER_MASK];
	
	bs.Read(cur->serverTime);
	cout << "	cur serverTime: " << cur->serverTime << endl;

	cur->messageSequenceNum = m_client.netchan.incomingSequence;

	bs.Read(m_client.lastAckCmdNum);

//	utl::clDebug("lastAckCmdNum", m_client.lastAckCmdNum);

	clientParsePlayers(prev, cur, bs);
	clientParseEntities(prev, cur, bs);

	m_client.curSnapshot = cur;
}


void FaceOff::clientParsePlayers(ClientSnapshot* prev, ClientSnapshot* cur, RakNet::BitStream& bs)
{
	int id1 = 0;

	while (true)
	{
		//bs.Read(id1);

		int flags = 0;
		bs.Read(flags);
		if (flags == END_OF_SV_SNAPSHOT_PLAYERS)
		{
			break;
		}

		bs.Read(id1);

		Player* p = cl_players.getByIndex(id1);

		bs.Read(p->m_position[0]);
		bs.Read(p->m_position[1]);
		bs.Read(p->m_position[2]);
		p->updateCollisionDetectionGeometry();

		if (id1 == m_defaultPlayerID)
		{
			cur->playerState = p->getState();
		}

		// cout << "id1 " << id1 << endl;
		// utl::cldebug("	pos is ", p->m_position);

		// p->debug();

		/*
		ObjectId objId(id1);

		if (flags & U_DELTA)
		{
			clientParseDeltaEntity(flags, objId, bs);
		}

		else if (flags & U_ADD)
		{
			clientParseAddEntity(objId, bs);
		}

		else if (flags & U_REMOVE)
		{
			clientParseRemoveEntity(objId, bs);
		}
		*/
	}
}



// we use the brute force way for now, 
// quake2/3 uses the prev snapshot to check if something is new or not
// for now, i'm sending the U_DELTA, U_ADD, U_REMOVE signal from the server side
void FaceOff::clientParseEntities(ClientSnapshot* prev, ClientSnapshot* cur, RakNet::BitStream& bs)
{
	int id0 = 0;
	int id1 = 0;

	
	// utl::debug("clientParseEntities");

	while (true)
	{
		int flags = 0;

		bs.Read(flags);	
		if (flags == END_OF_SV_SNAPSHOT_ENTITIES)
		{
			break;
		}

		// bs.Read(id1);

		// cout << "flags is " << flags << endl;
		// cout << "id1 is " << id1 << endl;


		// ObjectId objId(id1);
		/*
		cout << "flags is " << flags << endl;

		if (flags & U_DELTA)
		{
			cout << "U_DELTA " << endl;
		}
		else if (flags & U_ADD)
		{
			cout << "U_ADD " << flags << endl;
		}
		else if (flags & U_REMOVE)
		{
			cout << "U_REMOVE " << flags << endl;
		}

		cout << "id1 is " << objId.s.index << endl;
		*/
		if (flags & U_DELTA)
		{
	//		utl::debug("	U_DELTA snapshot");
			clientParseDeltaEntity(cur, flags, bs);
		}

		else if (flags & U_ADD)
		{
//			utl::debug("	U_ADD snapshot");
//			utl::debug("m_client sequence", m_client.netchan.incomingSequence);

			clientParseAddEntity(cur, flags, bs);
		}

		else if (flags & U_REMOVE)
		{
			clientParseRemoveEntity(cur, flags, bs);
		}
	}
}


void FaceOff::clientParseDeltaEntity(ClientSnapshot* cur, int flags, RakNet::BitStream& bs)
{
	int id = 0;
	bs.Read(id);

	ObjectId objId(id);
	
	WorldObject* obj = cl_objects.get(objId);

	clientParseEntityData(obj, flags, bs);


	ClientWorldObjectState state(flags, obj->getState());
	cur->set((int)objId.s.index, state);

//	cur->entities[objId.s.index].flags = flags;
//	cur->entities[objId.s.index].state = obj->GetState();
}


void FaceOff::clientParseAddEntity(ClientSnapshot* cur, int flags, RakNet::BitStream& bs)
{
	if (flags & U_SCENE_OBJECT)
	{
		WorldObject* obj = new WorldObject();
		deserializeEntityAndAddToWorld(obj, bs);

		ObjectId objId = obj->objectId;
		ClientWorldObjectState state(flags, obj->getState());
		cur->set((int)objId.s.index, state);
	}
	else if (flags & U_WEAPON)
	{
		Weapon* obj = new Weapon();
		deserializeEntityAndAddToWorld(obj, bs);

		ObjectId objId = obj->objectId;
		ClientWorldObjectState state(flags, obj->getState());
		cur->set((int)objId.s.index, state);

		if (obj->objectId.s.index == 27)
		{
			utl::clDebug("reading 27 new new mainWeapon");
		}

		/*
		if (obj.objectId.id == 27)
		{
			cout << "new weapon address is " << endl;
			cout << *(obj) << endl;;


			cout << endl;
			Player* p = cl_players.get(0);
			for (int i = 0; i <p->getWeapons().size(); i++)
			{
				Weapon* weapon = cl_players.get(0)->getWeapon(i);
				cout << "new weapon address is " << endl;
				cout << *obj << endl;;
			}
		}
		*/
	}




	/*
	if (obj->getObjectType() == WorldObjectType::SCENE_OBJECT)
	{
		flags |= U_SCENE_OBJECT;
	}
	else if (obj->getObjectType() == WorldObjectType::WEAPON)
	{
		flags |= U_WEAPON;
	}
	*/

	/*
	int type = 0;
	bs.Read(type);
	WorldObjectType objType = (WorldObjectType)type;


	if (objType == SCENE_OBJECT)
	{
		WorldObject* obj = new WorldObject();
		deserializeEntityAndAddToWorld(obj, bs);
	}
	else if (objType == WEAPON)
	{
		Weapon* obj = new Weapon();
		deserializeEntityAndAddToWorld(obj, bs);
	}
	*/
}

void FaceOff::clientParseRemoveEntity(ClientSnapshot* cur, int flags, RakNet::BitStream& bs)
{
	int id = 0;
	bs.Read(id);

	ObjectId objId(id);

	cl_objects.destroy(objId);

	ClientWorldObjectState state(flags);
	cur->set((int)objId.s.index, state);
}


void FaceOff::clientParseEntityData(WorldObject* obj, int flags, RakNet::BitStream& bs)
{
	if (flags & U_POSITION0)		bs.Read(obj->m_position[0]);
	if (flags & U_POSITION1)		bs.Read(obj->m_position[1]);
	if (flags & U_POSITION2)		bs.Read(obj->m_position[2]);

	float tmp = 0;

	if (flags & U_ANGLE0)
	{
		bs.Read(tmp);
	}
	if (flags & U_ANGLE1)
	{
		bs.Read(tmp);
	}
	if (flags & U_ANGLE2)
	{
		bs.Read(tmp);
	}
}

void FaceOff::clientSendCmd()
{

	if (!m_client.isConnected)
	{
		return;
	}
	UserCmd cmd = clientCreateNewCmd();

	++m_client.cmdNum;
	int newCmdIndex = m_client.cmdNum & CMD_BUFFER_MASK;
	

	// sprintf("		cl cmdNumber %d \n", m_client.cmdNum);


	m_client.cmds[newCmdIndex] = cmd;

	// send it off with Raknet

	/*
	// don't send a packet if the last packet was sent too recently
	if (!CL_ReadyToSendPacket()) {
		if (cl_showSend->integer) {
			Com_Printf(". ");
		}s
		return;
	}
	*/
	RakNet::BitStream bsOut;
	// bsOut.Write((RakNet::MessageID)CLIENT_INPUT);
	// utl::clDebug("m_defaultPlayerID", m_defaultPlayerID);
	bsOut.Write(m_defaultPlayerID);
	bsOut.Write(m_client.cmdNum);
	cmd.serialize(bsOut);

	bool b = (cmd.buttons == 0);

	/*
	if (cmd.buttons != 0)
	{
		cout << "client sending cmd" << endl;
		if (cmd.buttons & FORWARD)
		{
			cout << "	pressed FORWARD" << endl;
		}

		if (cmd.buttons & BACK)
		{
			cout << "	pressed BACK" << endl;
		}

		if (cmd.buttons & LEFT)
		{
			cout << "	pressed LEFT" << endl;
		}

		if (cmd.buttons & RIGHT)
		{
			cout << "	pressed RIGHT" << endl;
		}
	}
	*/
	clientSendPacket(bsOut);
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
			/*
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
			*/
			break;
			
		case SDL_MOUSEBUTTONDOWN:

			switch (event.button.button)
			{
				int tmpx, tmpy;
			case SDL_BUTTON_LEFT:
				SDL_GetMouseState(&tmpx, &tmpy);
				m_mouseState.m_leftButtonDown = true;
				/*
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

						m_objectKDtree.visitNodes(m_objecretKDtree.m_head, m_players[m_defaultPlayerID], lineStart, lineDir, 500.0f, hitObject, hitObjectSqDist, hitPoint, objectsAlreadyTested);

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
				*/
				cl_players.get(m_defaultPlayerID)->m_camera->setMouseIn(true);





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
				/*
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
				*/

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
				//m_players[m_defaultPlayerID]->reloadWeapon();
			}
			break;

			case SDLK_g:
			{

				utl::debug("Dropping Weapon");
				/*
				Weapon* droppedWeapon = m_players[m_defaultPlayerID]->drop();
				if (droppedWeapon != NULL)
				{
					m_objectKDtree.insert(droppedWeapon);
				}
				*/
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
				/*
				if (m_isServer)
					m_serverCamera.setMouseIn(false);
				else
					m_players[m_defaultPlayerID]->m_camera->setMouseIn(false);
				|*/
				break;
			}
			break;
		}
	}

}





void FaceOff::clientSendPacket(RakNet::BitStream& bs)
{
	RakNet::BitStream bsOut;

	bsOut.Write((RakNet::MessageID)CLIENT_INPUT);
	
	bsOut.Write(m_client.netchan.outgoingSequence);
	++m_client.netchan.outgoingSequence;

	// utl::clDebug("m_client.netchan.outgoingSequence", m_client.netchan.outgoingSequence);

	bsOut.Write(bs);


//	DelayedPacket dp(0, m_client.serverSystemAddress, bsOut);

//	myDP.address = m_client.serverSystemAddress;
//	myDP.bs.Reset();
//	myDP.bs.Write(bsOut);

//	m_client.peer->Send(&dp.bs, IMMEDIATE_PRIORITY, RELIABLE, 0, m_client.serverSystemAddress, false);

//	m_client.peer->Send(&dp.bs, IMMEDIATE_PRIORITY, RELIABLE, 0, dp.address, false);
	
//	m_client.peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, dp.address, false);
	
//	m_client.peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, m_client.serverSystemAddress, false);
//	bsOut.SetReadOffset(0);
	clientToServerSendPacket(m_client.serverSystemAddress, bsOut);
}

UserCmd FaceOff::clientCreateNewCmd()
{
	// CA_PRIMED got gamestate, waiting for first frame
	// no need to create usercmds until we have gamestate

	/*
	if ()
	{
		return;
	}
	*/

	UserCmd cmd;
//	cmd.playerId = m_defaultPlayerID;

	// get buttons
	SDL_PumpEvents();
	Uint8* state = SDL_GetKeyState(NULL);
	if (state[SDLK_w])	{cmd.buttons |= FORWARD;}
	if (state[SDLK_s])	{cmd.buttons |= BACK;}
	if (state[SDLK_a])	{cmd.buttons |= LEFT;}
	if (state[SDLK_d])	{cmd.buttons |= RIGHT;}
	if (state[SDLK_SPACE])	{cmd.buttons |= JUMP;}
	if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) { cmd.buttons |= ATTACK; }


	// get mouse movement
	int mx, my;	
	SDL_GetMouseState(&mx, &my);

	float deltaYaw = 0;
	float deltaPitch = 0;

	if (cl_players.get(m_defaultPlayerID)->m_camera->getMouseIn())
	{
		deltaYaw = TURN_SPEED * (utl::SCREEN_WIDTH_MIDDLE - mx);
		deltaPitch = FORWARD_SPEED * (utl::SCREEN_HEIGHT_MIDDLE - my);
	}

	// float deltaYaw = TURN_SPEED * (utl::SCREEN_WIDTH_MIDDLE - mx);
	// float deltaPitch = FORWARD_SPEED * (utl::SCREEN_HEIGHT_MIDDLE - my);



	// rate = sqrt(mx * mx + my * my) / (float)frame_msec;

	// cmd.angles[YAW] += deltaYaw;
	// cmd.angles[PITCH] += deltaPitch;

	float pitch = cl_players.get(m_defaultPlayerID)->getCameraPitch();
	float yaw = cl_players.get(m_defaultPlayerID)->getCameraYaw();

	pitch += deltaPitch;
	yaw += deltaYaw;

	cmd.angles[PITCH] = pitch;
	cmd.angles[YAW] = yaw;

	if (cmd.angles[PITCH] > 90)
		cmd.angles[PITCH] = 90;

	if (cmd.angles[PITCH] < -90)
		cmd.angles[PITCH] = -90;

	if (cmd.angles[YAW] < 0.0)
		cmd.angles[YAW] += 360.0;

	if (cmd.angles[YAW] > 360.0)
		cmd.angles[YAW] -= 360;



//	pitch = cl_players.get(m_defaultPlayerID)->getCameraPitch();
//	yaw = cl_players.get(m_defaultPlayerID)->getCameraYaw();


	cmd.weapon = m_client.weapon;
	cmd.serverTime = m_client.serverTime;

	return cmd;
}



void FaceOff::clientPrediction()
{
	UserCmd oldestCmd, latestCmd;


	if (cl_players.get(m_defaultPlayerID)->m_camera->getMouseIn())
	{
		SDL_WarpMouse(utl::SCREEN_WIDTH_MIDDLE, utl::SCREEN_HEIGHT_MIDDLE);
	}

	// int currentCmdIndex = m_client.cmdCounter & CMD_BUFFER_MASK;
	Player* p = cl_players.get(m_defaultPlayerID);



	p->updateGameStats();


	UserCmd cmd;

	if (predictionOn)
	{
		if (m_client.curSnapshot != NULL)
		{

			
			int cmdNum = m_client.lastAckCmdNum + 1;
			WorldObjectState playerState = m_client.curSnapshot->playerState;
			long long latestSnapshotTime = m_client.curSnapshot->serverTime;




			p->setPosition(playerState.position);
			p->updateCollisionDetectionGeometry();
			cl_objectKDtree.reInsert(p);
	//		utl::clDebug("m_client current cmdNum", m_client.cmdNum);
		
	//		cout << "m_client.cmdNum " << m_client.cmdNum << ", m_client.lastAckCmdNum " << m_client.lastAckCmdNum << endl;

			long long time0 = utl::getCurrentTime_ms();
			// uint64 time0 = utl::GetTimeMs64();
			while (cmdNum <= m_client.cmdNum)
			{
				int cmdIndex = cmdNum & CMD_BUFFER_MASK;
			//	utl::clDebug("	cmdIndex", cmdIndex);
				cmd = m_client.cmds[cmdIndex];
				p->processUserCmd(cmd);
	

		//		clientSimulatePlayerPhysics(cl_objectKDtree, p, m_defaultPlayerID, (cmdNum == m_client.lastAckCmdNum + 1));

				collisionDetectionTestPairs.clear();
				simulatePlayerPhysics(cl_objectKDtree, p, m_defaultPlayerID);
				++cmdNum;
			}
			long long time1 = utl::getCurrentTime_ms();
			// uint64 time1 = utl::GetTimeMs64();

			/*
			if (timeProfilerIndex == TIME_PROFILER_BUFFER)
			{
				timeProfilerIndex = 0;
			}	
			cout << time0 << " " << time1 << endl;
			cout << timeProfilerIndex << endl;
			timeProfiler[timeProfilerIndex++] = time1 - time0;
			GetTimeProfilerAverages();
			*/
			
			/*
			UserCmd oldestCmd;
			UserCmd latestCmd;

			for (int cmdNum = currentCmdIndex - CMD_BUFFER_SIZE + 1; cmdNum <= currentCmdIndex; cmdNum++)
			{
			UserCmd& cmd = m_client.cmds[cmdNum & CMD_BUFFER_MASK];

			if (cmd.serverTime <= latestSnapshotTime)
			{
			continue;
			}

			}
			*/
		}
	}

	cmd = m_client.cmds[m_client.cmdNum & CMD_BUFFER_MASK];
	p->m_camera->setPitch(cmd.angles[PITCH]);
	p->m_camera->setYaw(cmd.angles[YAW]);


// run all the commands
//	for (cmdNum = current - CMD_BUFFER_SIZE + 1; cmdNum <= current; cmdNum++)

}


void FaceOff::GetTimeProfilerAverages()
{
	
	long long total = 0;

	for (int i = 0; i < TIME_PROFILER_BUFFER; i++)
	{
		total += timeProfiler[i];
	}

	cout << "average is " << total / TIME_PROFILER_BUFFER << endl;
	
	/*
	uint64 total = 0;

	for (int i = 0; i < TIME_PROFILER_BUFFER; i++)
	{
		total += timeProfiler[i];
	}

	cout << "average is " << total / TIME_PROFILER_BUFFER << endl;
	*/
	
	//	utl::debug()
}


void FaceOff::start()
{
	cout << "Start" << endl;
//	Uint32 startTime = SDL_GetTicks();
//	Uint32 curTime = startTime;
//	m_nextGameTick = 0;

	long long dt = 0;
	long long oldTime = utl::getCurrentTime_ms(); 
	long long newTime = 0;
	while (isRunning)
	{
		// curTime = SDL_GetTicks();

		newTime = utl::getCurrentTime_ms();

		dt = newTime - oldTime;
		
		update();

		if (m_server.isInitalized == true)
		{	
			m_server.realTime = newTime;
			serverFrame(dt);
		}

		if (m_server.isDedicated == false)
		{
			m_client.realTime = newTime;
			clientFrame(dt);
		}

		// render();
		oldTime = newTime;
	}
}






#if 0
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
						m_spectatorCamera.setMouseIn(true);
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
						if (m_server.isInitalized)
							m_spectatorCamera.setMouseIn(false);
						break;
				}
				break;
		}
	}

}
#endif



void FaceOff::update()
{
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	// need this for GUI
	m_mouseState.m_pos = glm::vec2(mx, utl::SCREEN_HEIGHT - my);


	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_x:
					cl_players.get(m_defaultPlayerID)->m_camera->setMouseIn(true);
					break;
				case SDLK_z:
					cl_players.get(m_defaultPlayerID)->m_camera->setMouseIn(false);
					break;
				default:
					break;
			}
		}
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_F1:		
					if (EMULATE_LATENCY)
					{
						if (curLatencyOption == latencyOptions.size() - 1)
						{
							curLatencyOption = 0;
						}
						else
						{
							++curLatencyOption;
						}
						latency = latencyOptions[curLatencyOption];

						cout << "New Latency: " << latency << " milliseconds" << endl;
					}
					break;


				case SDLK_F2:
					if (EMULATE_LATENCY)
					{
						if (curPacketLossOption == packetLossOptions.size() - 1)
						{
							curPacketLossOption = 0;
						}
						else
						{
							++curPacketLossOption;
						}
						packetLoss = packetLossOptions[curPacketLossOption];

						cout << "New percentage PacketLoss: " << packetLoss << "%" << endl;
					}
					break;

				case SDLK_F3:
					predictionOn = !predictionOn;
					break;

				default:
					break;
			}
		}
	}
}


/*

Frame
	get input, update velocity
	gravity on velocity
	ser
	pos = pos + velocity

	resolveVelocity
	resolvePenetration

	update camera
*/


void FaceOff::serverSimulationTick(int msec)
{
	// it's possible that unodered_set.clear() retains memory, so it may be slightly faster
	// then deallocating memory
	collisionDetectionTestPairs.clear();
	// CollisionDetectionTestPairs collisionDetectionTestPairs;

	for (int i = 0; i < sv_players.getIterationEnd(); i++)
	{
		Player* p = sv_players.get(i);
		if (p == NULL)
		{
			continue;
		}

		simulatePlayerPhysics(sv_objectKDtree, p, i);
	}


	for (int i = 0; i < sv_objects.getIterationEnd(); i++)
	{
		WorldObject* object = sv_objects.getByIndex(i);
		if (object == NULL)
		{
			continue;
		}

		simulateObjectPhysics(sv_objectKDtree, sv_objects, object, i);
	}

	sv_objects.destroyObjects();
}





void FaceOff::simulatePlayerPhysics(KDTree& tree, Player* p, int i)
{
	if (p == NULL)
	{
		return;
	}

	p->updateGameInfo();

	p->m_velocity += utl::BIASED_HALF_GRAVITY;

	p->updateGameStats();

	p->m_position += p->m_velocity;

	/*
	if(p->m_velocity.x != 0)
	{
		utl::debug("player veloctiy", p->m_velocity);
	}
	*/

	p->updateMidAirVelocity();

	if (singlePlayerMode)
	{
		if (i == 1)
		{

			float tempDist = 100;

			if (incrFlag)
				p->m_position.z += 0.05;
			else
				p->m_position.z -= 0.05;

			if (p->m_position.z > tempDist)
				incrFlag = false;

			if (p->m_position.z < 20)
				incrFlag = true;




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

	}



	p->updateCollisionDetectionGeometry();

	checkNeighbors(tree, p);

	p->updateCollisionDetectionGeometry();
	tree.reInsert(p);

	p->updateWeaponTransform();

}





void FaceOff::clientSimulatePlayerPhysics(KDTree& tree, Player* p, int i, bool first)
{
	if (p == NULL)
	{
		return;
	}

	p->updateGameInfo();

	p->m_velocity += utl::BIASED_HALF_GRAVITY;

	p->updateGameStats();

	p->m_position += p->m_velocity;

	p->updateMidAirVelocity();

	p->updateCollisionDetectionGeometry();

//	if (first == true)
//	{
		tree.reInsert(p);
//	}

	clientCheckNeighbors(tree, p);

//	p->updateCollisionDetectionGeometry();
//	tree.reInsert(p);

	p->updateWeaponTransform();

}



void FaceOff::clientCheckNeighbors(KDTree& tree, WorldObject* obj)
{
	obj->inMidAir = true;

	vector<WorldObject*> neighbors;
//	glm::vec3 volNearPoint(obj->getPosition());
//	tree.visitOverlappedNodes(tree.m_head, obj, volNearPoint, neighbors);

	for (int i = 0; i < obj->m_parentNodes.size(); i++)
	{
		KDTreeNode* node = obj->m_parentNodes[i];

		if (node == NULL)
		{
			continue;
		}

		for (int j = 0; j < node->m_objects.size(); j++)
		{
			WorldObject* neighbor = node->m_objects[i];
			//		utl::debug("object name is", obj->m_name);
			if (neighbor == NULL)
				continue;

			//			if (obj->objectId.id == testObject->objectId.id)
			if (neighbor->getInstanceId() == obj->getInstanceId())
				continue;

			//			utl::debug("object name is", obj->m_name);
			neighbors.push_back(neighbor);
		}

	}


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

		/*
		if (collisionDetectionTestPairs.alreadyTested(obj->objectId.id, neighbor->objectId.id))
		continue;
		else
		collisionDetectionTestPairs.addPairs(obj->objectId.id, neighbor->objectId.id);
		*/

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


#if 0
void FaceOff::simulatePlayerPhysics1(KDTree& tree, Player* p, int i)
{
	if (p == NULL)
	{
		return;
	}

	p->updateGameInfo();

	p->m_velocity += utl::BIASED_HALF_GRAVITY;

	p->updateGameStats();

	p->m_position += p->m_velocity;

	if (p->m_velocity.x != 0)
	{
		utl::debug("player veloctiy", p->m_velocity);
	}

	p->updateMidAirVelocity();

	if (singlePlayerMode)
	{
		if (i == 1)
		{

			float tempDist = 100;

			if (incrFlag)
				p->m_position.z += 0.05;
			else
				p->m_position.z -= 0.05;

			if (p->m_position.z > tempDist)
				incrFlag = false;

			if (p->m_position.z < 20)
				incrFlag = true;




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

	}



	p->updateCollisionDetectionGeometry();

	checkNeighbors1(tree, p);

	p->updateCollisionDetectionGeometry();
	tree.reInsert(p);

	p->updateWeaponTransform();

}

void FaceOff::checkNeighbors1(KDTree& tree, WorldObject* obj)
{
	obj->inMidAir = true;

	vector<WorldObject*> neighbors;
	glm::vec3 volNearPoint(obj->getPosition());
	tree.visitOverlappedNodes(tree.m_head, obj, volNearPoint, neighbors);

	/*
	tree.print(5);
	tree.print(11);
	tree.print(12);


	utl::debug("checking neighbors", neighbors.size());

	if (obj->m_name == "player 0")
	{
	utl::debug("number of parent nodes are", obj->m_parentNodes.size());

	for (int i = 0; i < obj->m_parentNodes.size(); i++)
	{
	utl::debug("	parent node i", obj->m_parentNodes[i]->id);
	}
	}
	*/

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

		/*
		if (collisionDetectionTestPairs.alreadyTested(obj->objectId.id, neighbor->objectId.id))
		continue;
		else
		collisionDetectionTestPairs.addPairs(obj->objectId.id, neighbor->objectId.id);
		*/

		if (collisionDetectionTestPairs.alreadyTested(obj->getInstanceId(), neighbor->getInstanceId()))
			continue;
		else
			collisionDetectionTestPairs.addPairs(obj->getInstanceId(), neighbor->getInstanceId());

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

#endif





void FaceOff::simulateObjectPhysics(KDTree& tree, FOArray& objects, WorldObject* object, int i)
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

			objects.addToDestroyList(i);
			return;
		}
	}



	object->m_velocity += glm::vec3(0.0f, -9.81f, 0.0f) * 0.005f * 0.5f;
	object->m_position += object->m_velocity;
	// object->m_velocity += utl::BIASED_HALF_GRAVITY;
	// object->m_position += object->m_velocity;
	object->updateCollisionDetectionGeometry();

	checkNeighbors(tree, object);

	object->updateCollisionDetectionGeometry();
	tree.reInsert(object);


}



void FaceOff::checkNeighbors(KDTree& tree, WorldObject* obj)
{
	obj->inMidAir = true;

	vector<WorldObject*> neighbors;
	glm::vec3 volNearPoint(obj->getPosition());
	tree.visitOverlappedNodes(tree.m_head, obj, volNearPoint, neighbors);
	
	/*
	tree.print(5);
	tree.print(11);
	tree.print(12);
	

	utl::debug("checking neighbors", neighbors.size());

	if (obj->m_name == "player 0")
	{
		utl::debug("number of parent nodes are", obj->m_parentNodes.size());

		for (int i = 0; i < obj->m_parentNodes.size(); i++)
		{
			utl::debug("	parent node i", obj->m_parentNodes[i]->id);
		}
	}
	*/

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

		/*
		if (collisionDetectionTestPairs.alreadyTested(obj->objectId.id, neighbor->objectId.id))
			continue;
		else
			collisionDetectionTestPairs.addPairs(obj->objectId.id, neighbor->objectId.id);
		*/

		if (collisionDetectionTestPairs.alreadyTested(obj->getInstanceId(), neighbor->getInstanceId()))
			continue;
		else
			collisionDetectionTestPairs.addPairs(obj->getInstanceId(), neighbor->getInstanceId());

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

	/*
	if (in spectator mode)
	{
		m_spectatorCamera.control();
		m_spectatorCamera.updateViewMatrix(m_pipeline);

		o_skybox.setPosition(m_spectatorCamera.getEyePoint());

		serverSimulation();
	}
	else
	{
		clientSimulation();

		m_players[m_defaultPlayerID]->updateCamera(m_pipeline);

		o_skybox.setPosition(m_players[m_defaultPlayerID]->m_camera->getEyePoint());
	}
	*/


	
	
	
	cl_players.get(m_defaultPlayerID)->updateCamera(m_pipeline);



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

		for (int i = 0; i < cl_players.getIterationEnd(); i++)
		{
			Player* p = cl_players.get(i);

			if (p == NULL)
			{
				continue;
			}
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


		for (int i = 0; i < cl_objects.getIterationEnd(); i++)
		{
			WorldObject* object = cl_objects.getByIndex(i);

			if (object == NULL)
				continue;


			/*
			if (object->getObjectType() == WEAPON)
			{
				continue;
			}
			*/
			if (object->m_name == "player mainWeapon")
			{
				int a = 1;
			}


			if (!object->shouldRender())
			{
				continue;
			}

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
			cl_objectKDtree.renderCubeFrame(m_pipeline, p_renderer);
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
			cl_objectKDtree.renderWireFrame(m_pipeline, p_renderer);

		// rendering players
		for (int i = 0; i < cl_players.getIterationEnd(); i++)
		{
			Player* p = cl_players.get(i);

			if (p == NULL)
			{
				continue;
			}

			p->alreadyFireTested = false;

			if (p->isHit)
			{
				p_renderer->setData(R_FULL_COLOR::u_color, GREEN);
				p->renderGroup(m_pipeline, p_renderer);
			}
		}


		for (int i = 0; i < cl_objects.getIterationEnd(); i++)
		{
			WorldObject* object = cl_objects.getByIndex(i);

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


	m_gui.updateAndRender(m_mouseState);

	SDL_GL_SwapBuffers();
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

/*
void FaceOff::destroyWorldObjectByIndex(vector<WorldObject*>& objects, int i)
{
	WorldObject* object = objects[i];

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

	delete objects[i];
	objects[i] = NULL;
}
*/

#define MAX_CLIENTS 10
#define SERVER_PORT 60000



void RakNetFunction()
{
	char str[512];

	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	printf("(C) or (S)erver?\n");
	gets(str);

	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else {
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0] == 0){
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		peer->Connect(str, SERVER_PORT, 0, 0);

	}

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
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
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");

				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello world");
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer){
					printf("A client has disconnected.\n");
				}
				else {
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer){
					printf("A client lost the connection.\n");
				}
				else {
					printf("Connection lost.\n");
				}
				break;

			case ID_GAME_MESSAGE_1:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());

				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello world");
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}


	RakNet::RakPeerInterface::DestroyInstance(peer);

	return;


}




void RakNetFunction2()
{
	char str[512];

	RakNet::RakPeerInterface *peer1 = RakNet::RakPeerInterface::GetInstance();
	RakNet::RakPeerInterface *peer2 = RakNet::RakPeerInterface::GetInstance();

	RakNet::Packet *packet1;
	RakNet::Packet *packet2;

	printf("(C) or (S)erver?\n");
	gets(str);



	// server
	RakNet::SocketDescriptor sd1(SERVER_PORT, 0);
	peer1->Startup(MAX_CLIENTS, &sd1, 1);

	// client
	RakNet::SocketDescriptor sd2;
	peer2->Startup(1, &sd2, 1);



	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	peer1->SetMaximumIncomingConnections(MAX_CLIENTS);


	
	printf("						Enter server IP or hit enter for 127.0.0.1\n");
	gets(str);
	if (str[0] == 0){
		strcpy(str, "127.0.0.1");
	}
	printf("						Starting the client.\n");
	peer2->Connect(str, SERVER_PORT, 0, 0);

	int seq1 = 0;
	int seq2 = 0;

	while (1)
	{
		for (packet1 = peer1->Receive(); packet1; peer1->DeallocatePacket(packet1), packet1 = peer1->Receive())
		{
//			RakNet::BitStream bsIn(packet1->data, packet1->length, false);
//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			switch (packet1->data[0])
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
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");

				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				RakNet::BitStream bsOut;
				bsOut.Write(seq1);
				seq1++;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello world");
				peer1->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet1->systemAddress, false);
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;

			case ID_GAME_MESSAGE_1:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet1->data, packet1->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());

				RakNet::BitStream bsOut;
				bsOut.Write(seq1);
				seq1++;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello world");
				peer1->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet1->systemAddress, false);
			}
			break;

			default:
				printf("Message with identifier %i has arrived.\n", packet1->data[0]);
				break;
			}
		}





		for (packet2 = peer2->Receive(); packet2; peer2->DeallocatePacket(packet2), packet2 = peer2->Receive())
		{
			switch (packet2->data[0])
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
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");

				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello world");
				peer2->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet2->systemAddress, false);
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("We have been disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
				break;

			case ID_GAME_MESSAGE_1:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet2->data, packet2->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("						%s\n", rs.C_String());

				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				bsOut.Write("Hello world");
				peer2->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet2->systemAddress, false);
			}
			break;

			default:
				printf("Message with identifier %i has arrived.\n", packet2->data[0]);
				break;
			}
		}

	}


	RakNet::RakPeerInterface::DestroyInstance(peer1);
	RakNet::RakPeerInterface::DestroyInstance(peer2);
	return;


}

int main(int argc, char *argv[])
{
//	RakNetFunction2();
//	return 0;

	utl::debug("Game Starting"); 
	utl::initSDL(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT, pDisplaySurface);
	utl::initGLEW();

	FaceOff Martin1(2);
	FaceOff Martin;

	/*
	//Added_by_Fabien_Sanglard, build a console window so we can use printf
	HWND			consoleHandle;
	//END Added_by_Fabien_Sanglard

	//Added_by_Fabien_Sanglard, build a console window so we can use printf
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	consoleHandle = GetConsoleWindow();
	//MoveWindow(consoleHandle, 0, 0, 680, 650, 1);
	printf("[sys_win.c] Console initialized.\n");
	//END Added_by_Fabien_Sanglard
	*/

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
//	if (!m_isServer)
	{
		int xOffset = 55;
		int yOffset = 570;

		int BAR_WIDTH = 60;
		int BAR_HEIGHT = 10;

		Control* HPBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GREEN, "icon_hp.png");
		cl_players.get(m_defaultPlayerID)->m_healthBarGUI = (Bar*)HPBar;

		xOffset = 175;
		Control* armorBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GRAY, "icon_armor.png");
		cl_players.get(m_defaultPlayerID)->m_armorBarGUI = (Bar*)armorBar;

		xOffset = 700;
		Control* ammoBar = new Bar(xOffset, yOffset, BAR_WIDTH, BAR_HEIGHT, GRAY, "icon_ammo.png");
		cl_players.get(m_defaultPlayerID)->m_ammoBarGUI = (Bar*)ammoBar;

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
void FaceOff::destroyWorldObject(WorldObject* object)
{
	//remove object from its stored kdTree nodes
	for (int j = 0; j < object->m_parentNodes.size(); j++)
	{
		KDTreeNode* kNode = object->m_parentNodes[j];
		if (kNode == NULL)
			continue;
		kNode->remove(object);
	}
}
*/

/*
void FaceOff::removeObject(WorldObject* object)
{

}
*/

/*
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
*/

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



/*
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
							peer->Send(&bsOut, IMMEDIATE_PRIORITY, UNRELIABLE, 0, peer->GetSystemAddressFromGuid(m_players[i]->m_guid), false);
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
							peer->Send(&bsOut, IMMEDIATE_PRIORITY, UNRELIABLE, 0, packet->systemAddress, false);
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





#if 0
if (sv_players.getIterationEnd() > 0)
{
	// send existing clients about the new client notification 
	std::cout << "Signaling arrival of new clients, Sending new client's spaw position to each client" << endl;
	p->serialize((RakNet::MessageID)NEW_CLIENT, bsOut);

	bsOut.Write(p->weaponCount);

	for (int i = 0; i < p->getWeapons().size(); i++)
	{
		Weapon* weapon = p->getWeapons()[i];
		if (weapon != NULL)
		{
			weapon->serialize(bsOut);
		}
	}

	for (int i = 0; i < m_server.clients.size(); i++)
	{
		if (i == newPlayerId)
			continue;

		Client client = m_server.getClient(i);
		std::cout << " To: " << i << " - " << client.m_guid.g << endl;
		// peer->Send(&bsOut, IMMEDIATE_PRIORITY, UNRELIABLE, 0, peer->GetSystemAddressFromGuid(m_players[i]->m_guid), false);
		m_server.peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, client.systemAddress, false);
	}



	// Sending each client's position to new client
	utl::debug("m_players size", sv_players.count);
	std::cout << "Sending each client's position to new client" << endl;

	for (int i = 0; i < m_server.clients.size(); i++)
	{
		if (i == newPlayerId)
			continue;

		std::cout << "sending for " << i << endl;
		p->serialize((RakNet::MessageID)NEW_CLIENT, bsOut);
		m_server.peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, packet->systemAddress, false);
	}
}
else
{
	std::cout << "No clients yet, didn't send spawn pos to existing " << "nor each existing pos to new " << endl;
}



// Use a BitStream to write a custom user message
// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically

// write a WELCOME message and include the clients index + 1
// p->serializeSpawnInfo((RakNet::MessageID)SPAWN_INFORMATION, bsOut);



bsOut.Reset();
p->serialize((RakNet::MessageID)SPAWN_INFORMATION, bsOut);

// send the message back to the same address the current packet came from (the new client)
m_server.peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

/*
cout << "Player List" << endl;
for (int i = 0; i < m_players.size(); i++)
{
cout << i << " - " << m_players[i]->getId() << " position " << m_players[i]->m_position.x << " "
<< m_players[i]->m_position.y << " "
<< m_players[i]->m_position.z << endl;
}
*/
#endif




#if 0
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
				//	m_players[m_defaultPlayerID]->switchWeapon(WeaponSlotEnum::MAIN);
				break;

				// pistol
			case SDLK_2:
				//	m_players[m_defaultPlayerID]->switchWeapon(WeaponSlotEnum::PISTOL);
				break;

				// MELEE
			case SDLK_3:
				//	m_players[m_defaultPlayerID]->switchWeapon(WeaponSlotEnum::MELEE);
				break;

				// GRENADES
			case SDLK_4:
				//	m_players[m_defaultPlayerID]->switchWeapon(WeaponSlotEnum::PROJECTILE);
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
				//	m_players[m_defaultPlayerID]->reloadWeapon();
			}
			break;

			case SDLK_g:
			{
				utl::debug("Dropping Weapon");
				/*
				Weapon* droppedWeapon = m_players[m_defaultPlayerID]->drop();
				if (droppedWeapon != NULL)
				{
				m_objectKDtree.insert(droppedWeapon);
				}
				*/
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
				/*
				if (m_server.isInitalized)
				m_spectatorCamera.setMouseIn(false);
				else
				m_players[m_defaultPlayerID]->m_camera->setMouseIn(false);
				*/
				break;
			}
			break;
		}
	}

}
#endif



/*
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
	}

	
	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];
		simulateObjectPhysics(object, i);
	}
}
*/


/*
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
*/

#if 0
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
	sv_objectKDtree.reInsert(p);

	p->updateWeaponTransform();

}
#endif

#endif