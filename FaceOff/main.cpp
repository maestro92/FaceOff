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

// the server simluates the game in descirete time steps called ticks

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
const int CLIENT_INPUT_SENT_PER_SECOND = 30;
const int CLIENT_INPUT_SENT_TIME_STEP = 1000 / SERVER_SNAPSHOT_PER_SECOND;


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


	initModels();
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

void FaceOff::initModels()
{

	m_mm.init();


	float scale = 1.0;

	vector<string> textures;

	m_xyzModel = XYZAxisModel();



	tempTexture = utl::loadTexture("Assets/Images/chess.png");

	// m_groundModel.load("Assets/models/quad.obj");
	//	m_groundModel.load("Assets/models/ground.obj");
	// textures.clear();  textures.push_back("Assets/Images/chess.png"); //textures.push_back("Assets/tree.png"); // textures.push_back("Assets/Images/chess.png");
	// m_groundModel.setTextures(textures);
	// m_groundModel.setMeshRandTextureIdx();


	m_bulletModel.load("./Assets/models/cylinder_base.obj");




	textures.clear();  textures.push_back("Assets/lowPolyTree.png");
	m_lowPolyTree.load("Assets/lowPolyTree.obj", textures);


	textures.clear(); textures.push_back("Assets/models/basic stair/texture.png");
	m_stairs.load("Assets/models/basic stair/stairs.obj", textures);
	m_stairs.setMeshRandTextureIdx();

	textures.clear(); textures.push_back("Assets/models/wooden box/WoodPlanks_Color.jpg");
	m_woodenBox.load("Assets/models/wooden box/WoodenBoxOpen02.obj", textures);
	m_woodenBox.setMeshRandTextureIdx();


	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->setModel(m_mm.m_player);
	}
}



