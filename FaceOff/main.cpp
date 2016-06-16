#include "main.h"

#define NETWORK_FLAG 0

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



#define MAX_CLIENTS 10
#define SERVER_PORT 60000

bool incrFlag = true;

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

}

FaceOff::~FaceOff()
{
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
	initGUI();
	
	m_defaultPlayerID = 0;
	//Initialize clear color
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

	m_pipeline.setMatrixMode(PROJECTION_MATRIX);
	m_pipeline.loadIdentity();
	m_pipeline.perspective(45, utl::SCREEN_WIDTH / utl::SCREEN_HEIGHT, 0.5, 2000.0);

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	SDL_WM_SetCaption("FaceOff", NULL);
}

void FaceOff::initModels()
{

	m_mm.init();


	float scale = 1.0;

	vector<string> textures;

	m_xyzModel = XYZAxisModel();





	m_groundModel.load("Assets/models/quad.obj");
//	m_groundModel.load("Assets/models/ground.obj");
	textures.clear();  textures.push_back("Assets/Images/chess.png"); //textures.push_back("Assets/tree.png"); // textures.push_back("Assets/Images/chess.png");
	m_groundModel.setTextures(textures);
	m_groundModel.setMeshRandTextureIdx();

	m_gunModel.load("./Assets/models/weapons/Ak_47/Ak-47.obj");

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
	m_firstPersonCamera = FirstPersonCamera();
	m_firstPersonCamera.setEyePoint(glm::vec3(-59.362, 94.037, 153.189));


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
	o_temp->setRotation(glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f));
	o_temp->setModel(&m_groundModel);
	o_temp->updateAABB();
	o_temp->m_name = "ground";
	m_objects.push_back(o_temp);



	o_temp = new WorldObject();
	o_temp->m_name = "south wall";
	o_temp->setScale(xbound, ybound / 2, wallWidth);
	o_temp->setPosition(0, ybound / 2, zbound + wallWidth);
