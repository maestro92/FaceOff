#include "server.h"

Server::Server() : Server(false)
{}

Server::Server(bool dedicatedFlag)
{
	//clients.resize(MAX_CLIENTS);
	isDedicated = dedicatedFlag;
	isInitalized = false;
	peer = NULL;
}


Server::~Server()
{
	if (peer != NULL)
	{
		RakNet::RakPeerInterface::DestroyInstance(peer);
	}
}

// http://www.jenkinssoftware.com/raknet/manual/detailedimplementation.html
void Server::init(int fps)
{
	m_fps = fps;

	peer = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	// Sets how many incoming connections are allowed. 
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	isInitalized = true;


	snapshotObjectStatesBufferSize = NUM_SNAPSHOT_ENTITIES;	
	snapshotObjectStates.resize(snapshotObjectStatesBufferSize);
	nextSnapshotEntityIndex = 0;
	/*
	peer = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	// Sets how many incoming connections are allowed. 
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	*/
}

void Server::addClient(RakNet::Packet* packet)
{
	ClientHandle client(packet->guid);
	client.systemAddress = peer->GetSystemAddressFromGuid(packet->guid);
	client.isConnected = true;
	clients.push_back(client);
	
	numConnectedClient++;
}

RakNet::SystemAddress Server::getClientAddress(int i)
{
	return clients[i].systemAddress;
}

bool Server::isClientConnected(int i)
{
	return clients[i].isConnected;
}

int Server::getNumConnectedClients()
{
	return numConnectedClient;
}

int Server::getNumClients()
{
	return clients.size();
}

int Server::getClientOutgoingSequence(int i)
{
	return clients[i].netchan.outgoingSequence;
}

void Server::incrementClientOutgoingSequence(int i)
{
	++(clients[i].netchan.outgoingSequence);
}

void Server::setClientIncomingSequence(int i, int sequence)
{
	clients[i].netchan.incomingSequence = sequence;
}

int Server::getClientLastUserCmdFrame(int i)
{
	return clients[i].lastUserCmdFrame;
}

void Server::setClientLastUserCmdFrame(int i, int sequence)
{
	clients[i].lastUserCmdFrame = sequence;
}


int Server::getClientLastUserCmdIndex(int i)
{
	return clients[i].lastUserCmdIndex;
}

void Server::setClientLastUserCmdIndex(int i, int index)
{
	clients[i].lastUserCmdIndex = index;
}


Snapshot& Server::getClientSnapshot(int i, int si)
{
	return clients[i].snapshots[si];
}