void FaceOff::initObjects()
{
	if (m_isServer)
	{
		m_serverCamera = FirstPersonCamera();
		m_serverCamera.setEyePoint(glm::vec3(-59.362, 94.037, 153.189));
		m_serverCamera.setFreeMode(true);
	}

	float scale = 100.0;
	o_worldAxis.setScale(scale);
	o_worldAxis.setModel(m_mm.m_xyzAxis);



	o_gun.setPosition(200, 0, 200);
	o_gun.setRotation(glm::rotate(90.0f, 0.0f, 1.0f, 0.0f));
	o_gun.setScale(0.5);


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
	o_temp->setModel(m_mm.m_ground);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	o_temp->m_name = "ground";
	m_objects.push_back(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "south wall";
	o_temp->setScale(xbound, ybound / 2, wallWidth);
	o_temp->setPosition(0, ybound / 2, zbound + wallWidth);
	o_temp->setModel(m_mm.m_cube);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "north wall";
	o_temp->setPosition(0, ybound / 2, -zbound - wallWidth);
	o_temp->setScale(xbound, ybound / 2, wallWidth);
	o_temp->setRotation(glm::rotate(180.0f, 0.0f, 1.0f, 0.0f));
	o_temp->setModel(m_mm.m_cube);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "west wall";
	o_temp->setPosition(-xbound - wallWidth, ybound / 2, 0.0);
	o_temp->setScale(wallWidth, ybound / 2, zbound);
	o_temp->setModel(m_mm.m_cube);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "east wall";
	o_temp->setPosition(xbound + wallWidth, ybound / 2, 0.0);
	o_temp->setScale(wallWidth, ybound / 2, zbound);
	o_temp->setModel(m_mm.m_cube);
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
		o_temp->setModel(m_mm.m_woodenBox);
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
		o_temp->setModel(m_mm.m_woodenBox);
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
	o_temp->setModel(m_mm.m_woodenBox);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "NE Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(halfPosXMag, pillarYScale / 2, -halfPosZMag);
	o_temp->setModel(m_mm.m_woodenBox);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);
	utl::debug("id is", o_temp->m_instanceId);


	o_temp = new WorldObject();
	o_temp->m_name = "SW Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(-halfPosXMag, pillarYScale / 2, halfPosZMag);
	o_temp->setModel(m_mm.m_woodenBox);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);
	utl::debug("id is", o_temp->m_instanceId);


	o_temp = new WorldObject();
	o_temp->m_name = "SE Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(halfPosXMag, pillarYScale / 2, halfPosZMag);
	o_temp->setModel(m_mm.m_woodenBox);
	o_temp->setCollisionDetectionGeometry(CD_AABB);

	m_objects.push_back(o_temp);

	utl::debug("id is", o_temp->m_instanceId);

	float formationGap = 40.0f;
	



	o_temp = new Weapon(m_mm.getWeaponData(MAC_11));
	o_temp->m_name = "MAC_11";
	o_temp->setPosition(-3 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_mm.getWeaponData(AWM));
	o_temp->m_name = "AWM";
	o_temp->setPosition(-2 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);



	o_temp = new Weapon(m_mm.getWeaponData(MINIGUN));
	o_temp->m_name = "MINIGUN";
	o_temp->setPosition(-1 * formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);



	o_temp = new Weapon(m_mm.getWeaponData(KNIFE));
	o_temp->m_name = "knife";
	o_temp->setPosition(formationGap, 5, -110);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	/*
	o_temp = new Weapon(m_mm.getWeaponData(FRAG_GRENADE));
	o_temp->m_name = "FRAG_GRENADE";
	o_temp->setAABBByPosition(2 * formationGap, 5, -110);
	m_objects.push_back(o_temp);
	*/




	o_temp = new Weapon(m_mm.getWeaponData(MP5));
	o_temp->m_name = "MP5";
	o_temp->setPosition(-3 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_mm.getWeaponData(MG42));
	o_temp->m_name = "MG42";
	o_temp->setPosition(-2 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	// init weapons for the map
	o_temp = new Weapon(m_mm.getWeaponData(AK_47));
	o_temp->m_name = "AK 47";
	o_temp->setPosition(-formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_mm.getWeaponData(M16));
	o_temp->m_name = "M16";
	o_temp->setPosition(formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_mm.getWeaponData(KATANA));
	o_temp->m_name = "katana";
	o_temp->setPosition(2 * formationGap, 5, -140);
	o_temp->setCollisionDetectionGeometry(CD_AABB);
	m_objects.push_back(o_temp);



	o_temp = new Weapon(m_mm.getWeaponData(PISTOL_SHOTGUN));
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

	Weapon* mainWeapon = new Weapon(m_mm.getWeaponData(M16));
	mainWeapon->setCollisionDetectionGeometry(CD_AABB);
	mainWeapon->m_name = "player mainWeapon";

	//	Weapon* pistol = new Weapon(m_mm.getWeaponData());
	Weapon* knife = new Weapon(m_mm.getWeaponData(KNIFE));
	knife->setCollisionDetectionGeometry(CD_AABB);
	knife->m_name = "player knife";

	Weapon* grenade = new Weapon(m_mm.getWeaponData(FRAG_GRENADE));
	grenade->setCollisionDetectionGeometry(CD_AABB);
	grenade->m_name = "player grenade";


	m_defaultPlayerID = 0;
	Player* p = new Player(m_defaultPlayerID);
	p->m_name = "player 0";
	p->setModel(m_mm.m_player);
	p->setCollisionDetectionGeometry(CD_SPHERE);
	m_players.push_back(p);

	m_players[0]->pickUpWeapon(mainWeapon);
	m_players[0]->pickUpWeapon(knife);
	m_players[0]->pickUpWeapon(grenade);

	p = new Player(1);
	p->m_name = "player 1";
	p->setPosition(0, 5, 25);
	p->setModel(m_mm.m_player);
	p->setCollisionDetectionGeometry(CD_SPHERE);
	m_players.push_back(p);

	p = new Player(2);
	p->m_name = "player 2";
	p->setPosition(0, 5, -40);
	p->setModel(m_mm.m_player);
	p->setCollisionDetectionGeometry(CD_SPHERE);
	m_players.push_back(p);


	m_objects.push_back(mainWeapon);
	m_objects.push_back(knife);
	m_objects.push_back(grenade);

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
	smEffect->setPosition(glm::vec3(15.0, 5.0, -13.0));
	smEffect->setScale(5.0);

	smEffect->init();
	m_smokeEffects.push_back(smEffect);
	
	m_objectKDtree.print();
}