//	o_temp->setModel(&m_groundModel);
	o_temp->setModel(m_mm.m_cube);
	o_temp->updateAABB();
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "north wall";
	o_temp->setScale(xbound, ybound / 2, wallWidth);
	o_temp->setPosition(0, ybound / 2, -zbound - wallWidth);
	o_temp->setModel(m_mm.m_cube);
	o_temp->updateAABB();
	m_objects.push_back(o_temp);

	
	o_temp = new WorldObject();
	o_temp->m_name = "west wall";
	o_temp->setScale(wallWidth, ybound / 2, zbound);
	// o_temp->setRotation(glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f));
	o_temp->setPosition(-xbound - wallWidth, ybound / 2, 0.0);
	o_temp->setModel(m_mm.m_cube);
	o_temp->updateAABB();
	m_objects.push_back(o_temp);


	o_temp = new WorldObject();
	o_temp->m_name = "east wall";
	o_temp->setScale(wallWidth, ybound / 2, zbound);
	// o_temp->setRotation(glm::rotate(-90.0f, 0.0f, 1.0f, 0.0f));
	o_temp->setPosition(xbound + wallWidth, ybound / 2, 0.0);
	o_temp->setModel(m_mm.m_cube);
	o_temp->updateAABB();
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
		o_temp->setModel(&m_woodenBox);
		o_temp->updateAABB();
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
		o_temp->setModel(&m_woodenBox);
		o_temp->updateAABB();
		m_objects.push_back(o_temp);
	}

	

	float thinGap = 45, thickGap = 80;
	float halfThinGap = thinGap / 2, halfThickGap = thickGap / 2;

	float pillarXScale = (xbound * 2 - thinGap * 3) / 2;
	float pillarYScale = 100;
	float pillarZScale = (zbound * 2 - thickGap * 2 - thinGap) / 2;

	float halfPosXMag = halfThinGap + pillarXScale/2;
	float halfPosZMag = halfThinGap + pillarZScale/2;
	
	utl::debug("scale x is", pillarXScale);
	utl::debug("scale z is", pillarZScale);


	utl::debug("x is", halfPosXMag);
	utl::debug("z is", halfPosZMag);

	o_temp = new WorldObject();
	o_temp->m_name = "NW Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(-halfPosXMag, pillarYScale / 2, -halfPosZMag);
	o_temp->setModel(&m_woodenBox);
	o_temp->updateAABB();
	m_objects.push_back(o_temp);
	

	o_temp = new WorldObject();
	o_temp->m_name = "NE Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(halfPosXMag, pillarYScale / 2, -halfPosZMag);
	o_temp->setModel(&m_woodenBox);
	o_temp->updateAABB();
	m_objects.push_back(o_temp);
	utl::debug("id is", o_temp->m_instanceId);


	o_temp = new WorldObject();
	o_temp->m_name = "SW Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(-halfPosXMag, pillarYScale / 2, halfPosZMag);
	o_temp->setModel(&m_woodenBox);
	o_temp->updateAABB();
	m_objects.push_back(o_temp);
	utl::debug("id is", o_temp->m_instanceId);


	o_temp = new WorldObject();
	o_temp->m_name = "SE Pillar";

	o_temp->setScale(pillarXScale, pillarYScale, pillarZScale);
	o_temp->setPosition(halfPosXMag, pillarYScale / 2, halfPosZMag);
	o_temp->setModel(&m_woodenBox);
	o_temp->updateAABB();
	
	m_objects.push_back(o_temp);

	utl::debug("id is", o_temp->m_instanceId);

	float formationGap = 40.0f;





	o_temp = new Weapon(m_mm.getWeaponData(MAC_11));
	o_temp->m_name = "MAC_11";
	o_temp->setAABBByPosition(-3 * formationGap, 5, -110);
	m_objects.push_back(o_temp);

	
	o_temp = new Weapon(m_mm.getWeaponData(AWM));
	o_temp->m_name = "AWM";
	o_temp->setAABBByPosition(-2 * formationGap, 5, -110);
	m_objects.push_back(o_temp);




	o_temp = new Weapon(m_mm.getWeaponData(MINIGUN));
	o_temp->m_name = "MINIGUN";
	o_temp->setAABBByPosition(-1 * formationGap, 5, -110);
	m_objects.push_back(o_temp);



	o_temp = new Weapon(m_mm.getWeaponData(KNIFE));
	o_temp->m_name = "knife";
	o_temp->setAABBByPosition(formationGap, 5, -110);
	m_objects.push_back(o_temp);


	/*
	o_temp = new Weapon(m_mm.getWeaponData(FRAG_GRENADE));
	o_temp->m_name = "FRAG_GRENADE";
	o_temp->setAABBByPosition(2 * formationGap, 5, -110);
	m_objects.push_back(o_temp);
	*/




	o_temp = new Weapon(m_mm.getWeaponData(MP5));
	o_temp->m_name = "MP5";

//	o_temp->setPosition(-3 * formationGap, 5, -140);
//	o_temp->updateAABB();
	o_temp->setAABBByPosition(-3 * formationGap, 5, -140);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_mm.getWeaponData(MG42));
	o_temp->m_name = "MG42";

//	o_temp->setPosition(-2 * formationGap, 5, -140);
//	o_temp->updateAABB();
	o_temp->setAABBByPosition(-2 * formationGap, 5, -140);
	m_objects.push_back(o_temp);
	

	// init weapons for the map
	o_temp = new Weapon(m_mm.getWeaponData(AK_47));
	o_temp->m_name = "AK 47";

//	o_temp->setPosition(-formationGap, 5, -140);
//	o_temp->updateAABB();
	o_temp->setAABBByPosition(-formationGap, 5, -140);
	m_objects.push_back(o_temp);
	




	o_temp = new Weapon(m_mm.getWeaponData(M16));
	o_temp->m_name = "M16";

//	o_temp->setPosition(formationGap, 5, -140);
//	o_temp->updateAABB();
	o_temp->setAABBByPosition(formationGap, 5, -140);
	m_objects.push_back(o_temp);


	o_temp = new Weapon(m_mm.getWeaponData(KATANA));
	o_temp->m_name = "katana";

