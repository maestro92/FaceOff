#ifndef NETWORK_SERVER_H_
#define NETWORK_SERVER_H_


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

#include "network_utility.h"
#include "client.h"
#include "game_messages.h"

#include "shared.h"

#define NUM_MAX_CLIENTS 10


#define NUM_SNAPSHOT_ENTITIES 256;

using namespace std;

enum ClientState
{
	NON_EXISTENT,
	CONNECTED,
	DISCONNECTED
};



class Server
{
	
	public:
		Server();
		Server(bool dedicatedFlag);
		~Server();

		void init(int fps);

		void run();

		RakNet::RakPeerInterface* peer;

		bool isInitalized;
		bool isDedicated;
		int m_fps;
		int time;
		int timeout;

		int snapshotCounter;	// incremented for each snapshot built
		// int	timeResidual;		// <= 1000 / sv_frame->value
		// the circular objects states used when building snapshot on the server
		vector<WorldObjectState> snapshotObjectStates;
		vector<WorldObjectState> snapshotObjectStatesBaselines;

		int nextSnapshotEntityIndex;
		int snapshotObjectStatesBufferSize;



//		vector<WorldObjectState> snapshotPlayerStates;


		int m_deltaTimeAccumulatorMS;
		int	nextFrameTime;		// when time > nextFrameTime, process world

		int frameNum;

		int num_entities;		// current number, <= MAX_GENTITIES

		void addClient(RakNet::Packet* packet);
		Client getClient(int i);
		vector<Client> clients;

		int clientCount;

	private:

};



#endif