void FaceOff::initRenderers()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_rm.init(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
	m_rm.initSceneRendererStaticLightsData(m_lightManager);
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
					m_players.push_back(new Player(newPlayerId));
					m_players[newPlayerId]->m_guid = packet->guid;
					m_players[newPlayerId]->setPosition(newSpawnX, newSpawnY, newSpawnZ);

					
					Weapon* mainWeapon = new Weapon(m_mm.getWeaponData(M16));
					mainWeapon->m_name = "player mainWeapon";

					Weapon* knife = new Weapon(m_mm.getWeaponData(KNIFE));
					knife->m_name = "player knife";

					Weapon* grenade = new Weapon(m_mm.getWeaponData(FRAG_GRENADE));
					grenade->m_name = "player grenade";

					m_players[newPlayerId]->pickUpWeapon(mainWeapon);
					m_players[newPlayerId]->pickUpWeapon(knife);
					m_players[newPlayerId]->pickUpWeapon(grenade);

					m_objects.push_back(mainWeapon);
					m_objects.push_back(knife);
					m_objects.push_back(grenade);


					utl::debug("new_player_id", newPlayerId);
					utl::debug("player position", m_players[newPlayerId]->getPosition());

					if (m_players.size() > 0)
					{
						// send new client notification to existing clients
						cout << "Signaling arrival of new clients, Sending new client's spaw position to each client" << endl;
						m_players[newPlayerId]->toBitStream((RakNet::MessageID)NEW_CLIENT, bsOut);

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
							m_players[i]->toBitStream((RakNet::MessageID)NEW_CLIENT, bsOut);
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
					m_players[newPlayerId]->toBitStream((RakNet::MessageID)SPAWN_INFORMATION, bsOut);

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
		int int_message;
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
						Player* newPlayer = new Player();
						newPlayer->setFromBitStream(bsIn);

						m_defaultPlayerID = newPlayer->getId();

						if (m_defaultPlayerID + 1 >= m_players.size())
						{
							m_players.resize(m_defaultPlayerID + 1);
						}

						m_players[m_defaultPlayerID] = newPlayer;
						


						for (int i = 0; i < NUM_WEAPON_SLOTS; i++)
						{
							int weaponEnum = 0;
							bsIn.Read(weaponEnum);
							if (weaponEnum != -1)
							{
								Weapon* weapon = new Weapon(m_mm.getWeaponData((WeaponNameEnum)weaponEnum));
								newPlayer->pickUpWeapon(weapon);
								m_objects.push_back(weapon);

								utl::debug("weaponEnum is ", weaponEnum);
							}
							else
							{
								utl::debug("weaponEnum is None");
							}
						}
						
						
						server_address = packet->systemAddress;

						printf("Server said I'm client number %d at %f, %f, %f\n", newPlayer->getId(), newPlayer->getPosition().x, newPlayer->getPosition().y, newPlayer->getPosition().z);
						break;
					}


					case NEW_CLIENT:
					{
						Player* newPlayer = new Player();
						newPlayer->setFromBitStream(bsIn);

						int id = newPlayer->getId();

						if (id + 1 >= m_players.size())
						{
							m_players.resize(id + 1);
						}

						m_players[id] = newPlayer;


						printf("Received new client info for %d: %f, %f, %f", id, newPlayer->getPosition().x, newPlayer->getPosition().y, newPlayer->getPosition().z);

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
				cout << "sending server my inputs" << endl;
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
							hitPointMark->setModel(m_mm.m_cube);
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
				Weapon* droppedWeapon = m_players[m_defaultPlayerID]->dropWeapon();
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



void FaceOff::serverUpdate()
{
	// process client inputs
	
	if (m_inputQueue.size() > 0)
	{
		utl::debug("inputQueue size is", m_inputQueue.size());
	}

	
	for (int i = 0; i < m_inputQueue.size(); i++)
	{
		Move move = m_inputQueue.front();
		m_inputQueue.pop();
		move.print();

		int playerId = move.playerId;
		
		m_players[playerId]->processInput(move);
	}	
}


void FaceOff::clientUpdate()
{

}


void FaceOff::serverSimulation()
{
#if 0
	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];

		if (object == NULL)
			continue;

		object->updateGameInfo();


		if (object->getObjectType() == WEAPON)
		{
			Weapon* wObject = (Weapon*)object;
			if (wObject->getWeaponSlot() == PROJECTILE && wObject->shouldExplode())
			{
				utl::debug("Exploding");

				ParticleEffect* effect = wObject->explode();
				m_smokeEffects.push_back((SmokeEffect*)effect);

				delete wObject;
				wObject = NULL;
				m_objects[i] = NULL;
			}
		}

	}


	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];

		if (object == NULL)
			continue;

		if (object->getDynamicType() == STATIC)
		{
			continue;
		}

		if (object->getObjectType() == WEAPON)
		{
			Weapon* wObject = (Weapon*)object;

			if (wObject->hasOwner == true || wObject->getWeaponSlot() != PROJECTILE)
			{
				continue;
			}
		}


		object->m_velocity += glm::vec3(0.0f, -9.81f, 0.0f) * 0.005f * 0.5f;
		object->m_position += object->m_velocity;
		// object->m_velocity += utl::BIASED_HALF_GRAVITY;
		// object->m_position += object->m_velocity;
		object->updateAABB();


		vector<WorldObject*> neighbors;
		glm::vec3 volNearPoint(object->m_position);
		m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, object, volNearPoint, neighbors);

		unordered_set<int> objectsAlreadyTested;

		for (int i = 0; i < neighbors.size(); i++)
		{
			ContactData contactData;
			WorldObject* neighbor = neighbors[i];

			if (neighbor->getObjectType() == PLAYER)
			{
				continue;
			}


			if (CollisionDetection::testAABBAABB(object->m_aabb,
				neighbor->m_aabb,
				contactData))
			{

				// ground was getting inserted twice. We dont want that!
				if (objectsAlreadyTested.find(neighbor->m_instanceId) != objectsAlreadyTested.end())
					continue;
				else
					objectsAlreadyTested.insert(neighbor->m_instanceId);


				if (neighbor->getObjectType() == WEAPON)
				{
					continue;
				}

				contactData.pair[0] = object;
				contactData.pair[1] = NULL;

				contactData.resolveVelocity1();
				contactData.resolveInterpenetration();
			}
		}

		object->updateAABB();

		utl::debug("object name", object->m_name);
		utl::debug("object parent size", object->m_parentNodes.size());

		for (int j = 0; j < object->m_parentNodes.size(); j++)
		{
			KDTreeNode* kNode = object->m_parentNodes[j];
			if (kNode == NULL)
				continue;
			kNode->removeObject(object);
		}



		// removing, we pop empty our queue, and set everything in vector to NULL
		while (!object->m_emptyIndexPool.empty())
		{
			object->m_emptyIndexPool.pop();
		}
		for (int j = 0; j < object->m_parentNodes.size(); j++)
		{
			object->m_parentNodes[j] = NULL;
			object->m_emptyIndexPool.push(j);
		}

		m_objectKDtree.insert(object);
	}


	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->updateGameInfo();
	}