//	o_temp->setPosition(2 * formationGap, 5, -140);
//	o_temp->updateAABB();
	o_temp->setAABBByPosition(2 * formationGap, 5, -140);
	m_objects.push_back(o_temp);



	o_temp = new Weapon(m_mm.getWeaponData(PISTOL_SHOTGUN));
	o_temp->m_name = "shotgun";

//	o_temp->setPosition(3 * formationGap, 5, -140);
//	o_temp->updateAABB();
	o_temp->setAABBByPosition(3 * formationGap, 5, -140);
	m_objects.push_back(o_temp);












	vector<WorldObject*> objectsForKDTree;
	
	for (int i = 0; i < m_objects.size(); i++)
	{
	//	utl::debug("name", m_objects[i]->m_name);
	//	utl::debug("obj type", m_objects[i]->getObjectType());

		WorldObject* obj = m_objects[i];

		if (obj->getObjectType() == WEAPON && ((Weapon*)(obj))->hasOwner == true)
		{
			continue;
		}
		objectsForKDTree.push_back(obj);
	}


	m_objectKDtree.build(objectsForKDTree, glm::vec3(xboundWithWalls + 1, ybound + 1, zboundWithWalls + 1), glm::vec3(-xboundWithWalls - 1, -1, -zboundWithWalls - 1));




	/*
	if (m_players.size() == 0)
	{
		m_defaultPlayerID = 0;
		m_players.push_back(new Player(m_defaultPlayerID));

		// enemies
		Player* p1 = new Player(1);
		p1->setPosition(p1->m_id * 10, 5, p1->m_id * 10);
		m_players.push_back(p1);

		Player* p2 = new Player(2);
		p1->setPosition(p1->m_id * 10, 5, p1->m_id * 10);
		m_players.push_back(p2);
	}
	*/



	//	Weapon* mainWeapon = new Weapon(m_mm.getWeaponData(AWM));
	Weapon* mainWeapon = new Weapon(m_mm.getWeaponData(M16));
	mainWeapon->m_name = "player mainWeapon";

	//	Weapon* pistol = new Weapon(m_mm.getWeaponData());
	Weapon* knife = new Weapon(m_mm.getWeaponData(KNIFE));
	knife->m_name = "player knife";

	Weapon* grenade = new Weapon(m_mm.getWeaponData(FRAG_GRENADE));
	grenade->m_name = "player grenade";



	m_defaultPlayerID = 0;
	m_players.push_back(new Player(m_defaultPlayerID));




	m_players[0]->pickUpWeapon(mainWeapon);
	m_players[0]->pickUpWeapon(knife);
	m_players[0]->pickUpWeapon(grenade);



	m_objects.push_back(mainWeapon);
	m_objects.push_back(knife);
	m_objects.push_back(grenade);




	Player* p = new Player(1);
	p->setPosition(p->m_id * 25, 5, p->m_id * 10 - 8);
	p->updateAABB();
	m_players.push_back(p);

	p = new Player(2);
	p->setPosition( (p->m_id-1) * 25, 5, p->m_id * 10 + 8);
	p->updateAABB();
	m_players.push_back(p);

	utl::debug("0 max", m_players[0]->m_aabb.max);
	utl::debug("0 min", m_players[0]->m_aabb.min);

	utl::debug("1 max", m_players[1]->m_aabb.max);
	utl::debug("1 min", m_players[1]->m_aabb.min);
	for (int i = 0; i < m_players.size(); i++)
		m_objectKDtree.insert(m_players[i]);
	




	// Grenade particle effect
	ParticleEffect* sampleEffect = new ParticleEffect();
	sampleEffect->setPosition(glm::vec3(0.0, 0.0, 0.0));
	sampleEffect->setScale(50.0);

	sampleEffect->init();
	sampleEffect->setTexture("Assets/fireworks_red.jpg");

	m_particleEffects.push_back(sampleEffect);
}





void FaceOff::initRenderers()
{
	m_rm.init(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
	m_rm.initSceneRendererStaticLightsData(m_lightManager);
}



void FaceOff::initNetwork()
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




}





