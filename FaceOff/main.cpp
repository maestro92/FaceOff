#include "main.h"

#define NETWORK_FLAG 1

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

	m_defaultPlayerID = 0;
	//Initialize clear color
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

	m_pipeline.setMatrixMode(PROJECTION_MATRIX);
	m_pipeline.loadIdentity();
	m_pipeline.perspective(45, Utility::SCREEN_WIDTH / Utility::SCREEN_HEIGHT, 0.5, 5000.0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	SDL_WM_SetCaption("FaceOff", NULL);
}

FaceOff::~FaceOff()
{
	RakNet::RakPeerInterface::DestroyInstance(peer);
}



void FaceOff::initRenderers()
{
	RendererManager::init();
	tempTexture = Utility::loadTexture("Assets/Images/tank1B.png");
}


void FaceOff::initObjects()
{
	m_firstPersonCamera = FirstPersonCamera();

	float scale = 20.0;
	o_worldAxis.setScale(scale);
	o_ground.setRotation(glm::rotate(90.0f, 1.0f, 0.0f, 0.0f));



	o_skybox = SkyBox();

	int x = Utility::SCREEN_WIDTH - 200;
	int y = 0;
	int w = 200;
	int h = Utility::SCREEN_HEIGHT;
//	m_gui.init(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, x, y, w, h);
	
	// m_defaultPlayerID = 0;
	// m_players.push_back(new Player(m_defaultPlayerID));
	// p_defaultCamera = m_players[0]->m_camera;
	

}