#endif
}

void FaceOff::clientSimulation()
{

}




void FaceOff::updateObjectPhysics(WorldObject* object)
{

}


void FaceOff::render()
{
	m_pipeline.setMatrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();

	
	if (m_isServer)
	{
		m_serverCamera.control();
		m_serverCamera.updateViewMatrix(m_pipeline);
		o_skybox.setPosition(m_serverCamera.getEyePoint());
		serverUpdate();

	//	serverSimulation();
	}
	else
	{
		m_players[m_defaultPlayerID]->m_velocity += utl::BIASED_HALF_GRAVITY;
		m_players[m_defaultPlayerID]->m_camera->m_target += m_players[m_defaultPlayerID]->m_velocity;

		Uint8* state = SDL_GetKeyState(NULL);

		if (state[SDLK_SPACE])
		{
			if (m_players[m_defaultPlayerID]->isNotJumping())
				m_players[m_defaultPlayerID]->m_velocity += glm::vec3(0.0, 175.0, 0.0) * utl::GRAVITY_CONSTANT;
		}

		m_players[m_defaultPlayerID]->control();

#if NETWORK_FLAG == 1
		if (m_players[m_defaultPlayerID]->hasMoved())
		{
			m_inputQueue.push(m_players[m_defaultPlayerID]->getMoveState());
		}
#endif		

		m_players[m_defaultPlayerID]->updateCollisionDetectionGeometry();


#if 1
		vector<WorldObject*> neighbors;
		glm::vec3 volNearPoint(m_players[m_defaultPlayerID]->m_position);
		m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, m_players[m_defaultPlayerID], volNearPoint, neighbors);


		// collision between static object and dynamic object
		// Game Physics Engine Development P.109 - 119
		unordered_set<int> objectsAlreadyTested;

		for (int i = 0; i < neighbors.size(); i++)
		{
			neighbors[i]->isTested = true;

			ContactData contactData;

			bool collideFlag = testCollisionDetectionPlayerVersion(m_players[m_defaultPlayerID], neighbors[i], contactData);
			/*
			Sphere cd0 = *(m_players[m_defaultPlayerID]->m_sphere);
			AABB cd1 = *(neighbors[i]->m_aabb);
			utl::debug("i", i);
			if (neighbors[i] == NULL)
			{
				utl::debug("neighbor is null");
			}
			

			if (m_players[m_defaultPlayerID]->getGeometryType() == CD_AABB)
			{
				collideFlag = CollisionDetection::testAABBAABB(*(m_players[m_defaultPlayerID]->m_aabb), *(neighbors[i]->m_aabb), contactData);
			}
			else if (m_players[m_defaultPlayerID]->getGeometryType() == CD_SPHERE)
			{
				if (neighbors[i]->getDynamicType() == STATIC && neighbors[i]->getGeometryType() == CD_AABB)
				{
					collideFlag = CollisionDetection::testSphereAABBPlayerVersion(*(m_players[m_defaultPlayerID]->m_sphere), *(neighbors[i]->m_aabb), contactData);
				}
				else if (neighbors[i]->getGeometryType() == CD_AABB)
				{
					collideFlag = CollisionDetection::testSphereAABB(*(m_players[m_defaultPlayerID]->m_sphere), *(neighbors[i]->m_aabb), contactData);
				}
				else if (neighbors[i]->getGeometryType() == CD_SPHERE)
				{
					collideFlag = CollisionDetection::testSphereSphere(*(m_players[m_defaultPlayerID]->m_sphere), *(neighbors[i]->m_sphere), contactData);
				}
			}
			*/

			if (collideFlag)
			{
				neighbors[i]->isCollided = true;

				// ground was getting inserted twice. We dont want that!
				if (objectsAlreadyTested.find(neighbors[i]->m_instanceId) != objectsAlreadyTested.end())
					continue;
				else
					objectsAlreadyTested.insert(neighbors[i]->m_instanceId);

				contactData.pair[0] = m_players[m_defaultPlayerID];
				contactData.pair[1] = NULL;

				contactData.restitution = 0.0;

				contactData.resolveVelocity();
				contactData.resolveInterpenetration();
			}
		}
#endif

		m_players[m_defaultPlayerID]->updateGameStats();



#if NETWORK_FLAG == 0 
		int tempId = 1;
		float tempDist = 100;

		if (incrFlag)
			m_players[tempId]->m_position.z += 0.05;
		else
			m_players[tempId]->m_position.z -= 0.05;


		if (m_players[tempId]->m_position.z > tempDist)
			incrFlag = false;
		if (m_players[tempId]->m_position.z < 20)
			incrFlag = true;

		m_players[tempId]->setPosition(m_players[tempId]->m_position);
		m_players[tempId]->updateCollisionDetectionGeometry();


		tempId = 2;
		tempDist = 15;

		if (incrFlag2)
			m_players[tempId]->m_position.x += 0.05;
		else
			m_players[tempId]->m_position.x -= 0.05;


		if (m_players[tempId]->m_position.x > tempDist)
			incrFlag2 = false;
		if (m_players[tempId]->m_position.x < -tempDist)
			incrFlag2 = true;

		m_players[tempId]->setPosition(m_players[tempId]->m_position);
		m_players[tempId]->updateCollisionDetectionGeometry();
#endif


		for (int i = 0; i < m_players.size(); i++)
		{
			m_players[i]->updateGameInfo();
		}



		// update each player's place in the kdTree
		for (int i = 0; i < m_players.size(); i++)
		{
			Player* p = m_players[i];


			// first: remove the player from player's kdtree parent nodes
			for (int j = 0; j < p->m_parentNodes.size(); j++)
			{
				KDTreeNode* kNode = p->m_parentNodes[j];

				if (kNode == NULL)
					continue;

				kNode->removeObject(p);
			}


			// then: we clear player's stored parentNodes
			while (!p->m_emptyIndexPool.empty())
			{
				p->m_emptyIndexPool.pop();
			}

			for (int j = 0; j < p->m_parentNodes.size(); j++)
			{
				p->m_parentNodes[j] = NULL;
				p->m_emptyIndexPool.push(j);
			}

			// last: we re-insert the player back into the kdtree
			m_objectKDtree.insert(p);
		}


		// collision detection
		m_players[m_defaultPlayerID]->updateCamera(m_pipeline);

		o_skybox.setPosition(m_players[m_defaultPlayerID]->m_camera->getEyePoint());
	}





	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];

		if (object == NULL)
			continue;

		object->updateGameInfo();


		if (object->getObjectType() == WEAPON)
		{
			Weapon* wObject = (Weapon*)object;
			if (wObject->getWeaponSlot() == PROJECTILE && wObject->shouldExplode())
			{
				utl::debug("Exploding");

				ParticleEffect* effect = wObject->explode(); 
				m_smokeEffects.push_back((SmokeEffect*)effect);

				delete wObject;
				wObject = NULL;
				m_objects[i] = NULL;
			}
		}

	}