void FaceOff::initLobby()
{
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

						int new_player_id = m_players.size();
						float new_spawn_x = new_player_id * 30;
						float new_spawn_y = 5;
						float new_spawn_z = new_player_id * 30;


						utl::debug("new_player_id", new_player_id);
						utl::debug("new_spawn_x", new_spawn_x);
						utl::debug("new_spawn_y", new_spawn_y);
						utl::debug("new_spawn_z", new_spawn_z);


						if (m_players.size() > 0)
						{
							// send new client notification to existing clients
							cout << "Signaling arrival of new clients, Sending new client's spaw position to each client" << endl;
							bsOut.Reset();
							bsOut.Write((RakNet::MessageID)NEW_CLIENT);
							bsOut.Write(new_player_id);
							bsOut.Write(new_spawn_x);
							bsOut.Write(new_spawn_y);
							bsOut.Write(new_spawn_z);
							for (int i = 0; i < m_players.size(); i++)
							{
								if (i == m_defaultPlayerID)
									continue;

								cout << " To: " << i << " - " << m_players[i]->m_guid.g << endl;
								peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(m_players[i]->m_guid), false);
							}


							bsOut.Reset();


							utl::debug("m_players size", m_players.size()); 

							cout << "Sending each client's position to new client" << endl;

							for (int i = 0; i < m_players.size(); i++)
							{
								cout << "sending for " << i << endl;
								bsOut.Reset();
								bsOut.Write((RakNet::MessageID)NEW_CLIENT);
								bsOut.Write(i);

								bsOut.Write(m_players[i]->m_position.x);
								bsOut.Write(m_players[i]->m_position.y);
								bsOut.Write(m_players[i]->m_position.z);

								peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
							}
						}
						else
						{
							cout << "No clients yet, didn't send spawn pos to existing " << "nor each existing pos to new " << endl;
						}



						// Add Player
						m_players.push_back(new Player(new_player_id));
						m_players[new_player_id]->m_guid = packet->guid;
						m_players[new_player_id]->setPosition(new_spawn_x, new_spawn_y, new_spawn_z);

						// Use a BitStream to write a custom user message
						// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically

						// write a WELCOME message and include the clients index + 1
						bsOut.Reset();
						bsOut.Write((RakNet::MessageID)SPAWN_POSITION);
						bsOut.Write(new_player_id);
						bsOut.Write(new_spawn_x);
						bsOut.Write(new_spawn_y);
						bsOut.Write(new_spawn_z);

						// send the message back to the same address the current packet came from (the new client)
						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

						// reset the BitStream
						bsOut.Reset();


						cout << "Player List" << endl;
						for (int i = 0; i < m_players.size(); i++)
						{
							cout << i << " - " << m_players[i]->m_id << " position " << m_players[i]->m_position.x << " "
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
					case SPAWN_POSITION:
					{
						int my_player_id = 0;
						float x, y, z;

						bsIn.Read(my_player_id);
						bsIn.Read(x);
						bsIn.Read(y);
						bsIn.Read(z);
						printf("Server said I'm client number %d at %f, %f, %f\n", my_player_id, x, y, z);

						m_defaultPlayerID = my_player_id;

						if (my_player_id + 1 >= m_players.size())
						{
							m_players.resize(my_player_id + 1);
						}

						m_players[my_player_id] = new Player(my_player_id);
						m_players[my_player_id]->setPosition(x, y, z);
						server_address = packet->systemAddress;	

						break;
					}
						

					case NEW_CLIENT:
					{
						int other_player_id = 0;
						float x, y, z;
						
						bsIn.Read(other_player_id);
						bsIn.Read(x);
						bsIn.Read(y);
						bsIn.Read(z);
						printf("Received new client info for %d: %f, %f, %f", other_player_id, x, y, z);

						if (other_player_id + 1 >= m_players.size())
						{
							m_players.resize(other_player_id + 1);
						}

						m_players[other_player_id] = new Player(other_player_id);
						m_players[other_player_id]->setPosition(x, y, z);


						cout << "Player List" << endl;
						for (int i = 0; i < m_players.size(); i++)
						{

							if (m_players[i] == NULL)
							{
								cout << "player " << i << " is null" << endl;
							}
							else
							{
								cout << i << " - " << m_players[i]->m_id << " position " << m_players[i]->m_position.x << " "
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
			cout << i << " - " << m_players[i]->m_id << " position " << m_players[i]->m_position.x << " "
				<< m_players[i]->m_position.y << " "
				<< m_players[i]->m_position.z << endl;
		}
	}


	printf("Done waiting in the Lobby.\n");

}





void FaceOff::serverNetworkThread()
{
	while (isRunning)
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
			{
				// received new position from client    
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
}


void FaceOff::clientNetworkThread()
{
	while (isRunning)
	{
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
//		m_nextGameTick += DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES / 10;
		m_nextGameTick += DEFAULT_SERVER_MILLISECONDS_BETWEEN_UPDATES;



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
}

void FaceOff::start()
{

	cout << "Start" << endl;
	Uint32 startTime = SDL_GetTicks();
	m_nextGameTick = 0;

#if SERVER_NETWORK_THREAD == 1
	if (m_isServer)
	{
		m_networkThread = thread(&FaceOff::serverNetworkThread, this);
	}
#endif	

#if CLIENT_NETWORK_THREAD == 1
	if (!m_isServer)
	{
		m_networkThread = thread(&FaceOff::clientNetworkThread, this);
	}
#endif
	

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

					if (m_players[m_defaultPlayerID]->inGrenadeGatherMode())
					{
						utl::debug("here in Grenade gather mode");
						Weapon* grenade = m_players[m_defaultPlayerID]->throwGrenade();

					}

					hitNode = NULL;

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

					if (m_isServer)
						m_firstPersonCamera.setMouseIn(true);
					else
					{
						if (m_players[m_defaultPlayerID]->m_camera->getMouseIn())
						{
							m_players[m_defaultPlayerID]->fireWeapon();

							

							WorldObject* hitObject = NULL;

							glm::vec3 lineStart = m_players[m_defaultPlayerID]->getFirePosition();
							glm::vec3 lineDir = -m_players[m_defaultPlayerID]->m_camera->m_targetZAxis;


							utl::debug("lineStart", lineStart);
							utl::debug("lineDir", lineDir);

							// m_objectKDtree.visitNodes(m_objectKDtree.m_head, lineStart, lineDir, 500.0f, hitObject, 0, hitNode);

							float hitObjectSqDist = FLT_MAX;
							m_objectKDtree.visitNodes(m_objectKDtree.m_head, m_players[m_defaultPlayerID], lineStart, lineDir, 500.0f, hitObject, hitObjectSqDist);

						//	utl::debug("player pos", lineStart);
						//	utl::debug("target z", lineDir);

							if (hitObject != NULL)
							{
								utl::debug("name", hitObject->m_name);
								hitObject->isHit = true;
							}
							else
								utl::debug("hitObject is NULL");
							// VisitNodes
						}
						
						m_players[m_defaultPlayerID]->m_camera->setMouseIn(true);
						
					}
					
					
					
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
				{}
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
					if (m_players[m_defaultPlayerID]->m_velocity.y == 0.0)
						m_players[m_defaultPlayerID]->m_velocity += glm::vec3(0.0, 20.0, 0.0);
					break;

				case SDLK_z:
					if (m_isServer)
						m_firstPersonCamera.setMouseIn(false);
					else
						m_players[m_defaultPlayerID]->m_camera->setMouseIn(false);
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

	m_mouseState.m_pos = glm::vec2(mx, utl::SCREEN_HEIGHT - my);

	/*
	for (int i = 0; i < m_players.size(); i++)
	{
		if (i != m_defaultPlayerID && m_players[i] != NULL)
			m_players[i]->updateModel();
		
	}
	*/	



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
				break;
			}
		}

	}
#endif
#endif

}

