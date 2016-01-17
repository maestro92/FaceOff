
#ifndef NETWORK_MANAGER_H_
#define NETWORK_MANAGER_H_

#include <stdio.h>
#include <string.h>
#include <dos.h>

#include <conio.h>

#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "RakPeerInterface.h"
#include <RakNetTypes.h>
#include "MessageIdentifiers.h"
#include <vector>
#include "BitStream.h"
#include "RakNetTypes.h"	// Message ID

#include "game_messages.h"
#include "network_info.h"

#include "network_server.h"
#include "network_client.h"

using namespace std;



/*
enum GameMessages
{
	SPAWN_POSITION = ID_USER_PACKET_ENUM + 1,
	PLAYER_UPDATE = ID_USER_PACKET_ENUM + 2,
	YOUR_TURN = ID_USER_PACKET_ENUM + 3,
	NEW_CLIENT = ID_USER_PACKET_ENUM + 4,
};
*/


// http://classes.cs.kent.edu/gpg/trac/browser/EnginesS11/cmarshal/RakNet_POC
// http://classes.cs.kent.edu/gpg/trac/wiki/cmarshall


#if 0
void server()
{
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	// Sets how many incoming connections are allowed. 
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	// BitStreams are used to hold and read content of messges contained in packets
	// A RakNet RakString can be used when reading string data from a BitStream
	RakNet::BitStream bsOut;
	RakNet::RakString rakString;

	vector<Client> clients;


	while (1)
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
							peer->Send(&bsOut, HIGH_PRIORITY, RELIABGLE_ORDERED, 0, peer->GetSystemAddressFromGuid(clients[i].guid), false);
						}

					}


					// Use a BitStream to write a custom user message
					// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically

					// write a WELCOME message and include the clients index + 1
					bsOut.Write((RakNet::MessageID)NEW_CLIENT);
					bsOut.Write((int)clients.size());

					// send the message back to the same address the current packet came from (the new client)
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

					// reset the BitStream
					bsOut.Reset();

					// wait 3 seconds
					std::this_thread::sleep_for(std::chrono::seconds(3));

					// write and send a YOUR_TURN message to the same new client
					bsOut.Write((RakNet::MessageID)YOUR_TURN);
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);


					break;



				/*
				// the client sends an ID_NEW_INCOMING_CONNECTION message when it connects to the server
				case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n");

					// add the client to the vector  packet->guid (RakNetGUID)
					clients.push_back(packet->guid);

					// Use a BitStream to write a custom user message
					// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically

					// write a WELCOME message and include the clients index + 1
					bsOut.Write((RakNet::MessageID)NEW_CLIENT);
					bsOut.Write((int)clients.size());

					// send the message back to the same address the current packet came from (the new client)
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

					// reset the BitStream
					bsOut.Reset(); 

					// wait 3 seconds
					std::this_thread::sleep_for(std::chrono::seconds(3));

					// write and send a YOUR_TURN message to the same new client
					bsOut.Write((RakNet::MessageID)YOUR_TURN);
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);


					break;
				*/

				case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");
					break;
				case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
					break;
				case PLAYER_UPDATE:
					// received new position from client       

					printf("Client sent incre flag, incrementing counter\n");
					++counter;

					cout << "Sending new counter value to each client" << endl;

					bsOut.Reset();
					bsOut.Write((RakNet::MessageID)PLAYER_UPDATE);
					bsOut.Write(counter);

					for (int i = 0; i < (int)clients.size(); i++)
					{
						cout << "	To: " << i + 1 << " - " << clients[i].g << endl;
						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(clients[i]), false);

					}
					std::this_thread::sleep_for(std::chrono::seconds(3));

					// send a YOUR_TURN to the next client
					current_client = (++current_client) % (int)clients.size();
					bsOut.Reset();
					bsOut.Write((RakNet::MessageID)YOUR_TURN);
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromGuid(clients[current_client]), false);
					break;

				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
			}
			bsOut.Reset();
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);
}
#endif

void client()
{
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;

	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	char str[512];

	printf("Enter server IP or hit enter for 127.0.0.1\n");
	gets(str);
	if (str[0] == 0)
		strcpy(str, "127.0.0.1");

	printf("Starting the client.\n");
	peer->Connect(str, SERVER_PORT, 0, 0);

	bool connected = false;
	int int_message;

	RakNet::BitStream bsOut;


	while (1)
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
			case PLAYER_UPDATE:
				// report the server's new counter value
				std::cout << "Server said we are now at " << int_message << std::endl;
				break;
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
void networkManager()
{
	char str[512];

	printf("(C) or (S)erver?\n");
	gets(str);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		NetworkServer client;
		client.run();
	}
	else
	{
		NetworkServer server;
		server.run();
	}
}

#if 0
void networkManager()
{


	char str[512];

	// the main interface for network communication
	// the primary interface for RakNet, RakPeer contains all major functions for the library
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;

	printf("(C) or (S)erver?\n");
	gets(str);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else
	{
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		isServer = true;
	}

	// To knkow what can the RakPeerInterface do, we can refer to the specs but also look at the RakPeerInterface.h files
	// http://www.jenkinssoftware.com/raknet/manual/Doxygen/classRakNet_1_1RakPeerInterface.html





	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		// Sets how many incoming connections are allowed. 
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else
	{
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0] == 0)
			strcpy(str, "127.0.0.1");

		printf("Starting the client.\n");
		peer->Connect(str, SERVER_PORT, 0, 0);
	}


	/*

	Receive:
	gets a message from the incoming message queue, Use DealocatePacket() to deallocate the message after you are done iwht it. User-thread functions,
	this either reutns 0 if no packets are waiting or returns a pointer to a Packet Struct

	DeallcoatePacket:
	call this to adeallocate a message returned by Receive() when you are done handling it

	*/

	RakNet::Packet* packet;
	bool running = true;
	while (running)
	{
		for (packet = peer->Receive(); packet != NULL; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
				{
					// Use a BitStream to write a custom user message
					// Bitstreams are easier to use than sending casted structes, and handle endian swapping automatically
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
					bsOut.Write("Hello World");
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					//		running = false;
				}
				break;

			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer)
					printf("A client has disconnected.\n");
				else
					printf("We have been disconnected.\n");
				break;

			case ID_CONNECTION_LOST:
				if (isServer)
					printf("A client lost the connection.\n");
				else
					printf("Connection lost.\n");
				break;
				/*
			case ID_GAME_MESSAGE_1:
			{
									  RakNet::RakString rs;
									  RakNet::BitStream bsIn(packet->data, packet->length, false);
									  bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
									  bsIn.Read(rs);
									  printf("%s\n", rs.C_String());
									  running = false;
			}
				break;
				*/
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}

	}




	RakNet::RakPeerInterface::DestroyInstance(peer);

}

#endif

#endif