#if 1
	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];

		if (object == NULL)
			continue;

		if (object->getDynamicType() == STATIC)
		{
			continue;
		}

		if (object->getObjectType() == WEAPON)
		{
			Weapon* wObject = (Weapon*)object;

			if (wObject->hasOwner == true || wObject->getWeaponSlot() != PROJECTILE)
			{
				continue;
			}
		}


		object->m_velocity += glm::vec3(0.0f, -9.81f, 0.0f) * 0.005f * 0.5f;
		object->m_position += object->m_velocity;
		// object->m_velocity += utl::BIASED_HALF_GRAVITY;
		// object->m_position += object->m_velocity;
		object->setPosition(object->m_position);
		object->updateCollisionDetectionGeometry();

		vector<WorldObject*> neighbors;
		glm::vec3 volNearPoint(object->m_position);
		m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, object, volNearPoint, neighbors);

		unordered_set<int> objectsAlreadyTested;

		for (int i = 0; i < neighbors.size(); i++)
		{
			ContactData contactData;
			WorldObject* neighbor = neighbors[i];

			if (neighbor->getObjectType() == PLAYER)
			{
				continue;
			}


			/*
			if (CollisionDetection::testAABBAABB(object->m_aabb,
				neighbor->m_aabb,
				contactData))
			*/
			if (testCollisionDetection(object, neighbor, contactData))
			{
				// ground was getting inserted twice. We dont want that!
				if (objectsAlreadyTested.find(neighbor->m_instanceId) != objectsAlreadyTested.end())
					continue;
				else
					objectsAlreadyTested.insert(neighbor->m_instanceId);


				if (neighbor->getObjectType() == WEAPON)
				{
					continue;
				}

				contactData.pair[0] = object;
				contactData.pair[1] = NULL;

				contactData.resolveVelocity1();
				contactData.resolveInterpenetration();
			}
		}