void FaceOff::initModels()
{
	float scale = 50.0;

	m_xyzModel = XYZAxisModel();
	m_groundModel = QuadModel(-scale, scale, -scale, scale);

	m_gunModel.load("./Assets/models/weapons/Ak_47/Ak-47.obj");
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

		m_players.push_back(new Player(0));
		m_defaultPlayerID = 0;


		while (waitingInLobby)
		{
//			printf("In the lobby waiting loop.\n");
			/*
			Uint8* state = SDL_GetKeyState(NULL);
			if (state[SDLK_a])
			{
				Utility::debug("pressed A, ending waitingInLobby");
				waitingInLobby = false;
			}
			*/
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					case SDL_KEYDOWN:
						switch (event.key.keysym.sym)
						{
							case SDLK_a:
								Utility::debug("pressed A, ending waitingInLobby");
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
						float new_spawn_x = new_player_id * 10;
						float new_spawn_y = 5;
						float new_spawn_z = new_player_id * 10;


						Utility::debug("new_player_id", new_player_id);
						Utility::debug("new_spawn_x", new_spawn_x);
						Utility::debug("new_spawn_y", new_spawn_y);
						Utility::debug("new_spawn_z", new_spawn_z);


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


							Utility::debug("m_players size", m_players.size()); 

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
						/*
						m_players[new_player_id]->m_position.x = new_spawn_x;
						m_players[new_player_id]->m_position.y = new_spawn_y;
						m_players[new_player_id]->m_position.z = new_spawn_z;
						*/
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
	
	Utility::debug("m_defaultPlayerId", m_defaultPlayerID);

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


	printf("Done wiating in the Lobby.\n");

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
					m_players[m_defaultPlayerID]->m_camera->setMouseIn(true);
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

				case SDLK_z:
					m_firstPersonCamera.setMouseIn(false);
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

	m_mouseState.m_pos = glm::vec2(mx, Utility::SCREEN_HEIGHT - my);

	/*
	if (m_isServer)
	{
		// iterate over each message received
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			// we first initalized bitStream with the packet->data
			RakNet::BitStream bsIn(packet->data, packet->length, false);

			// we ignore the first part of each message (due to RakNet convention)
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			cout << "Client List" << endl;
			for (int i = 0; i < (int)clients.size(); i++)
				cout << i << " - " << clients[i].guid.g << endl;

			cout << endl << "New Packet from:" << packet->guid.g << endl;

			int client_id = 0;
			int x = 0, y = 0, z = 0;


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

				client_id = (int)clients.size();

				if ((int)clients.size() > 0)
				{
					// send new client notification to existing clients
					cout << "Sending new spaw position to each client" << endl;
					bsOut.Reset();
					bsOut.Write((RakNet::MessageID)NEW_CLIENT);
					bsOut.Write(client_id);
					bsOut.Write(20);
					bsOut.Write(20);
					bsOut.Write(10);
					for (int i = 0; i < (int)clients.size(); i++)
					{
						cout << " To: " << i << " - " << clients[i].guid.g << endl;
						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(clients[i].guid), false);
					}


					bsOut.Reset();

					cout << "Sending each client's position to new client" << endl;

					for (int i = 0; i < (int)clients.size(); i++)
					{
						cout << "sending for " << i << endl;
						bsOut.Reset();
						bsOut.Write((RakNet::MessageID)NEW_CLIENT);
						bsOut.Write(i);

						bsOut.Write(clients[i].x);
						bsOut.Write(clients[i].y);
						bsOut.Write(clients[i].z);

						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					}
				}
				else
				{
					cout << "No clients yet, didn't send spawn pos to existing " << "nor each existing pos to new " << endl;
				}



				// Add Client
				clients.push_back(Client(client_id));
				clients[client_id].guid = packet->guid;
				clients[client_id].x = 20;
				clients[client_id].y = 20;
				clients[client_id].z = 10;


				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically

				// write a WELCOME message and include the clients index + 1
				bsOut.Write((RakNet::MessageID)SPAWN_POSITION);
				bsOut.Write(client_id);
				bsOut.Write(20);
				bsOut.Write(20);
				bsOut.Write(10);

				// send the message back to the same address the current packet came from (the new client)
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

				// reset the BitStream
				bsOut.Reset();

				break;



			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;
			case POSITION_UPDATE:
				// received new position from client       
				bsIn.Read(client_id);
				bsIn.Read(x);
				bsIn.Read(y);
				bsIn.Read(z);
				printf("Client %d sent new position %d,%d,%d\n", client_id, x, y, z);

				clients[client_id].x = x;
				clients[client_id].y = y;
				clients[client_id].z = z;

				cout << "sending new position value to each client" << endl;

				bsOut.Reset();
				bsOut.Write((RakNet::MessageID)POSITION_UPDATE);
				bsOut.Write(client_id);
				bsOut.Write(x);
				bsOut.Write(y);
				bsOut.Write(z);

				for (int i = 0; i < (int)clients.size(); i++)
				{
					if (client_id != i)
					{
						cout << "	To: " << " - " << clients[i].guid.g << endl;
						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(clients[i].guid), false);
					}
					else
						cout << "	Not Sending to own client: " << client_id << endl;
				}
				break;

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
			bsOut.Reset();
		}
	}
	else
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{

			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(int_message);

			switch (packet->data[0])
			{
			case NEW_CLIENT:
				std::cout << "Server said I'm client number " << int_message << std::endl;
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				connected = true;
				break;
			case POSITION_UPDATE:
				// report the server's new counter value
				std::cout << "Server said we are now at " << int_message << std::endl;
				break;
			case YOUR_TURN:
				printf("My Turn. Sending message.\n");
				bsOut.Write((RakNet::MessageID)POSITION_UPDATE);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}
	*/
}

/*
so we want to keep somethings in the server

*/
void FaceOff::forwardRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	m_pipeline.setMatrixMode(VIEW_MATRIX);
	m_pipeline.loadIdentity();

//	m_firstPersonCamera.control(m_pipeline);
	

	
	m_players[m_defaultPlayerID]->update(m_pipeline);

//	o_skybox.setPosition(-m_firstPersonCamera.getEyePoint());
	o_skybox.setPosition(-m_players[m_defaultPlayerID]->m_camera->getEyePoint());
	
	o_skybox.render(m_pipeline);


	m_pipeline.setMatrixMode(MODEL_MATRIX);

	// render the players
	for (int i = 0; i < m_players.size(); i++)
	{
		if (i != m_defaultPlayerID)
			m_players[i]->render(m_pipeline);
	}
	


	p_renderer = &RendererManager::r_fullVertexColor;
	p_model = &m_xyzModel;
	o_worldAxis.renderSingle(m_pipeline, p_renderer, RENDER_PASS1, p_model);

	p_renderer = &RendererManager::r_fullColor;
	p_renderer->setData("u_color", glm::vec3(0.5, 0.0, 0.0));
	p_model = &m_groundModel;
	o_ground.renderSingle(m_pipeline, p_renderer, RENDER_PASS1, p_model);
	
	p_renderer = &RendererManager::r_fullTexture;
	p_renderer->setData("u_texture", 0, GL_TEXTURE_2D, 0);
	p_model = &m_gunModel;
	o_gun.renderSingle(m_pipeline, p_renderer, RENDER_PASS1, p_model);

	//   renderGUI();
}




int main(int argc, char *argv[])
{
	Utility::initSDL(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, pDisplaySurface);
	Utility::initGLEW();

	FaceOff Martin;

	string name = "";
	printf("Enter Your Name?\n");
	getline(cin, name);
	Utility::debug("Your Name:", name);

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
	{
		Martin.m_defaultPlayerID = 0;
		Martin.m_players.push_back(new Player(Martin.m_defaultPlayerID));
	}
#endif

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


void FaceOff::initGUI()
{

	Control::init("", 25, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT);

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


void FaceOff::renderGUI()
{

	m_gui.initGUIRenderingSetup();
	/// http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement
	unsigned int getTicks = SDL_GetTicks();

	//    string final_str = "(" + Utility::floatToStr(m_mouseState.m_pos.x) + ", " + Utility::floatToStr(m_mouseState.m_pos.y) + ")";

	m_gui.updateAndRender(m_mouseState);

}
