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

#include "game_messages.h"
#include "network_utility.h"
#include "utility.h"
// #include "client.h"

#include "shared.h"

const int NUM_MAX_CLIENTS = 10;
const int NUM_SNAPSHOT_ENTITIES = 128;


const unsigned int SV_SNAPSHOT_BUFFER_SIZE = 32;
const unsigned int SV_SNAPSHOT_BUFFER_MASK = (SV_SNAPSHOT_BUFFER_SIZE - 1);

using namespace std;

enum ClientState
{
	NON_EXISTENT,
	CONNECTED,
	DISCONNECTED
};

/*
// snapshot constructed on the server for the particular client
class Snapshot
{
	public:
		int	serverTime;

		// since snapshots are sent by server to the client,
		// the playerState is the info about that client
		PlayerState	playerState;

		int numEntities;

		int firstEntityIndex;	// first index into the circular curSVSnapshotObjects[]
								// the entities must be in increasing state number
};
*/



// snapshot constructed on the server for the particular client
class Snapshot
{
	public:
		int	serverTime;

		// since snapshots are sent by server to the client,
		// the playerState is the info about that client
		PlayerState	playerState;

		int numEntities;

		int firstEntityIndex;	// first index into the circular curSVSnapshotObjects[]
		// the entities must be in increasing state number
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
		RakNet::SystemAddress getClientAddress(int i);
		bool isClientConnected(int i);
		void setClientIncomingSequence(int i, int sequence);
		int getClientOutgoingSequence(int i);
		void incrementClientOutgoingSequence(int i);

		int getClientLastAckMessageSequence(int i);						// currently, mainly used as lastAckSnapshotSequence
		void setClientLastAckMessageSequence(int i, int ackMsgSequence);

		int getClientLastUserCmdNum(int i);
		void setClientLastUserCmdNum(int i, int index);

		Snapshot& getClientSnapshot(int i, int si);

		int getNumConnectedClients();
		int getNumClients();

		int time;
		long long realTime;

	private:

		// server's representation of client
		// the main and everywhere else shouldn't be aware of this class
		class ClientHandle
		{
			public:
				Snapshot snapshots[SV_SNAPSHOT_BUFFER_SIZE];
				RakNet::SystemAddress systemAddress;
				bool isConnected;
				int lastAckMessageSequence;			// the frame that the last client usercmd send over
				int lastUserCmdNum;
				NetChannel netchan;
				RakNet::RakNetGUID m_guid;

				ClientHandle(RakNet::RakNetGUID guid)
				{
					m_guid = guid;
					isConnected = false;

					lastUserCmdNum = -1;
				}
		};

		int numConnectedClient;

		vector<ClientHandle> clients;
};












#endif