/*
so we want to keep somethings in the server
*/



void FaceOff::forwardRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	// glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	m_pipeline.setMatrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();

	/*
	if (m_isServer)
	{
		m_firstPersonCamera.control(m_pipeline);
		o_skybox.setPosition(-m_firstPersonCamera.getEyePoint());
	}
	else
	*/
	{
	//	m_players[m_defaultPlayerID]->update(m_pipeline);
	//	m_players[m_defaultPlayerID]->updateCD(m_pipeline, &m_objectKDtree);

		m_players[m_defaultPlayerID]->m_velocity += glm::vec3(0.0f, -9.81f, 0.0f) * 0.0001f * 0.5f;
		m_players[m_defaultPlayerID]->m_camera->m_target += m_players[m_defaultPlayerID]->m_velocity;


//		m_players[m_defaultPlayerID]->m_camera->m_target.y -= (9.82 * 0.03);
		m_players[m_defaultPlayerID]->control();
		
		// utl::debug("Position is", m_players[m_defaultPlayerID]->m_position);


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
	
			if (CollisionDetection::testSphereAABB(m_players[m_defaultPlayerID]->m_boundingSphere, 
													neighbors[i]->m_aabb, 
													contactData))
			{
				neighbors[i]->isCollided = true;

				// ground was getting inserted twice. We dont want that!
				if (objectsAlreadyTested.find(neighbors[i]->m_instanceId) != objectsAlreadyTested.end())
					continue;
				else
					objectsAlreadyTested.insert(neighbors[i]->m_instanceId);

				/*
				if (neighbors[i]->getObjectType() == WEAPON)
				{
					continue;
				}
				*/

				contactData.pair[0] = m_players[m_defaultPlayerID];
				contactData.pair[1] = NULL;
				


				contactData.resolveVelocity();
				contactData.resolveInterpenetration();
		
			}
		}


		m_players[m_defaultPlayerID]->updateGameStats();




	//	utl::debug("player pos is ", m_players[m_defaultPlayerID]->m_position);
	//	utl::debug("player vel is ", m_players[m_defaultPlayerID]->m_velocity);


		int tempId = 2;
		float tempDist = 100;

		if (incrFlag)
			m_players[tempId]->m_position.x += 0.05;
		else
			m_players[tempId]->m_position.x -= 0.05;


		if (m_players[tempId]->m_position.x > tempDist)
			incrFlag = false;
		if (m_players[tempId]->m_position.x < -tempDist)
			incrFlag = true;
		

		m_players[tempId]->updateAABB();

		for (int i = 0; i < m_players.size(); i++)
		{
			Player* p = m_players[i];


			
			for (int j = p->m_parentNodes.size()-1; j >= 0; j--)
			{
				KDTreeNode* kNode = p->m_parentNodes[j];
				(kNode->m_objects2).erase(p->m_instanceId);
		//		p->m_parentNodes.pop_back();
			}
			p->m_parentNodes.clear();

			m_objectKDtree.insert(m_players[i]);
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

				

				m_particleEffects.push_back(effect); 
			
		
				delete wObject;
				wObject = NULL;
				m_objects[i] = NULL;
			}
		}
		
	}
	

	for (int i = 0; i<m_objects.size(); i++)
	{
		WorldObject* object = m_objects[i];
		

		if (object == NULL)
			continue;
		

//		object->updateGameInfo();



		if (object->getDynamicType() == STATIC)
		{
			continue;
		}

		// utl::debug("object name is", object->m_name);
		
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
		object->updateAABB();


		vector<WorldObject*> neighbors;
		glm::vec3 volNearPoint(object->m_position);
		m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, object, volNearPoint, neighbors);


		
		unordered_set<int> objectsAlreadyTested;

		// utl::debug("neighbor size is", neighbors.size());
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
				
				
				// utl::debug("object name is", object->m_name);
				/*
				utl::debug("normal is", contactData.normal);
				utl::debug("neighbor name is", neighbor->m_name);
				*/

				contactData.pair[0] = object;
				contactData.pair[1] = NULL;

				contactData.resolveVelocity1();
				contactData.resolveInterpenetration();
			}
		}

		object->updateAABB();

		for (int j = object->m_parentNodes.size() - 1; j >= 0; j--)
		{
			KDTreeNode* kNode = object->m_parentNodes[j];
			(kNode->m_objects2).erase(object->m_instanceId);
			//		p->m_parentNodes.pop_back();
		}
		object->m_parentNodes.clear();

		m_objectKDtree.insert(object);

		/*
		vector<WorldObject*> neighbors;
		glm::vec3 volNearPoint(m_objects[i]->m_position);
		m_objectKDtree.visitOverlappedNodes(m_objectKDtree.m_head, m_objects[i], volNearPoint, neighbors);
		
		unordered_set<int> objectsAlreadyTested;

		for (int i = 0; i < neighbors.size(); i++)
		{
			neighbors[i]->isTested = true;

			ContactData contactData;

			if (CollisionDetection::testSphereAABB(m_players[m_defaultPlayerID]->m_boundingSphere,
				neighbors[i]->m_aabb,
				contactData))
			{
				neighbors[i]->isCollided = true;

				if (names.find(neighbors[i]->m_instanceId) != names.end())
					continue;
				else
					names.insert(neighbors[i]->m_instanceId);

				contactData.pair[0] = m_players[m_defaultPlayerID];
				contactData.pair[1] = NULL;

				contactData.resolveVelocity();
				contactData.resolveInterpenetration();

			}
		}
		*/


	}
	









	for (int i = 0; i < m_players.size(); i++)
	{
		m_players[i]->updateGameInfo();

	}
	/*
	for (int i = 0; i<m_objects.size(); i++)
	{
		m_objects[i]->updateGameInfo();
	}
	*/

	m_pipeline.setMatrixMode(MODEL_MATRIX);
	// o_skybox.setPosition(0.0, 0.0, 0.0);
	// o_skybox.setScale(5.0);
	o_skybox.render(m_pipeline);
	
	// o_skybox.render(m_pipeline);


	
	glDisable(GL_CULL_FACE);

	p_renderer = &m_rm.r_texturedObject;
	p_renderer->enableShader();
	p_renderer->setData("u_texture", 0, GL_TEXTURE_2D, tempTexture);

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
			/*
			for (int i = 0; i < m_players.size(); i++)
			{
			if (i != m_defaultPlayerID && m_players[i] != NULL)
			{
			//		m_players[i]->render(m_pipeline);
			//		m_players[i]->renderWeapon(m_pipeline);
			m_players[i]->renderModel(m_pipeline, p_renderer);
			}
			else
			{
			//		m_players[i]->renderWeapon(m_pipeline);
			m_players[i]->render(m_pipeline, p_renderer);
			}
			}
			*/
			//	m_players[0]->render(m_pipeline, p_renderer, m_mm);
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
				
			if( object->isTested != true && object->isCollided != true && object->isHit != true)
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
		p_renderer->setData("u_color", GREEN);
		
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
				p_renderer->setData("u_color", GREEN);
				object->renderGroup(m_pipeline, p_renderer);
				
			}
			else if (object->isCollided)
			{
				p_renderer->setData("u_color", PURPLE);
				object->renderGroup(m_pipeline, p_renderer);
				object->isCollided = false;
			}
			else if (object->isTested)
			{
				p_renderer->setData("u_color", BLUE);
				object->renderGroup(m_pipeline, p_renderer);
				object->isTested = false;
			}
		}

		

		for (int i = 0; i < m_players.size(); i++)
		{

			Player* p = m_players[i];

			p->alreadyFireTested = false;
			if (i == m_defaultPlayerID)
			{

				continue;
			}

			//utl::debug("hit", p->isHit);
			//utl::debug("collided", p->isCollided);
			//utl::debug("tested", p->isTested);

			if (p->isHit)
			{
				p_renderer->setData("u_color", GREEN);
				p->renderGroup(m_pipeline, p_renderer);

			}
			else if (p->isCollided)
			{
				p_renderer->setData("u_color", PURPLE);
				p->renderGroup(m_pipeline, p_renderer);
				p->isCollided = false;
			}
			else if (p->isTested)
			{
				p_renderer->setData("u_color", BLUE);
				p->renderGroup(m_pipeline, p_renderer);
				p->isTested = false;
			}
		}


	p_renderer->disableShader();


	
