#include "network_server.h"

NetworkServer::NetworkServer()
{
}

NetworkServer::~NetworkServer()
{

}

void NetworkServer::init()
{
	peer = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	// Sets how many incoming connections are allowed. 
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);
}


void NetworkServer::run()
{
	// BitStreams are used to hold and read content of messges contained in packets
	RakNet::BitStream bsOut;

	RakNet::Packet* packet;

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
				case PLAYER_UPDATE:
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
					bsOut.Write((RakNet::MessageID)PLAYER_UPDATE);
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

	RakNet::RakPeerInterface::DestroyInstance(peer);
}