//		object->updateAABB();

		utl::debug("object name", object->m_name);
		utl::debug("object parent size", object->m_parentNodes.size());

		for (int j = 0; j < object->m_parentNodes.size(); j++)
		{
			KDTreeNode* kNode = object->m_parentNodes[j];
			if (kNode == NULL)
				continue;
			kNode->removeObject(object);
		}


		
		// removing, we pop empty our queue, and set everything in vector to NULL
		while (!object->m_emptyIndexPool.empty())
		{
			object->m_emptyIndexPool.pop();
		}
		for (int j = 0; j < object->m_parentNodes.size(); j++)
		{
			object->m_parentNodes[j] = NULL;
			object->m_emptyIndexPool.push(j);
		}

		m_objectKDtree.insert(object);
	}

#endif













	// *******************************************************
	// ************* Rendering *******************************
	// *******************************************************
	m_pipeline.setMatrixMode(MODEL_MATRIX);

	glBindFramebuffer(GL_FRAMEBUFFER, m_rm.m_backGroundLayerFBO.FBO);
	//	Model::enableVertexAttribArrays();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// the render function disables depth test Cull face already and enables it afterwards
	o_skybox.render(m_pipeline, &m_rm.r_skybox);
	glClear(GL_DEPTH_BUFFER_BIT);

#if RENDER_DEBUG_FLAG
	p_renderer = &m_rm.r_fullTexture;
	p_renderer->enableShader();
	p_renderer->setData((int)R_FULL_TEXTURE::u_texture, 0, GL_TEXTURE_2D, tempTexture);

	{
		// render the players
		if (m_isServer)
		{
			for (int i = 0; i < m_players.size(); i++)
			{
				if (i != m_defaultPlayerID && m_players[i] != NULL)
				{
					cout << "Player " << i << " at position " << m_players[i]->m_position.x << " " << m_players[i]->m_position.y << " " << m_players[i]->m_position.z << endl;
					//				m_players[i]->render(m_pipeline);
				}
			}
		}
		else
		{
			m_players[m_defaultPlayerID]->renderGroup(m_pipeline, p_renderer);
		}




		for (int i = 0; i < m_objects.size(); i++)
		{
			WorldObject* object = m_objects[i];

			if (object == NULL)
				continue;


			if (object->getObjectType() == WEAPON)
			{
				if (((Weapon*)object)->hasOwner == true)
					continue;
				else
					object->renderGroup(m_pipeline, p_renderer);
			}

			if (object->isTested != true && object->isCollided != true && object->isHit != true)
			{
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
	p_renderer = &m_rm.r_fullVertexColor;
	p_renderer->enableShader();

	o_worldAxis.renderGroup(m_pipeline, p_renderer);


	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];

		if (object == NULL)
			continue;


		if (object->getObjectType() == WEAPON)
		{
			if (((Weapon*)object)->hasOwner == true)
				continue;
		}

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



	p_renderer = &m_rm.r_fullColor;
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

			if (object->getObjectType() == WEAPON)
			{
				continue;
			}

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
	p_renderer = &m_rm.r_fullTexture;
	p_renderer->enableShader();
	p_renderer->setData((int)R_FULL_TEXTURE::u_texture, 0, GL_TEXTURE_2D, tempTexture);

	{
		for (int i = 0; i < m_objects.size(); i++)
		{
			WorldObject* object = m_objects[i];

			if (object == NULL)
				continue;

			if (object->getObjectType() == WEAPON)
			{
				if (((Weapon*)object)->hasOwner == true)
					continue;
			}


			if (object->isHit != true)
			{
				object->renderGroup(m_pipeline, p_renderer);
			}

		}



		for (int i = 0; i < m_players.size(); i++)
		{
			Player* player = m_players[i];
			if (i == m_defaultPlayerID)
			{
				if (m_players[i]->getCurWeapon() != NULL)
				{
					m_players[i]->getCurWeapon()->renderGroup(m_pipeline, p_renderer);
				}
			}
			else 
			{
				if (player->isHit != true)
				{
					m_players[i]->renderGroup(m_pipeline, p_renderer);
				}
			}
			
		}


	}
	p_renderer->disableShader();


	// Rendering wireframes
	p_renderer = &m_rm.r_fullVertexColor;
	p_renderer->enableShader();

	o_worldAxis.renderGroup(m_pipeline, p_renderer);


	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];

		if (object == NULL)
			continue;


		if (object->getObjectType() == WEAPON)
		{
			if (((Weapon*)object)->hasOwner == true)
				continue;
		}

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





	p_renderer = &m_rm.r_fullColor;
	p_renderer->enableShader();
	p_renderer->setData(R_FULL_COLOR::u_color, GREEN);

	if (!containedFlag)
		m_objectKDtree.renderWireFrame(m_pipeline, p_renderer);

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
	p_renderer = &m_rm.r_fireWorkEffectUpdate;
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


	p_renderer = &m_rm.r_fireWorkEffectRender;
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
		m_gui.renderTextureFullScreen(m_rm.m_backGroundLayerFBO.colorTexture);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	

	// smoke effects

	
	if (m_smokeEffects.size() > 0)
	{
		p_renderer = &m_rm.r_smokeEffectUpdate;
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


		p_renderer = &m_rm.r_smokeEffectRender;
		p_renderer->enableShader();
		for (int i = 0; i < m_smokeEffects.size(); i++)
		{
			SmokeEffect* effect = m_smokeEffects[i];

			p_renderer->setData(R_SMOKE_EFFECT_RENDER::u_angle, effect->m_particleRotation);
			p_renderer->setData(R_SMOKE_EFFECT_RENDER::u_texture, 0, GL_TEXTURE_2D, effect->m_textureId);
			p_renderer->setData(R_SMOKE_EFFECT_RENDER::u_depthTexture, 1, GL_TEXTURE_2D, m_rm.m_backGroundLayerFBO.depthTexture);
			effect->render(m_pipeline, p_renderer);
		}
		p_renderer->disableShader();
	}
	
	glDisable(GL_BLEND);

	if (m_zoomedIn)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RENDER_TO_SCREEN);
		m_gui.renderSnipeScopeView(m_rm.m_backGroundLayerFBO.colorTexture);
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
		else if (a->getGeometryType() == CD_SPHERE)
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
		else if (a->getGeometryType() == CD_SPHERE)
		{
			return CollisionDetection::testSphereSphere(*(a->m_sphere), *(b->m_sphere), contactData);
		}
	}

	return false;
}