//	long long timeNowMillis = utl::getCurrentTimeMillis();
	long long timeNowMillis = getCurrentTimeMillis();

	int deltaTimeMillis = (unsigned int)(timeNowMillis - m_currentTimeMillis);		
	m_currentTimeMillis = timeNowMillis;

	

	p_renderer = &m_rm.r_particleEffectUpdate;
	p_renderer->enableShader();
		for (int i = 0; i < m_particleEffects.size(); i++)
		{
			ParticleEffect* effect = m_particleEffects[i];
			
			effect->m_time += deltaTimeMillis;
			p_renderer->setData("u_randomTexture", 3, GL_TEXTURE_1D, effect->m_randomTextureId);
			p_renderer->setData("u_time", (float)effect->m_time);
			p_renderer->setData("u_deltaTimeMillis", (float)deltaTimeMillis);

			effect->update(m_pipeline, p_renderer);
		}

	p_renderer->disableShader();



	p_renderer = &m_rm.r_particleEffectRender;
	p_renderer->enableShader();
		for (int i = 0; i < m_particleEffects.size(); i++)
		{
			ParticleEffect* effect = m_particleEffects[i];

			p_renderer->setData("u_centerPosition", effect->getPosition());
			p_renderer->setData("u_texture", 0, GL_TEXTURE_2D, effect->m_textureId);
			effect->render(m_pipeline, p_renderer);
		}
	p_renderer->disableShader();



	glEnable(GL_CULL_FACE);

	renderGUI();
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



int main(int argc, char *argv[])
{
	utl::initSDL(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT, pDisplaySurface);
	utl::initGLEW();

	FaceOff Martin;

	string name = "";
	printf("Enter Your Name?\n");
	getline(cin, name);
	utl::debug("Your Name:", name);

	
#if NETWORK_FLAG == 1
	char str[512];
	printf("(C) or (S)erver?\n");
	gets(str);
	if ((str[0] == 's') || (str[0] == 'S'))
	{
		Martin.m_isServer = true;
	}
	else
	{
		Martin.m_isServer = false;
	}
	
	Martin.initNetwork();
	Martin.initLobby();
#else
	
	if (Martin.m_players.size() == 0)
	{/*
		Martin.m_defaultPlayerID = 0;
		Martin.m_players.push_back(new Player(Martin.m_defaultPlayerID));

		
		// enemies
		Player* p1 = new Player(1);
		p1->setPosition(p1->m_id * 10, 5, p1->m_id * 10);
		Martin.m_players.push_back(p1);

		Player* p2 = new Player(2);
		p1->setPosition(p1->m_id * 10, 5, p1->m_id * 10);
		Martin.m_players.push_back(p2);
	*/
	}
	
#endif
	Martin.init();
	Martin.start();


	if (Martin.m_isServer && SERVER_NETWORK_THREAD == 1)
		Martin.m_networkThread.join();

	if (!Martin.m_isServer && CLIENT_NETWORK_THREAD == 1)
		Martin.m_networkThread.join();

	
	utl::exitSDL(pDisplaySurface);
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


void FaceOff::initGUI()
{
	m_gui.init(utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);
	Control::init("", 25, utl::SCREEN_WIDTH, utl::SCREEN_HEIGHT);

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
	int aimY = utl::SCREEN_HEIGHT / 2 ;

	utl::debug("aimX", aimX);
	utl::debug("aimY", aimY);
	Control* horiAim = new Label("", aimX, aimY, aimWidth+2, 2, GREEN);

	aimX = utl::SCREEN_WIDTH / 2;
	aimY = utl::SCREEN_HEIGHT / 2 - aimHeight / 2;

	utl::debug("aimX", aimX);
	utl::debug("aimY", aimY);

	Control* vertAim = new Label("", aimX, aimY, 2, aimHeight+1, GREEN);

	m_gui.addGUIComponent(HPBar);
	m_gui.addGUIComponent(armorBar);
	m_gui.addGUIComponent(ammoBar);
	m_gui.addGUIComponent(horiAim);
	m_gui.addGUIComponent(vertAim);
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