bool FaceOff::testCollisionDetectionPlayerVersion(WorldObject* a, WorldObject* b, ContactData& contactData)
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
		if (b->getDynamicType() == STATIC && b->getGeometryType() == CD_AABB)
		{
			return CollisionDetection::testSphereAABBPlayerVersion(*(a->m_sphere), *(b->m_aabb), contactData);
		}
		else if (b->getGeometryType() == CD_AABB)
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









#if 0
void FaceOff::initNetworkThread()
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
				break;
			}
		}

	}
#endif
}
#endif





#if 0

m_pipeline.setMatrixMode(VIEW_MATRIX);
m_pipeline.loadIdentity();


if (m_isServer)
{
	m_serverCamera.control();
	m_serverCamera.updateViewMatrix(m_pipeline);
	o_skybox.setPosition(m_serverCamera.getEyePoint());
	serverUpdate();

	//	serverSimulation();
}
else
{
	m_players[m_defaultPlayerID]->m_velocity += utl::BIASED_HALF_GRAVITY;
	m_players[m_defaultPlayerID]->m_camera->m_target += m_players[m_defaultPlayerID]->m_velocity;


	Uint8* state = SDL_GetKeyState(NULL);

	if (state[SDLK_SPACE])
	{
		if (m_players[m_defaultPlayerID]->isNotJumping())
			m_players[m_defaultPlayerID]->m_velocity += glm::vec3(0.0, 175.0, 0.0) * utl::GRAVITY_CONSTANT;
	}

	m_players[m_defaultPlayerID]->control();

	if (m_players[m_defaultPlayerID]->hasMoved())
	{
		m_inputQueue.push(m_players[m_defaultPlayerID]->getMoveState());
	}

	// m_playerInputQueue

	vector<WorldObject*> neighbors;
	glm::vec3 volNearPoint(m_players[m_defaultPlayerID]->m_position);
	m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, m_players[m_defaultPlayerID], volNearPoint, neighbors);

	m_players[m_defaultPlayerID]->m_boundingSphere.center = m_players[m_defaultPlayerID]->m_position;
	// collision between static object and dynamic object
	// Game Physics Engine Development P.109 - 119

	unordered_set<int> objectsAlreadyTested;

	for (int i = 0; i < neighbors.size(); i++)
	{
		neighbors[i]->isTested = true;

		ContactData contactData;

		bool collideFlag = false;

		if (neighbors[i]->getDynamicType() == STATIC && neighbors[i]->getGeometryType() == GM_AABB)
		{
			collideFlag = CollisionDetection::testSphereAABBHackVersion(m_players[m_defaultPlayerID]->m_boundingSphere,
				neighbors[i]->m_aabb,
				contactData);
		}
		else
		{
			collideFlag = CollisionDetection::testSphereAABB(m_players[m_defaultPlayerID]->m_boundingSphere,
				neighbors[i]->m_aabb,
				contactData);
		}


		if (collideFlag)
		{
			neighbors[i]->isCollided = true;

			// ground was getting inserted twice. We dont want that!
			if (objectsAlreadyTested.find(neighbors[i]->m_instanceId) != objectsAlreadyTested.end())
				continue;
			else
				objectsAlreadyTested.insert(neighbors[i]->m_instanceId);

			contactData.pair[0] = m_players[m_defaultPlayerID];
			contactData.pair[1] = NULL;

			contactData.restitution = 0.0;

			contactData.resolveVelocity();
			contactData.resolveInterpenetration();
		}
	}


	m_players[m_defaultPlayerID]->updateGameStats();



#if NETWORK_FLAG == 0 
	int tempId = 1;
	float tempDist = 100;

	if (incrFlag)
		m_players[tempId]->m_position.z += 0.05;
	else
		m_players[tempId]->m_position.z -= 0.05;


	if (m_players[tempId]->m_position.z > tempDist)
		incrFlag = false;
	if (m_players[tempId]->m_position.z < 20)
		incrFlag = true;

	m_players[tempId]->updateAABB();



	int tempId2 = 2;
	float tempDist2 = 15;

	if (incrFlag2)
		m_players[tempId2]->m_position.x += 0.05;
	else
		m_players[tempId2]->m_position.x -= 0.05;


	if (m_players[tempId2]->m_position.x > tempDist2)
		incrFlag2 = false;
	if (m_players[tempId2]->m_position.x < -tempDist2)
		incrFlag2 = true;

	m_players[tempId]->updateAABB();
#endif


	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->updateGameInfo();
	}



	// update each player's place in the kdTree
	for (int i = 0; i < m_players.size(); i++)
	{
		Player* p = m_players[i];


		// first: remove the player from player's kdtree parent nodes
		for (int j = 0; j < p->m_parentNodes.size(); j++)
		{
			KDTreeNode* kNode = p->m_parentNodes[j];

			if (kNode == NULL)
				continue;

			kNode->removeObject(p);
		}


		// then: we clear player's stored parentNodes
		while (!p->m_emptyIndexPool.empty())
		{
			p->m_emptyIndexPool.pop();
		}

		for (int j = 0; j < p->m_parentNodes.size(); j++)
		{
			p->m_parentNodes[j] = NULL;
			p->m_emptyIndexPool.push(j);
		}

		// last: we re-insert the player back into the kdtree
		m_objectKDtree.insert(p);
	}


	// collision detection
	m_players[m_defaultPlayerID]->updateCamera(m_pipeline);

	o_skybox.setPosition(m_players[m_defaultPlayerID]->m_camera->getEyePoint());
}





for (int i = 0; i < m_objects.size(); i++)
{
	WorldObject* object = m_objects[i];

	if (object == NULL)
		continue;

	object->updateGameInfo();


	if (object->getObjectType() == WEAPON)
	{
		Weapon* wObject = (Weapon*)object;
		if (wObject->getWeaponSlot() == PROJECTILE && wObject->shouldExplode())
		{
			utl::debug("Exploding");

			ParticleEffect* effect = wObject->explode();
			m_smokeEffects.push_back((SmokeEffect*)effect);

			delete wObject;
			wObject = NULL;
			m_objects[i] = NULL;
		}
	}

}




for (int i = 0; i < m_objects.size(); i++)
{
	WorldObject* object = m_objects[i];

	if (object == NULL)
		continue;

	if (object->getDynamicType() == STATIC)
	{
		continue;
	}

	if (object->getObjectType() == WEAPON)
	{
		Weapon* wObject = (Weapon*)object;

		if (wObject->hasOwner == true || wObject->getWeaponSlot() != PROJECTILE)
		{
			continue;
		}
	}


	object->m_velocity += glm::vec3(0.0f, -9.81f, 0.0f) * 0.005f * 0.5f;
	object->m_position += object->m_velocity;
	// object->m_velocity += utl::BIASED_HALF_GRAVITY;
	// object->m_position += object->m_velocity;
	object->updateAABB();


	vector<WorldObject*> neighbors;
	glm::vec3 volNearPoint(object->m_position);
	m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, object, volNearPoint, neighbors);

	unordered_set<int> objectsAlreadyTested;

	for (int i = 0; i < neighbors.size(); i++)
	{
		ContactData contactData;
		WorldObject* neighbor = neighbors[i];

		if (neighbor->getObjectType() == PLAYER)
		{
			continue;
		}


		if (CollisionDetection::testAABBAABB(object->m_aabb,
			neighbor->m_aabb,
			contactData))
		{

			// ground was getting inserted twice. We dont want that!
			if (objectsAlreadyTested.find(neighbor->m_instanceId) != objectsAlreadyTested.end())
				continue;
			else
				objectsAlreadyTested.insert(neighbor->m_instanceId);


			if (neighbor->getObjectType() == WEAPON)
			{
				continue;
			}

			contactData.pair[0] = object;
			contactData.pair[1] = NULL;

			contactData.resolveVelocity1();
			contactData.resolveInterpenetration();
		}
	}

	object->updateAABB();

	utl::debug("object name", object->m_name);
	utl::debug("object parent size", object->m_parentNodes.size());

	for (int j = 0; j < object->m_parentNodes.size(); j++)
	{
		KDTreeNode* kNode = object->m_parentNodes[j];
		if (kNode == NULL)
			continue;
		kNode->removeObject(object);
	}



	// removing, we pop empty our queue, and set everything in vector to NULL
	while (!object->m_emptyIndexPool.empty())
	{
		object->m_emptyIndexPool.pop();
	}
	for (int j = 0; j < object->m_parentNodes.size(); j++)
	{
		object->m_parentNodes[j] = NULL;
		object->m_emptyIndexPool.push(j);
	}

	m_objectKDtree.insert(object);
}

#endif